#pragma once

#include <string>
#include <memory>

class Department;

class Course {
public:
    Course( int id = -1,
            std::string code = std::string(),
            std::string name = std::string(),
            std::string description = std::string(),
            int mark = -1,
            std::shared_ptr< Department > department = nullptr,
            std::string professor = std::string(),
            int max_capacity = -1,
            bool isActive = false
          );
    Course( const Course& C ) = default;
    Course( Course&& C ) = default;
    Course& operator=( const Course& C ) = default;
    Course& operator=( Course&& C ) = default;
    ~Course() {};

    int getId() const { return m_id; }
    void setId( const int& id ) { m_id=id; }

    const std::string& getCode() const { return m_code; }
    void setCode( const std::string& code ) { m_code = code; }

    const std::string& getName() const { return m_name; }
    void setName( const std::string& name ) { m_name = name; }

    const std::string& getDesc() const { return m_description; }
    void setDesc( const std::string& desc ) { m_description = desc; }

    std::shared_ptr< Department > getDepartment() const { return m_department; }
    void setDepartment( std::shared_ptr< Department > dep ) { m_department = dep; }

    const std::string& getProfessor() const { return m_professor; }
    void setProfessor( const std::string& prof ) { m_professor = prof; }

    int getMaxCapacity() const { return m_max_capacity; }
    void setMaxCapacity( int val ) { m_max_capacity = val; }
    bool isActive() const { return m_isActive; }
    void setActive( bool val ) { m_isActive = val; }

private:
    int m_id;
    std::string m_code;
    std::string m_name;
    std::string m_description;
    int m_mark;
    std::shared_ptr< Department > m_department;
    std::string m_professor;
    int m_max_capacity;
    bool m_isActive;
};
