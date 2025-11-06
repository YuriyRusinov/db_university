#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>

#include <pqxx/pqxx>
#include <pqxx/connection>
#include <pqxx/transaction>
#include <pqxx/result>
#include <pqxx/version>

#include "dbpgresult.hpp"
#include "university_pg_db.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::ios_base;
using pqxx::connection;

UniversityPgDb::UniversityPgDb()
    : _dbConnection ( nullptr ),
    _dbWork ( nullptr ), 
    _prepCounter( new int(0) ) {
}

UniversityPgDb::~UniversityPgDb() {
    disconnect();
    std::cout << __PRETTY_FUNCTION__ << ' ' << _prepCounter.use_count() << ' ' << *_prepCounter << std::endl;
    _prepCounter.reset();
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

bool UniversityPgDb::connectStr( std::string connectionString ) const {
    bool retVal = true;
    try {
        _dbConnection = std::make_shared< pqxx::connection >( connectionString );
        retVal = _dbConnection->is_open();
    }
    catch( const std::exception &e ) {
        std::cerr << __PRETTY_FUNCTION__ << ' ' << e.what() << std::endl;
        return false;
    }
    if( retVal ) {
        m_ipServer = _dbConnection->hostname();
        std::string portStr = _dbConnection->port();
        m_port = std::stoi(portStr);
        m_database = _dbConnection->dbname();
        m_user = _dbConnection->username ();
        m_password = std::string();//_password;
    }

    return retVal;
}

bool UniversityPgDb::connect( string _ipServer,
                                string _database,
                                string _user,
                                string _password, 
                                int _port,
                                bool reconnect ) const {
    if( reconnect )
        disconnect();
    stringstream connString;
    connString << " user= " << _user << " password= " << _password << " host=" << _ipServer << " dbname=" << _database << " port=" << _port;
    if( _dbConnection != nullptr )
        _dbConnection.reset();

    bool retVal = true;
    try {
        _dbConnection = std::make_shared< pqxx::connection >( connString.str() );
        retVal = _dbConnection->is_open();
    }
    catch( const std::exception &e ) {
        std::cerr << __PRETTY_FUNCTION__ << ' ' << e.what() << std::endl;
        return false;
    }
    if( retVal ) {
        m_ipServer = _ipServer;
        m_port = _port;
        m_database = _database;
        m_user = _user;
        m_password = _password;
    }

    return retVal;
}

bool UniversityPgDb::connect( bool reconnect ) const {
    return connect( m_ipServer, m_database, m_user, m_password, m_port, reconnect);
}

void UniversityPgDb::disconnect( bool reconnect ) const {
    if ( _dbConnection ) {
        _dbConnection.reset();
    }
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return ;
}

bool UniversityPgDb::connected() const {
    return (_dbConnection != nullptr && _dbConnection->is_open() );
}

std::shared_ptr< DbResult > UniversityPgDb::execute( const char* query ) const {
    if( !_dbConnection )
        return nullptr;
    if( _dbWork )
        _dbWork.reset();
    _dbWork = std::make_shared<pqxx::work >( *_dbConnection );
    string sQuery( query );

    std::shared_ptr<pqxx::result> res = std::make_shared<pqxx::result>(_dbWork->exec( sQuery ));
    if( res == nullptr ) {
        return nullptr;
    }
    std::shared_ptr< DbResult > pRes = std::make_shared< DbPgResult >( res );
    _dbWork->commit();

    return pRes;
}

std::shared_ptr< DbResult > UniversityPgDb::execSQL( const char * sql, ...) const {
    return nullptr;
}

std::shared_ptr< DbResult > UniversityPgDb::execPrepared(
                                const char* stmtName, 
                                int nParams, 
                                const char * const * paramValues, 
                                const int * paramLengths, 
                                const int * paramFormats, 
                                int resultFormat
                               ) const 
{
    return nullptr;
}

std::shared_ptr< DbResult > UniversityPgDb::execParams(
                                const char* command, 
                                int nParams, 
                                const int * paramTypes, 
                                const char * const * paramValues, 
                                const int * paramLengths, 
                                const int * paramFormats, 
                                int resultFormat
                               ) const 
{
    if( _dbConnection == nullptr )
        return nullptr;

    stringstream SQLstr;
    SQLstr << command << ' ' ;
    for(int i=0; i<nParams; i++) {
        SQLstr << "$" << (i+1) << (i<nParams-1 ? ", " : ");");
    }
    cerr << __PRETTY_FUNCTION__ << " SQL query is " << SQLstr.str() << endl;
    (*_prepCounter)++;
    stringstream eStrPC( "execParams", ios_base::app | ios_base::out );
    eStrPC << *_prepCounter;
    string eStr = eStrPC.str();
    _dbConnection->prepare(eStr, SQLstr.str().c_str());
    if( _dbWork )
        _dbWork.reset();
    _dbWork = std::make_shared<pqxx::work>( *_dbConnection );
#if PQXX_VERSION_MAJOR < 7
    std::vector< pqxx::binarystring > vparams;
#elif PQXX_VERSION_MAJOR == 0x07 && PQXX_VERSION_MINOR < 0x06
    std::vector< std::string > vparams;
#else
    pqxx::params vparams;
    vparams.reserve( nParams );
#endif
    for(int i=0; i<nParams; i++) {
        switch( paramTypes[i] ) {
            case DbResult::DataType::dtInt8 :
            case DbResult::DataType::dtInt4 :
            case DbResult::DataType::dtInt2 : {
               long long idValue (atoll( paramValues[i] ));
               stringstream vStr;
               vStr << idValue;
#if PQXX_VERSION_MAJOR < 7
                pqxx::binarystring vstr ( pqxx::to_string(idValue) );
#else
                const void* mStr = static_cast<const void *>( vStr.str().c_str() );
                int n = vStr.str().size();
                std::string vstr( vStr.str() );
#endif
                string s = ( vStr.str() );
                cerr << __PRETTY_FUNCTION__ << s << ' ' << idValue << endl;
#if PQXX_VERSION_MAJOR < 0x07 || (PQXX_VERSION_MAJOR == 0x07 && PQXX_VERSION_MINOR < 0x06)
                vparams.push_back( vstr );
#else
                vparams.append( vstr );
#endif
                break;
            }
            case DbResult::DataType::dtVarchar : default: {
                char* paramData = new char [paramLengths[i]];
                strncpy( paramData, paramValues[i], paramLengths[i]);
                cerr << __PRETTY_FUNCTION__ << ' ' << paramData << endl;
                string esc_str = _dbConnection->esc(paramData);
                cerr << __PRETTY_FUNCTION__ << ' ' << esc_str << endl;
#if PQXX_VERSION_MAJOR < 7
                pqxx::binarystring vchar( esc_str.c_str(), paramLengths[i]);
#else
                string pStr( paramValues[i], paramLengths[i]);
                const void* mStr = (const void *)paramValues[i];
                std::string vchar( pStr );
                cerr << __PRETTY_FUNCTION__ << ' ' << vchar;
#endif
#if PQXX_VERSION_MAJOR < 0x07 || (PQXX_VERSION_MAJOR == 0x07 && PQXX_VERSION_MINOR < 0x06)
                vparams.push_back( vchar );
#else
                vparams.append( vchar );
#endif
                delete [] paramData;
                break;
            }
            case DbResult::DataType::dtBytea : {
                void* paramData = (void *)paramValues[i];
                int paramSize = paramLengths[i];
#if PQXX_VERSION_MAJOR < 7
                pqxx::binarystring blob(paramData, paramSize);
#else
                const void* blobV = static_cast<const void *>(paramData);
                std::basic_string_view<std::byte> blob0( static_cast<const std::byte*>(blobV), paramSize);
                string blob = _dbConnection->esc_raw( blob0 );
#endif
#if PQXX_VERSION_MAJOR < 0x07 || (PQXX_VERSION_MAJOR == 0x07 && PQXX_VERSION_MINOR < 0x06)
                vparams.push_back( blob );
#else
                vparams.append( blob );
#endif
                break;
            }
        }
    }
    std::shared_ptr<pqxx::result> res = nullptr;
    std::cerr << __PRETTY_FUNCTION__ << std::hex << PQXX_VERSION_MAJOR << ' ' << std::hex << PQXX_VERSION_MINOR;
#if PQXX_VERSION_MAJOR < 0x07 || (PQXX_VERSION_MAJOR == 0x07 && PQXX_VERSION_MINOR < 0x06 )
    try {
        res = std::make_shared< pqxx::result >(_dbWork->exec_prepared(eStr, pqxx::prepare::make_dynamic_params(vparams)));
    }
#else
    try {
        res = std::make_shared< pqxx::result >(_dbWork->exec_prepared(eStr, vparams) );
    }
#endif
    catch( pqxx::failure& e) {
        cerr << __PRETTY_FUNCTION__ << ' ' << e.what();
        this->rollback();
        return nullptr;
    }

    if (std::empty(*res)) {
        this->rollback();
        throw std::runtime_error{"query does not executed!"};
    }
    _dbWork->commit();
    std::shared_ptr< DbResult > rParamRes = std::make_shared< DbPgResult > (res );
    _dbWork.reset();
    return rParamRes;
}

string UniversityPgDb::escapeAsciiString(const char * fromString) const {
    if(fromString == nullptr || _dbConnection == nullptr)
        return string();

    return _dbConnection->esc( fromString );
}

string UniversityPgDb::escapeBinaryString(const unsigned char * fromString) const {
    if(fromString == nullptr || _dbConnection == nullptr)
        return string();

    int n = strlen( (char *)fromString );
    //string fStr( (char *)fromString, n);
#if PQXX_VERSION_MAJOR < 7
    return _dbConnection->esc_raw( fromString, n );
#else
    const void* mStr = static_cast<const void *>( fromString );
    std::basic_string<std::byte> bFromString( static_cast<const std::byte *>(mStr), n);
    return _dbConnection->esc_raw( bFromString );
#endif
}

bool UniversityPgDb::begin() const {
    if( _dbConnection == nullptr || _dbWork != nullptr )
        return false;

    _dbWork = std::make_shared<pqxx::work>( *_dbConnection );
    return true;
}

bool UniversityPgDb::commit() const {
    if( _dbConnection == nullptr || _dbWork == nullptr )
        return false;

    _dbWork->commit();
    return true;
}

bool UniversityPgDb::rollback() const {
    if( _dbConnection == nullptr || _dbWork == nullptr )
        return false;

    _dbWork.reset();
    return true;
}
