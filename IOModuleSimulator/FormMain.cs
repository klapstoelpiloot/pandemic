using Google.Protobuf;
using IOModule;
using IOModuleSimulator.Properties;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace IOModuleSimulator
{
	public partial class FormMain : Form
	{
		private IOModuleLink link;
		private Image led_off;
		private Image led_on;
		private int animatestep;    // 0 = front sensor high, 1 = both high, 2 = back sensor high, 3 = end (both low)
		private Button animatefrontbutton;
		private Button animatebackbutton;
		private bool buttonleftdown;
		private bool buttonrightdown;
		private bool buttonacceptdown;
		private bool buttoncanceldown;
		private bool calibrationmode;

		public FormMain()
		{
			led_off = Resources.led_off;
			led_on = Resources.led_on;
			InitializeComponent();
			Disconnect();
		}

		protected override void Dispose(bool disposing)
		{
			if(link != null)
			{
				link.Dispose();
				link = null;
			}

			if(disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		private void connectbutton_Click(object sender, EventArgs e)
		{
			if(link != null)
			{
				Disconnect();
			}
			else
			{
				try
				{
					link = new IOModuleLink(connectaddress.Text, connectpipe.Text);
				}
				catch(Exception ex)
				{
					MessageBox.Show(this, "Unable to connect to the specified pipe.\r\n" + ex.GetType().Name + ": " + ex.Message,
						Application.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
					return;
				}

				link.MessageReceived += Link_MessageReceived;
				connectaddresslabel.Enabled = false;
				connectaddress.Enabled = false;
				connectpipe.Enabled = false;
				connectpipelabel.Enabled = false;
				connectbutton.Text = "Disconnect";
				SetSensorState(buttonbridgefront, true);
				SetSensorState(buttonbridgeback, true);
				SetSensorState(buttongate1front, true);
				SetSensorState(buttongate1back, true);
				SetSensorState(buttongate2front, true);
				SetSensorState(buttongate2back, true);
				SetSensorState(buttongate3front, true);
				SetSensorState(buttongate3back, true);
				SetSensorState(buttongate4front, true);
				SetSensorState(buttongate4back, true);
				buttonleft.BackgroundImage = Resources.button_yellow_off;
				buttonright.BackgroundImage = Resources.button_yellow_off;
				buttonok.BackgroundImage = Resources.button_green_off;
				buttoncancel.BackgroundImage = Resources.button_red_off;
				buttonsgroup.Enabled = true;
				bridgegroup.Enabled = true;
				gatesgroup.Enabled = true;
			}
		}

		private void Disconnect()
		{
			if(link != null)
			{
				link.MessageReceived -= Link_MessageReceived;
				link.Dispose();
				link = null;
			}
			connectbutton.Text = "Connect";
			connectaddresslabel.Enabled = true;
			connectaddress.Enabled = true;
			connectpipe.Enabled = true;
			connectpipelabel.Enabled = true;
			buttonleft.BackgroundImage = Resources.button_gray;
			buttonright.BackgroundImage = Resources.button_gray;
			buttonok.BackgroundImage = Resources.button_gray;
			buttoncancel.BackgroundImage = Resources.button_gray;
			SetSensorState(buttonbridgefront, false);
			SetSensorState(buttonbridgeback, false);
			SetSensorState(buttongate1front, false);
			SetSensorState(buttongate1back, false);
			SetSensorState(buttongate2front, false);
			SetSensorState(buttongate2back, false);
			SetSensorState(buttongate3front, false);
			SetSensorState(buttongate3back, false);
			SetSensorState(buttongate4front, false);
			SetSensorState(buttongate4back, false);
			buttonsgroup.Enabled = false;
			bridgegroup.Enabled = false;
			gatesgroup.Enabled = false;
			buttonleftdown = false;
			buttonrightdown = false;
			buttonacceptdown = false;
			buttoncanceldown = false;
		}

		private void Link_MessageReceived(IMessage obj)
		{
			switch(obj)
			{
				case SetButtonLightsMsg setbuttonlightsmsg:
					buttonleft.BackgroundImage = setbuttonlightsmsg.LeftButton ? Resources.button_yellow_on : Resources.button_yellow_off;
					buttonright.BackgroundImage = setbuttonlightsmsg.RightButton ? Resources.button_yellow_on : Resources.button_yellow_off;
					buttonok.BackgroundImage = setbuttonlightsmsg.AcceptButton ? Resources.button_green_on : Resources.button_green_off;
					buttoncancel.BackgroundImage = setbuttonlightsmsg.CancelButton ? Resources.button_red_on : Resources.button_red_off;
					break;

				case RequestGateStatesMsg requestgatestatesmsg:
					GateStatesMsg statesmsg = new GateStatesMsg();
					statesmsg.Gate1Blocked = !GetSensorState(buttongate1front) | !GetSensorState(buttongate1back);
					statesmsg.Gate2Blocked = !GetSensorState(buttongate2front) | !GetSensorState(buttongate2back);
					statesmsg.Gate3Blocked = !GetSensorState(buttongate3front) | !GetSensorState(buttongate3back);
					statesmsg.Gate4Blocked = !GetSensorState(buttongate4front) | !GetSensorState(buttongate4back);
					link.SendMessage(statesmsg);
					break;

				case CalibrateModeMsg calibrationmodemsg:
					calibrationmode = true;
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate1front), Sensor = Sensor.Gate1Front });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate2front), Sensor = Sensor.Gate2Front });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate3front), Sensor = Sensor.Gate3Front });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate4front), Sensor = Sensor.Gate4Front });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate1back), Sensor = Sensor.Gate1Back });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate2back), Sensor = Sensor.Gate2Back });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate3back), Sensor = Sensor.Gate3Back });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttongate4back), Sensor = Sensor.Gate4Back });
					link.SendMessage(new SensorStateMsg() { High = !buttonleftdown, Sensor = Sensor.LeftButton });
					link.SendMessage(new SensorStateMsg() { High = !buttonrightdown, Sensor = Sensor.RightButton });
					link.SendMessage(new SensorStateMsg() { High = !buttonacceptdown, Sensor = Sensor.AcceptButton });
					link.SendMessage(new SensorStateMsg() { High = !buttoncanceldown, Sensor = Sensor.CancelButton });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttonbridgefront), Sensor = Sensor.Bridge1Sensor });
					link.SendMessage(new SensorStateMsg() { High = GetSensorState(buttonbridgeback), Sensor = Sensor.Bridge2Sensor });
					break;

				case NormalModeMsg normalmodemsg:
					calibrationmode = false;
					break;
			}
		}

		private void animateSensorTimer_Tick(object sender, EventArgs e)
		{
			switch(animatestep)
			{
				case 0:
					animatefrontbutton.BackgroundImage = led_off;
					animatebackbutton.BackgroundImage = led_off;
					break;

				case 1:
					animatefrontbutton.BackgroundImage = led_on;
					animatebackbutton.BackgroundImage = led_off;
					break;

				case 2:
					animatefrontbutton.BackgroundImage = led_on;
					animatebackbutton.BackgroundImage = led_on;
					animateSensorTimer.Stop();
					animatefrontbutton = null;
					animatebackbutton = null;
					break;
			}

			animatestep++;
		}

		private void StartPassThroughAnimation(Button frontbutton, Button backbutton)
		{
			animateSensorTimer.Stop();

			if(animatefrontbutton != null)
				animatefrontbutton.BackgroundImage = led_on;
			if(animatebackbutton != null)
				animatebackbutton.BackgroundImage = led_on;

			animatefrontbutton = frontbutton;
			animatebackbutton = backbutton;
			animatestep = 0;
			animateSensorTimer.Start();

			animatefrontbutton.BackgroundImage = led_off;
			animatebackbutton.BackgroundImage = led_on;
		}

		private bool GetSensorState(Button button)
		{
			return (button.BackgroundImage == led_on);
		}

		private void SetSensorState(Button button, bool state)
		{
			button.BackgroundImage = state ? led_on : led_off;

			if(calibrationmode)
			{
				SensorStateMsg ssm = new SensorStateMsg() { High = state };

				if(button == buttongate1front) ssm.Sensor = Sensor.Gate1Front;
				if(button == buttongate2front) ssm.Sensor = Sensor.Gate2Front;
				if(button == buttongate3front) ssm.Sensor = Sensor.Gate3Front;
				if(button == buttongate4front) ssm.Sensor = Sensor.Gate4Front;
				if(button == buttongate1back) ssm.Sensor = Sensor.Gate1Back;
				if(button == buttongate2back) ssm.Sensor = Sensor.Gate2Back;
				if(button == buttongate3back) ssm.Sensor = Sensor.Gate3Back;
				if(button == buttongate4back) ssm.Sensor = Sensor.Gate4Back;
				if(button == buttonbridgefront) ssm.Sensor = Sensor.Bridge1Sensor;
				if(button == buttonbridgeback) ssm.Sensor = Sensor.Bridge2Sensor;

				link.SendMessage(ssm);
			}
		}

		private void buttonstartslide_Click(object sender, EventArgs e)
		{
			StartPassThroughAnimation(buttonbridgefront, buttonbridgeback);
			link.SendMessage(new StartSlideMsg());
		}

		private void buttonremovepuck_Click(object sender, EventArgs e)
		{
			SetSensorState(buttonbridgefront, false);
			SetSensorState(buttonbridgeback, false);
			link.SendMessage(new RemovePuckMsg());
		}

		private void buttonbridgeblocked_Click(object sender, EventArgs e)
		{
			SetSensorState(buttonbridgefront, false);
			SetSensorState(buttonbridgeback, false);
			link.SendMessage(new SensorBlockedMsg());
		}

		private void buttonbridgecleared_Click(object sender, EventArgs e)
		{
			SetSensorState(buttonbridgefront, true);
			SetSensorState(buttonbridgeback, true);
			link.SendMessage(new SensorClearedMsg());
		}

		private void buttonscoregate1_Click(object sender, EventArgs e)
		{
			StartPassThroughAnimation(buttongate1front, buttongate1back);
			link.SendMessage(new GateScoreMsg() { Gate = 1 });
		}

		private void buttonscoregate2_Click(object sender, EventArgs e)
		{
			StartPassThroughAnimation(buttongate2front, buttongate2back);
			link.SendMessage(new GateScoreMsg() { Gate = 2 });
		}

		private void buttonscoregate3_Click(object sender, EventArgs e)
		{
			StartPassThroughAnimation(buttongate3front, buttongate3back);
			link.SendMessage(new GateScoreMsg() { Gate = 3 });
		}

		private void buttonscoregate4_Click(object sender, EventArgs e)
		{
			StartPassThroughAnimation(buttongate4front, buttongate4back);
			link.SendMessage(new GateScoreMsg() { Gate = 4 });
		}

		private void buttonleft_MouseDown(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttonleftdown = true;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = false, Sensor = Sensor.LeftButton });
				else
					link.SendMessage(new LeftButtonPressedMsg());
			}
		}

		private void buttonright_MouseDown(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttonrightdown = true;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = false, Sensor = Sensor.RightButton });
				else
					link.SendMessage(new RightButtonPressedMsg());
			}
		}

		private void buttonok_MouseDown(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttonacceptdown = true;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = false, Sensor = Sensor.AcceptButton });
				else
					link.SendMessage(new AcceptButtonPressedMsg());
			}
		}

		private void buttoncancel_MouseDown(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttoncanceldown = true;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = false, Sensor = Sensor.CancelButton });
				else
					link.SendMessage(new CancelButtonPressedMsg());
			}
		}

		private void SensorButton_Click(object sender, EventArgs e)
		{
			Button button = sender as Button;
			SetSensorState(button, !GetSensorState(button));
		}

		private void LoseFocus(object sender, MouseEventArgs e)
		{
			focusbox.Focus();
		}

		private void FormMain_KeyDown(object sender, KeyEventArgs e)
		{
			switch(e.KeyCode)
			{
				case Keys.A: buttonleft_MouseDown(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
				case Keys.S: buttonright_MouseDown(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
				case Keys.K: buttonok_MouseDown(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
				case Keys.L: buttoncancel_MouseDown(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
			}
		}

		private void FormMain_KeyUp(object sender, KeyEventArgs e)
		{
			switch(e.KeyCode)
			{
				case Keys.A: buttonleft_MouseUp(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
				case Keys.S: buttonright_MouseUp(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
				case Keys.K: buttonok_MouseUp(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
				case Keys.L: buttoncancel_MouseUp(sender, new MouseEventArgs(MouseButtons.Left, 1, 0, 0, 0)); break;
			}
		}

		private void buttonleft_MouseUp(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttonleftdown = false;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = true, Sensor = Sensor.LeftButton });
			}
			LoseFocus(sender, e);
		}

		private void buttonright_MouseUp(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttonrightdown = false;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = true, Sensor = Sensor.RightButton });
			}
			LoseFocus(sender, e);
		}

		private void buttonok_MouseUp(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttonacceptdown = false;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = true, Sensor = Sensor.AcceptButton });
			}
			LoseFocus(sender, e);
		}

		private void buttoncancel_MouseUp(object sender, MouseEventArgs e)
		{
			if(e.Button == MouseButtons.Left)
			{
				buttoncanceldown = false;
				if(calibrationmode)
					link.SendMessage(new SensorStateMsg() { High = true, Sensor = Sensor.CancelButton });
			}
			LoseFocus(sender, e);
		}
	}
}
