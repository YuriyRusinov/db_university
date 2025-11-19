#include <QSettings>
#include <QStandardItemModel>
#include <QtDebug>

#include <university_db.hpp>
#include <university_pg_db.hpp>
#include <dbLoginForm.h>
#include <dbEntitiesForm.h>

#include <department.hpp>
#include <students.hpp>
#include <courses.hpp>
#include <enrollments.hpp>

#include "dbCore.h"
#include "dbLoader.h"

bool dbCore::GUIConnect( QWidget* parent, Qt::WindowFlags flags )
{
    qDebug() << __PRETTY_FUNCTION__;
    dbLoginForm* dbConnForm = new dbLoginForm( parent, flags );
    m_cvSettings->beginGroup("System Settings");
    QString iUser = m_cvSettings->value("Username").toString();
    QString iDatabase = m_cvSettings->value("Database").toString();
    QString iServer = m_cvSettings->value("Server").toString();
    bool retVal = false;
    int port = m_cvSettings->value("Port", QString::number(5432)).toInt();
    dbConnForm->setUserName( iUser );
    dbConnForm->setDatabase( iDatabase );
    dbConnForm->setIpServer( iServer );
    dbConnForm->setDbPort( port );
    if( dbConnForm != nullptr && dbConnForm->exec() == QDialog::Accepted ) {
        QString user = dbConnForm->getUserName();
        QString password = dbConnForm->getPassword();
        QString dataBase = dbConnForm->getDatabase();
        QString server = dbConnForm->getIpServer();
        int port = dbConnForm->getDbPort();
        qDebug() << __PRETTY_FUNCTION__ << user << password << dataBase << server << port;
        retVal = m_Db->connect( server.toStdString(), dataBase.toStdString(), user.toStdString(), password.toStdString(), port );
        qDebug() << __PRETTY_FUNCTION__ << retVal;
        if( retVal ) {
            m_cvSettings->setValue("Username", user);
            m_cvSettings->setValue("Database", dataBase);
            m_cvSettings->setValue("Server", server);
            m_cvSettings->setValue("Port", port);
        }
    }
    if( dbConnForm )
        delete dbConnForm;
    m_cvSettings->endGroup();
    return retVal;
}

void dbCore::dbDisconnect() {
    m_Db->disconnect();
}

std::unique_ptr<dbCore> dbCore::create( QObject* parent ) {
    return std::unique_ptr<dbCore>( new dbCore( parent ) );
}

dbCore::dbCore( QObject* parent )
    : QObject( parent ),
    m_Db( std::make_shared<UniversityPgDb>() ),
    m_dbLoader( std::make_shared<dbLoader>(m_Db) ),
    m_cvSettings( new QSettings(QSettings::NativeFormat, QSettings::UserScope, tr("UniversityStudents")) )
{
}

dbCore::~dbCore()
{
    qDebug() << __FUNCTION__;
    delete m_cvSettings;
}

std::shared_ptr<UniversityDb> dbCore::getDb() const
{
    return m_Db;
}

bool dbCore::GUIViewDepartments( QWidget* parent, Qt::WindowFlags flags )
{
    dbEntitiesForm* dbWidget = new dbEntitiesForm( entityTypes::eDepartments, parent, flags );
    dbWidget->setWindowTitle( tr("Departments") );
    if( dbWidget == nullptr )
        return false;
    QStandardItemModel * depModel = new QStandardItemModel;//(ndeps, 8);
    initDepModel( depModel );
    dbWidget->setEntitiesModel( depModel );
    connect( dbWidget, &dbEntitiesForm::refreshModel, this, &dbCore::refreshModel );
    emit setWidget( dbWidget );
    return true;
}

bool dbCore::GUIViewStudents( QWidget* parent, Qt::WindowFlags flags ) {
    dbEntitiesForm* dbWidget = new dbEntitiesForm( entityTypes::eStudents, parent, flags );
    dbWidget->setWindowTitle( tr("Students") );
    if( dbWidget == nullptr )
        return false;

    QStandardItemModel* sModel = new QStandardItemModel(0, 0);
    initStudentsModel( sModel );
    dbWidget->setEntitiesModel( sModel );

    connect( dbWidget, &dbEntitiesForm::refreshModel, this, &dbCore::refreshModel );
    emit setWidget( dbWidget );
    return true;
}

bool dbCore::GUIViewCourses( QWidget* parent, Qt::WindowFlags flags ) {
    dbEntitiesForm* dbWidget = new dbEntitiesForm( entityTypes::eCourses, parent, flags );
    dbWidget->setWindowTitle( tr("Courses") );
    if( dbWidget == nullptr )
        return false;

    QStandardItemModel* coursesMod = new QStandardItemModel(0, 8);
    initCourseModel( coursesMod );
    dbWidget->setEntitiesModel( coursesMod );
    connect( dbWidget, &dbEntitiesForm::refreshModel, this, &dbCore::refreshModel );
    emit setWidget( dbWidget );
    return true;
}

bool dbCore::GUIViewEnrollments( QWidget* parent, Qt::WindowFlags flags ) {
    dbEntitiesForm* dbWidget = new dbEntitiesForm( entityTypes::eEnrollments, parent, flags );
    dbWidget->setWindowTitle( tr("Enrollments") );
    if( dbWidget == nullptr )
        return false;
    QStandardItemModel* eModel = new QStandardItemModel;//(n, 4);
    initEnrollModel( eModel );
    dbWidget->setEntitiesModel( eModel );
    connect( dbWidget, &dbEntitiesForm::refreshModel, this, &dbCore::refreshModel );
    emit setWidget( dbWidget );
    return true;
}

void dbCore::refreshModel(int eType, QAbstractItemModel* mod ) {
    qDebug() << __PRETTY_FUNCTION__ << eType;
    if( mod == nullptr )
        return;

    switch( eType ) {
        case entityTypes::eStudents: initStudentsModel( mod ); break;
        case entityTypes::eDepartments: initDepModel( mod ); break;
        case entityTypes::eCourses: initCourseModel( mod ); break;
        case entityTypes::eEnrollments: initEnrollModel( mod ); break;
        default: break;
    }
    return;
}

void dbCore::initStudentsModel( QAbstractItemModel* model ) {
    if( model == nullptr )
        return;
    int nr = model->rowCount();
    int nc = model->columnCount();
    model->removeRows( 0, nr);
    model->removeColumns( 0, nc );
    model->insertColumns( 0, 2);
    QStringList sHeaders;
    sHeaders << tr("Student ID")
             << tr("Student full name");
    std::vector<Student> vStudents = m_dbLoader->loadStudents();
    size_t n = vStudents.size();
    model->insertRows( 0, n );
    for(int i=0; i<2; i++)
        model->setHeaderData(i, Qt::Horizontal, sHeaders[i], Qt::DisplayRole);
    for(int i=0; i<n; i++) {
        QModelIndex wIndex = model->index(i, 0);
        model->setData(wIndex, vStudents[i].getId(), Qt::DisplayRole);
        model->setData(wIndex, vStudents[i].getId(), Qt::UserRole);
        wIndex = model->index(i, 1);
        model->setData(wIndex, QString::fromStdString(vStudents[i].generateFullName()), Qt::DisplayRole);
    }
}

void dbCore::initDepModel( QAbstractItemModel* model ) {
    if( model == nullptr )
        return;

    int nr = model->rowCount();
    int nc = model->columnCount();
    model->removeRows( 0, nr );
    model->removeColumns( 0, nc );
    model->insertColumns( 0, 8 );
    std::vector< Department > vDeps = m_dbLoader->loadDepartments();
    size_t ndeps = vDeps.size();
    qDebug() << __PRETTY_FUNCTION__ << " number of departments is " << ndeps;
    model->insertRows( 0, ndeps );
    QStringList headers;
    headers << tr("ID")
            << tr("Department code")
            << tr("Department name")
            << tr("Chair person")
            << tr("Office location")
            << tr("Budget")
            << tr("Established date")
            << tr("Is Active");
    for(int i=0; i<8; i++)
        model->setHeaderData(i, Qt::Horizontal, headers[i], Qt::DisplayRole);
    for(int i=0; i<ndeps; i++) {
        QModelIndex wIndex = model->index(i, 0);
        model->setData(wIndex, vDeps[i].getId(), Qt::DisplayRole);
        model->setData(wIndex, vDeps[i].getId(), Qt::UserRole);
        wIndex = model->index(i, 1);
        model->setData(wIndex, QString::fromStdString(vDeps[i].getCode()), Qt::DisplayRole);
        wIndex = model->index(i, 2);
        model->setData(wIndex, QString::fromStdString(vDeps[i].getName()), Qt::DisplayRole);
        wIndex = model->index(i, 3);
        model->setData(wIndex, QString::fromStdString(vDeps[i].getChairPerson()), Qt::DisplayRole);
        wIndex = model->index(i, 4);
        model->setData(wIndex, QString::fromStdString(vDeps[i].getOfficeLocation()), Qt::DisplayRole);
        wIndex = model->index(i, 5);
        model->setData(wIndex, QString::number(vDeps[i].getBudget()), Qt::DisplayRole);
        wIndex = model->index(i, 6);
        model->setData(wIndex, vDeps[i].getEstablishedDate().toString(Qt::ISODate), Qt::DisplayRole);
        wIndex = model->index(i, 7);
        model->setData(wIndex, (vDeps[i].isActive() ? QString("Active") : QString("Not active")), Qt::DisplayRole);
    }
}

void dbCore::initCourseModel( QAbstractItemModel* model ) {
    if( model == nullptr )
        return;

    int nr = model->rowCount();
    int nc = model->columnCount();
    model->removeRows( 0, nr );
    model->removeColumns( 0, nc );
    std::vector< Course > vCourses = m_dbLoader->loadCourses();
    size_t n = vCourses.size();
    QStringList courseHeaders;
    courseHeaders << tr("ID Course")
                  << tr("Code")
                  << tr("Name")
                  << tr("Description")
                  << tr("Department name")
                  << tr("Professor name")
                  << tr("Max capacity")
                  << tr("Is Active");
    model->insertColumns( 0, courseHeaders.size() );
    model->insertRows(0, n);
    for(int i=0; i<courseHeaders.size(); i++)
        model->setHeaderData( i, Qt::Horizontal, courseHeaders[i], Qt::DisplayRole );
    for(int i=0; i<n; i++) {
        QModelIndex wIndex = model->index(i, 0);
        model->setData( wIndex, vCourses[i].getId(), Qt::DisplayRole );
        model->setData( wIndex, vCourses[i].getId(), Qt::UserRole );
        wIndex = model->index(i, 1);
        model->setData( wIndex, QString::fromStdString(vCourses[i].getCode()), Qt::DisplayRole );
        wIndex = model->index(i, 2);
        model->setData( wIndex, QString::fromStdString(vCourses[i].getName()), Qt::DisplayRole );
        wIndex = model->index(i, 3);
        model->setData( wIndex, QString::fromStdString(vCourses[i].getDesc()), Qt::DisplayRole );
        wIndex = model->index(i, 4);
        model->setData( wIndex, QString::fromStdString(vCourses[i].getDepartment()->getName()), Qt::DisplayRole );
        wIndex = model->index(i, 5);
        model->setData( wIndex, QString::fromStdString(vCourses[i].getProfessor()), Qt::DisplayRole );
        wIndex = model->index(i, 6);
        model->setData( wIndex, vCourses[i].getMaxCapacity(), Qt::DisplayRole );
        wIndex = model->index(i, 7);
        model->setData( wIndex, (vCourses[i].isActive() ? tr("Active") : tr("not active")), Qt::DisplayRole );
    }
}

void dbCore::initEnrollModel( QAbstractItemModel* model ) {
    if( model == nullptr )
        return;
    int nr = model->rowCount();
    int nc = model->columnCount();
    model->removeRows( 0, nr );
    model->removeColumns( 0, nc );
    std::vector< Enrollments > vEnrollments = m_dbLoader->loadEnrollments();
    int n = vEnrollments.size();
    model->insertColumns( 0, 4 );
    model->insertRows( 0, n );
    QStringList eHeaders;
    eHeaders << tr("Enrollment ID")
        << tr("Student")
        << tr("Course")
        << tr("Semester");
    for(int i=0; i<eHeaders.size(); i++)
        model->setHeaderData( i, Qt::Horizontal, eHeaders[i], Qt::DisplayRole );

    for(int i=0; i<n; i++) {
        QModelIndex wIndex = model->index(i, 0);
        model->setData( wIndex, vEnrollments[i].getId(), Qt::DisplayRole );
        model->setData( wIndex, vEnrollments[i].getId(), Qt::UserRole );
        wIndex = model->index(i, 1);
        model->setData( wIndex, QString::fromStdString(vEnrollments[i].getStudent()->generateFullName()), Qt::DisplayRole );
        wIndex = model->index(i, 2);
        model->setData( wIndex, QString::fromStdString(vEnrollments[i].getCourse()->getName()), Qt::DisplayRole );
        wIndex = model->index(i, 3);
        model->setData( wIndex, QString::fromStdString(vEnrollments[i].getSemester()), Qt::DisplayRole );
    }
}
