using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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

        byte[] output_mirror_values;

        public IO_401()
        {

            digital_outputs = 0;
            digital_inputs = 0;

            analogOutputs = 0;
            analogInputs = 0;

            output_mirror_values = new byte[8];

            for (int i = 0; i < output_mirror_values.Length; i++)
            {
                output_mirror_values[i] = 0;
            }

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

        private CanOpenStatus configureIoOutputs()
        {
            byte io_blocks;
            uint canopenErrorCode;
            CanOpenStatus ret;

            // Read number of I/O-outputs.
            ret = client_sdo.objectRead(0x2100, 0, out io_blocks, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            this.digital_outputs = io_blocks * 8;

            // Disable all I/O outputs.
            ret = client_sdo.objectWrite(0x1600, 0, 0, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            // Map output blocks. Signals are located on object index 6200 with subindex 1 -> n.
            for (int i = 0; i < io_blocks; i++)
            {
                UInt32 val = (UInt32)(0x62000008 + 0x100 * io_blocks);
                ret = client_sdo.objectWrite(0x1600, (byte)(1 + io_blocks), val, out canopenErrorCode);
                if (ret != CanOpenStatus.CANOPEN_OK)
                    return ret;
            }

            // Enable all I/O outputs.
            ret = client_sdo.objectWrite(0x1600, 0, io_blocks, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            UInt32 cobid;

            cobid = (UInt32)(0x00000200 + (node_id));

            if (io_blocks == 0)
                cobid = cobid | 0x80000000; // Disable PDO.

            // Set COBID for RPDO.
            ret = client_sdo.objectWrite(0x1400, 1, cobid, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            // Set transmission type to 254.
            ret = client_sdo.objectWrite(0x1400, 2, (byte)(254), out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            // Start node.
            ret = nmt_mlaster.nodeStart(node_id);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            return CanOpenStatus.CANOPEN_OK;
        }

        private CanOpenStatus configureIoInputs()
        {
            byte io_blocks;
            uint canopenErrorCode;
            CanOpenStatus ret;

            ret = client_sdo.objectRead(0x2000, 0, out io_blocks, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            this.digital_inputs = io_blocks * 8;

            // Disable all I/O inputs.
            ret = client_sdo.objectWrite(0x1A00, 0, 0, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            // Map input blocks. Signals are located on object index 6000 with subindex 1 -> n which means
            // represented as Uint32: 0x60000108 -> 0x60000n08
            for (int i = 0; i < io_blocks; i++)
            {
                UInt32 val = (UInt32)(0x60000008 + 0x100 * io_blocks);
                ret = client_sdo.objectWrite(0x1A00, (byte)(1 + i), val, out canopenErrorCode);
                if (ret != CanOpenStatus.CANOPEN_OK)
                    return ret;
            }

            // Enable all I/O inputs.
            ret = client_sdo.objectWrite(0x1A00, 0, io_blocks, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;


            UInt32 cobid = (UInt32)(0x00000180 + (node_id));

            if (io_blocks == 0)
                cobid = cobid | 0x80000000; // Disable PDO.

            // Set COBID for TPDO to 0x180 + node-id.
            ret = client_sdo.objectWrite(0x1800, 1, cobid, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            // Set transmission type to 254 for TPDO 
            ret = client_sdo.objectWrite(0x1800, 2, (byte)(254), out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            // Set periodic tranmission to 500ms for TPDO.
            if (io_blocks > 0)
            {
                ret = client_sdo.objectWrite(0x1800, 5, (UInt16)(500), out canopenErrorCode);
                if (ret != CanOpenStatus.CANOPEN_OK)
                    return ret;
            }

            return CanOpenStatus.CANOPEN_OK;
        }

        public CanOpenStatus setIO(int index, bool value)
        {
            byte ioByte = output_mirror_values[index / 8];

            byte bit = 0x1;
            int shifts = index % 8;
            ioByte = (byte)(bit << (byte)shifts);

            if (value == true)
            {
                output_mirror_values[index / 8] |= ioByte;
            }
            else
            {
                byte test = (byte)~ioByte;
                output_mirror_values[index / 8] &= (byte)~ioByte;
            }

            transmit_pdo.setup((uint)0x200 + this.node_id, this.output_mirror_values, (byte)(this.digital_outputs/8));
            return transmit_pdo.transmit();
        }

        public CanOpenStatus startupNode(byte nodeId)
        {
            CanOpenStatus ret = CanOpenStatus.CANOPEN_ERROR;

            this.node_id = nodeId;

            ret = client_sdo.connect(nodeId);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            nmt_mlaster.nodeGuardPollStart(nodeId, 3000);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = receive_pdo.setCobid((uint)0x180 + this.node_id);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = transmit_pdo.setup((uint)0x200 + this.node_id, this.output_mirror_values, 1);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = transmit_pdo.periodicTransmission(true);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = nmt_mlaster.nodeReset(node_id);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            Thread.Sleep(4000);

            ret = configureIoInputs();
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = configureIoOutputs();
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            uint canopenErrorCode;
            ret = client_sdo.objectWrite(0x100c, 0, (ushort)1000, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            ret = client_sdo.objectWrite(0x100d, 0, (byte)3, out canopenErrorCode);
            if (ret != CanOpenStatus.CANOPEN_OK)
                return ret;

            return ret;

        }
    }
}
