#pragma once

#include <QWidget>
#include <memory>

class Department;

namespace Ui {
    class department_widget;
};

class DepartmentWidget : public QWidget {
public:
    DepartmentWidget( std::shared_ptr< Department > pDep = nullptr, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags() );
    virtual ~DepartmentWidget();

    std::shared_ptr< Department > getDepartment() const;
    void setDepartment( std::shared_ptr< Department > pDep );

private slots:
    void saveDepartment();

signals:
    void saveDepartmentToDb( std::shared_ptr< Department > );

private:
    void initForm( );

private:
    Ui::department_widget* _UI;
    std::shared_ptr< Department > m_pDepartment;

private:
    Q_OBJECT
};
