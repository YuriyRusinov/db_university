#include <iostream>
#include "university_db.hpp"

UniversityDb::UniversityDb() :
    m_ipServer( std::string("localhost") ),
    m_port( 5432 ),
    m_database( std::string("template1") ),
    m_user( std::string("postgres") ),
    m_password( std::string() )
{

}

UniversityDb::~UniversityDb() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::shared_ptr< DbResult > UniversityDb::execute( const string & query) const {
    return execute( query.c_str() );
}
