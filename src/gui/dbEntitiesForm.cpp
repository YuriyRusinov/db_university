#include <QAbstractItemModel>
#include <QToolButton>
#include <QtDebug>

#include "ui_db_entities_form.h"
#include "dbEntitiesForm.h"

dbEntitiesForm::dbEntitiesForm( QWidget* parent, Qt::WindowFlags flags )
    : QWidget( parent, flags ),
    _UI( new Ui::db_entities_form )
{
    _UI->setupUi( this );

    connect( _UI->tbAddEntity, &QAbstractButton::clicked, this, &dbEntitiesForm::addEntity );
    connect( _UI->tbEditEntity, &QAbstractButton::clicked, this, &dbEntitiesForm::editEntity );
    connect( _UI->tbDelEntity, &QAbstractButton::clicked, this, &dbEntitiesForm::delEntity );
}

dbEntitiesForm::~dbEntitiesForm() {
    delete _UI;
}

void dbEntitiesForm::setEntitiesModel( QAbstractItemModel* mod ) {
    QAbstractItemModel *oldModel = _UI->tvViewEntities->model();
    _UI->tvViewEntities->setModel( mod );
    if( oldModel )
        delete oldModel;
}

void dbEntitiesForm::addEntity() {
    qDebug() << __PRETTY_FUNCTION__;
}

void dbEntitiesForm::editEntity() {
    qDebug() << __PRETTY_FUNCTION__;
}

void dbEntitiesForm::delEntity() {
    qDebug() << __PRETTY_FUNCTION__;
}
