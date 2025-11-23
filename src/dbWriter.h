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

private:
    int insertDepartment( std::shared_ptr< Department > pDepartment );
    int updateDepartment( std::shared_ptr< Department > pDepartment );
    int deleteDepartment( int idDep );

    friend class dbCore;

private:
    std::shared_ptr<UniversityDb> m_db;

private:
    Q_OBJECT
};
