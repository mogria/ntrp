#ifndef LIB_JSON_H
#define LIB_JSON_H

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/FIFOBuffer.h>

namespace ntrp {
namespace json {

void toBuffer(::Poco::JSON::Object::Ptr object, ::Poco::FIFOBuffer &buffer) {
    Poco::JSON::Stringifier stringifier;
    std::stringstream os;
    stringifier.stringify(object, os);
    std::string jsonString = os.str();
    buffer.write(jsonString.c_str(), jsonString.size());
}

::Poco::JSON::Object::Ptr fromBuffer(::Poco::FIFOBuffer &buffer) {
    Poco::JSON::Parser parser;
    std::string jsonString(buffer.begin(), buffer.used());
    buffer.drain(buffer.used());

    ::Poco::Dynamic::Var parsed = parser.parse(jsonString);
    return parsed.extract<Poco::JSON::Object::Ptr>();
}

};
};

#endif /* LIB_JSON_H */
