#pragma once

#include <string>
#include <optional>
#include <QDate>
#include "studentsprofile.hpp"

enum class StudentStatus {
    Unknown=-1,
    Active=0,
    Graduated,
    Transferred,
    Withdrawn,
    Suspended
};

class Student {
public:
    Student(int id=-1,
            std::string sNumber=std::string(),
            std::string first_name=std::string(),
            std::string middle_name=std::string(),
            std::string last_name=std::string(),
            std::string email=std::string(),
            const QDate& date_of_birth=QDate(),
            const QDate& enrollment_date=QDate(),
            const QDate& graduation_date=QDate(),
            StudentStatus status = StudentStatus::Unknown,
            const StudentProfile& profile = StudentProfile()
           );
    Student( const Student& S ) = default;
    Student( Student&& S ) = default;
    Student& operator= ( const Student& S ) = default;
    Student& operator= ( Student&& S ) = default;
    ~Student() {};

    int getId() const { return m_id; };
    void setId(int id) { m_id = std::move(id); };

    std::string getStudentNumber() const { return m_student_number; }
    void setStudentNumber( std::string sn ) { m_student_number = std::move(sn); }

    std::string getFirstName() const { return m_first_name; }
    void setFirstName( const std::string& fname ) { m_first_name = std::move(fname); }

    std::string getMiddleName() const { return m_middle_name; }
    void setMiddleName( const std::string& mname ) { m_middle_name = std::move(mname); }

    std::string getLastName() const { return m_last_name; }
    void setLastName( const std::string& lname ) { m_last_name = std::move(lname); }

    std::string generateFullName() const;// { std::string res(m_first_name)

    std::string getEmail() const { return m_email; }
    void setEmail( const std::string& email ) { m_email = std::move(email); }

    const QDate& getBirthDate() const { return m_birth_date; }
    void setBirthDate( const QDate& d ) { m_birth_date = std::move(d); }

    const QDate& getEnrollmentDate() const { return m_enrollment_date; }
    void setEnrollmentDate( const QDate& d ) { m_enrollment_date = std::move(d); }

    const QDate& getGraduationDate() const { return m_graduation_date; }
    void setGraduationDate( const QDate& d ) { m_graduation_date = std::move(d); }

    StudentStatus getStatus() const { return m_status; };
    void setStatus( StudentStatus s ) { m_status = std::move(s); };

    const std::optional< StudentProfile >& getProfile() const { return m_profile; }
    void setStudentProfile( const std::optional<StudentProfile>& SP ) { m_profile = SP; }

private:
    int m_id;
    std::string m_student_number;
    std::string m_first_name;
    std::string m_middle_name;
    std::string m_last_name;
    std::string m_email;
    QDate m_birth_date;
    QDate m_enrollment_date;
    QDate m_graduation_date;
    StudentStatus m_status;
    std::optional<StudentProfile> m_profile;
};
