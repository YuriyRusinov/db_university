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

class dbLoader : public QObject {
public:
    dbLoader( std::shared_ptr<UniversityDb> pdb, QObject* parent=nullptr );
    ~dbLoader();

    std::vector<Department> loadDepartments() const;
    std::vector<Student> loadStudents() const;
    std::vector<Course> loadCourses() const;
    std::vector<Enrollments> loadEnrollments() const;

    std::shared_ptr<Department> loadDepartment( int idDep ) const;
    std::shared_ptr<Student> loadStudent( int idStudent ) const;
    std::shared_ptr<Course> loadCourse( int idCourse ) const;
    std::optional<StudentProfile> loadStudentProfile( int idStudent ) const;

private:
    std::shared_ptr<UniversityDb> m_db;

private:
    Q_OBJECT
};
