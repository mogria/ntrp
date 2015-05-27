#ifndef NTRP_CONNECTION_H
#define NTRP_CONNECTION_H

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Dynamic/Var.h>
#include <lib/JSON.h>

namespace ntrp {
namespace srv {

class NTRPConnection : public ::Poco::Net::TCPServerConnection {
    private:
        Poco::FIFOBuffer sendBuffer;
        Poco::FIFOBuffer receiveBuffer;

    public:
        NTRPConnection(const ::Poco::Net::StreamSocket &s)
          : TCPServerConnection(s),
            receiveBuffer(1024),
            sendBuffer(1024)
        {
        }
        virtual ~NTRPConnection() { }

        void sendCommandResponse(std::string command, Poco::JSON::Object::Ptr result) {
            Poco::JSON::Object::Ptr object = new Poco::JSON::Object;
            object->set("command", command);
            object->set("results", result);
            ntrp::json::toBuffer(object, sendBuffer);
            socket().sendBytes(sendBuffer);
        }

        void receiveCommand() {
            socket().receiveBytes(receiveBuffer);
            ::Poco::JSON::Object::Ptr object = ntrp::json::fromBuffer(receiveBuffer);

            std::string commandName;
            object->get("command").convert<std::string>(commandName);

            Poco::JSON::Object::Ptr arguments = object->get("arguments").extract<Poco::JSON::Object::Ptr>();
            handleCommand(commandName, arguments);
        }

        void handleCommand(const std::string command, Poco::JSON::Object::Ptr arguments) {
            Poco::JSON::Object::Ptr results;
            if(command == "register") results = registerCommand(arguments);
            if(command == "login") results = registerCommand(arguments);
            else                      results = unknownCommnad;
            sendCommandResponse(command, results);
        }

        void registerCommand(Poco::JSON::Object::Ptr arguments) {
            Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
            ntrp::model::Player newPlayer;
            newPlayer.name = arguments->get("name").convert<std::string>(text);
            object->set("success", true);
            return result;
        }

        void loginCommand(Poco::JSON::Object::Ptr arguments) {
            Poco::JSON::Object::Ptr result = new Poco::JSON::Object;
            object->set("error", true);
            return result;
        }

        void run() {
            receiveCommand();
        }
};

};
};

#endif /*  NTRP_CONNECTION_H */
