#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <pqxx/pqxx>

#include "dbresult.hpp"

#include "university_db.hpp"

class UniversityPgDb : public UniversityDb {
public:
    UniversityPgDb();
    UniversityPgDb( const UniversityPgDb& ) = delete;
    UniversityPgDb& operator=( const UniversityPgDb& ) = delete;
    virtual ~UniversityPgDb();

    virtual bool connectStr( std::string connectionString ) const override;
    virtual bool connect( string _ipServer = string(),
                          string _database = string(),
                          string _user = string(),
                          string _password = string(), 
                          int _port = 5432,
                          bool reconnect = false ) const override;

    virtual bool connect( bool reconnect = false ) const override;
    virtual void disconnect( bool reconnect = false ) const override;
    virtual bool connected() const override;

    virtual std::shared_ptr< DbResult > execute( const char* query ) const override;
    virtual std::shared_ptr< DbResult > execSQL( const char * sql, ...) const override;

    virtual std::shared_ptr< DbResult > execPrepared(
                                        const char* stmtName,
                                        int nParams,
                                        const char * const * paramValues,
                                        const int * paramLengths,
                                        const int * paramFormats,
                                        int resultFormat
                                       ) const override;

    virtual std::shared_ptr< DbResult > execParams(
                                        const char* command,
                                        int nParams,
                                        const int * paramTypes,
                                        const char * const * paramValues,
                                        const int * paramLengths,
                                        const int * paramFormats,
                                        int resultFormat
                                       ) const override;
    virtual string   escapeAsciiString(const char * fromString) const override;
    virtual string   escapeBinaryString(const unsigned char * fromString) const override;

    virtual bool begin() const override;
    virtual bool commit() const override;
    virtual bool rollback() const override;

private:
    mutable std::shared_ptr<pqxx::connection> _dbConnection;
    mutable std::shared_ptr<pqxx::work> _dbWork;
    std::shared_ptr<int> _prepCounter;
};
