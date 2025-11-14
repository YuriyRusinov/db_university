#include "students.hpp"
#include "courses.hpp"
#include "enrollments.hpp"

Enrollments::Enrollments( int id,
             std::shared_ptr< Student > student,
             std::shared_ptr< Course > course,
             const QDate& enrollment_date,
             double grade,
             EnrollmentStatus eStatus,
             std::string semester,
             int credit_hours
        ) : m_id( id ),
    m_student( student ),
    m_course( course ),
    m_enrollment_date( enrollment_date ),
    m_grade( grade ),
    m_status( eStatus ),
    m_semester( semester ),
    m_credit_hours( credit_hours )
{
}
