#include "TCPSocket.h"

#include <cstring>
#include <sys/types.h>
#ifndef _WIN32
    #include <sys/socket.h>
    #include <netdb.h>
#endif //_WIN32

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Network {

    TCPSocket::TCPSocket(int p)
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
        ostringstream out;
        out << p;
        port = out.str();
    #endif //_WIN32       
        open = false;
    }

    TCPSocket::~TCPSocket()
    {
        if(open)
            Close();
    #ifdef _WIN32
        WSACleanup();
    #endif //_WIN32
    }

    //! @TODO: Needs work (windows port)
    void TCPSocket::Close()
    {
    #ifdef _WIN32
        closesocket(sock);
    #else
        shutdown(sock,SHUT_RDWR);
        close(sock);
    #endif
        open = false;
    }

    bool TCPSocket::Connect(string h)
    {
        host = h;        
        #ifdef _WIN32
            LPHOSTENT hostEntry;
            hostEntry = gethostbyname(host.c_str());
            
            sa.sin_family = AF_INET;
            sa.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);
            sa.sin_port = htons(port);

            int error = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
            if(error == SOCKET_ERROR)
            {
                logger.error << "Error Connecting socket!" << logger.end;
                return false;
            }
        #else //_WIN32
            struct addrinfo hints, *res, *res0;
            int error;
            int s;
            const char *cause = NULL;

            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            error = getaddrinfo(host.c_str(), port.c_str(), &hints, &res0);
            if(error)
            {
                logger.error << "Socket error: " <<gai_strerror(error) << logger.end;
                return false;
                //errx(1, "%s", gai_strerror(error));
                /*NOTREACHED*/
            }
            s = -1;
            for(res = res0; res; res = res->ai_next)
            {            
                s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
                if(s<0)
                {
                    cause = "socket";
                    continue;
                }

                if(connect(s, res->ai_addr, res->ai_addrlen) < 0)
                {
                    cause = "connect";
                    close(s);
                    s = -1;
                    continue;
                }
                break;  /* okay we got one */
            }
            if (s<0)
            {
                logger.error << "Socket error: " << cause << logger.end;
                return false;
                //err(1, "%s", cause);
                /*NOTREACHED*/
            }
            sock = s;
            freeaddrinfo(res0);
        #endif //_WIN32
        open = true;
        return true;
    }

    string TCPSocket::ReadLine()
    {
        ostringstream out;
        char buffer;
        do
        {            
            int err = recv(sock, &buffer, 1, 0);
            if (err<0)
            {
                open = false;
                return "";
            }
            out << buffer;
        } while(buffer != '\n');
        return out.str();
    }

    void TCPSocket::SendLine(string line)
    {
        int err = send(sock, line.c_str(), line.length(), 0);
        if (err < 0)
            open = false;
    }

}
}
