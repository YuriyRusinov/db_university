#pragma once

#include <QObject>
#include <vector>
#include <memory>
#include <optional>

class UniversityDb;

class Department;
class Student;
class Course;
class Enrollments;
class StudentProfile;

class dbWriter : public QObject {
public:
    dbWriter( std::shared_ptr<UniversityDb> pdb, QObject* parent=nullptr );
    virtual ~dbWriter();

public slots:
    int saveDepartment( std::shared_ptr< Department > pDepartment );
    int saveStudent( std::shared_ptr< Student > pStudent );

public:
    int insertDepartment( std::shared_ptr< Department > pDepartment );
    int updateDepartment( std::shared_ptr< Department > pDepartment );
    int deleteDepartment( int idDep );

    int insertStudent( std::shared_ptr< Student > pStudent );
    int updateStudent( std::shared_ptr< Student > pStudent );
    int deleteStudent( int idStudent );

    int updateStudentProfile( std::shared_ptr< StudentProfile > sP );
    int clearStudentProfile( int idStudent );
    int saveStudentProfile( std::shared_ptr< Student > pStudent );

private:
    std::shared_ptr<UniversityDb> m_db;

private:
    Q_OBJECT
};
