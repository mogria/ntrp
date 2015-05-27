#ifndef MODEL_PLAYER_H
#define MODEL_PLAYER_H

#include "ModelBase.h"

namespace ntrp {
namespace model {

using ::Poco::Data::Statement;

struct Player : public ModelBase {
    static ModelInfo modelInfo;
    std::string name;

    Player() : ModelBase(modelInfo) { }
    virtual void use(Statement &statement) {
        statement, ::Poco::Data::Keywords::use(name, "name");
    }

    virtual void into(Statement &statement) {
        statement, ::Poco::Data::Keywords::into(name);
    }
};

ModelInfo Player::modelInfo("players",
{
    {"name", "VARCHAR(30)"}
});

}
}
#endif /* MODEL_PLAYER_H */
