#include <QSettings>
#include <QtDebug>
#include <university_db.hpp>
#include <university_pg_db.hpp>
#include <dbLoginForm.h>
#include "dbCore.h"

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
}

std::unique_ptr<dbCore> dbCore::create( QObject* parent ) {
    return std::unique_ptr<dbCore>( new dbCore( parent ) );
}

dbCore::dbCore( QObject* parent )
    : QObject( parent ),
    m_Db( std::make_shared<UniversityPgDb>() ),
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
