#include <pqxx/pqxx>
#include "students.hpp"

Student::Student(int id,
            std::string sNumber,
            std::string first_name,
            std::string middle_name,
            std::string last_name,
            std::string email,
            const QDate& date_of_birth,
            const QDate& enrollment_date,
            const QDate& graduation_date,
            StudentStatus status,
            const StudentProfile& profile
      )
    : m_id( std::move(id) ),
    m_student_number( std::move(sNumber) ),
    m_first_name( std::move(first_name) ),
    m_middle_name( std::move(middle_name) ),
    m_last_name( std::move(last_name) ),
    m_email( std::move(email) ),
    m_birth_date( std::move(date_of_birth) ),
    m_enrollment_date( std::move(enrollment_date) ),
    m_graduation_date( std::move(graduation_date) ),
    m_status( std::move(status) )
{
    if( !profile.isValid() )
        m_profile = std::nullopt;
    else
        m_profile = std::move( profile );
}

std::string Student::generateFullName() const {
    std::string res(m_first_name);
    if( m_middle_name.length() != 0 )
        res += ' ' + m_middle_name;

    res += ' ' + m_last_name;
    return res;
}
