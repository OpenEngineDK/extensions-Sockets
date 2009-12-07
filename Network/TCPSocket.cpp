#include "TCPSocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <Logging/Logger.h>

namespace OpenEngine {
namespace Network {

    TCPSocket::TCPSocket(int p)  {
#ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORK(2,2), &wsaData);
#endif        
        ostringstream out;
        out << p;
        port = out.str();
        open = false;
    }

    void TCPSocket::Close() {
        close(sock);
        open = false;
    }

    void TCPSocket::Connect(string h) {
        host = h;        

        struct addrinfo hints, *res, *res0;
        int error;
        int s;
        const char *cause = NULL;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        error = getaddrinfo(host.c_str(), port.c_str(), &hints, &res0);
        if (error) {
            logger.error << "Socket error: " <<gai_strerror(error) << logger.end;
            //errx(1, "%s", gai_strerror(error));
            /*NOTREACHED*/
        }
        s = -1;
        for (res = res0; res; res = res->ai_next) {            
            s = socket(res->ai_family, res->ai_socktype,
                       res->ai_protocol);
            if (s < 0) {
                cause = "socket";
                continue;
            }

            if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
                cause = "connect";
                close(s);
                s = -1;
                continue;
            }

            break;  /* okay we got one */
        }
        if (s < 0) {
            logger.error << "Socket error: " << cause << logger.end;
            //err(1, "%s", cause);
            /*NOTREACHED*/
        }
        
        sock = s;
        open = true;

        freeaddrinfo(res0);
    }

    string TCPSocket::ReadLine() {
        ostringstream out;
        char buffer;
        do {            
            int err = recv(sock, &buffer, 1, 0);
            if (err < 0) {
                open = false;
                return "";
            }
            out << buffer;
        } while(buffer != '\n');
        return out.str();
    }

    void TCPSocket::SendLine(string line) {

        int err = send(sock, line.c_str(), line.length(), 0);
        if (err < 0)
            open = false;
    }

}
}
