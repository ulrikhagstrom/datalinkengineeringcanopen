<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="index.aspx.cs" Inherits="canopen_web_application.index" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>

        <asp:ScriptManager runat="server" id="ScriptManager1">
        </asp:ScriptManager>
        <asp:UpdatePanel runat="server" id="UpdatePanel">
        <ContentTemplate>
        <asp:Timer runat="server" id="Timer1" Interval="1000" OnTick="Timer1_Tick"></asp:Timer>
            <asp:Button ID="btnStartNodeGuardNode3" runat="server" 
                onclick="btnStartNodeGuardNode3_Click" Text="Button start node guard node 3" />
            <asp:Button ID="btnStartHeartBeatNode3" runat="server" onclick="btnStartHeartBeatNode3_Click" 
                Text="Start heartbeat node 3" />
            <br />
            <br />
            <asp:Button ID="btnStopNodeGuardNode3" runat="server" 
                Text="Stop node guard node 3" onclick="btnStopNodeGuardNode3_Click" />
            <asp:Button ID="btnStopHeartBeatNode3" runat="server" 
                onclick="btnStopHeartBeatNode3_Click" Text="Stop heart beat node 3" />
            <br />
            <br />
            <asp:Button ID="btnResetNode3" runat="server" onclick="btnResetNode3_Click" 
                Text="Reset node 3" />
            <asp:Button ID="btnResetCommunicationNode3" runat="server" 
                Text="Reset communication node 3" 
                onclick="btnResetCommunicationNode3_Click" />
            <br />
            <br />
            <asp:Button ID="btnStartNode3" runat="server" onclick="btnStartNode3_Click" 
                Text="Start node 3" />
            <asp:Button ID="btnStopNode3" runat="server" onclick="btnStopNode3_Click" 
                Text="Stop node 3" />
            <br />
            <br />
            <asp:Button ID="btnReadDeviceNameNode3" runat="server" 
                onclick="btnReadDeviceName_Click" Text="Read Device Name node 3" />
            <br />
            <br />
        <asp:Label runat="server" Text="Page not refreshed yet." id="Label1">
        </asp:Label>
        <asp:GridView ID="CanGrid" runat="server" EnableModelValidation="True" AutoGenerateColumns="false">
            <Columns> 
                <asp:BoundField HeaderText="Id" 
                    DataField="Id"  /> 
                <asp:BoundField HeaderText="Data" 
                    DataField="Data"  />
                <asp:BoundField HeaderText="Dlc" 
                    DataField="Dlc"  />
            </Columns> 
        </asp:GridView>

        </ContentTemplate>
        </asp:UpdatePanel>
    
    </div>
    </form>
</body>
</html>
