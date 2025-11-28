#include <QAction>
#include <QtDebug>
#include <QComboBox>
#include <QValidator>
#include <QDoubleValidator>
#include <students.hpp>
#include "studentwidget.h"
#include "ui_student_widget.h"

StudentWidget::StudentWidget( std::shared_ptr< Student > pStudent, QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    _UI( new Ui::student_widget ),
    m_pStudent( pStudent )
{
    _UI->setupUi( this );
    QAction* actStudentSave = new QAction( tr("Save Student"), this );
    addAction( actStudentSave );
    actStudentSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    initForm();

    connect( actStudentSave, &QAction::triggered, this, &StudentWidget::saveStudent );
    connect( _UI->chEnableProfile, &QCheckBox::stateChanged, this, &StudentWidget::setProfileEnabled );
}

StudentWidget::~StudentWidget() {
    delete _UI;
}

std::shared_ptr< Student > StudentWidget::getStudent() const {
    return m_pStudent;
}

void StudentWidget::setStudent( std::shared_ptr< Student > pStudent ) {
    m_pStudent = pStudent;
    initForm();
}

void StudentWidget::saveStudent() {
    qDebug() << __PRETTY_FUNCTION__;
    m_pStudent->setStudentNumber( _UI->lEStudentNumber->text().toStdString() );
    m_pStudent->setFirstName( _UI->lEStudentName->text().toStdString() );
    m_pStudent->setMiddleName( _UI->lEMiddleName->text().toStdString());
    m_pStudent->setLastName( _UI->lELastName->text().toStdString() );
    m_pStudent->setEmail( _UI->lEEmail->text().toStdString() );
    m_pStudent->setBirthDate( _UI->dEBirthDate->date() );
    m_pStudent->setEnrollmentDate( _UI->dEEnrollmentDate->date() );
    m_pStudent->setGraduationDate( _UI->dEGraduateDate->date() );
    std::optional< StudentProfile > sP = std::nullopt;
    if( _UI->chEnableProfile->checkState() == Qt::Checked ) {
        sP = StudentProfile();
        sP->setStudentId( m_pStudent->getId() );
        sP->setPhone( _UI->lEPhoneNumber->text().toStdString() );
        sP->setEmergencyPerson( _UI->lEEmergencyContactName->text().toStdString() );
        sP->setEmergencyPhone( _UI->lEEmergencyContactPhone->text().toStdString() );
        sP->setAddress( _UI->tEAddress->toPlainText().toStdString() );
        sP->setCity( _UI->lECity->text().toStdString() );
        sP->setState( _UI->lEState->text().toStdString() );
        sP->setPostalCode( _UI->lEPostalCode->text().toStdString() );
        sP->setCountry( _UI->lECountry->text().toStdString() );
        sP->setMedNotes( _UI->tEMedicalNotes->toPlainText().toStdString() );
        sP->setAcademicStanding( (AcademicStanding)_UI->cbAcademicStanding->currentData().toInt() );
        int pos;
        QString fStrBalance = _UI->lEFinanceBalance->text();
        if( _UI->lEFinanceBalance->validator()->validate( fStrBalance, pos ) == QValidator::Acceptable )
            sP->setFinanceBalance( _UI->lEFinanceBalance->text().toDouble() );
        qDebug() << __PRETTY_FUNCTION__ << sP.has_value();
        m_pStudent->setStudentProfile( sP );
    }
    else
        m_pStudent->setStudentProfile( std::nullopt );
    emit saveStudentToDb( m_pStudent );
}

void StudentWidget::initForm() {
    if( m_pStudent == nullptr )
        return;

    QStringList AcademicStrList;
    AcademicStrList << tr("Unknown")
                    << tr("Good")
                    << tr("Probation")
                    << tr("Warning")
                    << tr("Suspended")
                    << tr("Graduated");
    QVector<int> vAcademic;

    vAcademic += (int)AcademicStanding::Unknown;
    vAcademic += (int)AcademicStanding::Good;
    vAcademic += (int)AcademicStanding::Probation;
    vAcademic += (int)AcademicStanding::Warning;
    vAcademic += (int)AcademicStanding::Suspended;
    vAcademic += (int)AcademicStanding::Graduated;
    for(int i=0; i<AcademicStrList.size(); i++)
        _UI->cbAcademicStanding->addItem(AcademicStrList[i], QVariant(vAcademic[i]) );
    QValidator* vBalance = new QDoubleValidator( -50000, 50000, 2, this );
    _UI->lEFinanceBalance->setValidator( vBalance );
    _UI->lEStudentNumber->setText( QString::fromStdString( m_pStudent->getStudentNumber() ) );
    _UI->lEStudentName->setText( QString::fromStdString( m_pStudent->getFirstName() ) );
    _UI->lEMiddleName->setText( QString::fromStdString( m_pStudent->getMiddleName() ) );
    _UI->lELastName->setText( QString::fromStdString( m_pStudent->getLastName() ) );
    _UI->lEEmail->setText( QString::fromStdString( m_pStudent->getEmail() ) );
    _UI->dEBirthDate->setDate( m_pStudent->getBirthDate() );
    _UI->dEEnrollmentDate->setDate( m_pStudent->getEnrollmentDate() );
    _UI->dEGraduateDate->setDate( m_pStudent->getGraduationDate() );
    if( m_pStudent->getProfile().has_value() ) {
        _UI->lEPhoneNumber->setText( QString::fromStdString( m_pStudent->getProfile()->getPhone() ) );
        _UI->lEEmergencyContactName->setText( QString::fromStdString( m_pStudent->getProfile()->getEmergencyPerson() ) );
        _UI->lEEmergencyContactPhone->setText( QString::fromStdString( m_pStudent->getProfile()->getEmergencyPhone() ) );
        _UI->tEAddress->setPlainText( QString::fromStdString( m_pStudent->getProfile()->getAddress() ) );
        _UI->lECity->setText( QString::fromStdString( m_pStudent->getProfile()->getCity() ) );
        _UI->lEState->setText( QString::fromStdString( m_pStudent->getProfile()->getState() ) );
        _UI->lEPostalCode->setText( QString::fromStdString( m_pStudent->getProfile()->getPostalCode() ) );
        qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString( m_pStudent->getProfile()->getCountry() );
        _UI->lECountry->setText( QString::fromStdString( m_pStudent->getProfile()->getCountry() ) );
        _UI->tEMedicalNotes->setPlainText( QString::fromStdString( m_pStudent->getProfile()->getMedNotes() ) );
        AcademicStanding aSV = m_pStudent->getProfile()->getAcademicStanding();
        int ind =_UI->cbAcademicStanding->findData( (int)aSV );
        _UI->cbAcademicStanding->setCurrentIndex( ind );
        _UI->lEFinanceBalance->setText( QString::number( m_pStudent->getProfile()->getFinancialBalance() ) );
        _UI->chEnableProfile->setCheckState( Qt::Checked );
    }
    else
        _UI->chEnableProfile->setCheckState( Qt::Unchecked );
    setProfileEnabled( _UI->chEnableProfile->checkState() );
}

void StudentWidget::setProfileEnabled(int state) {
    bool isEnable = (state == Qt::Checked);
    _UI->lEPhoneNumber->setEnabled( isEnable );
    _UI->lEEmergencyContactName->setEnabled( isEnable );
    _UI->lEEmergencyContactPhone->setEnabled( isEnable );
    _UI->tEAddress->setEnabled( isEnable );
    _UI->lECity->setEnabled( isEnable );
    _UI->lEState->setEnabled( isEnable );
    _UI->lEPostalCode->setEnabled( isEnable );
    _UI->lECountry->setEnabled( isEnable );
    _UI->tEMedicalNotes->setEnabled( isEnable );
    _UI->cbAcademicStanding->setEnabled( isEnable );
    _UI->lEFinanceBalance->setEnabled( isEnable );
    _UI->chEnableProfile->setEnabled( true );
}
