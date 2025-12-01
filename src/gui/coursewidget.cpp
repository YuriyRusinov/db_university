#include <QAction>
#include <QtDebug>
#include <QValidator>
#include <QIntValidator>
#include <QMessageBox>

#include <courses.hpp>
#include <department.hpp>

#include "coursewidget.h"
#include "ui_course_widget.h"

CourseWidget::CourseWidget( std::shared_ptr< Course > pCourse, QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    _UI( new Ui::course_widget ),
    m_pCourse( pCourse )
{
    _UI->setupUi( this );
    QAction* actCourseSave = new QAction( tr("Save Course"), this );
    addAction( actCourseSave );
    actCourseSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    initForm();

    connect( actCourseSave, &QAction::triggered, this, &CourseWidget::saveCourse );
    connect( _UI->tbDepartment, &QAbstractButton::clicked, this, &CourseWidget::selectDepartment );
    connect( _UI->tbRefresh, &QAbstractButton::clicked, this, &CourseWidget::refreshStudents );
    connect( _UI->tbAddStudent, &QAbstractButton::clicked, this, &CourseWidget::addStudentToCourse );
    connect( _UI->tbEditStatus, &QAbstractButton::clicked, this, &CourseWidget::editStudentOnCourse );
    connect( _UI->tbDelStudent,  &QAbstractButton::clicked, this, &CourseWidget::withdrawStudentFromCourse );
}

CourseWidget::~CourseWidget() {
    delete _UI;
}

void CourseWidget::setCourse( std::shared_ptr< Course > pCourse ) {
    m_pCourse = pCourse;
    initForm();
}

void CourseWidget::saveCourse() {
    qDebug() << __PRETTY_FUNCTION__;
    m_pCourse->setCode( _UI->lECourseCode->text().toStdString() );
    m_pCourse->setName( _UI->lECourseName->text().toStdString() );
    m_pCourse->setDesc( _UI->tECourseDescription->toPlainText().toStdString() );
    m_pCourse->setCreditMark( _UI->lECourseCredits->text().toInt() );
    if( m_pCourse->getDepartment() == nullptr ) {
        QMessageBox::warning( this, tr("Course department"), tr("Error: please set lead department of this course"), QMessageBox::Ok );
        return;
    }
    m_pCourse->setProfessor( _UI->lEInstructorName->text().toStdString() );
    m_pCourse->setMaxCapacity( _UI->lEMaxCapacity->text().toInt() );
    m_pCourse->setActive( (_UI->chIsActive->checkState() == Qt::Checked) );
    emit saveCourseToDb( m_pCourse );
}

void CourseWidget::selectDepartment() {
    qDebug() << __PRETTY_FUNCTION__;
    emit loadDepartments( m_pCourse );
}

void CourseWidget::refreshStudents() {
    qDebug() << __PRETTY_FUNCTION__;
}

void CourseWidget::addStudentToCourse() {
    qDebug() << __PRETTY_FUNCTION__;
}

void CourseWidget::editStudentOnCourse() {
    qDebug() << __PRETTY_FUNCTION__;
}

void CourseWidget::withdrawStudentFromCourse() {
    qDebug() << __PRETTY_FUNCTION__;
}

void CourseWidget::initForm() {
    if( m_pCourse == nullptr )
        return;

    _UI->lECourseCode->setText( QString::fromStdString( m_pCourse->getCode() ) );
    _UI->lECourseName->setText( QString::fromStdString( m_pCourse->getName() ) );
    _UI->tECourseDescription->setPlainText( QString::fromStdString( m_pCourse->getDesc() ) );
    const QValidator* old_crVal = _UI->lECourseCredits->validator();
    if( old_crVal == nullptr ) {
        QValidator* crVal = new QIntValidator(1, 5, this);
        _UI->lECourseCredits->setValidator( crVal );
        delete old_crVal;
    }
    _UI->lECourseCredits->setText( QString::number( m_pCourse->getCreditMark() ) );
    if( m_pCourse->getDepartment() != nullptr )
        _UI->lEDepartment->setText( QString::fromStdString( m_pCourse->getDepartment()->getName() ) );
    _UI->lEInstructorName->setText( QString::fromStdString( m_pCourse->getProfessor() ) );
    const QValidator* old_capVal = _UI->lEMaxCapacity->validator();
    if( old_capVal == nullptr ) {
        QValidator* capVal = new QIntValidator(1, 10000, this );
        _UI->lEMaxCapacity->setValidator( capVal );
        delete old_capVal;
    }
    _UI->lEMaxCapacity->setText( QString::number( m_pCourse->getMaxCapacity() ) );
    if( m_pCourse->isActive() )
        _UI->chIsActive->setCheckState( Qt::Checked );
    else
        _UI->chIsActive->setCheckState( Qt::Unchecked );

    bool isStudentsEnable( m_pCourse->getId() >= 0 );
    _UI->gbCourseStudents->setEnabled( isStudentsEnable );
}

void CourseWidget::setStudentsModel( QAbstractItemModel* studCourseModel ) {
    QAbstractItemModel* oldMod = _UI->tvStudentsCourse->model();
    _UI->tvStudentsCourse->setModel( studCourseModel );
    if( oldMod )
        delete oldMod;
}

void CourseWidget::setCourseDepartment( std::shared_ptr< Department > pDep ) {
    if ( pDep == nullptr ) {
        _UI->lEDepartment->clear();
    }
    else
        _UI->lEDepartment->setText( QString::fromStdString(pDep->getName() ) );
}
