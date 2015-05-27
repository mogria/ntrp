#include "NTRPServerApplication.h"

#include <string>
#include <iostream>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/ServerSocket.h>

#include <lib/ModelHandler.h>
#include <Poco/Data/SQLite/Connector.h>

#include "NTRPConnectionFactory.h"

namespace ntrp {
namespace srv {

NTRPServerApplication::NTRPServerApplication()
  : helpRequested(false)
{
}

NTRPServerApplication::~NTRPServerApplication() {
}

void NTRPServerApplication::initialize(::Poco::Util::Application &self) {
    loadConfiguration();
    ::Poco::Util::ServerApplication::initialize(self);
    logger().information("starting up");
}

void NTRPServerApplication::uninitialize() {
    logger().information("shutting down");
    ::Poco::Util::ServerApplication::uninitialize();
}

void NTRPServerApplication::reinitialize(::Poco::Util::Application &self) {
    ::Poco::Util::ServerApplication::reinitialize(self);
}

void NTRPServerApplication::defineOptions(::Poco::Util::OptionSet &options) {
    ServerApplication::defineOptions(options);

    options.addOption(
        ::Poco::Util::Option("help", "h", "display this help message")
            .required(false)
            .repeatable(false)
            .callback(::Poco::Util::OptionCallback<NTRPServerApplication>(this, &NTRPServerApplication::handleHelp))
    );
}
void NTRPServerApplication::handleHelp(const ::std::string &name, const ::std::string &value) {
    helpRequested = true;
    stopOptionsProcessing();
}

int NTRPServerApplication::main(const ::std::vector<std::string> &args) {
    if(helpRequested) {
        ::Poco::Util::HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("Server for the NTRP game");
        helpFormatter.format(std::cout);
        return ::Poco::Util::Application::EXIT_OK;
    }

    ::Poco::Data::SQLite::Connector::registerConnector();
    ::Poco::Data::Session session("SQLite", "sample.db");
    ::ntrp::model::ModelHandler db(session);

    unsigned short port = (unsigned short)config().getInt("NTRPServer.port", 37011);

    ::Poco::Net::ServerSocket socket(port);
    ::Poco::Net::TCPServer server(new NTRPConnectionFactory(), socket);
    server.start();

    waitForTerminationRequest();

    server.stop();
    return ::Poco::Util::Application::EXIT_OK;
}

};
};
