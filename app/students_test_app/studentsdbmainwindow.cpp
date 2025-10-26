#include <QMdiArea>

#include <QtDebug>
#include "studentsdbmainwindow.h"
#include "ui_students_main_window.h"

StudentsDbMainWindow::StudentsDbMainWindow( QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow( parent, flags ),
    _UI( new Ui::students_db_main_window ),
    m_mdiArea( new QMdiArea ),
    m_UnivSt( UniversitySingleton::getUnivStudentsS() ) 
{
    _UI->setupUi( this );
    setCentralWidget( m_mdiArea.get() );

    _UI->act_Connect->setIcon(QIcon(":/icons/db_connect.png"));
    _UI->act_Disconnect->setIcon(QIcon(":/icons/db_disconnect.png"));
    connect( _UI->act_Connect, &QAction::triggered, this, &StudentsDbMainWindow::slotDbConnect );
    connect( _UI->act_Disconnect, &QAction::triggered, this, &StudentsDbMainWindow::slotDbDisconnect );
    connect( _UI->act_Quit, &QAction::triggered, this, &QMainWindow::close );
}

StudentsDbMainWindow::~StudentsDbMainWindow() {
    delete _UI;
}

void StudentsDbMainWindow::slotDbConnect() {
    qDebug() << __PRETTY_FUNCTION__;
}

void StudentsDbMainWindow::slotDbDisconnect() {
    qDebug() << __PRETTY_FUNCTION__;
}


