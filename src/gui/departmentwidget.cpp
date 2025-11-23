#include <QAction>
#include <QDoubleValidator>
#include <QtDebug>

#include <department.hpp>
#include "departmentwidget.h"
#include "ui_department_widget.h"

DepartmentWidget::DepartmentWidget( std::shared_ptr< Department > pDep, QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    _UI( new Ui::department_widget ),
    m_pDepartment( pDep )
{
    _UI->setupUi( this );
    QAction* actDepSave = new QAction( tr("Save Department"), this );
    addAction( actDepSave );
    actDepSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    initForm();

    connect( actDepSave, &QAction::triggered, this, &DepartmentWidget::saveDepartment );
}

DepartmentWidget::~DepartmentWidget() {
    delete _UI;
}

std::shared_ptr< Department > DepartmentWidget::getDepartment() const {
    return m_pDepartment;
}

void DepartmentWidget::setDepartment( std::shared_ptr< Department > pDep ) {
    m_pDepartment = pDep;
    initForm();
}

void DepartmentWidget::saveDepartment() {
    qDebug() << __PRETTY_FUNCTION__;
    m_pDepartment->setCode( _UI->leDepartmentCode->text().toStdString() );
    m_pDepartment->setName( _UI->lEDepartmentName->text().toStdString() );
    m_pDepartment->setChairPerson( _UI->lEChairPerson->text().toStdString() );
    m_pDepartment->setOfficeLocation( _UI->tEOfficeLocation->toPlainText().toStdString() );
    m_pDepartment->setBudget( _UI->lEBudget->text().toDouble() );
    m_pDepartment->setEstablishedDate( _UI->dEEstablishedDate->date() );
    m_pDepartment->setActive( (_UI->chDepActive->checkState() == Qt::Checked) );

    emit saveDepartmentToDb( m_pDepartment );
}

void DepartmentWidget::initForm( ) {
    if( m_pDepartment == nullptr )
        return;

    if( _UI->lEBudget->validator() == nullptr ) {
        QValidator* budgetVal = new QDoubleValidator(0.0, 100000.0, 2);
        _UI->lEBudget->setValidator( budgetVal );
    }
    _UI->leDepartmentCode->setText( QString::fromStdString( m_pDepartment->getCode() ) );
    _UI->lEDepartmentName->setText( QString::fromStdString( m_pDepartment->getName() ) );
    _UI->lEChairPerson->setText( QString::fromStdString( m_pDepartment->getChairPerson() ) );
    _UI->tEOfficeLocation->setPlainText( QString::fromStdString( m_pDepartment->getOfficeLocation() ) );
    _UI->lEBudget->setText( QString::number( m_pDepartment->getBudget() ) );
    _UI->dEEstablishedDate->setDate( (m_pDepartment->getEstablishedDate().isValid() ? m_pDepartment->getEstablishedDate() : QDate::currentDate() ) );
    _UI->chDepActive->setCheckState( (m_pDepartment->isActive() ? Qt::Checked : Qt::Unchecked) );
}
