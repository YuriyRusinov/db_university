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
