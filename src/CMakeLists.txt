# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2024-20xx XMuli
# SPDX-GitHub: https://github.com/XMuli/Sunny
# SPDX-Author: XMuli <xmulitech@gmail.com>

cmake_minimum_required(VERSION 3.5)

set(project_name MyProject)
set(project_version_major 1)
set(project_version_minor 0)
set(project_version_path 0)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

add_definitions(-DXBUILD_TIME="2024.08.08")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(qt_moduls Widgets)    # Network Multimedia WebSockets Xml Svg Network Charts 是区分大小写
set(qt_version_moduls)
foreach(it ${qt_moduls})
    list(APPEND qt_version_moduls Qt${QT_VERSION_MAJOR}::${it})
endforeach()

message("[using qt_version_moduls] ${qt_version_moduls}" )

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls} LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls} LinguistTools) # LinguistTools No link

set(source_files
    main.cpp
    widget.cpp
    widget.h
    widget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${project_name}
        MANUAL_FINALIZATION
        ${source_files}
    )
else()
    add_executable(${project_name}
        ${source_files}
    )
endif()

target_link_libraries(${project_name} PRIVATE ${qt_version_moduls})

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${project_name})
endif()

set_target_properties(${project_name} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${project_name}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${project_name})
endif()
