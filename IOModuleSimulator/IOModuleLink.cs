using Google.Protobuf;
using Google.Protobuf.Reflection;
using IOModule;
using System;
using System.IO.Pipes;
using System.Threading;

namespace IOModuleSimulator
{
	///
	/// Communication between IO Module and the Pi uses the nanopb Protobuf implementation.
	/// The protobuf messages are sent as follows:
	///
	///    2 bytes  1 byte     # bytes    
	///   |_______|_______|______________|
	///       1       2          3
	///
	/// 1: Bytes 0xBA and 0xBE indicate the start of the message.
	/// 2: Total size of the message in bytes. This includes the start bytes and this length byte.
	/// 3: Protobuf message data.
	/// 
	/// Since we're communicating over a USB connection (which has a guaranteed transfer protocol) and the chances
	/// of losing data are neglectable, the code below does not implement a system to guarantee correct arrival.
	///

	public class IOModuleLink : IDisposable
	{
		private const int BUFFER_SIZE = 256;
		private const int NUM_START_BYTES = 2;
		private const int NUM_LENGTH_BYTES = 1;
		private const int MINIMUM_MESSAGE_LENGTH = NUM_START_BYTES + NUM_LENGTH_BYTES;
		private const byte STARTBYTE1 = 0xBA;
		private const byte STARTBYTE2 = 0xBE;

		private NamedPipeClientStream stream;
		private byte[] rxbuffer = new byte[BUFFER_SIZE];
		private int rxlength = 0;

		public event Action<IMessage> MessageReceived;

		public IOModuleLink(string host, string pipe)
		{
			stream = new NamedPipeClientStream(host, pipe, PipeDirection.InOut, PipeOptions.Asynchronous | PipeOptions.WriteThrough);
			stream.Connect(1000);
			BeginReading();
		}

		public void Dispose()
		{
			try { stream.Close(); } catch(Exception) { }
			try { stream.Dispose(); } catch(Exception) { }
			stream = null;
		}

		private void BeginReading()
		{
			stream.BeginRead(rxbuffer, rxlength, 1, ReadCallback, this);
		}

		private void ReadCallback(IAsyncResult ar)
		{
			int bytesread;
			try
			{
				bytesread = stream.EndRead(ar);
			}
			catch(NullReferenceException)
			{
				// Stream was disposed
				return;
			}
			catch(ObjectDisposedException)
			{
				// Stream was disposed
				return;
			}

			// Check if the stream was closed on the other end
			if(bytesread == 0)
				return;

			rxlength += bytesread;

			// Make sure the first bytes are the start bytes and discard the data if invalid
			if((rxlength >= 1) && (rxbuffer[0] != STARTBYTE1))
			{
				Console.WriteLine("Message trashed: First byte is not a start byte.");
				rxlength = 0;
			}
			else if((rxlength >= 2) && (rxbuffer[1] != STARTBYTE2))
			{
				Console.WriteLine("Message trashed: Second byte is not a start byte.");
				rxlength = 0;
			}
			// Check if the length of the message can be determined
			else if(rxlength >= (NUM_START_BYTES + NUM_LENGTH_BYTES))
			{
				// Determine the message length
				int msglen = rxbuffer[NUM_START_BYTES];
				if((msglen < MINIMUM_MESSAGE_LENGTH) || (msglen > BUFFER_SIZE))
				{
					Console.WriteLine("Message trashed: Length is invalid ( " + msglen + ")");
					rxlength = 0;
				}

				if(rxlength >= msglen)
				{
					// Enough data received for a complete message
					int payloadlen = msglen - MINIMUM_MESSAGE_LENGTH;
					if(payloadlen > 0)
					{
						MessageParser<PiMessage> parser = new MessageParser<PiMessage>(() => new PiMessage());
						PiMessage wrapmsg = parser.ParseFrom(rxbuffer, MINIMUM_MESSAGE_LENGTH, payloadlen);

						// Find the message contained in the wrapper message
						OneofDescriptor contentdesc = (wrapmsg as IMessage).Descriptor.Oneofs[0];
						FieldDescriptor contentfield = contentdesc.Accessor.GetCaseFieldDescriptor(wrapmsg);
						IMessage msg = contentfield.Accessor.GetValue(wrapmsg) as IMessage;

						// Raise event with message
						MessageReceived?.Invoke(msg);
					}

					// Move remaining data to the start of the buffer.
					if(rxlength > msglen)
						Buffer.BlockCopy(rxbuffer, msglen, rxbuffer, 0, rxlength - msglen);
					rxlength -= msglen;
				}
			}

			BeginReading();
		}

		public void SendMessage(IMessage msg)
		{
			// Create the wrapper message
			IOMessage wrapmsg = new IOMessage();

			// One of the Content members must be the same type as the given message.
			// We set the given message on this member.
			bool contentset = false;
			OneofDescriptor contentdesc = (wrapmsg as IMessage).Descriptor.Oneofs[0];
			foreach(FieldDescriptor fd in contentdesc.Fields)
			{
				if(fd.MessageType.ClrType.IsEquivalentTo(msg.GetType()))
				{
					fd.Accessor.SetValue(wrapmsg, msg);
					contentset = true;
					break;
				}
			}

			// Make sure we are successfull
			if(!contentset)
				throw new NotImplementedException("Missing content member of '" + msg.GetType().Name + "' message type in the wrapper message. Are you sure you added it?");

			// Determine the serialized data size
			int datalengthint = MINIMUM_MESSAGE_LENGTH + wrapmsg.CalculateSize();

			// Serialize the message
			// The first 2 bytes is the start marker
			byte[] data = new byte[datalengthint];
			data[0] = STARTBYTE1;
			data[1] = STARTBYTE2;
			data[2] = (byte)datalengthint;
			byte[] payload = wrapmsg.ToByteArray();
			Array.Copy(payload, 0, data, MINIMUM_MESSAGE_LENGTH, payload.Length);

			// Send it
			stream.Write(data, 0, datalengthint);
		}
	}
}
