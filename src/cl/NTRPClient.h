#include <Poco/Util/Application.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/FIFOBuffer.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <iostream>

class NTRPClient : public ::Poco::Util::Application {
public:
	NTRPClient()
      : helpRequested(false)
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
	
	int main(const ArgVec& args)
	{
		if (helpRequested)
		{
            displayHelp();
            return ::Poco::Util::Application::EXIT_OK;
		}

        unsigned short port = (unsigned short)config().getInt("NTRPClient.port", 37011);
        ::Poco::Net::SocketAddress address(port);
        ::Poco::Net::StreamSocket socket(address);
        ::Poco::FIFOBuffer buffer(1024);
        socket.receiveBytes(buffer);
        logger().information(buffer.begin());
		return ::Poco::Util::Application::EXIT_OK;
	}
	
private:
	bool helpRequested;
};

