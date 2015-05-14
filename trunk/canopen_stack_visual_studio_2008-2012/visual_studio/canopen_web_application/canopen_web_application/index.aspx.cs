﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Text;
using System.Timers;

namespace canopen_web_application
{
    class CanFrame
    {
        public String Id { get; set; }
        public String Data { get; set; }
        public String Dlc { get; set; }
    }

    public partial class index : System.Web.UI.Page
    {
        private static ClientSDO_NET client_SDO;
        private static NMT_Master_NET nmt_Master;
        private static CanMonitor_NET can_monitor;

        private static Queue<CanFrame> frames = new Queue<CanFrame>();

        static CANOPEN_LIB_ERROR.CanOpenStatus canReceiveCallback(object obj, uint id, byte[] data, byte dlc, uint flags)
        {
            StringBuilder tempString = new StringBuilder();
            tempString.AppendFormat("0x{0:X3} {1}: ", id, dlc);
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
                frames.Enqueue(new CanFrame() { Id = id.ToString(), Data = tempString.ToString(), Dlc = dlc.ToString() });

                if (frames.Count() > 10)
                    frames.Dequeue();
            }

            return CANOPEN_LIB_ERROR.CanOpenStatus.CANOPEN_OK;
        }

        protected void Timer1_Tick(object sender, EventArgs e)
        {
            Label1.Text = "Panel refreshed at: " +
            DateTime.Now.ToLongTimeString();

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

            CANOPEN_LIB_ERROR.CanOpenStatus stat;

            if (client_SDO == null || nmt_Master == null || can_monitor == null)
            {

                client_SDO = new ClientSDO_NET();
                nmt_Master = new NMT_Master_NET();
                can_monitor = new CanMonitor_NET();

                stat = can_monitor.canHardwareConnect(0, 125000);
                if (stat != CANOPEN_LIB_ERROR.CanOpenStatus.CANOPEN_OK)
                {
                }

                can_monitor.registerCanReceiveCallback((Object)this, new CanReceiveDelegate(canReceiveCallback));

                stat = client_SDO.canHardwareConnect(0, 125000);
                if (stat != CANOPEN_LIB_ERROR.CanOpenStatus.CANOPEN_OK)
                {
                }
                stat = client_SDO.connect(3);

                stat = nmt_Master.canHardwareConnect(0, 125000);
                if (stat != CANOPEN_LIB_ERROR.CanOpenStatus.CANOPEN_OK)
                {
                }

                CanGrid.DataSource = frames;

            }

            
        }

        protected void btnStartNodeGuardNode3_Click(object sender, EventArgs e)
        {
            nmt_Master.nodeGuardPollStart(3, 1000);
        }



        protected void btnStopNodeGuardNode3_Click(object sender, EventArgs e)
        {
            nmt_Master.nodeGuardPollStop(3);
        }

        protected void btnReadDeviceName_Click(object sender, EventArgs e)
        {
            byte[] receive_buffer = new byte[100];
            UInt32 error_code;
            uint valid;
            client_SDO.objectRead(0x1008, 0, receive_buffer, out valid, out error_code);
        }
    }
}