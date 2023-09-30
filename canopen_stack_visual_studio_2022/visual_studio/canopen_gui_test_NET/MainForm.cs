/*             _____        _        _      _       _    
              |  __ \      | |      | |    (_)     | |   
              | |  | | __ _| |_ __ _| |     _ _ __ | | __
              | |  | |/ _` | __/ _` | |    | | '_ \| |/ /
              | |__| | (_| | || (_| | |____| | | | |   < 
              |_____/ \__,_|\__\__,_|______|_|_| |_|_|\_\
         ______             _                      _             
        |  ____|           (_)                    (_)            
        | |__   _ __   __ _ _ _ __   ___  ___ _ __ _ _ __   __ _ 
        |  __| | '_ \ / _` | | '_ \ / _ \/ _ \ '__| | '_ \ / _` |
        | |____| | | | (_| | | | | |  __/  __/ |  | | | | | (_| |
        |______|_| |_|\__, |_|_| |_|\___|\___|_|  |_|_| |_|\__, |
                       __/ |                                __/ |
                      |___/                                |___/ 

      Web: http://www.datalink.se E-mail: ulrik.hagstrom@datalink.se

    *******************************************************************
    *    CANopen API (C++/C#) distributed by Datalink Enginnering.    *
    *             Copyright (C) 2009-2013 Ulrik Hagström.             *
    *******************************************************************
*/


using DatalinkEngineering.CANopen;
using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace CANopenDiagnostic
{
  public partial class MainForm : Form
  {
    private ClientSDO_NET client_SDO;
    private ServerSDO_NET server_SDO;
    private NMT_Master_NET nmt_Master;
    private NMT_Slave_NET nmt_slave;
    private CanMonitor_NET can_monitor;
    private ReceivePDO_NET receive_PDO;
    private TransmitPDO_NET transmit_PDO;
    private SyncProducer_NET sync_producer;

    LogMessages log;

    public MainForm()
    {
      InitializeComponent();

      MainForm.CheckForIllegalCrossThreadCalls = true;

      //
      log = new LogMessages();
      log.Log += new LogMessages.LogHandler(consolePrint);

      client_SDO = new ClientSDO_NET();
      server_SDO = new ServerSDO_NET();
      nmt_Master = new NMT_Master_NET();
      nmt_slave = new NMT_Slave_NET();
      can_monitor = new CanMonitor_NET();
      receive_PDO = new ReceivePDO_NET();
      transmit_PDO = new TransmitPDO_NET();
      sync_producer = new SyncProducer_NET();


      System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
      byte[] file = encoding.GetBytes("C:\\dev\\closed_rep\\trunk\\canopen_stack_and_tools\\visual_studio\\canopenDLL\\Debug\\canopen_lic.txt\0");
      byte[] pass = encoding.GetBytes("Ulrik\0");


      //can_interface.unlockCanopenLibrary(file, pass);

      CanOpenStatus can_monitor_stat;

      can_monitor_stat = can_monitor.registerCanReceiveCallback((Object)this, new CanReceiveDelegate(canReceiveCallback));
      if (can_monitor_stat == CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Added callback OK!");
      }
      else
      {
        log.OnLog("Added callback ERROR!");
      }



      CanOpenStatus stat;
      stat = nmt_Master.registerNodeStateCallback(new NMTOperationalStateDelegate(node_state_callback), (Object)this);
      if (stat == CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Added callback OK!");
      }
      else
      {
        log.OnLog("Added callback ERROR!");
      }


    }

    ~MainForm()
    {

      log.Log -= new LogMessages.LogHandler(consolePrint);

      client_SDO.Dispose();
      client_SDO = null;

      server_SDO.Dispose();
      server_SDO = null;

      nmt_Master.Dispose();
      nmt_Master = null;

      nmt_slave.Dispose();
      nmt_slave = null;

      receive_PDO.Dispose();
      receive_PDO = null;

      transmit_PDO.Dispose();
      transmit_PDO = null;

      sync_producer.Dispose();
      sync_producer = null;
    }

    private delegate void consolePrintDelegate(string message);
    public void consolePrint(string message)
    {
      if (InvokeRequired)
      {
        consolePrintDelegate d = new consolePrintDelegate(consolePrint);
        BeginInvoke(d, new object[] { message });
        return;
      }
      try
      {
        if (consoleTextBox.TextLength < 10000)
        {
          consoleTextBox.AppendText(message + "\n");
          consoleTextBox.SelectionStart = consoleTextBox.TextLength;
          consoleTextBox.ScrollToCaret();
        }
        else
        {
          consoleTextBox.Text = "CLEARED";
        }
      }
      catch
      {
        ;
      }
    }

    private delegate void canTracePrintDelegate(string message);
    public void canTracePrint(string message)
    {
      if (InvokeRequired)
      {
        canTracePrintDelegate d = new canTracePrintDelegate(canTracePrint);
        BeginInvoke(d, new object[] { message });
        return;
      }
      try
      {
        rich_text_can_trace.AppendText(message + "\n");
        if (rich_text_can_trace.TextLength < 10000)
        {
          rich_text_can_trace.SelectionStart = rich_text_can_trace.TextLength;
          rich_text_can_trace.ScrollToCaret();
        }
        else
        {
          rich_text_can_trace.Text = "CLEARED!\n";
        }
      }
      catch
      {
        ;
      }
    }

    private void btnConnect_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      int can_bitrate;
      int can_port;

      can_bitrate = Convert.ToInt32(comboBitrateCAN.Text);
      can_port = Convert.ToInt32(comboChannelCAN.Text);


      stat = can_monitor.canHardwareConnect(can_port, can_bitrate);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error connecting CAN Monitor to CAN hardware!");
      }
      else
      {
        log.OnLog("Successful connect CAN monitor to CAN hardware!");
      }


      stat = client_SDO.canHardwareConnect(can_port, can_bitrate);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error connecting Client SDO to CAN hardware!");
      }
      else
      {
        client_SDO.connect((byte)this.numRemoteNode.Value);
        log.OnLog("Successful connect Client SDO to CAN hardware!");
      }


      stat = nmt_Master.canHardwareConnect(can_port, can_bitrate);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error connecting Network Master to CAN hardware!");
      }
      else
      {
        log.OnLog("Successful connect MNT Master to CAN hardware!");
      }


      stat = receive_PDO.canHardwareConnect(can_port, can_bitrate);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error connecting RX PDO to CAN hardware!");
      }
      else
      {
        log.OnLog("Successful connect RX PDO to CAN hardware!");
      }

      stat = transmit_PDO.canHardwareConnect(can_port, can_bitrate);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error connecting TX PDO to CAN hardware!");
      }
      else
      {
        log.OnLog("Successful connect TX PDO to CAN hardware!");
      }

      stat = sync_producer.canHardwareConnect(can_port, can_bitrate);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error connecting sync producer to CAN hardware!");
      }
      else
      {
        log.OnLog("Successful connect sync producer to CAN hardware!");
      }


    }

    private void btnDisconnect_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = client_SDO.canHardwareDisconnect();
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error disconnecting Client SDO from CAN hardware");
      }
      else
      {
        log.OnLog("Successful disconnect Client SDO!");
      }
      stat = nmt_Master.canHardwareDisconnect();
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Error disconnecting NMT Master from CAN hardware");
      }
      else
      {
        log.OnLog("Successful disconnect NMT Master!");
      }

      stat = can_monitor.canHardwareDisconnect();
      if (stat == CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Success disconnecting CAN Monitor from CAN hardware!");
      }
      else
      {
        log.OnLog("Error disconnecting CAN monitor from CAN hardware!");
      }

      stat = receive_PDO.canHardwareDisconnect();
      if (stat == CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Success disconnecting RX PDO from CAN hardware!");
      }
      else
      {
        log.OnLog("Error disconnecting RX PDO from CAN hardware!");
      }

      stat = transmit_PDO.canHardwareDisconnect();
      if (stat == CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Success disconnecting TX PDO from CAN hardware!");
      }
      else
      {
        log.OnLog("Error disconnecting TX PDO from CAN hardware!");
      }

      stat = sync_producer.canHardwareDisconnect();
      if (stat == CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog("Success disconnecting SYNC PRODUCER from CAN hardware!");
      }
      else
      {
        log.OnLog("Error disconnecting SYNC PRODUCER from CAN hardware!");
      }
    }

    static DateTime _callbackLastMessageTime;
    static long _callbackCountOfMissedMessages;
    static CanOpenStatus canReceiveCallback(object obj, uint id, byte[] data, byte dlc, uint flags)
    {
      MainForm form = (MainForm)obj;

      // prevent more then 10 messages per 0.5s
      const double maxMessagesIntervalInSeconds = 0.5;
      const int maxMessagesPerInterval = 10;
      DateTime now = DateTime.UtcNow;
      string countContinuation = string.Empty;
      if (_callbackLastMessageTime.AddSeconds(maxMessagesIntervalInSeconds) < now)
      {
        if (_callbackCountOfMissedMessages >= maxMessagesPerInterval)
        {
          countContinuation = string.Format("... count {0} callbacks within {1}\r\n",
              _callbackCountOfMissedMessages, TimeSpan.FromSeconds(maxMessagesIntervalInSeconds));
        }
        _callbackLastMessageTime = now;
        _callbackCountOfMissedMessages = 0;
      }
      else
      {
        System.Threading.Interlocked.Increment(ref _callbackCountOfMissedMessages);
      }
      // printing will not overload / freeze GUI
      if (_callbackCountOfMissedMessages < maxMessagesPerInterval)
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

        // "yyyy.MM.dd HH:mm:ss fffff" "HH:mm:ss fffff"
        form.canTracePrint(string.Format("{0}{1}: {2}", countContinuation, now.ToLocalTime().ToString("HH:mm:ss fffff"), tempString));
      }
      return CanOpenStatus.CANOPEN_OK;
    }


    static CanOpenStatus node_state_callback(object any, byte node_id, byte state)
    {
      MainForm form = (MainForm)any;
      form.consolePrint(String.Format("Node State result : node_id: {0}, state: {1}", node_id, state));
      return CanOpenStatus.CANOPEN_OK;
    }

    private void btnStartNodeGuard_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.nodeGuardPollStart((byte)this.numRemoteNode.Value, 3000);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("Hardware not connected!");
        }
        else
        {
          log.OnLog(String.Format("Could not start nodeguarding for node {0}",
              this.numRemoteNode.Value.ToString()));
        }

      }
      else
      {
        log.OnLog(String.Format("Nodeguard started successfully for node {0}",
            this.numRemoteNode.Value.ToString()));

      }
    }

    private void btnStopNodeGuard_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.nodeGuardPollStop((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not stop nodeguarding for node {0}",
            this.numRemoteNode.Value.ToString()));

      }
      else
      {
        log.OnLog(String.Format("Nodeguard stopped successfully for node {0}",
            this.numRemoteNode.Value.ToString()));

      }
    }


    private void btnStartHeartbeatMonitor_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.heartbeatMonitorStart((byte)this.numRemoteNode.Value, 3000);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not start heartbeat monitoring for node {0}",
            this.numRemoteNode.Value.ToString()));
      }
      else
      {
        log.OnLog(String.Format("heartbeat monitoring started successfully for node {0}",
            this.numRemoteNode.Value.ToString()));
      }
    }

    private void btnStartNode_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.nodeStart((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("Hardware not connected!");
        }
        else
        {
          log.OnLog(String.Format("Could not send start node command to node {0}",
              this.numRemoteNode.Value.ToString()));
        }
      }
      else
      {
        log.OnLog(String.Format("Start node command sent successfully to node {0}",
            this.numRemoteNode.Value.ToString()));

      }
    }

    private void btnStopNode_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.nodeStop((byte)this.numRemoteNode.Value);

      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("Hardware not connected!");
        }
        else
        {
          log.OnLog(String.Format("Could not send stop node command to node {0}",
              this.numRemoteNode.Value.ToString()));
        }
      }
      else
      {
        log.OnLog(String.Format("Stop node command sent successfully to node {0}",
            this.numRemoteNode.Value.ToString()));

      }
    }

    private void btnEnterPreOperational_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.nodePreoperational((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("Hardware not connected!");
        }
        else
        {
          log.OnLog(String.Format("Could not send enter preoperational command to node {0}",
              this.numRemoteNode.Value.ToString()));
        }

      }
      else
      {
        log.OnLog(String.Format("Enter preoperationalcommand sent successfully to node {0}",
            this.numRemoteNode.Value.ToString()));
      }
    }

    private void btnResetComm_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.nodeResetCommunication((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("Hardware not connected!");
        }
        else
        {
          log.OnLog(String.Format("Could not send reset communication command to node {0}",
          this.numRemoteNode.Value.ToString()));
        }
      }
      else
      {
        log.OnLog(String.Format("Reset communication command successfully sent to node {0}",
            this.numRemoteNode.Value.ToString()));
      }
    }

    private void button1_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      stat = nmt_Master.nodeReset((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("Hardware not connected!");
        }
        else
        {
          log.OnLog(String.Format("Could not send reset node command to node {0}",
          this.numRemoteNode.Value.ToString()));
        }

      }
      else
      {
        log.OnLog(String.Format("Reset node command successfully sent to node {0}",
            this.numRemoteNode.Value.ToString()));
      }
    }



    private void readVisibleString(/*byte remotenode_id,*/ ushort object_index, byte sub_index, string parameterName, bool readable)
    {
      CanOpenStatus stat;
      byte[] receive_buffer = new byte[100];
      UInt32 error_code;
      uint valid;


      stat = client_SDO.objectRead(object_index, sub_index, receive_buffer, out valid, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol read timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not read from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        printReadResult(object_index, sub_index, parameterName, receive_buffer, (ushort)valid, readable);
      }
    }

    private void readUint32(byte remotenode_id, ushort object_index, byte sub_index, string parameterName)
    {
      CanOpenStatus stat;
      uint value;
      UInt32 error_code;

      stat = client_SDO.connect(remotenode_id);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            remotenode_id.ToString()));
      }

      stat = client_SDO.objectRead(object_index, sub_index, out value, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol read timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not read from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        printReadResult(object_index, sub_index, parameterName, value);
      }

    }

    private void printReadResult(ushort object_index, byte sub_index, string parameterName, uint value)
    {
      string total_string = ""; ;
      string temp_str = "";

      temp_str = String.Format("{0}: [0x{1:x4}, 0x{2:x2}]:\n", parameterName, object_index, sub_index);
      total_string += temp_str;
      temp_str = "Hex: 0x";
      temp_str += String.Format("{0:x8} ", value);
      total_string += temp_str + "\n";
      log.OnLog(total_string);
    }


    private void printReadResult(ushort object_index, byte sub_index, string parameterName, byte[] data, ushort valid, bool readable)
    {
      string total_string = ""; ;
      string temp_str = "";

      temp_str = String.Format("{0}: [0x{1:x4}, 0x{2:x2}]:\n", parameterName, object_index, sub_index);
      total_string += temp_str;

      if (readable)
      {
        temp_str = "String: ";
        for (int i = 0; i < valid; i++)
        {
          temp_str += Convert.ToChar(data[i]);
        }
        total_string += temp_str;
      }
      log.OnLog(total_string);
      total_string = "";
      temp_str = "Hex: ";
      for (int i = 0; i < valid; i++)
      {
        temp_str += String.Format("{0:x2} ", data[i]);
      }
      total_string += temp_str + "\n";
      log.OnLog(total_string);
    }

    private void btnDeviceType_Click(object sender, EventArgs e)
    {
      readUint32((byte)this.numRemoteNode.Value, 0x1000, 0, "Device Type");
    }

    private void btnHardwareVersion_Click(object sender, EventArgs e)
    {
      readVisibleString(0x1009, 0, "Hardware version", true);
    }

    private void btnSoftwareVersion_Click(object sender, EventArgs e)
    {
      readVisibleString(0x100a, 0, "Software version", true);
    }

    private void btnDeviceName_Click(object sender, EventArgs e)
    {
      readVisibleString(0x1008, 0, "Device Name", true);
    }

    private void btnErrorRegister_Click(object sender, EventArgs e)
    {
      readUint32((byte)this.numRemoteNode.Value, 0x1001, 0, "Error Register");

    }

    private void btnManfStatReg_Click(object sender, EventArgs e)
    {
      readUint32((byte)this.numRemoteNode.Value, 0x1002, 0, "Manufacturer status register");
    }

    private void btnReadPredefErrField_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint registred_errors;
      uint registred_error_code;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      stat = client_SDO.objectRead(0x1003, 0, out registred_errors, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol read timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not read from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }

        }
      }
      else
      {
        log.OnLog(String.Format("Found {0} registred errors", registred_errors));
        for (int i = 0; i < registred_errors; i++)
        {
          stat = client_SDO.objectRead(0x1003, (byte)(i + 1), out registred_error_code, out error_code);
          if (stat != CanOpenStatus.CANOPEN_OK)
          {
            log.OnLog(String.Format("Could not read from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
          else
          {
            log.OnLog(String.Format("Stored error [{0}] : 0x{1:x8}", i, registred_error_code));
          }
        }
      }
    }

    private void btnClearErrors_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;

      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      stat = client_SDO.objectWrite(0x1003, 0, (byte)0, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Errors cleared!");
      }

    }

    private void linkClick_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
    {
      //System.Diagnostics.Process.Start("IExplore", " http://www.datalink.se");
    }

    private void numRemoteNode_ValueChanged(object sender, EventArgs e)
    {
      client_SDO.connect((byte)this.numRemoteNode.Value);
    }

    void printError(CanOpenStatus library_error_code, uint node_error_code)
    {
      if (library_error_code == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
      {
        log.OnLog("CAN hardware not connected!");
      }
      else
      {
        if (library_error_code == CanOpenStatus.CANOPEN_TIMEOUT)
        {
          log.OnLog(String.Format("Protocol read timeout from node {0}!",
              this.numRemoteNode.Value.ToString()));
        }
        else
        {
          log.OnLog(String.Format("Could not read from node {0}, CANopen error: 0x{1:x8}",
              this.numRemoteNode.Value.ToString(), node_error_code));
        }

      }
    }

    private void btnReadPDO_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint pdo_data;
      uint registred_error_code;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1400;
      else
        object_dictionary_offset = 0x1800;



      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      // Read COBID
      stat = client_SDO.objectRead(obj_idx, 1, out pdo_data, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        printError(stat, error_code);
      }
      else
      {
        log.OnLog(String.Format("PDO COBID: {0:x8} ", pdo_data));
      }

      // Read Transmission type
      stat = client_SDO.objectRead(obj_idx, 2, out pdo_data, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        printError(stat, error_code);
      }
      else
      {
        log.OnLog(String.Format("PDO Transmission Type: {0:x8} ", pdo_data));
      }

      // Read Transmission type
      stat = client_SDO.objectRead(obj_idx, 3, out pdo_data, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        printError(stat, error_code);
      }
      else
      {
        log.OnLog(String.Format("PDO inhibit time: {0:x8} ", pdo_data));
      }

      // Event timer
      stat = client_SDO.objectRead(obj_idx, 5, out pdo_data, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        printError(stat, error_code);
      }
      else
      {
        log.OnLog(String.Format("PDO event timer: {0:x8} ", pdo_data));
      }

    }

    private void btnEnablePdo_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1400;
      else
        object_dictionary_offset = 0x1800;



      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      if (cbPdoRx.Checked)
        stat = client_SDO.objectWrite(obj_idx, 1, (uint)numRemoteNode.Value +
            (uint)(0x200 + 0x100 * (numUpDownPdo.Value - 1)), out error_code);
      else
        stat = client_SDO.objectWrite(obj_idx, 1, (uint)numRemoteNode.Value +
            (uint)(0x180 + 0x100 * (numUpDownPdo.Value - 1)), out error_code);



      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Transmit PDO enabled (with default COBID)");
      }

    }

    private void btnSetEventTimer_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1400;
      else
        object_dictionary_offset = 0x1800;



      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      stat = client_SDO.objectWrite(obj_idx, 5, (ushort)500, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Event timer set!");
      }

    }

    private void btnTransmType254_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1400;
      else
        object_dictionary_offset = 0x1800;



      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      stat = client_SDO.objectWrite(obj_idx, 2, (byte)254, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Transmission type 254 set!");
      }

    }

    private void btnReadMappedObjects_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint pdo_data;
      uint registred_error_code;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1600;
      else
        object_dictionary_offset = 0x1a00;

      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      // Read no mapped obejcts
      stat = client_SDO.objectRead(obj_idx, 0, out pdo_data, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        printError(stat, error_code);
      }
      else
      {
        log.OnLog(String.Format("Mapped objects: {0:x8} ", pdo_data));
      }

      for (byte i = 1; i < 8; i++)
      {
        stat = client_SDO.objectRead(obj_idx, i, out pdo_data, out error_code);
        if (stat != CanOpenStatus.CANOPEN_OK)
        {
          printError(stat, error_code);
        }
        else
        {
          log.OnLog(String.Format("Default map object {0}: {1:x8} ", i, pdo_data));
        }
      }

    }

    private void btnMap8bits_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1600;
      else
        object_dictionary_offset = 0x1a00;



      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      stat = client_SDO.objectWrite(obj_idx, 0, 1, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not map 64 bits on node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("8 bits mapped!");
      }

    }

    private void btnWritePDO_Click(object sender, EventArgs e)
    {

    }

    private void btnMap32bits_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1600;
      else
        object_dictionary_offset = 0x1a00;



      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      stat = client_SDO.objectWrite(obj_idx, 0, 4, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("ould not map 64 bits on node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("32 bits mapped!");
      }


    }

    private void btnMap64bits_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1600;
      else
        object_dictionary_offset = 0x1a00;

      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      stat = client_SDO.objectWrite(obj_idx, 0, 8, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not map 64 bits on node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("32 bits mapped!");
      }
    }

    private void btnRequestPdo_Click(object sender, EventArgs e)
    {
      receive_PDO.setCobid((uint)numRemoteNode.Value +
          (uint)(0x180 + 0x100 * (numUpDownPdo.Value - 1)));
      receive_PDO.requestPDO(1);
    }

    private void btnConfigNodeGuard_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }


      stat = client_SDO.objectWrite(0x100c, 0, (ushort)1000, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Nodeguard configured!");
      }

      stat = client_SDO.objectWrite(0x100d, 0, (byte)3, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Lifetime factor configured!");
      }

    }

    private void btnConfHeartBeat_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      stat = client_SDO.objectWrite(0x1017, 0, (ushort)1000, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Heartbeat configured!");
      }

    }

    private void btnSendPdo_Click(object sender, EventArgs e)
    {
      transmit_PDO.setup((uint)0x200 + Convert.ToUInt32(numRemoteNode.Value), new byte[] { 0xFF }, 1);
      transmit_PDO.transmit();
    }

    private void btnSendPdo50_Click(object sender, EventArgs e)
    {
      transmit_PDO.setup((uint)0x200 + Convert.ToUInt32(numRemoteNode.Value), new byte[] { 0xAA }, 1);
      transmit_PDO.transmit();
    }

    private void btnSendPdoClearAll_Click(object sender, EventArgs e)
    {
      transmit_PDO.setup((uint)0x200 + Convert.ToUInt32(numRemoteNode.Value), new byte[] { 0x00 }, 1);
      transmit_PDO.transmit();
    }

    private void btnStartSync_Click(object sender, EventArgs e)
    {
      sync_producer.setSyncCOBID(0x80);
      sync_producer.startPeriodicTransmission(true);
    }

    private void button2_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      ushort object_dictionary_offset;
      if (cbPdoRx.Checked)
        object_dictionary_offset = 0x1400;
      else
        object_dictionary_offset = 0x1800;



      ushort obj_idx = (ushort)(numUpDownPdo.Value - 1);
      if (cbPdoRx.Checked)
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);
      else
        obj_idx = (ushort)((ushort)(numUpDownPdo.Value - 1) + object_dictionary_offset);

      stat = client_SDO.objectWrite(obj_idx, 2, (byte)10, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Transmission type 30 set!");
      }

    }

    private void btnEnableSync_Click(object sender, EventArgs e)
    {
      CanOpenStatus stat;
      uint error_code;
      // Read number of stored errors
      stat = client_SDO.connect((byte)this.numRemoteNode.Value);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Could not assign client SDO to comm with node {0}",
            this.numRemoteNode.Value.ToString()));
      }

      stat = client_SDO.objectWrite(0x1005, 0, (uint)0x80, out error_code);
      if (stat != CanOpenStatus.CANOPEN_OK)
      {
        if (stat == CanOpenStatus.CANOPEN_ERROR_HW_NOT_CONNECTED)
        {
          log.OnLog("CAN hardware not connected!");
        }
        else
        {
          if (stat == CanOpenStatus.CANOPEN_TIMEOUT)
          {
            log.OnLog(String.Format("Protocol write timeout from node {0}!",
                this.numRemoteNode.Value.ToString()));
          }
          else
          {
            log.OnLog(String.Format("Could not write from node {0}, CANopen error: 0x{1:x8}",
                this.numRemoteNode.Value.ToString(), error_code));
          }
        }
      }
      else
      {
        log.OnLog("Nodeguard configured!");
      }
    }

    private void button2_Click_1(object sender, EventArgs e)
    {
      openDcfFileDialog.Title = "Open DCF File";
      openDcfFileDialog.Filter = "DCF files|*.dcf";
      openDcfFileDialog.InitialDirectory = Directory.GetCurrentDirectory();
      if (openDcfFileDialog.ShowDialog() == DialogResult.OK)
      {
        MessageBox.Show(openDcfFileDialog.FileName.ToString());
      }

      ushort failingObjIndex;
      byte failingSubIndex;
      uint canopenErrorCode;

      CanOpenStatus ret = client_SDO.sendConfigurationData(openDcfFileDialog.FileName.ToString(), out failingObjIndex, out failingSubIndex, out canopenErrorCode);

      if (ret != CanOpenStatus.CANOPEN_OK)
      {
        log.OnLog(String.Format("Dcf file download failed! (ret = 0x{0}, objIndex = 0x{1:X}, subIndex = 0x{2:X}, Error code:0x{3:X})", ret, failingObjIndex, failingSubIndex, canopenErrorCode));
      }
      else
      {
        log.OnLog(String.Format("Dcf file download success!"));
      }
    }
  }
}
