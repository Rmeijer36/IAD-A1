/*
 * FILE        : MainForm.cs
 * PROJECT     : Windows and Mobile Programming - Assignment #6
 * AUTHOR      : Richard Meijer
 * SUBMIT DATE : 11/04/2014
 * DESCRIPTION : The main interface for the solution. This is a TCP chat server that listens for client connections over port 5000.
 * 
 * DEBUGGING   : Server isn't properly shutting down all of the threads. While the current version will run, the process will stay active after exiting.
 */
/* ----- LIBRARIES ----- */
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Windows.Forms;



/* ----- NAMESPACE ----- */
namespace WAMP_A6_Server
{
    /* ----- CLASS ----- */
    // Purpose: Handles the form input/events, and listens for client connections.
    public partial class MainForm : Form
    {
        /* ----- ATTRIBUTES ----- */
        public volatile bool _shouldStop = false;
        public static Hashtable clientsList = new Hashtable();
        Thread listen;

        // ***Please edit the IP/Port to match your configuration
        public IPAddress IP = IPAddress.Parse("192.168.0.150");
        public int PORT = 5000;


        /*
         * Name       : MainForm
         * Purpose    : Class Constructor. Also starts the listen thread.
         * Parameters : N/A
         * Returns    : N/A
         */
        public MainForm()
        {
            InitializeComponent();
            mainTextBox.Text = " - - - - - SERVER LOG - - - - - " + Environment.NewLine;
            listen = new Thread(StartListening);
            listen.Start();
        }

        /*
         * Name       : StartListening
         * Purpose    : Does what the name says. Listens for clients connections over port 5000, logs client names, and fires off the client handler.
         * Parameters : void
         * Returns    : void
         */
        private void StartListening()
        {
            List<HandleClient> hClients = new List<HandleClient>();
            TcpListener serverSocket = new TcpListener(IP, PORT);
            TcpClient clientSocket = default(TcpClient);
            serverSocket.Start();

            AddText(Environment.NewLine + "Chat Server Started ....");

            while (!_shouldStop)
            {
                clientSocket = serverSocket.AcceptTcpClient();

                if (clientSocket != default(TcpClient))
                {
                    byte[] bytesFrom = new byte[10025];
                    string dataFromClient = null;

                    NetworkStream networkStream = clientSocket.GetStream();
                    networkStream.Read(bytesFrom, 0, (int)clientSocket.ReceiveBufferSize);
                    dataFromClient = System.Text.Encoding.ASCII.GetString(bytesFrom);
                    dataFromClient = dataFromClient.Substring(0, dataFromClient.IndexOf("$"));

                    clientsList.Add(dataFromClient, clientSocket);
                    Broadcast(dataFromClient + " Joined ", dataFromClient, false);

                    AddText(Environment.NewLine + dataFromClient + " Joined chat room");
                    HandleClient client = new HandleClient(this);
                    hClients.Add(client);
                    client.StartClient(clientSocket, dataFromClient, clientsList);
                }
            }

            try
            {
                clientSocket.Close();
                serverSocket.Stop();
                if (hClients != null)
                {
                    foreach (HandleClient clients in hClients)
                    {
                        clients.CleanUp();
                    }
                }
            }
            catch (Exception ex)
            {
                AddText(ex.ToString());
            }
        }

        /*
         * Name       : AddText
         * Purpose    : Thread safe way of appending text to the rich text box.
         * Parameters : string text - text to append
         * Returns    : void
         */
        public void AddText(string text)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(AddText), new object[] { text });
                return;
            }
            mainTextBox.AppendText(text);
        }

        /*
         * Name       : Broadcast
         * Purpose    : Broadcasts a message over the network.
         * Parameters : string msg - message to be broadcast, string uName - user name, bool flag - display a formatted/unformatted message
         * Returns    : void
         */
        public static void Broadcast(string msg, string uName, bool flag)
        {
            foreach (DictionaryEntry Item in clientsList)
            {
                TcpClient broadcastSocket;
                broadcastSocket = (TcpClient)Item.Value;
                NetworkStream broadcastStream = broadcastSocket.GetStream();
                Byte[] broadcastBytes = null;

                if (flag == true)
                {
                    broadcastBytes = Encoding.ASCII.GetBytes(uName + " says : " + msg);
                }
                else
                {
                    broadcastBytes = Encoding.ASCII.GetBytes(msg);
                }

                broadcastStream.Write(broadcastBytes, 0, broadcastBytes.Length);
                broadcastStream.Flush();
            }
        }

        /*
         * Name       : MainForm_FormClosing
         * Purpose    : Cleans up before closing the form.
         * Parameters : object sender, FormClosingEventArgs e
         * Returns    : void
         */
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                if (listen.IsAlive)
                {
                    _shouldStop = true;
                    //.Interrupt();
                    //something isn't cleaning up properly D:
                    listen.Join();
                }
            }
            catch (Exception ex)
            {
                AddText(ex.ToString());
            }
        }

    }
}