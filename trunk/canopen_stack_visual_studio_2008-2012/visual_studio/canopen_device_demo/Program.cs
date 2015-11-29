using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CanopenDevices;
using System.Threading;

namespace canopen_device_demo
{
    class Program
    {
        static void Main(string[] args)
        {
            IO_401 ioUnit = new IO_401();

            ioUnit.canHardwareConnect(0, 125000);

            ioUnit.startupNode(3);

            Thread.Sleep(100000);
        }
    }
}
