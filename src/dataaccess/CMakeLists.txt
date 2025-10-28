cmake_minimum_required (VERSION 3.30)
project(db_students_data_access VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

find_package(PkgConfig REQUIRED)
pkg_check_modules(Libpqxx libpqxx)

include_directories(${PROJECT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${Libpqxx_INCLUDE_DIRS}
                    )

set(CMAKE_BUILD_TYPE Debug)

set(HEADERS
    university_db.hpp
    university_pg_db.hpp
    dbresult.hpp
    dbpgresult.hpp
)

set(SOURCES
    university_db.cpp
    university_pg_db.cpp
    dbresult.cpp
    dbpgresult.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} )
message("dataaccess current source dir ${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME} -lpqxx -lpq )
target_include_directories(${PROJECT_NAME} PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
