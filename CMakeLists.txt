cmake_minimum_required(VERSION 3.14)

project(login VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED Widgets)

set(PROJECT_SOURCES
     main.cpp
     LoginRegisterDlg.h
     LoginRegisterDlg.cpp
     EventFilterObject.h
     EventFilterObject.cpp
     "Resource.qrc"
)

#添加宏定义
#add_compile_definitions(QT_NO_DEBUG_OUTPUT)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE  Qt6::Widgets)

