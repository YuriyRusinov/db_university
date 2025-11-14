#pragma once

#include <QDate>
#include <memory>
#include <string>

class Student;
class Course;

enum class EnrollmentStatus {
    Unknown = -1,
    Active = 0,
    Completed,
    Withdrawn,
    Dropped
};

class Enrollments {
public:
    Enrollments( int id=-1,
                 std::shared_ptr< Student > student = nullptr,
                 std::shared_ptr< Course > course = nullptr,
                 const QDate& enrollment_date = QDate(),
                 double grade = -1.0,
                 EnrollmentStatus eStatus = EnrollmentStatus::Unknown,
                 std::string semester = std::string(),
                 int credit_hours = -1
            );
    Enrollments( const Enrollments& e ) = default;
    Enrollments( Enrollments&& e ) = default;
    Enrollments& operator=( const Enrollments& e ) = default;
    Enrollments& operator=( Enrollments&& e ) = default;
    ~Enrollments() {};

    int getId() const { return m_id; }
    void setId( int id ) { m_id = id; }

    std::shared_ptr< Student > getStudent() const { return m_student; }
    void setStudent(  std::shared_ptr< Student > s ) { m_student = std::move(s); }

    std::shared_ptr< Course > getCourse() const { return m_course; }
    void setCourse( std::shared_ptr< Course > c ) { m_course = std::move(c); }

    const QDate& getEnrollmentDate() const { return m_enrollment_date; }
    void setEnrollmentDate( const QDate& d ) { m_enrollment_date = d; }

    double getGrade() const { return m_grade; }
    void setGrade( double grade ) { m_grade = std::move( grade ); }

    EnrollmentStatus getStatus() const { return m_status; }
    void setStatus( EnrollmentStatus status ) { m_status = status; }

    const std::string& getSemester() const { return m_semester; }
    void setSemester( const std::string& s ) { m_semester = s; }

    int getHours() const { return m_credit_hours; }
    void setHours( int h ) { m_credit_hours = h; }

private:
    int m_id;
    std::shared_ptr< Student > m_student;
    std::shared_ptr< Course > m_course;
    QDate m_enrollment_date;
    double m_grade;
    EnrollmentStatus m_status;
    std::string m_semester;
    int m_credit_hours;
};
