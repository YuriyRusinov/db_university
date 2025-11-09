#include "dbdepartment.hpp"

Department::Department( int id,
            std::string department_code,
            std::string department_name,
            std::string chair_person,
            std::string office_location,
            double budget,
            const QDate& established_date,
            bool isActive
        )
    : m_id( id ),
    m_department_code( department_code ),
    m_department_name( department_name ),
    m_chair_person( chair_person ),
    m_office_location( office_location ),
    m_budget( budget ),
    m_established_date( established_date ),
    m_isActive( isActive )
{
}
