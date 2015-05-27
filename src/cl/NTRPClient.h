#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <iostream>
#include <lib/JSON.h>
#include <lib/Player.h>
#include <lib/ModelHandler.h>
#include <Poco/Data/SQLite/Connector.h>

class NTRPClient : public ::Poco::Util::Application {
private:
	bool helpRequested;
    ::Poco::Net::StreamSocket socket;
    ::Poco::FIFOBuffer buffer;

public:
	NTRPClient()
      : helpRequested(false),
        buffer(1024)
	{
	}

    virtual ~NTRPClient()
    {
    }

protected:	
	void initialize(::Poco::Util::Application &self)
	{
		loadConfiguration();
		::Poco::Util::Application::initialize(self);
	}
	
	void uninitialize()
	{
		::Poco::Util::Application::uninitialize();
	}
	
	void reinitialize(::Poco::Util::Application &self)
	{
		::Poco::Util::Application::reinitialize(self);
	}
	
	void defineOptions(::Poco::Util::OptionSet &options)
	{
		::Poco::Util::Application::defineOptions(options);

		options.addOption(
			::Poco::Util::Option("help", "h", "display this help message")
				.required(false)
				.repeatable(false)
				.callback(::Poco::Util::OptionCallback<NTRPClient>(this, &NTRPClient::handleHelp)));

	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		helpRequested = true;
		stopOptionsProcessing();
	}
	
	void displayHelp()
	{
		::Poco::Util::HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("command line client for the NTRP game");
		helpFormatter.format(std::cout);
	}

    ::Poco::JSON::Object::Ptr sendCommand(std::string command, ::Poco::JSON::Object::Ptr arguments) {
        ::Poco::JSON::Object::Ptr object = new ::Poco::JSON::Object;
        object->set("command", command);
        object->set("arguments", arguments);
        ntrp::json::toBuffer(object, buffer);
        socket.sendBytes(buffer);
        socket.receiveBytes(buffer);
        return ntrp::json::fromBuffer(buffer);
    }
	
	int main(const ArgVec& args)
	{
		if (helpRequested)
		{
            displayHelp();
            return ::Poco::Util::Application::EXIT_OK;
		}

        unsigned short port = (unsigned short)config().getInt("NTRPClient.port", 37011);
        ::Poco::Net::SocketAddress address(port);
        socket.connect(address);
        ::Poco::JSON::Object::Ptr arg = new ::Poco::JSON::Object;
        arg->set("text", "void");
        sendCommand("test", arg);
        logger().information(buffer.begin());
		return ::Poco::Util::Application::EXIT_OK;
	}
};

