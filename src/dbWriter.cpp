#include <QtDebug>
#include <university_db.hpp>
#include <dbresult.hpp>
#include <department.hpp>
#include "dbWriter.h"

dbWriter::dbWriter( std::shared_ptr<UniversityDb> pdb, QObject* parent )
    : QObject( parent ),
    m_db( pdb )
{
}

dbWriter::~dbWriter() {
}

int dbWriter::saveDepartment( std::shared_ptr< Department > pDepartment ) {
    qDebug() << __PRETTY_FUNCTION__ << (pDepartment == nullptr);
    if( pDepartment == nullptr )
        return -1;
    else if( pDepartment->getId() < 0 )
        return insertDepartment( pDepartment );
    else
        return updateDepartment( pDepartment );
}

int dbWriter::insertDepartment( std::shared_ptr< Department > pDepartment )
{
    QString sql = QString("select add_department( %1, %2, %3, %4, %5, %6, %7 );")
                        .arg( pDepartment->getCode().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString(pDepartment->getCode())+QString("'") )
                        .arg( pDepartment->getName().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString(pDepartment->getName())+QString("'") )
                        .arg( pDepartment->getChairPerson().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString( pDepartment->getChairPerson() )+QString("'") )
                        .arg( pDepartment->getOfficeLocation().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString( pDepartment->getOfficeLocation() )+QString("'") )
                        .arg( pDepartment->getBudget() )
                        .arg( pDepartment->getEstablishedDate().isValid() ? QString("'")+pDepartment->getEstablishedDate().toString(Qt::ISODate)+QString("'::date") : QString("null::date") )
                        .arg( pDepartment->isActive() ? QString("true") : QString("false") );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    return res->getCellAsInt(0, 0);
}

int dbWriter::updateDepartment( std::shared_ptr< Department > pDepartment ) {
    QString sql = QString("select update_department( %1, %2, %3, %4, %5, %6, %7, %8 );")
                        .arg( pDepartment->getId() )
                        .arg( pDepartment->getCode().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString(pDepartment->getCode())+QString("'")  )
                        .arg( pDepartment->getName().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString(pDepartment->getName())+QString("'") )
                        .arg( pDepartment->getChairPerson().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString( pDepartment->getChairPerson() )+QString("'") )
                        .arg( pDepartment->getOfficeLocation().empty() ? QString("null::varchar") : QString("'")+QString::fromStdString( pDepartment->getOfficeLocation() ) + QString("'") )
                        .arg( pDepartment->getBudget() )
                        .arg( pDepartment->getEstablishedDate().isValid() ? QString("'")+pDepartment->getEstablishedDate().toString(Qt::ISODate)+QString("':;date") : QString("null::date") )
                        .arg( pDepartment->isActive() ? QString("true") : QString("false") );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    return res->getCellAsInt(0, 0);
}

int dbWriter::deleteDepartment( int idDep ) {
    QString sql = QString( "select delete_department( %1 );").arg ( idDep );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    return res->getCellAsInt(0, 0);
}
