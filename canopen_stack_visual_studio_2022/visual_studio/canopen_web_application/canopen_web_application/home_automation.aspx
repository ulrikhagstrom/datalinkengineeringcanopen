<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="home_automation.aspx.cs" Inherits="canopen_web_application.home_automation" %>

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
<%--            <br />
        <asp:Image ID="Image1" runat="server" Height="255px" 
            ImageUrl="~/images/beckhoff_5120.jpg" Width="176px" />
        <asp:Image ID="Image2" runat="server" Height="234px" 
            ImageUrl="~/images/kvaser_leaf.jpg" Width="189px" />
        <br />
        <br />--%>
        ONLINE DEMO (Datalink Engineering CANopen API + Kvaser Leaf Light + Beckhoff 
        5120 unit)<br />
        <br />
        <div style="height:300px; padding:50px; float:left">

            <asp:GridView ID="CanGrid" runat="server" EnableModelValidation="True" AutoGenerateColumns="false" Width="300px">
                <Columns> 
                    <asp:BoundField HeaderText="Id" 
                        DataField="Id"  /> 
                    <asp:BoundField HeaderText="Data" 
                        DataField="Data"  />
                    <asp:BoundField HeaderText="Dlc" 
                        DataField="Dlc"  />
                </Columns> 
            </asp:GridView>
        </div>
        <div style="float:left">
            <asp:Button ID="btnRestartSystem" runat="server" Text="Restart system" Width="196px" OnClick="btnRestartSystem_Click" />
            <br />
            <br />
            <asp:Button ID="btnChistmasLightsOn" runat="server" Text="Chistmas lights on" Width="196px" OnClick="btnChistmasLightsOn_Click" />
            <asp:Label ID="lblChistmansLightsStatus" runat="server" Text="Chistmas lights status"></asp:Label>
            <br />
            <asp:Button ID="btnChistmasLightsOff" runat="server" Text="Chistmas lights off" Width="196px" OnClick="btnChistmasLightsOff_Click" />
            <br />
        </div>
        </ContentTemplate>
        </asp:UpdatePanel>
    
    </div>
    </form>
</body>
</html>
