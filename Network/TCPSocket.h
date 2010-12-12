// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------
#ifndef _SOCKETS_TCP_SOCKET_H_
#define _SOCKETS_TCP_SOCKET_H_

#include <string>
#ifdef _WIN32
    #include <winsock2.h>
    typedef SOCKET Socket;
#else //_WIN32
    #include <netinet/in.h>
    typedef int Socket;
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

            struct sockaddr_in sa;
            Socket sock;
        #ifdef _WIN32
            int port;
        #else //_WIN32
            string port;
        #endif //_WIN32

        public:
            TCPSocket(int port);
            TCPSocket(Socket init_sock);
            ~TCPSocket();

            bool Connect(string host);
            void Close();

            bool IsOpen() {return open;}
            
            string ReadLine();
            void SendLine(string line);
    };
} // NS Network
} // NS OpenEngine

#endif
