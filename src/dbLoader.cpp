#include <QtDebug>
#include <university_db.hpp>
#include <dbresult.hpp>
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
                      res->getCellAsBool(i, 7)
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
                   res->getCellAsInt64(i, 2),
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
        vStudents[i] = std::move( s );
    }
 
    return vStudents;
}
