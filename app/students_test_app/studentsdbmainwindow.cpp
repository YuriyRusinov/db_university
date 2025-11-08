#include <QMdiArea>
#include <QMdiSubWindow>
#include <QToolBar>

#include <QtDebug>
#include <dbCore.h>
#include "studentsdbmainwindow.h"
#include "ui_students_main_window.h"

StudentsDbMainWindow::StudentsDbMainWindow( QWidget* parent, Qt::WindowFlags flags)
    : QMainWindow( parent, flags ),
    _UI( new Ui::students_db_main_window ),
    m_mdiArea( new QMdiArea ),
    m_ToolBar( new QToolBar ),
    m_isDbconnect( false ),
    m_UnivSt( UniversitySingleton::getUnivStudentsS() ),
    m_dbC( m_UnivSt.getCore() )
{
    _UI->setupUi( this );
    setCentralWidget( m_mdiArea.get() );
    slotActionsEnable( m_isDbconnect );
    addToolBar( Qt::TopToolBarArea, m_ToolBar.get() );
    QAction* aConnect = m_ToolBar->addAction(QIcon(":/icons/db_connect.png"), tr("Connect to DB..."));
    connect( aConnect, &QAction::triggered, this, &StudentsDbMainWindow::slotDbConnect );
    QAction* aDisConnect = m_ToolBar->addAction( QIcon(":/icons/db_disconnect.png"), tr("Disconnect from DB"));
    connect( aDisConnect, &QAction::triggered, this, &StudentsDbMainWindow::slotDbDisconnect );
    m_ToolBar->addSeparator();
    QAction* aQuit = m_ToolBar->addAction( QIcon(":/icons/db_quit.png"), tr("Quit"));
    connect( aQuit, &QAction::triggered, this, &QMainWindow::close );

    connect( _UI->actView_departments, &QAction::triggered, this, &StudentsDbMainWindow::slotDepartments );
    connect( _UI->actView_students, &QAction::triggered, this, &StudentsDbMainWindow::slotStudents );
    connect( _UI->actView_courses, &QAction::triggered, this, &StudentsDbMainWindow::slotCourses );
    connect( _UI->actView_enrollments, &QAction::triggered, this, &StudentsDbMainWindow::slotEnrollments );

    _UI->act_Connect->setIcon( QIcon(":/icons/db_connect.png") );
    _UI->act_Disconnect->setIcon( QIcon(":/icons/db_disconnect.png") );
    _UI->act_Quit->setIcon( QIcon(":/icons/db_quit.png") );
    connect( _UI->act_Connect, &QAction::triggered, this, &StudentsDbMainWindow::slotDbConnect );
    connect( _UI->act_Disconnect, &QAction::triggered, this, &StudentsDbMainWindow::slotDbDisconnect );
    connect( _UI->act_Quit, &QAction::triggered, this, &QMainWindow::close );
    connect( m_dbC, &dbCore::setWidget, this, &StudentsDbMainWindow::slotAddWidget );
}

StudentsDbMainWindow::~StudentsDbMainWindow() {
    delete _UI;
}

void StudentsDbMainWindow::slotDbConnect() {
    m_isDbconnect = m_UnivSt.getCore()->GUIConnect();
    slotActionsEnable( m_isDbconnect );
}

void StudentsDbMainWindow::slotDbDisconnect() {
    qDebug() << __PRETTY_FUNCTION__;
    m_isDbconnect = false;
    m_UnivSt.getCore()->dbDisconnect();
    slotActionsEnable( m_isDbconnect );
}

void StudentsDbMainWindow::slotAddWidget( QWidget* w ) {
    if(w == nullptr)
        return;
    QMdiSubWindow * mdiW = m_mdiArea->addSubWindow( w, Qt::SubWindow );
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
    mdiW->setAttribute( Qt::WA_DeleteOnClose );
    mdiW->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    mdiW->setMinimumSize( QSize(512, 512) );
}

void StudentsDbMainWindow::slotDepartments() {
    qDebug() << __PRETTY_FUNCTION__;
    m_dbC->GUIViewDepartments();
}

void StudentsDbMainWindow::slotStudents() {
    qDebug() << __PRETTY_FUNCTION__;

}

void StudentsDbMainWindow::slotCourses() {
    qDebug() << __PRETTY_FUNCTION__;

}

void StudentsDbMainWindow::slotEnrollments() {
    qDebug() << __PRETTY_FUNCTION__;
}

void StudentsDbMainWindow::slotActionsEnable( bool isEnable ) {
    _UI->menu_University->setEnabled( isEnable );
}
