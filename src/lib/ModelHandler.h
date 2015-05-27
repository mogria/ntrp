#ifndef MODEL_HANDLER_H
#define MODEL_HANDLER_H

#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/Session.h>
#include <iostream>

namespace ntrp {
namespace model {

using namespace ::Poco::Data::Keywords;
using ::Poco::Data::Session;
using ::Poco::Data::Statement;

class ModelHandler {
    private:
        Session &session;
    public:
        ModelHandler(Session &session) : session(session) { };

        virtual ~ModelHandler() {}

        void createTable(ModelInfo &info) {
            session << "DROP TABLE IF EXISTS %s", info.tableName, now;
            std::string tableFields = "id INTEGER PRIMARY KEY";
            std::for_each(info.fieldTypeMap.begin(),
                          info.fieldTypeMap.end(),
                          [&tableFields] (std::pair<std::string, std::string> pair) {
                tableFields += ", " + pair.first + " " + pair.second;
            });
            session << "CREATE TABLE %s (%s)", info.tableName, tableFields, now;
        }

        void save(ModelBase &base) {
            if(base.id == 0) {
                insert(base);
                return;
            }

            std::string tableFields = "";
            std::for_each(base.info.fieldTypeMap.begin(),
                          base.info.fieldTypeMap.end(),
                          [&tableFields] (std::pair<std::string, std::string> pair) {
                if(tableFields.size() > 0) tableFields += ", ";
                tableFields += pair.first + " = :" + pair.first;
            });

            std::string sql = "UPDATE " + base.info.tableName + "  SET " + tableFields + " WHERE id = :id";

            Statement updateSet(session);
            updateSet << sql;
            base.use(updateSet);
            base.ModelBase::use(updateSet);
            updateSet.execute();
        }

        void insert(ModelBase &base) {
            std::string fields = "";
            std::string values = "";
            std::for_each(base.info.fieldTypeMap.begin(),
                          base.info.fieldTypeMap.end(),
                          [&fields, &values] (std::pair<std::string, std::string> pair) {
                if(fields.size() > 0) fields += ", ";
                if(values.size() > 0) fields += ", ";
                fields += pair.first;
                values += ":" + pair.first;
            });

            std::string sql = "INSERT INTO " + base.info.tableName
                            + "(" + fields + ") VALUES (" + values + ")";
            Statement insertInto(session);
            insertInto << sql;
            base.use(insertInto);
            insertInto.execute();
            unsigned int id;
            Statement lastInsertId(session);
            lastInsertId << "SELECT last_insert_rowid() FROM %s",  base.info.tableName, into(id), now;
            base.id = id;
        }

        void fetch(ModelBase &base) {
            Statement select(session);

            std::string fields = "";
            std::for_each(base.info.fieldTypeMap.begin(),
                          base.info.fieldTypeMap.end(),
                          [&fields] (std::pair<std::string, std::string> pair) {
                if(fields.size() > 0) fields += ", ";
                fields += pair.first;
            });

            std::string sql = "SELECT " + fields + " FROM " + base.info.tableName + " WHERE id = :id";
            select << sql;
            base.ModelBase::use(select);
            base.into(select);
            select.execute();
        }
};

}
}

#endif /* MODEL_HANDLER_H */
