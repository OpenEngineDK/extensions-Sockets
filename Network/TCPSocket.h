#ifndef _SOCKETS_TCP_SOCKET_H_
#define _SOCKETS_TCP_SOCKET_H_
// 
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include <string>

#include <netinet/in.h>

namespace OpenEngine {
namespace Network {

/**
 * Short description.
 *
 * @class TCPSocket TCPSocket.h ons/Sockets/Network/TCPSocket.h
 */

using namespace std;

class TCPSocket {
    

private:
    string port;
    string host;
    volatile bool open;

    struct sockaddr_in sa;
    
#ifdef _WIN32
    SOCKET sock;
#else
    int sock;
#endif
    
public:

    TCPSocket(int port);
    void Connect(string host);

    bool IsOpen() {return open;}
    
    void Close();

    string ReadLine();
    void SendLine(string line);
    
};


} // NS Network
} // NS OpenEngine

#endif
