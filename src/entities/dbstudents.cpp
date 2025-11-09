#include <pqxx/pqxx>
#include "dbstudents.hpp"

Student::Student(int id,
            long long sNumber,
            std::string first_name,
            std::string middle_name,
            std::string last_name,
            std::string email,
            const QDate& date_of_birth,
            const QDate& enrollment_date,
            const QDate& graduation_date,
            StudentStatus status
      )
    : m_id( id ),
    m_student_number( sNumber ),
    m_first_name( first_name ),
    m_middle_name( middle_name ),
    m_last_name( last_name ),
    m_email( email ),
    m_birth_date( date_of_birth ),
    m_enrollment_date( enrollment_date ),
    m_graduation_date( graduation_date ),
    m_status( status )
{
}

std::string Student::generateFullName() const {
    std::string res(m_first_name);
    if( m_middle_name.length() != 0 )
        res += ' ' + m_middle_name;

    res += ' ' + m_last_name;
    return res;
}
