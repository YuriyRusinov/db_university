#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>

using std::string;
using std::vector;
using std::variant;

class DbResult;

class UniversityDb {
public:
    UniversityDb();
    UniversityDb( const  UniversityDb& ) = delete;
    UniversityDb& operator= ( const UniversityDb& ) = delete;
    virtual ~UniversityDb();
    virtual bool connectStr( std::string connectionString ) const = 0;
    virtual bool connect( std::string _ipServer = std::string(),
                          std::string _database = std::string(),
                          std::string _user = std::string(),
                          std::string _password = std::string(),
                          int _port = 5432,
                          bool reconnect = false ) const = 0;

    virtual bool connect( bool reconnect = false ) const = 0;
    virtual void disconnect( bool reconnect = false ) const = 0;
    virtual bool connected() const = 0;

    virtual std::shared_ptr< DbResult > execute( const char* query ) const = 0;
    std::shared_ptr< DbResult > execute( const string & query) const;
    int executeCmd( const string & sql ) const;
    virtual std::shared_ptr< DbResult > execSQL( const char * sql, ...) const = 0;

    virtual std::shared_ptr< DbResult > execPrepared(
                                        const char* stmtName,
                                        int nParams,
                                        const char * const * paramValues,
                                        const int * paramLengths,
                                        const int * paramFormats,
                                        int resultFormat
                                       ) const = 0;


    virtual std::shared_ptr<DbResult> execParams(
                                        const char* command,
                                        int nParams,
                                        const int * paramTypes,
                                        const char * const * paramValues,
                                        const int * paramLengths,
                                        const int * paramFormats,
                                        int resultFormat
                                       ) const = 0;

    virtual string   escapeAsciiString(const char * fromString) const = 0;
    virtual string   escapeBinaryString(const unsigned char * fromString) const = 0;

    virtual bool begin() const = 0;
    virtual bool commit() const = 0;
    virtual bool rollback() const = 0;

    const std::string& getHost() const { return m_ipServer; }
    int getPort() const { return m_port; }
    const std::string& getDb() const { return m_database; }
    const std::string& getUser() const { return m_user; }
    const std::string& getPassword() const { return m_password; }

protected:
    mutable std::string m_ipServer;
    mutable int m_port;
    mutable std::string m_database;
    mutable std::string m_user;
    mutable std::string m_password;

};
