// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------
#ifndef _SOCKETS_TCP_SOCKET_H_
#define _SOCKETS_TCP_SOCKET_H_

//! @TODO: Add server function or whatever, to let this work as server.

#include <string>
#ifdef _WIN32
    #include <winsock2.h>
#else //_WIN32
    #include <netinet/in.h>
#endif //_WIN32

namespace OpenEngine {
namespace Network {

    /**
     * Short description.
     *
     * @class TCPSocket TCPSocket.h ons/Sockets/Network/TCPSocket.h
     */

    using namespace std;

    class TCPSocket
    {
        private:
            string host;
            volatile bool open;

        #ifdef _WIN32
            int port;
            sockaddr_in sa;
            SOCKET sock;
        #else //_WIN32
            string port;
            struct sockaddr_in sa;
            int sock;
        #endif //_WIN32

        public:
            TCPSocket(int port);
            ~TCPSocket();

            bool Connect(string host);
            bool IsOpen() {return open;}
            void Close();

            string ReadLine();
            void SendLine(string line);
    };
} // NS Network
} // NS OpenEngine

#endif
