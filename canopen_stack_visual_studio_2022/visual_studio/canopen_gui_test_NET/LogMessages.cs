using System;
using System.Collections.Generic;
using System.Text;

namespace CANopenDiagnostic
{
    public class LogMessages
    {
        // Define a delegate named LogHandler, which will encapsulate
        // any method that takes a string as the parameter and returns no value
        public delegate void LogHandler(string message);

        // Define an Event based on the above Delegate
        public event LogHandler Log;


        // By Default, create an OnXXXX Method, to call the Event
        public void OnLog(string message)
        {
            if (Log != null)
            {
                Log(message);
            }
        }

    }
}
