#pragma once

#include <QDate>
#include <string>

class Department {
public:
    Department( int id=-1,
                std::string department_code = std::string(),
                std::string department_name = std::string(),
                std::string chair_person = std::string(),
                std::string office_location = std::string(),
                double budget = 0.0,
                const QDate& established_date = QDate(),
                bool isActive = false
            );
    Department( const Department& D ) = default;
    Department& operator=( const Department& D ) = default;
    Department( Department&& D ) = default;
    Department& operator=( Department&& D ) = default;
    ~Department() {};

    int getId() const { return m_id; }
    void setId( int id ) { m_id = id; }

    const std::string& getCode() const { return m_department_code; }
    void setCode( const std::string& code ) { m_department_code = code; }

    const std::string& getName() const { return m_department_name; }
    void setName( const std::string& name ) { m_department_name = name; }

    const std::string& getChairPerson() const { return m_chair_person; }
    void setChairPerson( const std::string& person ) { m_chair_person=person; }

    const std::string& getOfficeLocation() const { return m_office_location; }
    void setOfficeLocation( const std::string& office_loc ) { m_office_location = office_loc; }

    double getBudget() const { return m_budget; }
    void setBudget( double val ) { m_budget=val; }

    const QDate& getEstablishedDate() const { return m_established_date; }
    void setEstablishedDate( const QDate& d ) { m_established_date = d; }

    bool isActive() const { return m_isActive; }
    void setActive( bool val ) { m_isActive=val; }

private:
    int m_id;
    std::string m_department_code;
    std::string m_department_name;
    std::string m_chair_person;
    std::string m_office_location;
    double m_budget;
    QDate m_established_date;
    bool m_isActive;
};
