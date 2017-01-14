namespace CANopenDiagnostic
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabSettings = new System.Windows.Forms.TabPage();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.grpHardwareSetup = new System.Windows.Forms.GroupBox();
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.comboBitrateCAN = new System.Windows.Forms.ComboBox();
            this.comboChannelCAN = new System.Windows.Forms.ComboBox();
            this.lblBitrateCAN = new System.Windows.Forms.Label();
            this.lblChannelCAN = new System.Windows.Forms.Label();
            this.tabNMT = new System.Windows.Forms.TabPage();
            this.grpModuleControl = new System.Windows.Forms.GroupBox();
            this.button1 = new System.Windows.Forms.Button();
            this.btnResetComm = new System.Windows.Forms.Button();
            this.btnEnterPreOperational = new System.Windows.Forms.Button();
            this.btnStopNode = new System.Windows.Forms.Button();
            this.btnStartNode = new System.Windows.Forms.Button();
            this.grperror_control = new System.Windows.Forms.GroupBox();
            this.btnConfHeartBeat = new System.Windows.Forms.Button();
            this.btnConfigNodeGuard = new System.Windows.Forms.Button();
            this.btnStopHeartbeatMonitor = new System.Windows.Forms.Button();
            this.btnStartHeartbeatMonitor = new System.Windows.Forms.Button();
            this.btnStopNodeGuard = new System.Windows.Forms.Button();
            this.btnStartNodeGuard = new System.Windows.Forms.Button();
            this.tabReadDeviceParam = new System.Windows.Forms.TabPage();
            this.grpErrorInformation = new System.Windows.Forms.GroupBox();
            this.btnClearErrors = new System.Windows.Forms.Button();
            this.btnReadPredefErrField = new System.Windows.Forms.Button();
            this.btnManfStatReg = new System.Windows.Forms.Button();
            this.btnErrorRegister = new System.Windows.Forms.Button();
            this.grpDeviceParams = new System.Windows.Forms.GroupBox();
            this.btnSoftwareVersion = new System.Windows.Forms.Button();
            this.btnHardwareVersion = new System.Windows.Forms.Button();
            this.btnDeviceType = new System.Windows.Forms.Button();
            this.btnDeviceName = new System.Windows.Forms.Button();
            this.tabPDO = new System.Windows.Forms.TabPage();
            this.btnEnableSync = new System.Windows.Forms.Button();
            this.btnSyncTrType = new System.Windows.Forms.Button();
            this.btnStartSync = new System.Windows.Forms.Button();
            this.btnSendPdoClearAll = new System.Windows.Forms.Button();
            this.btnSendPdo50 = new System.Windows.Forms.Button();
            this.btnSendPdo = new System.Windows.Forms.Button();
            this.btnRequestPdo = new System.Windows.Forms.Button();
            this.btnMap64bits = new System.Windows.Forms.Button();
            this.btnMap32bits = new System.Windows.Forms.Button();
            this.btnMap8bits = new System.Windows.Forms.Button();
            this.btnReadMappedObjects = new System.Windows.Forms.Button();
            this.btnTransmType254 = new System.Windows.Forms.Button();
            this.btnSetEventTimer = new System.Windows.Forms.Button();
            this.btnEnablePdo = new System.Windows.Forms.Button();
            this.cbPdoRx = new System.Windows.Forms.CheckBox();
            this.btnReadPDO = new System.Windows.Forms.Button();
            this.lblPDO = new System.Windows.Forms.Label();
            this.numUpDownPdo = new System.Windows.Forms.NumericUpDown();
            this.tab_can_trace = new System.Windows.Forms.TabPage();
            this.rich_text_can_trace = new System.Windows.Forms.RichTextBox();
            this.tabDcfFile = new System.Windows.Forms.TabPage();
            this.button2 = new System.Windows.Forms.Button();
            this.numRemoteNode = new System.Windows.Forms.NumericUpDown();
            this.consoleTextBox = new System.Windows.Forms.RichTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.linkClick = new System.Windows.Forms.LinkLabel();
            this.errorProvider1 = new System.Windows.Forms.ErrorProvider(this.components);
            this.openDcfFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.tabControl1.SuspendLayout();
            this.tabSettings.SuspendLayout();
            this.grpHardwareSetup.SuspendLayout();
            this.tabNMT.SuspendLayout();
            this.grpModuleControl.SuspendLayout();
            this.grperror_control.SuspendLayout();
            this.tabReadDeviceParam.SuspendLayout();
            this.grpErrorInformation.SuspendLayout();
            this.grpDeviceParams.SuspendLayout();
            this.tabPDO.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownPdo)).BeginInit();
            this.tab_can_trace.SuspendLayout();
            this.tabDcfFile.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numRemoteNode)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Location = new System.Drawing.Point(0, 366);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(382, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabSettings);
            this.tabControl1.Controls.Add(this.tabNMT);
            this.tabControl1.Controls.Add(this.tabReadDeviceParam);
            this.tabControl1.Controls.Add(this.tabPDO);
            this.tabControl1.Controls.Add(this.tab_can_trace);
            this.tabControl1.Controls.Add(this.tabDcfFile);
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(381, 244);
            this.tabControl1.TabIndex = 1;
            // 
            // tabSettings
            // 
            this.tabSettings.Controls.Add(this.label4);
            this.tabSettings.Controls.Add(this.label3);
            this.tabSettings.Controls.Add(this.label2);
            this.tabSettings.Controls.Add(this.grpHardwareSetup);
            this.tabSettings.Location = new System.Drawing.Point(4, 22);
            this.tabSettings.Name = "tabSettings";
            this.tabSettings.Padding = new System.Windows.Forms.Padding(3);
            this.tabSettings.Size = new System.Drawing.Size(373, 218);
            this.tabSettings.TabIndex = 0;
            this.tabSettings.Text = "CAN";
            this.tabSettings.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(66, 63);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(94, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "as this application!";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(29, 38);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(182, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "for your CAN hardware in same folder";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(29, 16);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(171, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Place the can \"canopenlib_hw.dll\"";
            // 
            // grpHardwareSetup
            // 
            this.grpHardwareSetup.Controls.Add(this.btnDisconnect);
            this.grpHardwareSetup.Controls.Add(this.btnConnect);
            this.grpHardwareSetup.Controls.Add(this.comboBitrateCAN);
            this.grpHardwareSetup.Controls.Add(this.comboChannelCAN);
            this.grpHardwareSetup.Controls.Add(this.lblBitrateCAN);
            this.grpHardwareSetup.Controls.Add(this.lblChannelCAN);
            this.grpHardwareSetup.Location = new System.Drawing.Point(8, 96);
            this.grpHardwareSetup.Name = "grpHardwareSetup";
            this.grpHardwareSetup.Size = new System.Drawing.Size(217, 92);
            this.grpHardwareSetup.TabIndex = 1;
            this.grpHardwareSetup.TabStop = false;
            this.grpHardwareSetup.Text = "Channel and Bitrate";
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Location = new System.Drawing.Point(137, 47);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(74, 21);
            this.btnDisconnect.TabIndex = 7;
            this.btnDisconnect.Text = "Disconnect";
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(137, 21);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(74, 21);
            this.btnConnect.TabIndex = 6;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // comboBitrateCAN
            // 
            this.comboBitrateCAN.FormattingEnabled = true;
            this.comboBitrateCAN.Items.AddRange(new object[] {
            "125000",
            "250000",
            "500000",
            "1000000"});
            this.comboBitrateCAN.Location = new System.Drawing.Point(61, 47);
            this.comboBitrateCAN.Name = "comboBitrateCAN";
            this.comboBitrateCAN.Size = new System.Drawing.Size(70, 21);
            this.comboBitrateCAN.TabIndex = 5;
            this.comboBitrateCAN.Text = "500000";
            // 
            // comboChannelCAN
            // 
            this.comboChannelCAN.FormattingEnabled = true;
            this.comboChannelCAN.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.comboChannelCAN.Items.AddRange(new object[] {
            "1",
            "2",
            "3",
            "4"});
            this.comboChannelCAN.Location = new System.Drawing.Point(61, 18);
            this.comboChannelCAN.Name = "comboChannelCAN";
            this.comboChannelCAN.Size = new System.Drawing.Size(70, 21);
            this.comboChannelCAN.TabIndex = 2;
            this.comboChannelCAN.Text = "0";
            // 
            // lblBitrateCAN
            // 
            this.lblBitrateCAN.AutoSize = true;
            this.lblBitrateCAN.Location = new System.Drawing.Point(6, 50);
            this.lblBitrateCAN.Name = "lblBitrateCAN";
            this.lblBitrateCAN.Size = new System.Drawing.Size(40, 13);
            this.lblBitrateCAN.TabIndex = 4;
            this.lblBitrateCAN.Text = "Bitrate:";
            // 
            // lblChannelCAN
            // 
            this.lblChannelCAN.AutoSize = true;
            this.lblChannelCAN.Location = new System.Drawing.Point(6, 24);
            this.lblChannelCAN.Name = "lblChannelCAN";
            this.lblChannelCAN.Size = new System.Drawing.Size(29, 13);
            this.lblChannelCAN.TabIndex = 2;
            this.lblChannelCAN.Text = "Port:";
            // 
            // tabNMT
            // 
            this.tabNMT.Controls.Add(this.grpModuleControl);
            this.tabNMT.Controls.Add(this.grperror_control);
            this.tabNMT.Location = new System.Drawing.Point(4, 22);
            this.tabNMT.Name = "tabNMT";
            this.tabNMT.Size = new System.Drawing.Size(373, 218);
            this.tabNMT.TabIndex = 2;
            this.tabNMT.Text = "NMT";
            this.tabNMT.UseVisualStyleBackColor = true;
            // 
            // grpModuleControl
            // 
            this.grpModuleControl.Controls.Add(this.button1);
            this.grpModuleControl.Controls.Add(this.btnResetComm);
            this.grpModuleControl.Controls.Add(this.btnEnterPreOperational);
            this.grpModuleControl.Controls.Add(this.btnStopNode);
            this.grpModuleControl.Controls.Add(this.btnStartNode);
            this.grpModuleControl.Location = new System.Drawing.Point(0, 118);
            this.grpModuleControl.Name = "grpModuleControl";
            this.grpModuleControl.Size = new System.Drawing.Size(234, 97);
            this.grpModuleControl.TabIndex = 4;
            this.grpModuleControl.TabStop = false;
            this.grpModuleControl.Text = "Module Control";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(121, 44);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(106, 22);
            this.button1.TabIndex = 4;
            this.button1.Text = "Reset Node";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // btnResetComm
            // 
            this.btnResetComm.Location = new System.Drawing.Point(121, 17);
            this.btnResetComm.Name = "btnResetComm";
            this.btnResetComm.Size = new System.Drawing.Size(106, 23);
            this.btnResetComm.TabIndex = 3;
            this.btnResetComm.Text = "Reset Comm.";
            this.btnResetComm.UseVisualStyleBackColor = true;
            this.btnResetComm.Click += new System.EventHandler(this.btnResetComm_Click);
            // 
            // btnEnterPreOperational
            // 
            this.btnEnterPreOperational.Location = new System.Drawing.Point(10, 70);
            this.btnEnterPreOperational.Name = "btnEnterPreOperational";
            this.btnEnterPreOperational.Size = new System.Drawing.Size(106, 22);
            this.btnEnterPreOperational.TabIndex = 2;
            this.btnEnterPreOperational.Text = "Preoperational";
            this.btnEnterPreOperational.UseVisualStyleBackColor = true;
            this.btnEnterPreOperational.Click += new System.EventHandler(this.btnEnterPreOperational_Click);
            // 
            // btnStopNode
            // 
            this.btnStopNode.Location = new System.Drawing.Point(10, 44);
            this.btnStopNode.Name = "btnStopNode";
            this.btnStopNode.Size = new System.Drawing.Size(106, 22);
            this.btnStopNode.TabIndex = 1;
            this.btnStopNode.Text = "Stop";
            this.btnStopNode.UseVisualStyleBackColor = true;
            this.btnStopNode.Click += new System.EventHandler(this.btnStopNode_Click);
            // 
            // btnStartNode
            // 
            this.btnStartNode.Location = new System.Drawing.Point(9, 17);
            this.btnStartNode.Name = "btnStartNode";
            this.btnStartNode.Size = new System.Drawing.Size(106, 23);
            this.btnStartNode.TabIndex = 0;
            this.btnStartNode.Text = "Start";
            this.btnStartNode.UseVisualStyleBackColor = true;
            this.btnStartNode.Click += new System.EventHandler(this.btnStartNode_Click);
            // 
            // grperror_control
            // 
            this.grperror_control.Controls.Add(this.btnConfHeartBeat);
            this.grperror_control.Controls.Add(this.btnConfigNodeGuard);
            this.grperror_control.Controls.Add(this.btnStopHeartbeatMonitor);
            this.grperror_control.Controls.Add(this.btnStartHeartbeatMonitor);
            this.grperror_control.Controls.Add(this.btnStopNodeGuard);
            this.grperror_control.Controls.Add(this.btnStartNodeGuard);
            this.grperror_control.Location = new System.Drawing.Point(3, 3);
            this.grperror_control.Name = "grperror_control";
            this.grperror_control.Size = new System.Drawing.Size(231, 109);
            this.grperror_control.TabIndex = 3;
            this.grperror_control.TabStop = false;
            this.grperror_control.Text = "Error Control";
            // 
            // btnConfHeartBeat
            // 
            this.btnConfHeartBeat.Location = new System.Drawing.Point(124, 80);
            this.btnConfHeartBeat.Name = "btnConfHeartBeat";
            this.btnConfHeartBeat.Size = new System.Drawing.Size(100, 23);
            this.btnConfHeartBeat.TabIndex = 6;
            this.btnConfHeartBeat.Text = "Conf. Heartbeat";
            this.btnConfHeartBeat.UseVisualStyleBackColor = true;
            this.btnConfHeartBeat.Click += new System.EventHandler(this.btnConfHeartBeat_Click);
            // 
            // btnConfigNodeGuard
            // 
            this.btnConfigNodeGuard.Location = new System.Drawing.Point(7, 80);
            this.btnConfigNodeGuard.Name = "btnConfigNodeGuard";
            this.btnConfigNodeGuard.Size = new System.Drawing.Size(106, 23);
            this.btnConfigNodeGuard.TabIndex = 5;
            this.btnConfigNodeGuard.Text = "Conf. Nodeguard";
            this.btnConfigNodeGuard.UseVisualStyleBackColor = true;
            this.btnConfigNodeGuard.Click += new System.EventHandler(this.btnConfigNodeGuard_Click);
            // 
            // btnStopHeartbeatMonitor
            // 
            this.btnStopHeartbeatMonitor.Location = new System.Drawing.Point(122, 43);
            this.btnStopHeartbeatMonitor.Name = "btnStopHeartbeatMonitor";
            this.btnStopHeartbeatMonitor.Size = new System.Drawing.Size(102, 34);
            this.btnStopHeartbeatMonitor.TabIndex = 4;
            this.btnStopHeartbeatMonitor.Text = "Stop Heartbeat Monitor";
            this.btnStopHeartbeatMonitor.UseVisualStyleBackColor = true;
            // 
            // btnStartHeartbeatMonitor
            // 
            this.btnStartHeartbeatMonitor.Location = new System.Drawing.Point(6, 43);
            this.btnStartHeartbeatMonitor.Name = "btnStartHeartbeatMonitor";
            this.btnStartHeartbeatMonitor.Size = new System.Drawing.Size(106, 34);
            this.btnStartHeartbeatMonitor.TabIndex = 3;
            this.btnStartHeartbeatMonitor.Text = "Start Heartbeat Monitor";
            this.btnStartHeartbeatMonitor.UseVisualStyleBackColor = true;
            this.btnStartHeartbeatMonitor.Click += new System.EventHandler(this.btnStartHeartbeatMonitor_Click);
            // 
            // btnStopNodeGuard
            // 
            this.btnStopNodeGuard.Location = new System.Drawing.Point(119, 15);
            this.btnStopNodeGuard.Name = "btnStopNodeGuard";
            this.btnStopNodeGuard.Size = new System.Drawing.Size(106, 25);
            this.btnStopNodeGuard.TabIndex = 2;
            this.btnStopNodeGuard.Text = "Stop Node Guard";
            this.btnStopNodeGuard.UseVisualStyleBackColor = true;
            this.btnStopNodeGuard.Click += new System.EventHandler(this.btnStopNodeGuard_Click);
            // 
            // btnStartNodeGuard
            // 
            this.btnStartNodeGuard.Location = new System.Drawing.Point(6, 15);
            this.btnStartNodeGuard.Name = "btnStartNodeGuard";
            this.btnStartNodeGuard.Size = new System.Drawing.Size(107, 25);
            this.btnStartNodeGuard.TabIndex = 1;
            this.btnStartNodeGuard.Text = "Start Node Guard";
            this.btnStartNodeGuard.UseVisualStyleBackColor = true;
            this.btnStartNodeGuard.Click += new System.EventHandler(this.btnStartNodeGuard_Click);
            // 
            // tabReadDeviceParam
            // 
            this.tabReadDeviceParam.Controls.Add(this.grpErrorInformation);
            this.tabReadDeviceParam.Controls.Add(this.grpDeviceParams);
            this.tabReadDeviceParam.Location = new System.Drawing.Point(4, 22);
            this.tabReadDeviceParam.Name = "tabReadDeviceParam";
            this.tabReadDeviceParam.Padding = new System.Windows.Forms.Padding(3);
            this.tabReadDeviceParam.Size = new System.Drawing.Size(373, 218);
            this.tabReadDeviceParam.TabIndex = 1;
            this.tabReadDeviceParam.Text = "Device";
            this.tabReadDeviceParam.UseVisualStyleBackColor = true;
            // 
            // grpErrorInformation
            // 
            this.grpErrorInformation.Controls.Add(this.btnClearErrors);
            this.grpErrorInformation.Controls.Add(this.btnReadPredefErrField);
            this.grpErrorInformation.Controls.Add(this.btnManfStatReg);
            this.grpErrorInformation.Controls.Add(this.btnErrorRegister);
            this.grpErrorInformation.Location = new System.Drawing.Point(6, 97);
            this.grpErrorInformation.Name = "grpErrorInformation";
            this.grpErrorInformation.Size = new System.Drawing.Size(221, 92);
            this.grpErrorInformation.TabIndex = 1;
            this.grpErrorInformation.TabStop = false;
            this.grpErrorInformation.Text = "Error Information";
            // 
            // btnClearErrors
            // 
            this.btnClearErrors.Location = new System.Drawing.Point(126, 49);
            this.btnClearErrors.Name = "btnClearErrors";
            this.btnClearErrors.Size = new System.Drawing.Size(89, 23);
            this.btnClearErrors.TabIndex = 4;
            this.btnClearErrors.Text = "Clear Errors";
            this.btnClearErrors.UseVisualStyleBackColor = true;
            this.btnClearErrors.Click += new System.EventHandler(this.btnClearErrors_Click);
            // 
            // btnReadPredefErrField
            // 
            this.btnReadPredefErrField.Location = new System.Drawing.Point(126, 19);
            this.btnReadPredefErrField.Name = "btnReadPredefErrField";
            this.btnReadPredefErrField.Size = new System.Drawing.Size(89, 24);
            this.btnReadPredefErrField.TabIndex = 3;
            this.btnReadPredefErrField.Text = "Scan Errors";
            this.btnReadPredefErrField.UseVisualStyleBackColor = true;
            this.btnReadPredefErrField.Click += new System.EventHandler(this.btnReadPredefErrField_Click);
            // 
            // btnManfStatReg
            // 
            this.btnManfStatReg.Location = new System.Drawing.Point(6, 49);
            this.btnManfStatReg.Name = "btnManfStatReg";
            this.btnManfStatReg.Size = new System.Drawing.Size(89, 37);
            this.btnManfStatReg.TabIndex = 2;
            this.btnManfStatReg.Text = "Manufacturer status register";
            this.btnManfStatReg.UseVisualStyleBackColor = true;
            this.btnManfStatReg.Click += new System.EventHandler(this.btnManfStatReg_Click);
            // 
            // btnErrorRegister
            // 
            this.btnErrorRegister.Location = new System.Drawing.Point(7, 19);
            this.btnErrorRegister.Name = "btnErrorRegister";
            this.btnErrorRegister.Size = new System.Drawing.Size(89, 24);
            this.btnErrorRegister.TabIndex = 1;
            this.btnErrorRegister.Text = "Error Register";
            this.btnErrorRegister.UseVisualStyleBackColor = true;
            this.btnErrorRegister.Click += new System.EventHandler(this.btnErrorRegister_Click);
            // 
            // grpDeviceParams
            // 
            this.grpDeviceParams.Controls.Add(this.btnSoftwareVersion);
            this.grpDeviceParams.Controls.Add(this.btnHardwareVersion);
            this.grpDeviceParams.Controls.Add(this.btnDeviceType);
            this.grpDeviceParams.Controls.Add(this.btnDeviceName);
            this.grpDeviceParams.Location = new System.Drawing.Point(6, 3);
            this.grpDeviceParams.Name = "grpDeviceParams";
            this.grpDeviceParams.Size = new System.Drawing.Size(222, 88);
            this.grpDeviceParams.TabIndex = 0;
            this.grpDeviceParams.TabStop = false;
            this.grpDeviceParams.Text = "Device parameters";
            // 
            // btnSoftwareVersion
            // 
            this.btnSoftwareVersion.Location = new System.Drawing.Point(126, 49);
            this.btnSoftwareVersion.Name = "btnSoftwareVersion";
            this.btnSoftwareVersion.Size = new System.Drawing.Size(89, 24);
            this.btnSoftwareVersion.TabIndex = 3;
            this.btnSoftwareVersion.Text = "Software ver.";
            this.btnSoftwareVersion.UseVisualStyleBackColor = true;
            this.btnSoftwareVersion.Click += new System.EventHandler(this.btnSoftwareVersion_Click);
            // 
            // btnHardwareVersion
            // 
            this.btnHardwareVersion.Location = new System.Drawing.Point(7, 49);
            this.btnHardwareVersion.Name = "btnHardwareVersion";
            this.btnHardwareVersion.Size = new System.Drawing.Size(89, 24);
            this.btnHardwareVersion.TabIndex = 2;
            this.btnHardwareVersion.Text = "Hardware ver.";
            this.btnHardwareVersion.UseVisualStyleBackColor = true;
            this.btnHardwareVersion.Click += new System.EventHandler(this.btnHardwareVersion_Click);
            // 
            // btnDeviceType
            // 
            this.btnDeviceType.Location = new System.Drawing.Point(126, 19);
            this.btnDeviceType.Name = "btnDeviceType";
            this.btnDeviceType.Size = new System.Drawing.Size(90, 24);
            this.btnDeviceType.TabIndex = 1;
            this.btnDeviceType.Text = "Device Type";
            this.btnDeviceType.UseVisualStyleBackColor = true;
            this.btnDeviceType.Click += new System.EventHandler(this.btnDeviceType_Click);
            // 
            // btnDeviceName
            // 
            this.btnDeviceName.Location = new System.Drawing.Point(6, 19);
            this.btnDeviceName.Name = "btnDeviceName";
            this.btnDeviceName.Size = new System.Drawing.Size(90, 24);
            this.btnDeviceName.TabIndex = 0;
            this.btnDeviceName.Text = "Device Name";
            this.btnDeviceName.UseVisualStyleBackColor = true;
            this.btnDeviceName.Click += new System.EventHandler(this.btnDeviceName_Click);
            // 
            // tabPDO
            // 
            this.tabPDO.Controls.Add(this.btnEnableSync);
            this.tabPDO.Controls.Add(this.btnSyncTrType);
            this.tabPDO.Controls.Add(this.btnStartSync);
            this.tabPDO.Controls.Add(this.btnSendPdoClearAll);
            this.tabPDO.Controls.Add(this.btnSendPdo50);
            this.tabPDO.Controls.Add(this.btnSendPdo);
            this.tabPDO.Controls.Add(this.btnRequestPdo);
            this.tabPDO.Controls.Add(this.btnMap64bits);
            this.tabPDO.Controls.Add(this.btnMap32bits);
            this.tabPDO.Controls.Add(this.btnMap8bits);
            this.tabPDO.Controls.Add(this.btnReadMappedObjects);
            this.tabPDO.Controls.Add(this.btnTransmType254);
            this.tabPDO.Controls.Add(this.btnSetEventTimer);
            this.tabPDO.Controls.Add(this.btnEnablePdo);
            this.tabPDO.Controls.Add(this.cbPdoRx);
            this.tabPDO.Controls.Add(this.btnReadPDO);
            this.tabPDO.Controls.Add(this.lblPDO);
            this.tabPDO.Controls.Add(this.numUpDownPdo);
            this.tabPDO.Location = new System.Drawing.Point(4, 22);
            this.tabPDO.Name = "tabPDO";
            this.tabPDO.Padding = new System.Windows.Forms.Padding(3);
            this.tabPDO.Size = new System.Drawing.Size(373, 218);
            this.tabPDO.TabIndex = 4;
            this.tabPDO.Text = "PDO";
            this.tabPDO.UseVisualStyleBackColor = true;
            // 
            // btnEnableSync
            // 
            this.btnEnableSync.Location = new System.Drawing.Point(133, 185);
            this.btnEnableSync.Name = "btnEnableSync";
            this.btnEnableSync.Size = new System.Drawing.Size(94, 27);
            this.btnEnableSync.TabIndex = 18;
            this.btnEnableSync.Text = "Enable sync";
            this.btnEnableSync.UseVisualStyleBackColor = true;
            this.btnEnableSync.Click += new System.EventHandler(this.btnEnableSync_Click);
            // 
            // btnSyncTrType
            // 
            this.btnSyncTrType.Location = new System.Drawing.Point(10, 185);
            this.btnSyncTrType.Name = "btnSyncTrType";
            this.btnSyncTrType.Size = new System.Drawing.Size(117, 25);
            this.btnSyncTrType.TabIndex = 17;
            this.btnSyncTrType.Text = "TransType 10";
            this.btnSyncTrType.UseVisualStyleBackColor = true;
            this.btnSyncTrType.Click += new System.EventHandler(this.button2_Click);
            // 
            // btnStartSync
            // 
            this.btnStartSync.Location = new System.Drawing.Point(234, 129);
            this.btnStartSync.Name = "btnStartSync";
            this.btnStartSync.Size = new System.Drawing.Size(132, 23);
            this.btnStartSync.TabIndex = 16;
            this.btnStartSync.Text = "Start Sync";
            this.btnStartSync.UseVisualStyleBackColor = true;
            this.btnStartSync.Click += new System.EventHandler(this.btnStartSync_Click);
            // 
            // btnSendPdoClearAll
            // 
            this.btnSendPdoClearAll.Location = new System.Drawing.Point(234, 99);
            this.btnSendPdoClearAll.Name = "btnSendPdoClearAll";
            this.btnSendPdoClearAll.Size = new System.Drawing.Size(133, 23);
            this.btnSendPdoClearAll.TabIndex = 15;
            this.btnSendPdoClearAll.Text = "Send PDO (0% set)";
            this.btnSendPdoClearAll.UseVisualStyleBackColor = true;
            this.btnSendPdoClearAll.Click += new System.EventHandler(this.btnSendPdoClearAll_Click);
            // 
            // btnSendPdo50
            // 
            this.btnSendPdo50.Location = new System.Drawing.Point(234, 70);
            this.btnSendPdo50.Name = "btnSendPdo50";
            this.btnSendPdo50.Size = new System.Drawing.Size(133, 23);
            this.btnSendPdo50.TabIndex = 14;
            this.btnSendPdo50.Text = "Send PDO (50% set)";
            this.btnSendPdo50.UseVisualStyleBackColor = true;
            this.btnSendPdo50.Click += new System.EventHandler(this.btnSendPdo50_Click);
            // 
            // btnSendPdo
            // 
            this.btnSendPdo.Location = new System.Drawing.Point(233, 41);
            this.btnSendPdo.Name = "btnSendPdo";
            this.btnSendPdo.Size = new System.Drawing.Size(133, 23);
            this.btnSendPdo.TabIndex = 13;
            this.btnSendPdo.Text = "Send PDO (100% set)";
            this.btnSendPdo.UseVisualStyleBackColor = true;
            this.btnSendPdo.Click += new System.EventHandler(this.btnSendPdo_Click);
            // 
            // btnRequestPdo
            // 
            this.btnRequestPdo.Location = new System.Drawing.Point(152, 9);
            this.btnRequestPdo.Name = "btnRequestPdo";
            this.btnRequestPdo.Size = new System.Drawing.Size(75, 23);
            this.btnRequestPdo.TabIndex = 12;
            this.btnRequestPdo.Text = "PDO RTR";
            this.btnRequestPdo.UseVisualStyleBackColor = true;
            this.btnRequestPdo.Click += new System.EventHandler(this.btnRequestPdo_Click);
            // 
            // btnMap64bits
            // 
            this.btnMap64bits.Location = new System.Drawing.Point(133, 154);
            this.btnMap64bits.Name = "btnMap64bits";
            this.btnMap64bits.Size = new System.Drawing.Size(94, 25);
            this.btnMap64bits.TabIndex = 11;
            this.btnMap64bits.Text = "Map 64 bits";
            this.btnMap64bits.UseVisualStyleBackColor = true;
            this.btnMap64bits.Click += new System.EventHandler(this.btnMap64bits_Click);
            // 
            // btnMap32bits
            // 
            this.btnMap32bits.Location = new System.Drawing.Point(133, 121);
            this.btnMap32bits.Name = "btnMap32bits";
            this.btnMap32bits.Size = new System.Drawing.Size(94, 27);
            this.btnMap32bits.TabIndex = 10;
            this.btnMap32bits.Text = "Map 32 bits";
            this.btnMap32bits.UseVisualStyleBackColor = true;
            this.btnMap32bits.Click += new System.EventHandler(this.btnMap32bits_Click);
            // 
            // btnMap8bits
            // 
            this.btnMap8bits.Location = new System.Drawing.Point(133, 86);
            this.btnMap8bits.Name = "btnMap8bits";
            this.btnMap8bits.Size = new System.Drawing.Size(94, 27);
            this.btnMap8bits.TabIndex = 9;
            this.btnMap8bits.Text = "Map 8 bits";
            this.btnMap8bits.UseVisualStyleBackColor = true;
            this.btnMap8bits.Click += new System.EventHandler(this.btnMap8bits_Click);
            // 
            // btnReadMappedObjects
            // 
            this.btnReadMappedObjects.Location = new System.Drawing.Point(133, 41);
            this.btnReadMappedObjects.Name = "btnReadMappedObjects";
            this.btnReadMappedObjects.Size = new System.Drawing.Size(95, 38);
            this.btnReadMappedObjects.TabIndex = 8;
            this.btnReadMappedObjects.Text = "Read mapping params";
            this.btnReadMappedObjects.UseVisualStyleBackColor = true;
            this.btnReadMappedObjects.Click += new System.EventHandler(this.btnReadMappedObjects_Click);
            // 
            // btnTransmType254
            // 
            this.btnTransmType254.Location = new System.Drawing.Point(10, 154);
            this.btnTransmType254.Name = "btnTransmType254";
            this.btnTransmType254.Size = new System.Drawing.Size(117, 25);
            this.btnTransmType254.TabIndex = 7;
            this.btnTransmType254.Text = "TransType 254";
            this.btnTransmType254.UseVisualStyleBackColor = true;
            this.btnTransmType254.Click += new System.EventHandler(this.btnTransmType254_Click);
            // 
            // btnSetEventTimer
            // 
            this.btnSetEventTimer.Location = new System.Drawing.Point(10, 86);
            this.btnSetEventTimer.Name = "btnSetEventTimer";
            this.btnSetEventTimer.Size = new System.Drawing.Size(117, 29);
            this.btnSetEventTimer.TabIndex = 6;
            this.btnSetEventTimer.Text = "Set Event Timer";
            this.btnSetEventTimer.UseVisualStyleBackColor = true;
            this.btnSetEventTimer.Click += new System.EventHandler(this.btnSetEventTimer_Click);
            // 
            // btnEnablePdo
            // 
            this.btnEnablePdo.Location = new System.Drawing.Point(10, 121);
            this.btnEnablePdo.Name = "btnEnablePdo";
            this.btnEnablePdo.Size = new System.Drawing.Size(117, 27);
            this.btnEnablePdo.TabIndex = 5;
            this.btnEnablePdo.Text = "Enable PDO";
            this.btnEnablePdo.UseVisualStyleBackColor = true;
            this.btnEnablePdo.Click += new System.EventHandler(this.btnEnablePdo_Click);
            // 
            // cbPdoRx
            // 
            this.cbPdoRx.AutoSize = true;
            this.cbPdoRx.Location = new System.Drawing.Point(107, 15);
            this.cbPdoRx.Name = "cbPdoRx";
            this.cbPdoRx.Size = new System.Drawing.Size(39, 17);
            this.cbPdoRx.TabIndex = 4;
            this.cbPdoRx.Text = "Rx";
            this.cbPdoRx.UseVisualStyleBackColor = true;
            // 
            // btnReadPDO
            // 
            this.btnReadPDO.Location = new System.Drawing.Point(11, 51);
            this.btnReadPDO.Name = "btnReadPDO";
            this.btnReadPDO.Size = new System.Drawing.Size(116, 27);
            this.btnReadPDO.TabIndex = 2;
            this.btnReadPDO.Text = "Read comm. params";
            this.btnReadPDO.UseVisualStyleBackColor = true;
            this.btnReadPDO.Click += new System.EventHandler(this.btnReadPDO_Click);
            // 
            // lblPDO
            // 
            this.lblPDO.AutoSize = true;
            this.lblPDO.Location = new System.Drawing.Point(8, 15);
            this.lblPDO.Name = "lblPDO";
            this.lblPDO.Size = new System.Drawing.Size(33, 13);
            this.lblPDO.TabIndex = 1;
            this.lblPDO.Text = "PDO:";
            // 
            // numUpDownPdo
            // 
            this.numUpDownPdo.Location = new System.Drawing.Point(45, 12);
            this.numUpDownPdo.Maximum = new decimal(new int[] {
            127,
            0,
            0,
            0});
            this.numUpDownPdo.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numUpDownPdo.Name = "numUpDownPdo";
            this.numUpDownPdo.Size = new System.Drawing.Size(56, 20);
            this.numUpDownPdo.TabIndex = 0;
            this.numUpDownPdo.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // tab_can_trace
            // 
            this.tab_can_trace.Controls.Add(this.rich_text_can_trace);
            this.tab_can_trace.Location = new System.Drawing.Point(4, 22);
            this.tab_can_trace.Name = "tab_can_trace";
            this.tab_can_trace.Padding = new System.Windows.Forms.Padding(3);
            this.tab_can_trace.Size = new System.Drawing.Size(373, 218);
            this.tab_can_trace.TabIndex = 3;
            this.tab_can_trace.Text = "CAN trace";
            this.tab_can_trace.UseVisualStyleBackColor = true;
            // 
            // rich_text_can_trace
            // 
            this.rich_text_can_trace.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rich_text_can_trace.Location = new System.Drawing.Point(3, 3);
            this.rich_text_can_trace.Name = "rich_text_can_trace";
            this.rich_text_can_trace.Size = new System.Drawing.Size(367, 212);
            this.rich_text_can_trace.TabIndex = 1;
            this.rich_text_can_trace.Text = "";
            // 
            // tabDcfFile
            // 
            this.tabDcfFile.Controls.Add(this.button2);
            this.tabDcfFile.Location = new System.Drawing.Point(4, 22);
            this.tabDcfFile.Name = "tabDcfFile";
            this.tabDcfFile.Padding = new System.Windows.Forms.Padding(3);
            this.tabDcfFile.Size = new System.Drawing.Size(373, 218);
            this.tabDcfFile.TabIndex = 5;
            this.tabDcfFile.Text = "DCF file";
            this.tabDcfFile.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(8, 21);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 23);
            this.button2.TabIndex = 0;
            this.button2.Text = "Select DCF file";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click_1);
            // 
            // numRemoteNode
            // 
            this.numRemoteNode.Location = new System.Drawing.Point(184, 245);
            this.numRemoteNode.Maximum = new decimal(new int[] {
            127,
            0,
            0,
            0});
            this.numRemoteNode.Name = "numRemoteNode";
            this.numRemoteNode.Size = new System.Drawing.Size(54, 20);
            this.numRemoteNode.TabIndex = 0;
            this.numRemoteNode.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numRemoteNode.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
            this.numRemoteNode.ValueChanged += new System.EventHandler(this.numRemoteNode_ValueChanged);
            // 
            // consoleTextBox
            // 
            this.consoleTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.consoleTextBox.Location = new System.Drawing.Point(0, 266);
            this.consoleTextBox.Name = "consoleTextBox";
            this.consoleTextBox.Size = new System.Drawing.Size(381, 101);
            this.consoleTextBox.TabIndex = 0;
            this.consoleTextBox.Text = "";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(55, 247);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(123, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Remote CANopen node:";
            // 
            // linkClick
            // 
            this.linkClick.AutoSize = true;
            this.linkClick.Location = new System.Drawing.Point(1, 370);
            this.linkClick.Name = "linkClick";
            this.linkClick.Size = new System.Drawing.Size(116, 13);
            this.linkClick.TabIndex = 3;
            this.linkClick.TabStop = true;
            this.linkClick.Text = "http://www.datalink.se";
            this.linkClick.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkClick_LinkClicked);
            // 
            // errorProvider1
            // 
            this.errorProvider1.ContainerControl = this;
            // 
            // openDcfFileDialog
            // 
            this.openDcfFileDialog.FileName = "openDcfFileDialog";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(382, 388);
            this.Controls.Add(this.linkClick);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.consoleTextBox);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.numRemoteNode);
            this.Controls.Add(this.statusStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "CANopen Diagnostic Simulator";
            this.tabControl1.ResumeLayout(false);
            this.tabSettings.ResumeLayout(false);
            this.tabSettings.PerformLayout();
            this.grpHardwareSetup.ResumeLayout(false);
            this.grpHardwareSetup.PerformLayout();
            this.tabNMT.ResumeLayout(false);
            this.grpModuleControl.ResumeLayout(false);
            this.grperror_control.ResumeLayout(false);
            this.tabReadDeviceParam.ResumeLayout(false);
            this.grpErrorInformation.ResumeLayout(false);
            this.grpDeviceParams.ResumeLayout(false);
            this.tabPDO.ResumeLayout(false);
            this.tabPDO.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownPdo)).EndInit();
            this.tab_can_trace.ResumeLayout(false);
            this.tabDcfFile.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numRemoteNode)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabSettings;
        private System.Windows.Forms.TabPage tabReadDeviceParam;
        private System.Windows.Forms.GroupBox grpHardwareSetup;
        private System.Windows.Forms.Label lblChannelCAN;
        private System.Windows.Forms.Label lblBitrateCAN;
        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ComboBox comboBitrateCAN;
        private System.Windows.Forms.ComboBox comboChannelCAN;
        private System.Windows.Forms.RichTextBox consoleTextBox;
        private System.Windows.Forms.TabPage tabNMT;
        private System.Windows.Forms.GroupBox grperror_control;
        private System.Windows.Forms.Button btnStopNodeGuard;
        private System.Windows.Forms.Button btnStartNodeGuard;
        private System.Windows.Forms.NumericUpDown numRemoteNode;
        private System.Windows.Forms.Button btnStopHeartbeatMonitor;
        private System.Windows.Forms.Button btnStartHeartbeatMonitor;
        private System.Windows.Forms.GroupBox grpModuleControl;
        private System.Windows.Forms.Button btnEnterPreOperational;
        private System.Windows.Forms.Button btnStopNode;
        private System.Windows.Forms.Button btnStartNode;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button btnResetComm;
        private System.Windows.Forms.GroupBox grpErrorInformation;
        private System.Windows.Forms.GroupBox grpDeviceParams;
        private System.Windows.Forms.Button btnDeviceType;
        private System.Windows.Forms.Button btnDeviceName;
        private System.Windows.Forms.Button btnHardwareVersion;
        private System.Windows.Forms.Button btnSoftwareVersion;
        private System.Windows.Forms.Button btnErrorRegister;
        private System.Windows.Forms.Button btnManfStatReg;
        private System.Windows.Forms.Button btnReadPredefErrField;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.LinkLabel linkClick;
        private System.Windows.Forms.ErrorProvider errorProvider1;
        private System.Windows.Forms.Button btnClearErrors;
        private System.Windows.Forms.TabPage tab_can_trace;
        private System.Windows.Forms.RichTextBox rich_text_can_trace;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TabPage tabPDO;
        private System.Windows.Forms.Label lblPDO;
        private System.Windows.Forms.NumericUpDown numUpDownPdo;
        private System.Windows.Forms.CheckBox cbPdoRx;
        private System.Windows.Forms.Button btnReadPDO;
        private System.Windows.Forms.Button btnEnablePdo;
        private System.Windows.Forms.Button btnSetEventTimer;
        private System.Windows.Forms.Button btnTransmType254;
        private System.Windows.Forms.Button btnReadMappedObjects;
        private System.Windows.Forms.Button btnMap8bits;
        private System.Windows.Forms.Button btnMap32bits;
        private System.Windows.Forms.Button btnMap64bits;
        private System.Windows.Forms.Button btnRequestPdo;
        private System.Windows.Forms.Button btnConfigNodeGuard;
        private System.Windows.Forms.Button btnConfHeartBeat;
    private System.Windows.Forms.Button btnSendPdo;
    private System.Windows.Forms.Button btnSendPdo50;
    private System.Windows.Forms.Button btnSendPdoClearAll;
    private System.Windows.Forms.Button btnStartSync;
    private System.Windows.Forms.Button btnSyncTrType;
    private System.Windows.Forms.Button btnEnableSync;
    private System.Windows.Forms.TabPage tabDcfFile;
    private System.Windows.Forms.OpenFileDialog openDcfFileDialog;
    private System.Windows.Forms.Button button2;
  }
}

