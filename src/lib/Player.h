#ifndef MODEL_PLAYER_H
#define MODEL_PLAYER_H

#include "ModelBase.h"

namespace ntrp {
namespace model {

using ::Poco::Data::Statement;

struct Player : public ModelBase {
    static ModelInfo modelInfo;
    std::string name;
    std::string password;
    std::string salt;
    bool loggedIn;

    Player() : ModelBase(modelInfo) { }
    virtual void use(Statement &statement) {
        statement, ::Poco::Data::Keywords::use(name, "name")
                ,  ::Poco::Data::Keywords::use(password, "password")
                ,  ::Poco::Data::Keywords::use(salt, "salt")
                ,  ::Poco::Data::Keywords::use(loggedIn, "loggedIn");
    }

    virtual void into(Statement &statement) {
        statement, ::Poco::Data::Keywords::into(name)
                 , ::Poco::Data::Keywords::into(password)
                 , ::Poco::Data::Keywords::into(salt)
                 , ::Poco::Data::Keywords::into(loggedIn);
    }
};

ModelInfo Player::modelInfo("players",
{
    {"name", "VARCHAR(30)"}
    {"salt", "VARCHAR(255)"}
    {"password", "VARCHAR(255)"}
    {"password", "INTEGER"}
});

}
}
#endif /* MODEL_PLAYER_H */
