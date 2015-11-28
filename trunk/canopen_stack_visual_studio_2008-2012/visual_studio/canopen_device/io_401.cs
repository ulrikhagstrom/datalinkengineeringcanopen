using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CanopenDevices
{
    public class IO_401
    {
        ClientSDO_NET client_sdo;
        ReceivePDO_NET receive_pdo;
        TransmitPDO_NET transmit_pdo;
        NMT_Master_NET nmt_mlaster;

        int digital_outputs;
        int digital_inputs;

        int analogOutputs;
        int analogInputs;

        byte node_id;

        public IO_401()
        {

            digital_outputs = 0;
            digital_inputs = 0;

            analogOutputs = 0;
            analogInputs = 0;

            client_sdo = new ClientSDO_NET();
            receive_pdo = new ReceivePDO_NET();
            transmit_pdo = new TransmitPDO_NET();
            nmt_mlaster = new NMT_Master_NET();
        }

        ~IO_401()
        {
            client_sdo.Dispose();
            client_sdo = null;

            receive_pdo.Dispose();
            receive_pdo = null;

            transmit_pdo.Dispose();
            transmit_pdo = null;

            nmt_mlaster.Dispose();
            nmt_mlaster = null;
        }

        public CanOpenStatus canHardwareConnect(int can_port, int can_bitrate)
        {
            CanOpenStatus ret = CanOpenStatus.CANOPEN_ERROR;

            ret = client_sdo.canHardwareConnect(can_port, can_bitrate);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = receive_pdo.canHardwareConnect(can_port, can_bitrate);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = transmit_pdo.canHardwareConnect(can_port, can_bitrate);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = nmt_mlaster.canHardwareConnect(can_port, can_bitrate);
            return ret;
        }

        public CanOpenStatus canHardwareDisconnect()
        {
            CanOpenStatus ret = CanOpenStatus.CANOPEN_ERROR;

            ret = client_sdo.canHardwareDisconnect();
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = receive_pdo.canHardwareDisconnect();
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = transmit_pdo.canHardwareDisconnect();
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = nmt_mlaster.canHardwareDisconnect();
            return ret;
        }

        public void setNodeId(byte nodeId)
        {
            
            node_id = nodeId;
            client_sdo.connect(nodeId);
            nmt_mlaster.nodeGuardPollStart(nodeId,3000);
        }
           


        public CanOpenStatus startupNode()
        {
            CanOpenStatus ret = CanOpenStatus.CANOPEN_ERROR;

            byte io_blocks;
            uint canopenErrorCode;

            // Read number of I/O-inputs.
            ret = client_sdo.objectRead(0x2000, 0, out io_blocks, out canopenErrorCode);
            if (ret == CanOpenStatus.CANOPEN_OK)
            {
                digital_inputs = io_blocks * 8;
            }

            ret = client_sdo.objectWrite(0x1A00, 0, 0, out canopenErrorCode);
            if (ret == CanOpenStatus.CANOPEN_OK)
            {
                for (int i = 0; i < io_blocks; i++)
                {
                    // Map inputs to transmit PDO.

                    UInt32 val = (UInt32)(0x80000008 + 0x100 * io_blocks);

                    ret = client_sdo.objectWrite(0x1A00, (byte)(1 + io_blocks), val, out canopenErrorCode);
                }
            }
            ret = client_sdo.objectWrite(0x1A00, 0, io_blocks, out canopenErrorCode);

            ret = client_sdo.objectWrite(0x1800, 1, (UInt32)(0x00000180 + (node_id)), out canopenErrorCode);

            ret = client_sdo.objectWrite(0x1800, 2, (byte)(254), out canopenErrorCode);

            ret = client_sdo.objectWrite(0x1800, 5, (UInt16)(500), out canopenErrorCode);

            ret = nmt_mlaster.nodeStart(node_id);

            return CanOpenStatus.CANOPEN_ERROR;
        }
    }
}
