/*
 * FILE        : MainForm.cs
 * PROJECT     : Windows and Mobile Programming - Assignment #6
 * AUTHOR      : Richard Meijer
 * SUBMIT DATE : 11/04/2014
 * DESCRIPTION : The main interface for the solution. This is a TCP chat client that talks with a server at 192.0.0.150:5000.
 */
/* ----- LIBRARIES ----- */
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;



/* ----- NAMESPACE ----- */
namespace WAMP_A6_Client
{
    /* ----- CLASS ----- */
    // Purpose: Handles the form input/events, connects, and chats with the server.
    public partial class MainForm : Form
    {
        /* ----- ATTRIBUTES ----- */
        public System.Net.Sockets.TcpClient clientSocket = new System.Net.Sockets.TcpClient();
        public NetworkStream serverStream = default(NetworkStream);
        public string readData = null;

        /* Please edit the IP/Port to match your configuration */
        public string IP = "192.168.0.150";
        public int PORT = 5000;

        /*
         * Name       : MainForm
         * Purpose    : Class Constructor.
         * Parameters : N/A
         * Returns    : N/A
         */
        public MainForm()
        {
            InitializeComponent();
        }

        /*
         * Name       : connectButton_Click
         * Purpose    : Handles the button click for the connect button. 
         *              Connects to the server with the user-defined name, and launches a listener thread.
         * Parameters : object sender, EventArgs e
         * Returns    : void
         */
        private void connectButton_Click(object sender, EventArgs e)
        {
            if (nameTextBox.Text != "")
            {
                readData = "Conected to Chat Server ...";
                WriteMessage();
                clientSocket.Connect(IP, PORT);
                serverStream = clientSocket.GetStream();

                byte[] outStream = System.Text.Encoding.ASCII.GetBytes(nameTextBox.Text + "$");
                serverStream.Write(outStream, 0, outStream.Length);
                serverStream.Flush();

                Thread ctThread = new Thread(GetMessage);
                ctThread.Start();
            }
        }

        /*
         * Name       : sendButton_Click
         * Purpose    : Handles the button click for the send button.
         *              Writes the message to the server stream.
         * Parameters : object sender, EventArgs e
         * Returns    : void
         */
        private void sendButton_Click(object sender, EventArgs e)
        {
            if (msgTextBox.Text != "")
            {
                byte[] outStream = System.Text.Encoding.ASCII.GetBytes(msgTextBox.Text + "$");
                serverStream.Write(outStream, 0, outStream.Length);
                serverStream.Flush();
                msgTextBox.Clear();
            }
        }

        /*
         * Name       : GetMessage
         * Purpose    : Does what the name says. Reads from the server stream.
         * Parameters : void
         * Returns    : void
         */
        private void GetMessage()
        {
            while (true)
            {
                serverStream = clientSocket.GetStream();
                int buffSize = 0;
                byte[] inStream = new byte[10025];
                buffSize = clientSocket.ReceiveBufferSize;
                serverStream.Read(inStream, 0, buffSize);
                string returndata = System.Text.Encoding.ASCII.GetString(inStream);
                readData = "" + returndata;
                WriteMessage();
            }
        }

        /*
         * Name       : WriteMessage
         * Purpose    : Thread safe way of appending text to the rich text box. Outputs a formatted version of readData.
         * Parameters : void
         * Returns    : void
         */
        private void WriteMessage()
        {
            if (this.InvokeRequired)
            {
                this.Invoke(new MethodInvoker(WriteMessage));
            }
            else
            {
                mainTextBox.AppendText(Environment.NewLine + " >> " + readData);
            }
        }

        /*
         * Name       : msgTextBox_KeyUp
         * Purpose    : Handles the KeyUp event for the msgTextBox.
         *              If the user is writing in the msgTextBox and presses enter, fire off the sendButton_Click method.
         * Parameters : object sender, KeyEventArgs e
         * Returns    : void
         */
        private void msgTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                sendButton_Click(null, null);
            }
        }

        /*
         * Name       : nameTextBox_KeyUp
         * Purpose    : Handles the KeyUp event for the msgTextBox.
         *              If the user is writing in the nameTextBox and presses enter, fire off the connectButton_Click method.
         * Parameters : object sender, KeyEventArgs e
         * Returns    : void
         */
        private void nameTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                connectButton_Click(null, null);
            }
        }

    }
}