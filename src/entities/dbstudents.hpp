#pragma once

#include <string>
#include <QDate>

enum StudentStatus {
    Unknown=-1,
    Active=0,
    Graduated,
    Transferred,
    Withdrawn
};

class Student {
public:
    Student(int id=-1,
            long long sNumber=-1,
            std::string first_name=std::string(),
            std::string middle_name=std::string(),
            std::string last_name=std::string(),
            std::string email=std::string(),
            const QDate& date_of_birth=QDate(),
            const QDate& enrollment_date=QDate(),
            const QDate& graduation_date=QDate(),
            StudentStatus status = StudentStatus::Unknown
           );
    Student( const Student& S ) = default;
    Student( Student&& S ) = default;
    Student& operator= ( const Student& S ) = default;
    Student& operator= ( Student&& S ) = default;

    int getId() const { return m_id; };
    void setId(int id) { m_id=id; };

    long long getStudentNumber() const { return m_student_number; }
    void setStudentNumber( long long sn ) { m_student_number=sn; }

    std::string getFirstName() const { return m_first_name; }
    void setFirstName( const std::string& fname ) { m_first_name=fname; }

    std::string getMiddleName() const { return m_middle_name; }
    void setMiddleName( const std::string& mname ) { m_middle_name = mname; }

    std::string getLastName() const { return m_last_name; }
    void setLastName( const std::string& lname ) { m_last_name = lname; }

    std::string getEmail() const { return m_email; }
    void setEmail( const std::string& email ) { m_email=email; }

    const QDate& getBirthDate() const { return m_birth_date; }
    void setBirthDate( const QDate& d ) { m_birth_date = d; }

    const QDate& getEnrollmentDate() const { return m_enrollment_date; }
    void setEnrollmentDate( const QDate& d ) { m_enrollment_date = d; }

    const QDate& getGraduationDate() const { return m_graduation_date; }
    void setGraduationDate( const QDate& d ) { m_graduation_date = d; }

    StudentStatus getStatus() const { return m_status; };
    void setStatus(int s) { m_status = (StudentStatus)s; };

private:
    int m_id;
    long long m_student_number;
    std::string m_first_name;
    std::string m_middle_name;
    std::string m_last_name;
    std::string m_email;
    QDate m_birth_date;
    QDate m_enrollment_date;
    QDate m_graduation_date;
    StudentStatus m_status;
};
