#pragma once

#include <QWidget>

class QAbstractItemModel;

namespace Ui {
    class db_entities_form;
};

enum entityTypes {
    eDepartments = 0,
    eStudents,
    eCourses,
    eEnrollments
};

class dbEntitiesForm : public QWidget {
public:
    dbEntitiesForm( int entityType, QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    virtual ~dbEntitiesForm();

    void setEntitiesModel( QAbstractItemModel* mod );

private slots:
    void addEntity();
    void editEntity();
    void delEntity();
    void refreshEntities();

signals:
    void addEntityToDB( int eType, QAbstractItemModel* mod );
    void updateEntityInDB( int eType, const QModelIndex& wIndex, QAbstractItemModel* mod );
    void delEntityFromDB( int eType, const QModelIndex& wIndex, QAbstractItemModel* mod );
    void refreshModel( int eType, QAbstractItemModel* mod );

private:
    Ui::db_entities_form* _UI;
    entityTypes m_Type;

private:
    Q_OBJECT
};
