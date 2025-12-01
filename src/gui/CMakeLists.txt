cmake_minimum_required(VERSION 3.20)

project(db_gui VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
                    ${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/src
                    )

link_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/classificator)

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
    dbLoginForm.h
    dbEntitiesForm.h
    departmentwidget.h
    studentwidget.h
    coursewidget.h
    departmentdialog.h
)

set(SOURCES
    dbLoginForm.cpp
    dbEntitiesForm.cpp
    departmentwidget.cpp
    studentwidget.cpp
    coursewidget.cpp
    departmentdialog.cpp
)

set(FORMS
    db_login_form.ui
    db_entities_form.ui
    department_widget.ui
    students_widget.ui
    course_widget.ui
    enrollment_dialog.ui
    department_dialog.ui
)

set(RESOURCES
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES}  )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets db_entities) 

target_include_directories(${PROJECT_NAME} PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

