# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2024-20xx XMuli
# SPDX-GitHub: https://github.com/XMuli
# SPDX-Author: XMuli <xmulitech@gmail.com>

cmake_minimum_required(VERSION 3.5)

set(project_name MyProject)
set(project_version_major 1)
set(project_version_minor 0)
set(project_version_path 0)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

add_definitions(-DXBUILD_TIME="2024.08.09")

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

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls} LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls} LinguistTools) # LinguistTools No link

set(src_files
    main.cpp

    ui/main/mainwin.h
    ui/main/mainwin.cpp
    ui/main/mainwin.ui

    ui/setting/settingui.h
    ui/setting/settingui.cpp
    ui/setting/settingui.ui

    resources/resources.qrc
    resources/logo/resources.rc        # Exe 在大图模式和窗口标题栏都指定 logo 图片
)

message("[using qt_version_moduls] ${qt_version_moduls}" )

add_executable(${project_name}
    ${src_files}


)

set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                                 # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false "${src_root_path}/*")
# file(GLOB_RECURSE source_lists LIST_DIRECTORIES false "${src_root_path}/*.cpp" "${src_root_path}/*.h")
source_group(TREE ${src_root_path} FILES ${source_lists})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ui/include")                                    # 修复 Qt Design 使用提升自定义控件后， MinGW 找不到而编译失败; 且 MSVC 和 MinGW 使用提升时候，都不需要加上 相对路径
# include_directories("${CMAKE_SOURCE_DIR}/tripartite/QHotkey")
# include_directories("${CMAKE_SOURCE_DIR}/tripartite/include/json")
# include_directories("${CMAKE_SOURCE_DIR}/tripartite/include/qt_aes")
# include_directories("${CMAKE_SOURCE_DIR}/tripartite/include/easyloggingpp")

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
