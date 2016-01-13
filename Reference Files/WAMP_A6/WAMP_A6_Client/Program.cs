/*
 * FILE        : Program.cs
 * PROJECT     : Windows and Mobile Programming - Assignment #6
 * AUTHOR      : Richard Meijer
 * SUBMIT DATE : 11/04/2014
 * DESCRIPTION : The main entry point for the solution. This is a TCP chat client that talks with a server at 192.0.0.150:5000.
 */
/* ----- LIBRARIES ----- */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WAMP_A6_Client
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
