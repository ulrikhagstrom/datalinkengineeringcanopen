using System;
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
        private static ClientSDO_NET client_SDO_local;
        private static NMT_Master_NET nmt_Master;
        private static CanMonitor_NET can_monitor;
        private static ServerSDO_NET server_SDO;

        public static byte s_state = 255;

        private static Queue<CanFrame> frames = new Queue<CanFrame>();

        static CanOpenStatus nmtCallback(object obj, byte node_id, byte state)
        {
            s_state = state;
            return CanOpenStatus.CANOPEN_OK;
        }


        static CanOpenStatus canReceiveCallback(object obj, uint id, byte[] data, byte dlc, uint flags)
        {
            StringBuilder tempString = new StringBuilder();
            //tempString.AppendFormat("0x{0:X3} {1}: ", id, dlc);
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
                frames.Enqueue(new CanFrame() { Id = String.Format("0x{0:X3}", id.ToString()), Data = tempString.ToString(), Dlc = dlc.ToString() });

                if (frames.Count() > 20)
                    frames.Dequeue();
            }

            return CanOpenStatus.CANOPEN_OK;
        }

        protected void Timer1_Tick(object sender, EventArgs e)
        {
            Label1.Text = "Panel refreshed at: " +
            DateTime.Now.ToLongTimeString();


            lblOperationalState.Text = String.Format("Node operational state: {0}", s_state.ToString());

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


            Console.Beep(100, 500);


            CanOpenStatus stat;

            if (client_SDO == null || nmt_Master == null || can_monitor == null)
            {

                client_SDO = new ClientSDO_NET();
                nmt_Master = new NMT_Master_NET();
                can_monitor = new CanMonitor_NET();
                server_SDO = new ServerSDO_NET();
                client_SDO_local = new ClientSDO_NET();

                stat = can_monitor.canHardwareConnect(0, 125000);
                if (stat != CanOpenStatus.CANOPEN_OK)
                {
                }

                stat = can_monitor.canDispatcherPerformance(5, 5);
                if (stat != CanOpenStatus.CANOPEN_OK)
                {
                }

                stat = server_SDO.canHardwareConnect(0, 125000);
                if (stat != CanOpenStatus.CANOPEN_OK)
                {
                }

                stat = server_SDO.nodeSetId(5);
                if (stat != CanOpenStatus.CANOPEN_OK)
                {
                }

                server_SDO.registerObjectReadCallback(new SrvReadDelegate(canopenReadCallback), (Object)server_SDO);
                server_SDO.registerObjectWriteCallback(new SrvWriteDelegate(canopenWriteCallback), (Object)server_SDO);
                server_SDO.registerObjectGetAttributesCallback(new SrvGetAttrDelegate(getAttributesCallback), (Object)server_SDO);

                can_monitor.registerCanReceiveCallback((Object)this, new CanReceiveDelegate(canReceiveCallback));

                stat = client_SDO.canHardwareConnect(0, 125000);
                if (stat != CanOpenStatus.CANOPEN_OK)
                {
                }
                stat = client_SDO.connect(3);

                stat = client_SDO_local.canHardwareConnect(0, 125000);
                if (stat != CanOpenStatus.CANOPEN_OK)
                {
                }
                stat = client_SDO_local.connect(5);

                nmt_Master.registerNodeStateCallback(new NMTOperationalStateDelegate(nmtCallback), this);

                stat = nmt_Master.canHardwareConnect(0, 125000);
                if (stat != CanOpenStatus.CANOPEN_OK)
                {
                }

                CanGrid.DataSource = frames;

            }

            
        }

        protected void btnStartNodeGuardNode3_Click(object sender, EventArgs e)
        {
            lock (nmt_Master)
            {
                nmt_Master.nodeGuardPollStart(3, 1000);
            }
        }



        protected void btnStopNodeGuardNode3_Click(object sender, EventArgs e)
        {
            lock (nmt_Master)
            {
                nmt_Master.nodeGuardPollStop(3);
            }
        }

        protected void btnReadDeviceName_Click(object sender, EventArgs e)
        {
            lock (client_SDO)
            {
                byte[] receive_buffer = new byte[100];
                UInt32 error_code;
                uint valid;
                client_SDO.objectRead(0x1008, 0, receive_buffer, out valid, out error_code);

                string temp_str = "Device name: ";
                for (int i = 0; i < valid; i++)
                {
                    temp_str += Convert.ToChar(receive_buffer[i]);
                }

                lblDeviceName.Text = temp_str;

            }
        }

        protected void btnStartHeartBeatNode3_Click(object sender, EventArgs e)
        {
            uint error_code;
            lock (client_SDO)
            {
                client_SDO.objectWrite(0x1017, 0, (ushort)1000, out error_code);
            }
        }

        protected void btnStopHeartBeatNode3_Click(object sender, EventArgs e)
        {
            uint error_code;
            lock (client_SDO)
            {
                client_SDO.objectWrite(0x1017, 0, (ushort)0, out error_code);
            }
        }

        protected void btnResetNode3_Click(object sender, EventArgs e)
        {
            lock (nmt_Master)
            {
                nmt_Master.nodeReset(3);
            }
        }

        protected void btnStartNode3_Click(object sender, EventArgs e)
        {
            nmt_Master.nodeStart(3);
        }

        protected void btnStopNode3_Click(object sender, EventArgs e)
        {
            nmt_Master.nodeStop(3);
        }

        protected void btnResetCommunicationNode3_Click(object sender, EventArgs e)
        {
            nmt_Master.nodeResetCommunication(3);
        }

        protected void btnCfgNodeGuardNode3_Click(object sender, EventArgs e)
        {
            uint error_code;
            lock (client_SDO)
            {
                client_SDO.connect(3);
                client_SDO.objectWrite(0x100c, 0, (ushort)1000, out error_code);
                client_SDO.objectWrite(0x100d, 0, (byte)3, out error_code);
            }
        }

        protected void btnCfgNodeGuardNode7_Click(object sender, EventArgs e)
        {
            uint error_code;
            lock (client_SDO)
            {
                client_SDO.connect(7);
                client_SDO.objectWrite(0x100c, 0, (ushort)1000, out error_code);
                client_SDO.objectWrite(0x100d, 0, (byte)3, out error_code);
            }
        }

        protected void btnEnterPreopNode3_Click(object sender, EventArgs e)
        {
            lock (nmt_Master)
            {
                nmt_Master.nodePreoperational(3);
            }
        }

        protected void btnSetTransmitPdo1Timer_Click(object sender, EventArgs e)
        {
            lock (client_SDO)
            {
                uint error_code;
                client_SDO.objectWrite(0x1800, 5, (ushort)1000, out error_code);
            }

        }

        protected void btnEnableTPDO1Node3_Click(object sender, EventArgs e)
        {
            lock (client_SDO)
            {
                uint error_code;
                client_SDO.objectWrite(0x1800, 1, 0x280, out error_code);
            }
        }

        protected void btnReadSoftwareVersion_Click(object sender, EventArgs e)
        {
            lock (client_SDO)
            {
                byte[] receive_buffer = new byte[100];
                UInt32 error_code;
                uint valid;
                client_SDO.objectRead(0x100a, 0, receive_buffer, out valid, out error_code);

                string temp_str = "Software version: ";
                for (int i = 0; i < valid; i++)
                {
                    temp_str += Convert.ToChar(receive_buffer[i]);
                }

                lblSoftwareVersion.Text = temp_str;
            }
        }


        static CanOpenStatus getAttributesCallback(object anyObject, ushort object_index, byte sub_index, out ushort flags)
        {
            flags = 0; // Nor R/W-accessable.
            switch (object_index)
            {
                case 0xA:
                    if (sub_index == 0xA)
                        flags = OBJECT_ATTRIBUTES_NET.OBJECT_READABLE;
                    break;
                case 0xB0:
                    if (sub_index == 0xB0)
                        flags = OBJECT_ATTRIBUTES_NET.OBJECT_READABLE;
                    break;
                case 0xC0:
                    if (sub_index == 0xC0)
                        flags = OBJECT_ATTRIBUTES_NET.OBJECT_READABLE;
                    break;
                case 0xD0:
                    if (sub_index == 0xD0)
                        flags = OBJECT_ATTRIBUTES_NET.OBJECT_READABLE;
                    break;
                case 0xE0:
                    if (sub_index == 0x00)
                        flags = OBJECT_ATTRIBUTES_NET.OBJECT_WRITEABLE;
                    break;
                case 0x8000:
                    flags = OBJECT_ATTRIBUTES_NET.OBJECT_WRITEABLE;
                    break;
            }

            return CanOpenStatus.CANOPEN_OK;
        }



        static CanOpenStatus canopenReadCallback(object anyObject, ushort objectIndex, byte subIndex, byte[] data, out uint valid, out uint coErrorCode)
        {
            Console.BackgroundColor = ConsoleColor.Cyan;
            Console.WriteLine("CALLBACK: Read from Server SDO ObjIdx: {0:X2}, SubIdx: {1:X2} request!", objectIndex, subIndex);
            Console.BackgroundColor = ConsoleColor.Black;

            Console.Beep();
            valid = 0;
            coErrorCode = 0;

            if (objectIndex == 0xb0 && subIndex == 0xb0)
            {
                valid = 40;

                for (byte i = 0; i < valid; i++)
                    data[i] = i;

                coErrorCode = 0;

                return CanOpenStatus.CANOPEN_OK;
            }

            if (objectIndex == 0xc0 && subIndex == 0xc0)
            {
                valid = 1024;

                for (int i = 0; i < valid; i++)
                    data[i] = (byte)i;

                coErrorCode = 0;

                return CanOpenStatus.CANOPEN_OK;
            }

            // Expedited response on Object 10, sub 10.
            if (objectIndex == 10 && subIndex == 10)
            {
                valid = 4;
                data[0] = 0x10;
                data[1] = 0x20;
                data[2] = 0x30;
                data[3] = 0x40;
                coErrorCode = 0;
                return CanOpenStatus.CANOPEN_OK;
            }
            return CanOpenStatus.CANOPEN_OK;

        }


        static CanOpenStatus canopenWriteCallback(object anyObject, ushort objectIndex, byte subIndex, byte[] data, uint valid, out uint coErrorCode)
        {
            Console.BackgroundColor = ConsoleColor.Yellow;
            Console.WriteLine("SDO SERVER CALLBACK: Write to Server SDO ObjIdx: {0:X2}, SubIdx: {1:X2} request!", objectIndex, subIndex);
            Console.BackgroundColor = ConsoleColor.Black;


            string objectData = String.Empty;
            for (int i = 0; i < valid; i++)
            {
                objectData += String.Format("0x{0:X2} ", data[i]);
            }
            Console.WriteLine(objectData);
            coErrorCode = 0;

            return CanOpenStatus.CANOPEN_OK;
        }

        protected void DemoSegmentWrite_Click(object sender, EventArgs e)
        {
            byte[] tx_buffer = new byte[20];

            for (int i = 0; i < tx_buffer.Length; i++)
                tx_buffer[i] = (byte)i;

            uint error_code;
            client_SDO_local.objectWrite(0x8000, 0x1, tx_buffer, (uint)tx_buffer.Length, out error_code);

        }

        protected void btnStartNodeGuardNode7_Click(object sender, EventArgs e)
        {
            lock (nmt_Master)
            {
                nmt_Master.nodeGuardPollStart(7, 1000);
            }
        }

        protected void stopNodeGuardNode7_Click(object sender, EventArgs e)
        {
            lock (nmt_Master)
            {
                nmt_Master.nodeGuardPollStop(7);
            }
        }
    }
}