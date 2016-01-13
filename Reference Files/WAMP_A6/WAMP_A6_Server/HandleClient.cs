/*
 * FILE        : HandleClient.cs
 * PROJECT     : Windows and Mobile Programming - Assignment #6
 * AUTHOR      : Richard Meijer
 * SUBMIT DATE : 11/04/2014
 * DESCRIPTION : Class to handle client threads and chat logic.
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
using System.Net.Sockets;
using System.Windows.Forms;



/* ----- NAMESPACE ----- */
namespace WAMP_A6_Server
{
    /* ----- CLASS ----- */
    // Purpose: Handles client threads and chat logic.
    public class HandleClient
    {
        /* ----- ATTRIBUTES ----- */
        public volatile bool _shouldStop = false;
        public List<Thread> threads = new List<Thread>();
        TcpClient clientSocket;
        string clName;
        Hashtable clientsList;
        MainForm formObj;

        /*
         * Name       : HandleClient
         * Purpose    : Class Constructor. Saves the MainForm object for later use.
         * Parameters : N/A
         * Returns    : N/A
         */
        public HandleClient(MainForm obj)
        {
            formObj = obj;
        }

        /*
         * Name       : StartClient
         * Purpose    : Starts the client handler thread.
         * Parameters : TcpClient inClientSocket - saves object reference, string cName - client name, Hashtable cList - list of clients
         * Returns    : void
         */
        public void StartClient(TcpClient inClientSocket, string cName, Hashtable cList)
        {
            this.clientSocket = inClientSocket;
            this.clName = cName;
            this.clientsList = cList;
            Thread ctThread = new Thread(HandleChat);
            ctThread.Start();
            threads.Add(ctThread);
        }

        /*
         * Name       : CleanUp
         * Purpose    : Attempts to end all threads gracefully.
         * Parameters : void
         * Returns    : void
         */
        public void CleanUp()
        {
            try
            {
                _shouldStop = true;
                if (threads != null)
                {
                    foreach (Thread thread in threads)
                    {
                        thread.Interrupt();
                        //thread.Join();
                    }
                }
            }
            catch (Exception ex)
            {
                formObj.AddText(ex.ToString());
            }
        }

        /*
         * Name       : HandleChat
         * Purpose    : Responsible for managing client chat logic. Listens for client data and broadcasts appropriately.
         * Parameters : void
         * Returns    : void
         */
        private void HandleChat()
        {
            int requestCount = 0;
            byte[] bytesFrom = new byte[10025];
            string dataFromClient = null;
            string rCount = null;
            requestCount = 0;

            while (!_shouldStop)
            {
                try
                {
                    requestCount = requestCount + 1;
                    NetworkStream networkStream = clientSocket.GetStream();
                    networkStream.Read(bytesFrom, 0, (int)clientSocket.ReceiveBufferSize);
                    dataFromClient = System.Text.Encoding.ASCII.GetString(bytesFrom);
                    dataFromClient = dataFromClient.Substring(0, dataFromClient.IndexOf("$"));

                    formObj.AddText(Environment.NewLine + "From client - " + clName + " : " + dataFromClient);

                    rCount = Convert.ToString(requestCount);

                    MainForm.Broadcast(dataFromClient, clName, true);
                }
                catch (Exception ex)
                {
                    formObj.AddText(Environment.NewLine + ex.ToString());
                }
            }
        }

    }
}