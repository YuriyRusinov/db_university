#pragma once

#include <QDialog>

class QAbstractItemModel;

namespace Ui {
    class department_dialog;
};

class DepartmentDialog : public QDialog {
public:
    DepartmentDialog( QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags() );
    virtual ~DepartmentDialog();

    void setModel( QAbstractItemModel* mod );
    int getDepartmentId() const;

private:
    Ui::department_dialog* _UI;

private:
    Q_OBJECT
};
