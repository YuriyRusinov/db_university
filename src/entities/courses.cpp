#include "department.hpp"
#include "courses.hpp"

Course::Course( int id,
        std::string code,
        std::string name,
        std::string description,
        int mark,
        std::shared_ptr< Department > department,
        std::string professor,
        int max_capacity,
        bool isActive
      ) :
    m_id( id ),
    m_code( code ),
    m_name( name ),
    m_description( description ),
    m_mark( mark ),
    m_department( department ),
    m_professor( professor ),
    m_max_capacity( max_capacity ),
    m_isActive( isActive )
{
}
