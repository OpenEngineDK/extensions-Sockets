#ifndef _SOCKETS_TCP_TYPES_H
#define _SOCKETS_TCP_TYPES_H

#ifdef _WIN32
    #include <winsock2.h>
    typedef SOCKET Socket;
#else //_WIN32
    #include <netinet/in.h>
    typedef int Socket;
#endif //_WIN32

#endif //_SOCKETS_TCP_TYPES_H
