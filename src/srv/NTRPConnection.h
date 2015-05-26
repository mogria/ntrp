#ifndef NTRP_CONNECTION_H
#define NTRP_CONNECTION_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>

namespace ntrp {
namespace srv {

class NTRPConnection : public ::Poco::Net::TCPServerConnection {
    public:
        NTRPConnection(const ::Poco::Net::StreamSocket &s)
          : TCPServerConnection(s)
        {
        }
        virtual ~NTRPConnection() { }

        void run() {
            socket().sendBytes("Hello, World!", 13);
        }
};

};
};

#endif /*  NTRP_CONNECTION_H */
