#include "TCPSocket.h"

#include <sys/types.h>
#ifndef _WIN32
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#endif //_WIN32

#include <Logging/Logger.h>

namespace OpenEngine {
    namespace Network {

        TCPSocket::TCPSocket() {
            open = false;
        }

        TCPSocket::TCPSocket(int p)
        {
#ifdef _WIN32
            WSADATA wsaData;
            int error = WSAStartup(MAKEWORD(2,2), &wsaData);
            if(error != NO_ERROR)
                {
                    WSACleanup();
                    logger.error << "Error starting WSA" << logger.end;
                    logger.error << "ErrorCode: " << WSAGetLastError() << logger.end;
                }
        
            sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
            if(sock==INVALID_SOCKET)
                {
                    WSACleanup();
                    logger.error << "Error creating basic socket" << logger.end;
                    logger.error << "ErrorCode: " << WSAGetLastError() << logger.end;
                }
            port = p;
#else //_WIN32
            ostringstream out;
            out << p;
            port = out.str();
#endif //_WIN32       
            open = false;
        }

        TCPSocket* TCPSocket::Steal(Socket init_sock)
        {
            TCPSocket* s = new TCPSocket();
#ifdef _WIN32
            WSADATA wsaData;
            int error = WSAStartup(MAKEWORD(2,2), &wsaData);
            if(error != NO_ERROR)
                {
                    WSACleanup();
                    logger.error << "Error starting WSA" << logger.end;
                    logger.error << "ErrorCode: " << WSAGetLastError() << logger.end;
                }

            s->sock = init_sock;
            if(s->sock==INVALID_SOCKET)
                {
                    WSACleanup();
                    logger.error << "Error creating basic socket" << logger.end;
                    logger.error << "ErrorCode: " << WSAGetLastError() << logger.end;
                }
#else //_WIN32
            s->sock = init_sock;
#endif //_WIN32
            s->open = true;
            return s;
        }

        TCPSocket::~TCPSocket()
        {
            if(open)
                Close();
#ifdef _WIN32
            WSACleanup();
#endif //_WIN32
        }

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
                    logger.error << "ErrorCode: " << WSAGetLastError() << logger.end;
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

        string TCPSocket::ReadLine(string termination)
        {
            ostringstream out;
            char buffer;
            int status=0;
            bool useTermChars = (termination.length() > 0);
            do
                {       
                    status = recv(sock, &buffer, 1, 0);
                    if (status==0) //Connection Closed
                        {
                            open = false;
                            return "";
                        }
                    else if (status<0) //Connection Broke
                        {
                            open=false;
#ifdef _WIN32
                            logger.info << "Socket connection failed with ErrorCode:\t" << WSAGetLastError() << logger.end;
#else //_WIN32
                            //! @TODO: Make a unix error message?
#endif //_WIN32
                            return "";
                        }
                    out << buffer;
                    // If termination characters are defined check if received message is terminated.
                    string recv = out.str();
                    unsigned int termLen = termination.length();
                    if( useTermChars && recv.length() >= termLen ){
                        if( recv.compare(recv.length()-termLen, termLen, termination) == 0 ){
                            // Message terminated
                            return out.str();
                        }
                    }


                } while(status > 0); // status>0 means we're still recieving data
            return out.str();
        }

        void TCPSocket::SendLine(string line)
        {
            int err = send(sock, line.c_str(), line.length(), 0);
            if (err < 0)
                {
                    open = false;
                }
        }
    }}
