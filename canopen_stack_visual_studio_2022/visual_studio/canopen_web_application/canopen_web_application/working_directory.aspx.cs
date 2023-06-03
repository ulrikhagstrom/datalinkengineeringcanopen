using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace canopen_web_application
{
    public partial class working_directory : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            Page.Response.Write(System.IO.Directory.GetCurrentDirectory());

        }
    }
}