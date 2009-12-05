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
    int port;
    string host;
public:

    TCPSocket(int port);
    void Connect(string host);
    
    
};


} // NS Network
} // NS OpenEngine

#endif
