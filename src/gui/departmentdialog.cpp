#include <QAbstractItemModel>
#include <QItemSelection>

#include "departmentdialog.h"
#include "ui_department_dialog.h"

DepartmentDialog::DepartmentDialog( QWidget* parent, Qt::WindowFlags flags )
    : QDialog( parent, flags ),
    _UI( new Ui::department_dialog )
{
    _UI->setupUi( this );
}

DepartmentDialog::~DepartmentDialog() {
    delete _UI;
}

void DepartmentDialog::setModel( QAbstractItemModel* mod ) {
    QAbstractItemModel* oldMod = _UI->tvDepartments->model();
    _UI->tvDepartments->setModel( mod );
    if( oldMod && oldMod != mod )
        delete oldMod;
}

int DepartmentDialog::getDepartmentId() const {
    QItemSelection depSel = _UI->tvDepartments->selectionModel()->selection();
    if( depSel.indexes().size() == 0 )
        return -1;

    int id = depSel.indexes().at(0).data( Qt::UserRole ).toInt();
    return id;
}

