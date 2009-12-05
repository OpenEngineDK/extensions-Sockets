#include "TCPSocket.h"

namespace OpenEngine {
namespace Network {

    TCPSocket::TCPSocket(int port) : port(port) {

    }

    void TCPSocket::Connect(string h) {
        host = h;
    }

}
}
