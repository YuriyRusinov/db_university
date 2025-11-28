#include <QtDebug>
#include <university_db.hpp>
#include <dbresult.hpp>
#include <department.hpp>
#include <students.hpp>
#include <studentsprofile.hpp>
#include <courses.hpp>
#include <enrollments.hpp>

#include "dbLoader.h"

dbLoader::dbLoader( std::shared_ptr<UniversityDb> pdb, QObject* parent )
    : QObject( parent ),
    m_db( pdb )
{
}

dbLoader::~dbLoader()
{
}

std::vector<Department> dbLoader::loadDepartments() const
{
    QString sql = QString("select * from get_departments (null::integer);");
    if( m_db == nullptr )
        return std::vector< Department >();

    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );

    if( res == nullptr ) 
        return std::vector< Department >();

    int n = res->getRowCount();
    std::vector< Department > deps;
    for(int i=0; i<n; i++) {
        Department d( res->getCellAsInt(i, 0), // id
                      res->getCellAsString(i, 2), // code
                      res->getCellAsString(i, 3), // name
                      res->getCellAsString(i, 4), // chair_person
                      res->getCellAsString(i, 5), // location
                      res->getCellAsDouble(i, 6), // budget
                      QDate::fromString(QString::fromStdString(res->getCellAsString(i, 7)), Qt::ISODate), // established date
                      res->getCellAsBool(i, 8)
                );
        deps.push_back( d );
    }
    return deps;
}

std::vector<Student> dbLoader::loadStudents() const {
    QString sql = QString("select * from getstudents(null::integer);");
    std::vector< Student > vStudents;
    if( m_db == nullptr )
        return vStudents;

    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );

    if( res == nullptr ) 
        return vStudents;

    int n = res->getRowCount();
    qDebug() << __PRETTY_FUNCTION__ << n;
    vStudents.resize(n);
    for(int i=0; i<n; i++) {
        Student s( res->getCellAsInt(i, 0),
                   res->getCellAsString(i, 2),
                   res->getCellAsString(i, 3),
                   res->getCellAsString(i, 4),
                   res->getCellAsString(i, 5),
                   res->getCellAsString(i, 6),
                   QDate::fromString( QString::fromStdString(res->getCellAsDateTime(i, 7)), Qt::ISODate),
                   QDate::fromString( QString::fromStdString(res->getCellAsDateTime(i, 8)), Qt::ISODate),
                   QDate::fromString( QString::fromStdString(res->getCellAsDateTime(i, 9)), Qt::ISODate)
                );
        if( !res->getCellAsString(i, 10).compare("active")  )
            s.setStatus( StudentStatus::Active );
        else if( !res->getCellAsString(i, 10).compare("graduated") )
            s.setStatus( StudentStatus::Graduated );
        else if( !res->getCellAsString(i, 10).compare("transferred") )
            s.setStatus( StudentStatus::Transferred );
        else if( !res->getCellAsString(i, 10).compare("withdrawn") )
            s.setStatus( StudentStatus::Withdrawn );
        std::optional<StudentProfile> sP = loadStudentProfile( s.getId() );
        if( sP.has_value() )
            s.setStudentProfile( sP.value() );
        vStudents[i] = std::move( s );
        qDebug() << __PRETTY_FUNCTION__ << sP.has_value();
    }
 
    return vStudents;
}

std::vector<Course> dbLoader::loadCourses() const {
    QString sql = QString("select * from get_courses (null::integer);");
    std::vector<Course> vCourses;
    if( m_db == nullptr )
        return vCourses;

    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );

    if( res == nullptr ) 
        return vCourses;

    int n = res->getRowCount();
    qDebug() << __PRETTY_FUNCTION__ << n;
    vCourses.resize(n);
    for(int i=0; i<n; i++) {
        int idDep = res->getCellAsInt(i, 6);
        Course c( res->getCellAsInt(i, 0), // id
                  res->getCellAsString(i, 2), // code
                  res->getCellAsString(i, 3), // name
                  res->getCellAsString(i, 4), // description
                  res->getCellAsInt(i, 5), // mark
                  loadDepartment( idDep ),  // department
                  res->getCellAsString(i, 9), // professor
                  res->getCellAsInt(i, 10),   // max capacity
                  res->getCellAsBool(i, 11)  // is active
                );
        vCourses[i] = c;
    }

    return vCourses;
}

std::shared_ptr<Department> dbLoader::loadDepartment(int idDep) const {
    QString sql = QString("select * from get_departments ( %1 );").arg( idDep );
    if( m_db == nullptr )
        return nullptr;

    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );

    if( res == nullptr ) 
        return nullptr;

    int n = res->getRowCount();
    if( n != 1 )
        return nullptr;
    int i(0);
    std::shared_ptr<Department> pd = std::make_shared<Department>( res->getCellAsInt(i, 0), // id
                  res->getCellAsString(i, 2), // code
                  res->getCellAsString(i, 3), // name
                  res->getCellAsString(i, 4), // chair_person
                  res->getCellAsString(i, 5), // location
                  res->getCellAsDouble(i, 6), // budget
                  QDate::fromString(QString::fromStdString(res->getCellAsString(i, 7)), Qt::ISODate), // established date
                  res->getCellAsBool(i, 8)
            );
    return pd;
}

std::shared_ptr<Student> dbLoader::loadStudent( int idStudent ) const {
    QString sql = QString("select * from getstudents( %1 );").arg( idStudent );
    std::shared_ptr< Student > pStudent( nullptr );
    if( m_db == nullptr )
        return pStudent;

    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );

    if( res == nullptr ) 
        return pStudent;

    int n = res->getRowCount();
    qDebug() << __PRETTY_FUNCTION__ << n;
    if( n != 1 ) {
        qDebug() << __PRETTY_FUNCTION__ << "Invalid query " << sql;
        return pStudent;
    }
    int i=0;
    pStudent = std::make_shared<Student>( res->getCellAsInt(i, 0),
               res->getCellAsString(i, 2),
               res->getCellAsString(i, 3),
               res->getCellAsString(i, 4),
               res->getCellAsString(i, 5),
               res->getCellAsString(i, 6),
               QDate::fromString( QString::fromStdString(res->getCellAsDateTime(i, 7)), Qt::ISODate),
               QDate::fromString( QString::fromStdString(res->getCellAsDateTime(i, 8)), Qt::ISODate),
               QDate::fromString( QString::fromStdString(res->getCellAsDateTime(i, 9)), Qt::ISODate)
            );
    if( !res->getCellAsString(i, 10).compare("active")  )
        pStudent->setStatus( StudentStatus::Active );
    else if( !res->getCellAsString(i, 10).compare("graduated") )
        pStudent->setStatus( StudentStatus::Graduated );
    else if( !res->getCellAsString(i, 10).compare("transferred") )
        pStudent->setStatus( StudentStatus::Transferred );
    else if( !res->getCellAsString(i, 10).compare("withdrawn") )
        pStudent->setStatus( StudentStatus::Withdrawn );

    std::optional<StudentProfile> sP = loadStudentProfile( pStudent->getId() );
    if( sP.has_value() )
        pStudent->setStudentProfile( sP.value() );
    return pStudent;
}

std::shared_ptr<Course> dbLoader::loadCourse( int idCourse ) const {
    QString sql = QString("select * from get_courses (%1::integer);").arg( idCourse );
    std::shared_ptr<Course> pCourse( nullptr );
    if( m_db == nullptr )
        return pCourse;

    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );

    if( res == nullptr || res->getRowCount() != 1 ) 
        return pCourse;

    int i=0;
    int idDep = res->getCellAsInt(i, 6);
    pCourse = std::make_shared< Course >( res->getCellAsInt(i, 0), // id
              res->getCellAsString(i, 2), // code
              res->getCellAsString(i, 3), // name
              res->getCellAsString(i, 4), // description
              res->getCellAsInt(i, 5), // mark
              loadDepartment( idDep ),  // department
              res->getCellAsString(i, 9), // professor
              res->getCellAsInt(i, 10),   // max capacity
              res->getCellAsBool(i, 11)  // is active
            );

    return pCourse;
}

std::optional<StudentProfile> dbLoader::loadStudentProfile( int idStudent ) const {
    std::optional<StudentProfile> sProfile = std::nullopt;
    QString sql = QString("select * from get_student_profile( %1 );").arg( idStudent );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 ) {
        qDebug() << __PRETTY_FUNCTION__ << sql;
        return std::nullopt;
    }

    int i=0;
    std::string aStanding = res->getCellAsString(i, 2);
    AcademicStanding aStudentSt = AcademicStanding::Unknown;
    if( aStanding.compare("good") == 0 )
        aStudentSt = AcademicStanding::Good;
    else if( aStanding.compare("probation") == 0 )
        aStudentSt = AcademicStanding::Probation;
    else if( aStanding.compare("warning") == 0 )
        aStudentSt = AcademicStanding::Warning;
    else if( aStanding.compare("suspended") == 0 )
        aStudentSt = AcademicStanding::Suspended;
    else if( aStanding.compare("graduated") == 0 )
        aStudentSt = AcademicStanding::Graduated;

    std::optional<std::string> optCountry (res->getCellAsString(i, 12));
    StudentProfile sP = StudentProfile( res->getCellAsInt(i, 0), // id
                               idStudent,
                               aStudentSt,
                               res->getCellAsDouble(i, 3), // finance
                               res->getCellAsString(i, 5), // phone
                               res->getCellAsString(i, 6), // emergency contact
                               res->getCellAsString(i, 7), // emergency contact phone
                               res->getCellAsString(i, 8), // address
                               res->getCellAsString(i, 9), // city
                               res->getCellAsString(i, 10), // state
                               res->getCellAsString(i, 11), // postal_code
                               res->getCellAsString(i, 12), // country
                               res->getCellAsString(i, 13) // medical notes
            );
    qDebug() << __PRETTY_FUNCTION__ << sql << ' ' << (int)aStudentSt << sP.isValid() << QString::fromStdString(res->getCellAsString(i, 12)) << optCountry.value().c_str() << QString::fromStdString(sP.getCountry());
    if( sP.isValid() )
        sProfile = sP;
    return sProfile;
}

std::vector<Enrollments> dbLoader::loadEnrollments() const {
    std::vector< Enrollments > vEnrollments;
    QString sql = QString("select * from get_enrollments (null::integer);");
    if( m_db == nullptr )
        return vEnrollments;

    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );

    if( res == nullptr ) 
        return vEnrollments;
    int n = res->getRowCount();
    for(int i=0; i<n; i++) {
        Enrollments e( res->getCellAsInt(i, 0),
                       loadStudent( res->getCellAsInt(i, 2) ),
                       loadCourse( res->getCellAsInt(i, 3) ),
                       QDate::fromString(QString::fromStdString(res->getCellAsString(i, 4)), Qt::ISODate),
                       res->getCellAsDouble(i, 5),
                       EnrollmentStatus::Unknown,
                       res->getCellAsString(i, 7),
                       res->getCellAsInt(i, 8)
                       );
        if( res->getCellAsString(i, 6).compare( "active" ) == 0 )
            e.setStatus( EnrollmentStatus::Active );
        else if( res->getCellAsString(i, 6).compare( "completed" ) == 0 )
            e.setStatus( EnrollmentStatus::Completed );
        else if( res->getCellAsString(i, 6).compare( "withdrawn" ) == 0 )
            e.setStatus( EnrollmentStatus::Withdrawn );
        else if( res->getCellAsString(i, 6).compare( "dropped" ) == 0 )
            e.setStatus( EnrollmentStatus::Dropped );
        vEnrollments.push_back( e );
    }

    return vEnrollments;
}
