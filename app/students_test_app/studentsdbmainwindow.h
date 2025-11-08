#pragma once

#include <QMainWindow>
#include <QSharedPointer>
#include <memory>

#include <university_singleton.hpp>

class QMdiArea;
class QToolBar;

namespace Ui{
    class students_db_main_window;
};

class StudentsDbMainWindow : public QMainWindow {
public:
    StudentsDbMainWindow( QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~StudentsDbMainWindow();

private slots:
    void slotDbConnect();
    void slotDbDisconnect();
    void slotAddWidget( QWidget* w );
    void slotDepartments();
    void slotStudents();
    void slotCourses();
    void slotEnrollments();
    void slotActionsEnable( bool isEnable );

private:
    Ui::students_db_main_window* _UI;
    QSharedPointer<QMdiArea> m_mdiArea;
    QSharedPointer<QToolBar> m_ToolBar;
    bool m_isDbconnect;
    UniversitySingleton& m_UnivSt;
    dbCore* m_dbC;

private:
    Q_OBJECT
};
