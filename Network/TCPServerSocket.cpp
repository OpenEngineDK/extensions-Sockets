#include "TCPServerSocket.h"

#include <sys/types.h>
#ifndef _WIN32
    #include <sys/socket.h>
    #include <netdb.h>
#endif //_WIN32

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Network {

    TCPServerSocket::TCPServerSocket(int p)
    {
    #ifdef _WIN32
        WSADATA wsaData;
        int error = WSAStartup(MAKEWORD(2,2), &wsaData);
        if(error != NO_ERROR)
        {
            WSACleanup();
            logger.error << "Error starting WSA" << logger.end;
        }
        
        sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
        if(sock==INVALID_SOCKET)
        {
            WSACleanup();
            logger.error << "Error creating basic socket" << logger.end;
        }
        port = p;
    #else //_WIN32
        port = p;
    #endif //_WIN32       
        open = false;
    }

    TCPServerSocket::~TCPServerSocket()
    {
        if(open)
            Close();
    #ifdef _WIN32
        WSACleanup();
    #endif //_WIN32
    }

    bool TCPServerSocket::Listen()
    {
        #ifdef _WIN32
            sa.sin_family = AF_INET;
            sa.sin_addr.s_addr = INADDR_ANY;
            sa.sin_port = htons(port);

            int error = bind(sock, (SOCKADDR*)&sa, sizeof(sa));
            if(error == SOCKET_ERROR)
            {
                logger.error << "Error Binding socket!" << logger.end;
                return false;
            }

            error = listen(sock, 10);
            if(error)
            {
                logger.error << "Error trying to play with socket" << logger.end;
                return false;
            }
        #else //_WIN32
            sa.sin_family = AF_INET;
            sa.sin_addr.s_addr = INADDR_ANY;
            sa.sin_port = htons(port);

            int error = bind(sock, (struct sockaddr*)&sa, sizeof(sa));
            if(error == -1)
            {
                logger.error << "Error Binding socket!" << logger.end;
                return false;
            }

            error = listen(sock, 10);
            if(error)
            {
                logger.error << "Error trying to play with socket" << logger.end;
                return false;
            }
        #endif //_WIN32
        open = true;
        return true;
    }

    void TCPServerSocket::Close()
    {
    #ifdef _WIN32
        closesocket(sock);
    #else
        shutdown(sock,SHUT_RDWR);
        close(sock);
    #endif
        open = false;
    }
  
    //! @TODO: UNIX VERSION
    TCPSocket* TCPServerSocket::Accept()
    {
        if(open)
        {
            #ifdef _WIN32
                SOCKET client;
                sockaddr_in from;
                int fromlen=sizeof(from);
                client=accept(sock, (struct sockaddr*)&from, &fromlen);
            #else //_WIN32
                Socket client;
                sockaddr_in from;
                socklen_t fromlen=sizeof(from);
                client=accept(sock, (struct sockaddr*)&from, &fromlen);
                //UNIX LISTEN
            #endif //_WIN32
                return TCPSocket::Steal(client);
        }
        else
        {
            logger.error << "Trying to accept, without having an open server socket!" << logger.end;
            return 0;
        }
    }
}
}
