#pragma once

#include <QObject>
#include <QWidget>

#include <memory>

class QAbstractItemModel;
class QSettings;
class QAbstractItemView;

class UniversityDb;
class dbLoader;

using std::unique_ptr;
using std::shared_ptr;

class dbCore : public QObject {
private:
    dbCore( QObject* parent = nullptr );

public:
    virtual ~dbCore();
    bool GUIConnect( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    void dbDisconnect();
    bool GUIViewDepartments( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    bool GUIViewStudents( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    bool GUIViewCourses( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );
    bool GUIViewEnrollments( QWidget* parent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags() );

    std::shared_ptr<UniversityDb> getDb() const;

    static std::unique_ptr<dbCore> create( QObject* parent=nullptr );

private:
    void initStudentsModel( QAbstractItemModel* model );
    void initDepModel( QAbstractItemModel* model );
    void initCourseModel( QAbstractItemModel* model );
    void initEnrollModel( QAbstractItemModel* model );

private slots:
    void refreshModel(int eType, QAbstractItemModel* mod );

signals:
    void setWidget( QWidget* w );

private:
    QSettings* m_cvSettings;
    friend class UniversitySingleton;
    std::shared_ptr<UniversityDb> m_Db;
    std::shared_ptr<dbLoader> m_dbLoader;

private:
    Q_OBJECT
};
