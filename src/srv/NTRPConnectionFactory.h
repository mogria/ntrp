#ifndef NTRP_CONNECTION_FACTORY_H
#define NTRP_CONNECTION_FACTORY_H

#include <Poco/Net/TCPServerConnectionFactory.h>
#include "NTRPConnection.h"
#include <lib/ModelHandler.h>

namespace ntrp {
namespace srv {

class NTRPConnectionFactory : public ::Poco::Net::TCPServerConnectionFactory {
    private:
        ::ntrp::model::ModelHandler &db;
    public:
        NTRPConnectionFactory() { }
        virtual ~NTRPConnectionFactory() { }
        ::Poco::Net::TCPServerConnection *createConnection(const ::Poco::Net::StreamSocket &socket) {
            return new NTRPConnection(socket);
        }
};

};
};

#endif /* NTRP_CONNECTION_FACTORY_H */
