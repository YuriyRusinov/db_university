#include <QSettings>
#include <QStandardItemModel>
#include <QtDebug>

#include <university_db.hpp>
#include <university_pg_db.hpp>
#include <dbLoginForm.h>
#include <dbEntitiesForm.h>

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
    std::vector< Department > vDeps = m_dbLoader->loadDepartments();
    size_t ndeps = vDeps.size();
    qDebug() << __PRETTY_FUNCTION__ << " number of departments is " << ndeps;
    QStandardItemModel * depModel = new QStandardItemModel(ndeps, 8);
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
        depModel->setHeaderData(i, Qt::Horizontal, headers[i], Qt::DisplayRole);
    for(int i=0; i<ndeps; i++) {
        QModelIndex wIndex = depModel->index(i, 0);
        depModel->setData(wIndex, QString::number(vDeps[i].getId()), Qt::DisplayRole);
        wIndex = depModel->index(i, 1);
        depModel->setData(wIndex, QString::fromStdString(vDeps[i].getCode()), Qt::DisplayRole);
        wIndex = depModel->index(i, 2);
        depModel->setData(wIndex, QString::fromStdString(vDeps[i].getName()), Qt::DisplayRole);
        wIndex = depModel->index(i, 3);
        depModel->setData(wIndex, QString::fromStdString(vDeps[i].getChairPerson()), Qt::DisplayRole);
        wIndex = depModel->index(i, 4);
        depModel->setData(wIndex, QString::fromStdString(vDeps[i].getOfficeLocation()), Qt::DisplayRole);
        wIndex = depModel->index(i, 5);
        depModel->setData(wIndex, QString::number(vDeps[i].getBudget()), Qt::DisplayRole);
        wIndex = depModel->index(i, 6);
        depModel->setData(wIndex, vDeps[i].getEstablishedDate().toString(Qt::ISODate), Qt::DisplayRole);
        wIndex = depModel->index(i, 7);
        depModel->setData(wIndex, (vDeps[i].isActive() ? QString("Active") : QString("Not active")), Qt::DisplayRole);
    }
    dbWidget->setEntitiesModel( depModel );
    emit setWidget( dbWidget );
    return true;
}

bool dbCore::GUIViewStudents( QWidget* parent, Qt::WindowFlags flags ) {
    dbEntitiesForm* dbWidget = new dbEntitiesForm( entityTypes::eStudents, parent, flags );
    dbWidget->setWindowTitle( tr("Students") );
    if( dbWidget == nullptr )
        return false;

    std::vector<Student> vStudents = m_dbLoader->loadStudents();
    size_t n = vStudents.size();
    QStandardItemModel* sModel = new QStandardItemModel(n, 2);
    QStringList sHeaders;
    sHeaders << tr("Student ID")
             << tr("Student full name");
    for(int i=0; i<2; i++)
        sModel->setHeaderData(i, Qt::Horizontal, sHeaders[i], Qt::DisplayRole);
    for(int i=0; i<n; i++) {
        QModelIndex wIndex = sModel->index(i, 0);
        sModel->setData(wIndex, vStudents[i].getId(), Qt::DisplayRole);
        sModel->setData(wIndex, vStudents[i].getId(), Qt::UserRole);
        wIndex = sModel->index(i, 1);
        sModel->setData(wIndex, QString::fromStdString(vStudents[i].generateFullName()), Qt::DisplayRole);
    }
    dbWidget->setEntitiesModel( sModel );

    emit setWidget( dbWidget );
    return true;
}

bool dbCore::GUIViewCourses( QWidget* parent, Qt::WindowFlags flags ) {
    dbEntitiesForm* dbWidget = new dbEntitiesForm( entityTypes::eCourses, parent, flags );
    dbWidget->setWindowTitle( tr("Courses") );
    if( dbWidget == nullptr )
        return false;

    emit setWidget( dbWidget );
    return true;

}

bool dbCore::GUIViewEnrollments( QWidget* parent, Qt::WindowFlags flags ) {
    dbEntitiesForm* dbWidget = new dbEntitiesForm( entityTypes::eEnrollments, parent, flags );
    dbWidget->setWindowTitle( tr("Enrollments") );
    if( dbWidget == nullptr )
        return false;

    emit setWidget( dbWidget );
    return true;
}
