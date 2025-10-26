cmake_minimum_required(VERSION 3.30)

project(students_db VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(Libpqxx libpqxx)
message("Libpqxx include dir ${Libpqxx_INCLUDE_DIRS}")
message("Libpqxx library dir ${Libpqxx_LIBRARIES}")
message("Libpqxx library dir ${Libpqxx_LIBRARY_DIRS}")

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Gui_INCLUDE_DIRS}
                    ${Libpqxx_INCLUDE_DIRS}
                    )

set(HEADERS
    dbstudents.hpp
    university_db.hpp
    dbresult.hpp
    dbpgresult.hpp
    university_singleton.hpp
)

set(SOURCES
    dbstudents.cpp
    university_db.cpp
    dbresult.cpp
    dbpgresult.cpp
    university_singleton.cpp
)

set(RESOURCES
)

qt5_wrap_ui(UIS_HDRS ${FORMS} )

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${UIS_HDRS} ${RESOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${Libpqxx_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION lib
)
