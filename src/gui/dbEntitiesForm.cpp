#include <QAbstractItemModel>
#include <QToolButton>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QMessageBox>
#include <QtDebug>

#include "ui_db_entities_form.h"
#include "dbEntitiesForm.h"

dbEntitiesForm::dbEntitiesForm( int entityType, QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    _UI( new Ui::db_entities_form ),
    m_Type( (entityTypes)entityType )
{
    _UI->setupUi( this );

    connect( _UI->tbAddEntity, &QAbstractButton::clicked, this, &dbEntitiesForm::addEntity );
    connect( _UI->tbEditEntity, &QAbstractButton::clicked, this, &dbEntitiesForm::editEntity );
    connect( _UI->tbDelEntity, &QAbstractButton::clicked, this, &dbEntitiesForm::delEntity );
    connect( _UI->tbRefresh, &QAbstractButton::clicked, this, &dbEntitiesForm::refreshEntities );
}

dbEntitiesForm::~dbEntitiesForm() {
    delete _UI;
}

void dbEntitiesForm::setEntitiesModel( QAbstractItemModel* mod ) {
    QAbstractItemModel* oldModel = _UI->tvViewEntities->model();
    _UI->tvViewEntities->setModel( mod );
    if( oldModel )
        delete oldModel;
}

void dbEntitiesForm::addEntity() {
    qDebug() << __PRETTY_FUNCTION__ << m_Type;
    QAbstractItemModel* mod = _UI->tvViewEntities->model();
    emit addEntityToDB( m_Type, mod );
}

void dbEntitiesForm::editEntity() {
    QItemSelectionModel* selMod = _UI->tvViewEntities->selectionModel();
    QItemSelection sel = selMod->selection();
    QModelIndexList sIndexes = sel.indexes();
    if( sIndexes.size() == 0 )
    {
        QMessageBox::warning(this, tr("Edit"), tr("Select record for update"), QMessageBox::Ok );
        return;
    }
    QModelIndex wIndex = sIndexes[0];
    wIndex = wIndex.sibling(wIndex.row(), 0);
    qDebug() << __PRETTY_FUNCTION__ << wIndex << m_Type;
    emit updateEntityInDB( m_Type, wIndex, _UI->tvViewEntities->model() );
}

void dbEntitiesForm::delEntity() {
    qDebug() << __PRETTY_FUNCTION__;
    QItemSelectionModel* selMod = _UI->tvViewEntities->selectionModel();
    QItemSelection sel = selMod->selection();
    QModelIndexList sIndexes = sel.indexes();
    if( sIndexes.size() == 0 )
    {
        QMessageBox::warning(this, tr("Delete"), tr("Select record for delete"), QMessageBox::Ok );
        return;
    }
    QModelIndex wIndex = sIndexes[0];
    wIndex = wIndex.sibling(wIndex.row(), 0);
    qDebug() << __PRETTY_FUNCTION__ << wIndex << m_Type;
    QMessageBox::StandardButton isDel = QMessageBox::question(this, tr("Delete record"), tr("Do you really want to delete?"));
    if( isDel == QMessageBox::Yes ) {
        emit delEntityFromDB( m_Type, wIndex, _UI->tvViewEntities->model() );
    }
}

void dbEntitiesForm::refreshEntities() {
    qDebug() << __PRETTY_FUNCTION__ << m_Type;
    emit refreshModel( m_Type, _UI->tvViewEntities->model() );
}
