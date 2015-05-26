#ifndef NTRP_SERVER_APPLICATION_H
#define NTRP_SERVER_APPLICATION_H

#include <string>
#include <vector>
#include <Poco/Util/ServerApplication.h>

namespace ntrp {
namespace srv {


class NTRPServerApplication : public ::Poco::Util::ServerApplication {
    private:
        bool helpRequested;

    public:
        NTRPServerApplication();
        virtual ~NTRPServerApplication();

        void initialize(::Poco::Util::Application &self);
        void uninitialize();
        void reinitialize(::Poco::Util::Application &self);

        virtual void defineOptions(::Poco::Util::OptionSet &options);
        virtual void handleHelp(const ::std::string &name, const ::std::string &value);

        virtual int main(const ::std::vector<std::string> &args);
};

};
};

#endif /* NTRP_SERVER_APPLICATION_H */
