#ifndef MODEL_BASE_H
#define MODEL_BASE_H

#include <Poco/Data/Statement.h>

namespace ntrp {
namespace model {

using ::Poco::Data::Statement;

struct ModelInfo {
    std::string tableName;
    std::map<std::string, std::string> fieldTypeMap;
    ModelInfo(std::string tableName, std::map<std::string, std::string> fieldTypeMap) : tableName(tableName), fieldTypeMap(fieldTypeMap) {}
};

struct ModelBase {
    ModelInfo &info;
    unsigned int id;

    ModelBase(ModelInfo &info) : info(info), id(0) {}
    virtual void use(Statement &statement) {
        statement, ::Poco::Data::Keywords::use(id, "id");
    }
    virtual void into(Statement &statement) {
        statement, ::Poco::Data::Keywords::into(id);
    }
};

}
}

#endif /* MODEL_BASE_H */
