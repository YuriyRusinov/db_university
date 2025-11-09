#pragma once

#include <QObject>
#include <vector>
#include <memory>

#include <dbdepartment.hpp>
#include <dbstudents.hpp>

class UniversityDb;

class dbLoader : public QObject {
public:
    dbLoader( std::shared_ptr<UniversityDb> pdb, QObject* parent=nullptr );
    ~dbLoader();

    std::vector<Department> loadDepartments() const;
    std::vector<Student> loadStudents() const;

private:
    std::shared_ptr<UniversityDb> m_db;

private:
    Q_OBJECT
};
