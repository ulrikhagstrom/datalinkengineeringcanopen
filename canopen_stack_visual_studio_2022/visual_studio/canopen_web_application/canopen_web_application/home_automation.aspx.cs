using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Text;
using System.Timers;
using CanopenDevices;
using System.Drawing;

namespace canopen_web_application
{
    class CanFrame
    {
        public String Id { get; set; }
        public String Data { get; set; }
        public String Dlc { get; set; }
    }

    public partial class home_automation : System.Web.UI.Page
    {
        private static CanMonitor_NET can_monitor;

        ReceivePDO_NET receive_pdo;

        public static IO_401 ioUnit;

        public static byte s_state = 255;

        public static bool christmas_lights;

        private static Queue<CanFrame> frames = new Queue<CanFrame>();


        static CanOpenStatus canReceiveCallback(object obj, uint id, byte[] data, byte dlc, uint flags)
        {
            StringBuilder tempString = new StringBuilder();

            if ((flags & CanMessageTypes.CAN_MSG_RTR_FLAG) != 0)
            {
                tempString.Append("RTR");
            }
            else
            {
                for (int i = 0; i < dlc; i++)
                {
                    tempString.AppendFormat("{0:X2} ", data[i]);
                }
            }

            lock (frames)
            {
                frames.Enqueue(new CanFrame() { Id = String.Format("0x{0:X3}", id), Data = tempString.ToString(), Dlc = dlc.ToString() });

                if (frames.Count() > 21)
                    frames.Dequeue();
            }

            return CanOpenStatus.CANOPEN_OK;
        }

        protected void Timer1_Tick(object sender, EventArgs e)
        {
            lblChistmansLightsStatus.Text = christmas_lights ? "Lights on" : "Lights off";
            lblChistmansLightsStatus.ForeColor = christmas_lights ? Color.DarkGreen : Color.DarkRed;

            lock (frames)
            {
                CanGrid.DataSource = frames;
                CanGrid.DataBind();
            }
        }

        public void UpdateLabel(string text)
        {
            CanGrid.DataBind();

        }

        protected void Page_Load(object sender, EventArgs e)
        {
            if (IsPostBack == true)
                return;

            if (ioUnit == null)
                ioUnit = new IO_401();
            else
                return;

            ioUnit.canHardwareConnect(0, 125000);

            can_monitor = new CanMonitor_NET();

            can_monitor.canHardwareConnect(0, 125000);

            can_monitor.canDispatcherPerformance(5, 5);

            can_monitor.registerCanReceiveCallback((Object)this, new CanReceiveDelegate(canReceiveCallback));

            CanGrid.DataSource = frames;

            ioUnit.startupNode(7);

            receive_pdo = new ReceivePDO_NET();

            receive_pdo.canHardwareConnect(0, 125000);

            receive_pdo.setCobid(0x207);

            receive_pdo.registerReceivePdoMessageCallBack((object)this, receive_pdo_callback);
        }


        static CanOpenStatus receive_pdo_callback(object obj, uint id, byte[] data, byte len)
        {
            home_automation.christmas_lights = ((data[0] & 0x01) != 0);

            return CanOpenStatus.CANOPEN_OK;
        }

        protected void btnChistmasLightsOff_Click(object sender, EventArgs e)
        {
            ioUnit.setIO(0, false);
        }

        protected void btnChistmasLightsOn_Click(object sender, EventArgs e)
        {
            ioUnit.setIO(0, true);

        }

        protected void btnRestartSystem_Click(object sender, EventArgs e)
        {
            ioUnit.canHardwareDisconnect();

            ioUnit = null;

            ioUnit = new IO_401();

            ioUnit.canHardwareConnect(0, 125000);

            ioUnit.startupNode(7);
        }
    }
}