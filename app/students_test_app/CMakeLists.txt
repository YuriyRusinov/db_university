cmake_minimum_required(VERSION 3.30)

project(students_test_app VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
                    ${CMAKE_SOURCE_DIR}
                    )

link_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
    studentsdbmainwindow.h
)

set(FORMS
    students_main_window.ui
)

set(SOURCES
    studentsdbmainwindow.cpp
    main.cpp
)

set(RESOURCES
    ../../src/students_db.qrc
)

qt5_wrap_ui(UIS_HDRS ${FORMS} )

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UIS_HDRS} ${RESOURCES} )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets students_db db_students_data_access db_gui db_entities)

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)
