// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------
#ifndef _SOCKETS_TCP_SERVER_SOCKET_H_
#define _SOCKETS_TCP_SERVER_SOCKET_H_

#include "TCPSocket.h"
#include "TCPTypes.h"

namespace OpenEngine {
namespace Network {

    /**
     * Short description.
     *
     * @class TCPServerSocket TCPServerSocket.h ons/Sockets/Network/TCPServerSocket.h
     */

    using namespace std;

    class TCPServerSocket
    {
        private:
            volatile bool open;

            struct sockaddr_in sa;
            Socket sock;

            int port;


        public:
            TCPServerSocket(int port);
            ~TCPServerSocket();

            bool Listen();
            void Close();
            TCPSocket* Accept();
    
            bool IsOpen() {return open;}
    };
} // NS Network
} // NS OpenEngine

#endif
