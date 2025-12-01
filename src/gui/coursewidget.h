#pragma once

#include <QWidget>
#include <QAbstractItemModel>
#include <memory>

class Course;
class Department;

namespace Ui {
    class course_widget;
};

class CourseWidget : public QWidget {
public:
    CourseWidget( std::shared_ptr< Course > pCourse = nullptr, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags() );
    virtual ~CourseWidget();

    void setCourse( std::shared_ptr< Course > pCourse );
    void setStudentsModel( QAbstractItemModel* studCourseModel );

private slots:
    void saveCourse();
    void selectDepartment();
    void refreshStudents();
    void addStudentToCourse();
    void editStudentOnCourse();
    void withdrawStudentFromCourse();

public slots:
    void setCourseDepartment( std::shared_ptr< Department > pDep );

signals:
    void saveCourseToDb( std::shared_ptr< Course > pCourse );
    void refreshStudentsOnCourse( QAbstractItemModel* mod );
    void enrollStudentToCourse( std::shared_ptr< Course > pCourse );
    void editEnrollmentStudentOnCourse( std::shared_ptr< Course > pCourse, int idStudent );
    void removeEnrollmentStudentFromCourse( std::shared_ptr< Course > pCourse, int idStudent );

    void loadDepartments(std::shared_ptr< Course > pCourse);
    void loadDepartment( int idDep );

private:
    void initForm();

private:
    Ui::course_widget* _UI;
    std::shared_ptr< Course > m_pCourse;

private:
    Q_OBJECT
};
