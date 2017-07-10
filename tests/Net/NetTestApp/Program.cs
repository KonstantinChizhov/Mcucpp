using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace NetTestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            do
            {
                try
                {
                    Thread.Sleep(2000);
                    Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

                    IPAddress serverAddr = IPAddress.Parse("192.168.1.123");

                    IPEndPoint endPoint = new IPEndPoint(serverAddr, 11000);

                    string text = "Hello";
                    byte[] send_buffer = Encoding.ASCII.GetBytes(text);

                    sock.SendTo(send_buffer, endPoint);
                   
                }
                catch (Exception e)
                {
                    Console.WriteLine(e);
                }
            }
            while (true);
        }
    }
}
