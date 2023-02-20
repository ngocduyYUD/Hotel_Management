cmake_minimum_required(VERSION 2.6)

add_executable(hotel hotel.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

target_link_libraries(hotel ${GTK3_LIBRARIES})


set (CMAKE_INCLUDE_CURRENT_DIR ON)

find_package (PostgreSQL REQUIRED)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories (${CMAKE_SOURCE_DIR}/src ${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})
target_link_libraries(hotel ${PostgreSQL_LIBRARIES})
