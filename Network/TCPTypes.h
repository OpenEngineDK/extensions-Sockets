#ifndef _SOCKETS_TCP_TYPES_H
#define _SOCKETS_TCP_TYPES_H

#ifdef _WIN32
    #include <winsock2.h>
    typedef SOCKET Socket;
#else //_WIN32
    #include <netinet/in.h>
    typedef int Socket;
#endif //_WIN32

/*
 * These status flags are not used directly by the 
 * current socket implementation but they might be use-full 
 * for the concrete implementation.
 */
typedef enum {
    NOT_CONNECTED,   // Idle and not connected yet.
    CONNECTING,      // Connection in progress.
    CONNECTED,       // Connected successfully and receiving data.
    CONNECTION_ERR,  // Could not connect.
    DATA_ERR,        // Connected but there is IO crap.
} NetStat;

#endif //_SOCKETS_TCP_TYPES_H
