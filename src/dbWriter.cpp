#include <QtDebug>
#include <university_db.hpp>
#include <dbresult.hpp>
#include <department.hpp>
#include <students.hpp>
#include <courses.hpp>
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
                        .arg( pDepartment->getEstablishedDate().isValid() ? QString("'")+pDepartment->getEstablishedDate().toString(Qt::ISODate)+QString("'::date") : QString("null::date") )
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

int dbWriter::saveStudent( std::shared_ptr< Student > pStudent ) {
    if( pStudent == nullptr )
        return -1;
    qDebug() << __PRETTY_FUNCTION__ << pStudent->getId();
    int res = -1;
    if( pStudent->getId() < 0 )
        res = insertStudent( pStudent );
    else
        res = updateStudent( pStudent );

    return res;
}

int dbWriter::insertStudent( std::shared_ptr< Student > pStudent ) {
    if( pStudent == nullptr )
        return -1;
    QString st_status;
    switch( pStudent->getStatus() ) {
        case StudentStatus::Unknown: default: st_status = QString("null::varchar"); break;
        case StudentStatus::Active: st_status = QString("'active'"); break;
        case StudentStatus::Graduated: st_status = QString("'graduated'"); break;
        case StudentStatus::Transferred: st_status = QString("'transferred'"); break;
        case StudentStatus::Withdrawn: st_status = QString("'withdrawn'"); break;
        case StudentStatus::Suspended: st_status = QString("'suspended'"); break;
    }
    QString sql = QString( "select insert_student( %1, %2, %3, %4, %5, %6, %7, %8, %9 );")
                    .arg( pStudent->getStudentNumber().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString(pStudent->getStudentNumber())) )
                    .arg( pStudent->getFirstName().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString(pStudent->getFirstName()) ) )
                    .arg( pStudent->getMiddleName().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString(pStudent->getMiddleName()) ) )
                    .arg( pStudent->getLastName().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString( pStudent->getLastName() ) ) )
                    .arg( pStudent->getEmail().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString( pStudent->getEmail() ) ) )
                    .arg( pStudent->getBirthDate().isValid() ? QString("'%1'::date").arg( pStudent->getBirthDate().toString(Qt::ISODate) ) : QString("null::date"))
                    .arg( pStudent->getEnrollmentDate().isValid() ? QString("'%1'::date").arg( pStudent->getEnrollmentDate().toString( Qt::ISODate ) ) : QString("null::date") )
                    .arg( pStudent->getGraduationDate().isValid() ? QString("'%1'::date").arg( pStudent->getGraduationDate().toString( Qt::ISODate ) ) : QString("null::date") )
                    .arg(st_status);
    qDebug() << __PRETTY_FUNCTION__ << sql;
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    int idStudent = res->getCellAsInt(0, 0);
    pStudent->setId( idStudent );
    saveStudentProfile( pStudent );
    return idStudent;
}

int dbWriter::updateStudent( std::shared_ptr< Student > pStudent ) {
    if( pStudent == nullptr )
        return -1;
    QString st_status;
    switch( pStudent->getStatus() ) {
        case StudentStatus::Unknown: default: st_status = QString("null::varchar"); break;
        case StudentStatus::Active: st_status = QString("'active'"); break;
        case StudentStatus::Graduated: st_status = QString("'graduated'"); break;
        case StudentStatus::Transferred: st_status = QString("'transferred'"); break;
        case StudentStatus::Withdrawn: st_status = QString("'withdrawn'"); break;
        case StudentStatus::Suspended: st_status = QString("'suspended'"); break;
    }
    QString sql = QString( "select update_student( %1, %2, %3, %4, %5, %6, %7, %8, %9, %10 );")
                    .arg( pStudent->getId() )
                    .arg( pStudent->getStudentNumber().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString(pStudent->getStudentNumber())) )
                    .arg( pStudent->getFirstName().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString(pStudent->getFirstName()) ) )
                    .arg( pStudent->getMiddleName().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString(pStudent->getMiddleName()) ) )
                    .arg( pStudent->getLastName().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString( pStudent->getLastName() ) ) )
                    .arg( pStudent->getEmail().empty() ? QString("null::varchar") : QString("'%1'").arg(QString::fromStdString( pStudent->getEmail() ) ) )
                    .arg( pStudent->getBirthDate().isValid() ? QString("'%1'::date").arg( pStudent->getBirthDate().toString(Qt::ISODate) ) : QString("null::date"))
                    .arg( pStudent->getEnrollmentDate().isValid() ? QString("'%1'::date").arg( pStudent->getEnrollmentDate().toString( Qt::ISODate ) ) : QString("null::date") )
                    .arg( pStudent->getGraduationDate().isValid() ? QString("'%1'::date").arg( pStudent->getGraduationDate().toString( Qt::ISODate ) ) : QString("null::date") )
                    .arg(st_status);
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    int idStudent = res->getCellAsInt(0, 0);
    saveStudentProfile( pStudent );
    return idStudent;
}

int dbWriter::saveStudentProfile( std::shared_ptr< Student > pStudent ) {
    if( pStudent == nullptr )
        return -1;

    int idsp = -1;
    if( pStudent->getProfile() ) {
        std::shared_ptr< StudentProfile > sProfile = std::make_shared< StudentProfile >(*pStudent->getProfile() );
        sProfile->setStudentId( pStudent->getId() );
        idsp = updateStudentProfile( sProfile );
        if( idsp >= 0 && sProfile != nullptr ) {
            std::optional< StudentProfile > oSP( *sProfile );
            pStudent->setStudentProfile( oSP );
        }
    }
    else {
        idsp = clearStudentProfile( pStudent->getId() );
        if( idsp < 0 )
            qDebug() << __PRETTY_FUNCTION__ << " Error clearing profile, code = " << idsp;
    }
    return idsp;
}

int dbWriter::deleteStudent( int idStudent ) {
    QString sql = QString("select delete_student( %1 );").arg( idStudent );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    return res->getCellAsInt(0, 0);
}

int dbWriter::updateStudentProfile( std::shared_ptr< StudentProfile > sP ) {
    if( sP == nullptr )
        return -1;

    QString sql = QString("select update_student_profile( %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12 );")
                    .arg( sP->getStudentId() )
                    .arg( sP->getPhone().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( sP->getPhone() ) ) )
                    .arg( sP->getEmergencyPerson().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( sP->getEmergencyPerson() ) ) )
                    .arg( sP->getEmergencyPhone().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( sP->getEmergencyPhone() ) ) )
                    .arg( sP->getAddress().empty() ? QString("null::text") : QString("'%1'").arg( QString::fromStdString( sP->getAddress() ) ) )
                    .arg( sP->getCity().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( sP->getCity() ) ) )
                    .arg( sP->getState().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( sP->getState() ) ) )
                    .arg( sP->getPostalCode().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( sP->getPostalCode() ) ) )
                    .arg( sP->getCountry().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( sP->getCountry() ) ) )
                    .arg( sP->getMedNotes().empty() ? QString("null::text") : QString("'%1'").arg( QString::fromStdString( sP->getMedNotes() ) ) )
                    .arg( sP->getAcademicStanding() != AcademicStanding::Unknown ? QString("'%1'").arg( QString::fromStdString( sP->getFullAcademicStanding() ) ) : QString("null::varchar") )
                    .arg( sP->getFinancialBalance() );
    qDebug() << __PRETTY_FUNCTION__ << sql;
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    int idStudentProfile = res->getCellAsInt(0, 0);
    return idStudentProfile;
}

int dbWriter::clearStudentProfile( int idStudent ) {
    QString sql = QString("select clear_student_profile( %1 );").arg( idStudent );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    int idStudentProfile = res->getCellAsInt(0, 0);
    return idStudentProfile;
}

int dbWriter::saveCourse( std::shared_ptr< Course > pCourse ) {
    if( pCourse == nullptr )
        return -1;

    qDebug() << __PRETTY_FUNCTION__;
    int courseId = -1;
    if( pCourse->getId() < 0 ) {
        courseId = insertCourse( pCourse );
    }
    else {
        courseId = updateCourse( pCourse );
    }
    if( courseId >= 0 )
        pCourse->setId( courseId );

    return courseId;
}

int dbWriter::insertCourse( std::shared_ptr< Course > pCourse ) {
    if( pCourse == nullptr )
        return -1;

    QString sql = QString("select add_course( %1, %2, %3, %4, %5, %6, %7, %8 );")
                    .arg( pCourse->getCode().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( pCourse->getCode() ) ) )
                    .arg( pCourse->getName().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( pCourse->getName() ) ) )
                    .arg( pCourse->getDesc().empty() ? QString("null::text") : QString("'%1'").arg( QString::fromStdString( pCourse->getDesc() ) ) )
                    .arg( pCourse->getCreditMark() < 0 ? QString("null::integer") : QString::number( pCourse->getCreditMark() ) )
                    .arg( pCourse->getDepartment() == nullptr ? QString("null::integer") : QString::number( pCourse->getDepartment()->getId() ) )
                    .arg( pCourse->getProfessor().empty() ? QString("null::varchar") :  QString("'%1'").arg( QString::fromStdString( pCourse->getProfessor() ) ) )
                    .arg( pCourse->getMaxCapacity() < 0 ? QString("null::integer") : QString::number( pCourse->getMaxCapacity() ) )
                    .arg( pCourse->isActive() ? QString("true") : QString("false") );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    int courseId = res->getCellAsInt( 0, 0 );
    return courseId;
}

int dbWriter::updateCourse( std::shared_ptr< Course > pCourse ) {
    if( pCourse == nullptr || pCourse->getId() < 0 )
        return -1;
    QString sql = QString("select update_course( %1, %2, %3, %4, %5, %6, %7, %8, %9 );")
                    .arg(  pCourse->getId() )
                    .arg( pCourse->getCode().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( pCourse->getCode() ) ) )
                    .arg( pCourse->getName().empty() ? QString("null::varchar") : QString("'%1'").arg( QString::fromStdString( pCourse->getName() ) ) )
                    .arg( pCourse->getDesc().empty() ? QString("null::text") : QString("'%1'").arg( QString::fromStdString( pCourse->getDesc() ) ) )
                    .arg( pCourse->getCreditMark() < 0 ? QString("null::integer") : QString::number( pCourse->getCreditMark() ) )
                    .arg( pCourse->getDepartment() == nullptr ? QString("null::integer") : QString::number( pCourse->getDepartment()->getId() ) )
                    .arg( pCourse->getProfessor().empty() ? QString("null::varchar") :  QString("'%1'").arg( QString::fromStdString( pCourse->getProfessor() ) ) )
                    .arg( pCourse->getMaxCapacity() < 0 ? QString("null::integer") : QString::number( pCourse->getMaxCapacity() ) )
                    .arg( pCourse->isActive() ? QString("true") : QString("false") );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;

    int courseId = res->getCellAsInt( 0, 0 );
    return courseId;

}

int dbWriter::deleteCourse( int idCourse ) {
    if( idCourse < 0 )
        return idCourse;
    QString sql = QString("select delete_course( %1 );").arg( idCourse );
    std::shared_ptr< DbResult > res = m_db->execute( sql.toStdString().c_str() );
    if( res == nullptr || res->getRowCount() != 1 )
        return -1;
    return idCourse;
}
