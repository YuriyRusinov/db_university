#pragma once

#include <QWidget>

class QAbstractItemModel;

namespace Ui {
    class db_entities_form;
};

class dbEntitiesForm : public QWidget {
public:
    dbEntitiesForm( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~dbEntitiesForm();

    void setEntitiesModel( QAbstractItemModel* mod );

private slots:
    void addEntity();
    void editEntity();
    void delEntity();

private:
    Ui::db_entities_form* _UI;

private:
    Q_OBJECT
};
