#pragma once

#include <QMainWindow>
#include <QSharedPointer>
#include <memory>

#include "university_singleton.hpp"

class QMdiArea;

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

private:
    Ui::students_db_main_window* _UI;
    QSharedPointer<QMdiArea> m_mdiArea;
    UniversitySingleton& m_UnivSt;

private:
    Q_OBJECT
};
