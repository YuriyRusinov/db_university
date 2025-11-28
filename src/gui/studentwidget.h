#pragma once

#include <QWidget>
#include <memory>

class Student;

namespace Ui {
    class student_widget;
};

class StudentWidget : public QWidget {
public:
    StudentWidget( std::shared_ptr< Student > pStudent = nullptr, QWidget* parent=nullptr, Qt::WindowFlags flags = Qt::WindowFlags() );
    virtual ~StudentWidget();

    std::shared_ptr< Student > getStudent() const;
    void setStudent( std::shared_ptr< Student > pStudent );

private slots:
    void saveStudent();
    void setProfileEnabled(int state);

signals:
    void saveStudentToDb( std::shared_ptr< Student > );

private:
    void initForm();

private:
    Ui::student_widget* _UI;
    std::shared_ptr< Student > m_pStudent;

private:
    Q_OBJECT
};
