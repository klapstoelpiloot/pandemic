using IOModule;
using System;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;

namespace IOModuleSimulator
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void connectbutton_Click(object sender, EventArgs e)
        {
            IPHostEntry hostEntry = Dns.GetHostEntry(connectaddress.Text);
            if (hostEntry.AddressList.Length > 0)
            {
                var ip = hostEntry.AddressList.Where(a => a.AddressFamily == AddressFamily.InterNetwork).FirstOrDefault();
                Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.IP);
                s.Connect(ip, int.Parse(connectport.Text));
            }
        }
    }
}
