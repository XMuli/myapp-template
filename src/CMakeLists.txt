# SPDX-FileCopyrightText: 2023-2024 XMuli
# SPDX-GitHub: https://github.com/XMuli
# SPDX-Author: XMuli <xmulitech@gmail.com>

cmake_minimum_required(VERSION 3.16)

set(project_name myapp-qt-template)
set(project_version_major 1)
set(project_version_minor 0)
set(project_version_path 0)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DXBUILD_TIME="2024.11.16")

set(qt_moduls Widgets Network)    #  Multimedia WebSockets Xml Svg Network Charts 是区分大小写
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls} LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls} LinguistTools) # LinguistTools No link

set(src_files
    main.cpp

    ui/tray/tray.h
    ui/tray/tray.cpp

    ui/main/mainwin.h
    ui/main/mainwin.cpp
    ui/main/mainwin.ui

    ui/setting/settingui.h
    ui/setting/settingui.cpp
    ui/setting/settingui.ui
    ui/setting/sub_general.ui
    ui/setting/sub_update.ui
    ui/setting/sub_about.ui

    resources/resources.qrc
    resources/logo/resources.rc        # Exe 在大图模式和窗口标题栏都指定 logo 图片
)

set(xwidgets_files
    xwidgets/include/licenseui.h
    xwidgets/sources/licenseui.cpp
    xwidgets/sources/licenseui.ui
)

set(basics_files
    basics/isingleton.h

    basics/communication.h
    basics/communication.cpp
    basics/configjson.h
    basics/configjson.cpp
    basics/xapphelper.h
    basics/xapphelper.cpp

    network/version_updater/versionupdater.h
    network/version_updater/versionupdater.cpp
)


#********************************************** 判断且定义 x64/x86 变量 (get kit architecture bit) ****************************
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(arch_bit "86")
    set(is_x64bit false)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(arch_bit "64")
    set(is_x64bit true)
endif()

# auto get qt install path by Qt::Core 的 LOCATION 属性，该属性指向核心模块库文件的路径。
get_target_property(QtCore_location Qt${QT_VERSION_MAJOR}::Core LOCATION)
get_filename_component(qt_binray_dir ${QtCore_location} DIRECTORY)
message("Qt installation directory: ${qt_binray_dir}")  # C:/Qt/6.8.0/msvc2022_64/bin

if(UNIX)  # using apt install qt5-devtools*
    execute_process(COMMAND lsb_release -i OUTPUT_VARIABLE LSB_ID)
    if (LSB_ID MATCHES "Deepin" OR LSB_ID MATCHES "UOS")
        set(lupdate_path "/usr/lib/qt5/bin/lupdate")
        set(lrelease_path "/usr/lib/qt5/bin/lrelease")
        message("This is a Deepin or UOS system.")
    else()
        set(lupdate_path "lupdate")
        set(lrelease_path "lrelease")
        message("This is other linux system.")
    endif()

else()
    set(lupdate_path "${qt_binray_dir}/lupdate")
    set(lrelease_path "${qt_binray_dir}/lrelease")

    if(WIN32)
        set(windeployqt "${qt_binray_dir}/windeployqt.exe")
    elseif(APPLE)
        set(windeployqt "${qt_binray_dir}/macdeployqt")
    elseif(UNIX)
        # custom deployqt
    endif()
endif()

message("qt_binray_dir: ${qt_binray_dir}")
message("windeployqt: ${windeployqt}")
#************************************************* copy xconfig.json and resources files *******************************************************
# copy "resources" 文件夹下的指定文件夹
set(folders_to_copy "licenses" "logo" "cpack")
set(source_root "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(traget_root "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

# 遍历复制文件夹集合中的每个文件夹
foreach(it ${folders_to_copy})
    set(copy_source_dir "${source_root}/${it}")
    file(COPY ${copy_source_dir} DESTINATION ${traget_root})
endforeach()

if(APPLE)
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/config/xconfig.json" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${project_name}.app/Contents/MacOS")
else()
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/config/xconfig.json" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")  # windows and linux
    # file(COPY "${source_root}/debian" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

if(UNIX)
   file(GLOB sh_files "${source_root}/linux/debian/*.sh")
   file(COPY ${sh_files} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
#************************************************* update .ts, and generate .qm *******************************************************
set(ts_dir "${CMAKE_CURRENT_SOURCE_DIR}/translations")
if(APPLE)
    set(qm_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${project_name}.app/Contents/MacOS/translations)
else()
    set(qm_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)
endif()

file(MAKE_DIRECTORY "${ts_dir}" "${qm_dir}")
file(GLOB ts_files "${ts_dir}/*.ts")

set(ts_files_name "")    # 获取文件名部分（不包括路径）
foreach(it ${ts_files})
    get_filename_component(ts_filename ${it} NAME_WE)
    list(APPEND ts_files_name ${ts_filename})
endforeach()

# 编译时更新 .ts 和生成 .qm, # 命令参数问题：确保命令的参数传递方式正确。有时候在使用 execute_process 时，参数的传递需要一些特殊的格式，比如将参数放到列表中传递。
foreach(it ${ts_files_name})
    # execute_process(COMMAND "${lupdate_path}" -locations none -no-obsolete "${CMAKE_CURRENT_SOURCE_DIR}" -ts "${ts_dir}/${it}.ts")
    execute_process(COMMAND "${lrelease_path}" "${ts_dir}/${it}.ts" -qm "${qm_dir}/${it}.qm")
endforeach()

file(GLOB_RECURSE qm_files "${qm_dir}/*.qm")

# message("ts_files_name: ${ts_files_name}")
# message("ts_files: ${ts_files}")
# message("qm_files: ${qm_files}")
#******************************************* 调用 WindowsRect DLL 共 ①~④ 步骤 **********************************************
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows_rect/include/)                           # ①设置头文件目录
# link_directories(${trage_runtime_output_dir})                                                    # ②添加链接库路径
#***************************************************************************************************************************

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${project_name} MANUAL_FINALIZATION ${src_files} ${xwidgets_files} ${basics_files})
else()
    add_executable(${project_name} ${src_files} ${xwidgets_files} ${basics_files})
endif()

set(qt_version_moduls)
foreach(it ${qt_moduls})
    list(APPEND qt_version_moduls Qt${QT_VERSION_MAJOR}::${it})
endforeach()

target_link_libraries(${project_name} PRIVATE ${qt_version_moduls})

# 按实际目录结构显示项目结构
set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                                 # default root path curr path （CMakeList.txt）
# file(GLOB_RECURSE source_lists LIST_DIRECTORIES false "${src_root_path}/*")
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false "${src_root_path}/*.cpp" "${src_root_path}/*.h" "${src_root_path}/*.ui" "${src_root_path}/*.rc")
source_group(TREE ${src_root_path} FILES ${source_lists})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ui/include")                                  # 修复 Qt Design 使用提升自定义控件后， MinGW 找不到而编译失败; 且 MSVC 和 MinGW 使用提升时候，都不需要加上 相对路径
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/xwidgets/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/basics")
include_directories("${CMAKE_SOURCE_DIR}/tripartite/QHotkey")
include_directories("${CMAKE_SOURCE_DIR}/tripartite/include/json")
include_directories("${CMAKE_SOURCE_DIR}/tripartite/include/qt_aes")
include_directories("${CMAKE_SOURCE_DIR}/tripartite/include/easyloggingpp")


# Define some variables that can be used in *.cpp
target_compile_definitions(${project_name} PRIVATE XPROJECT_NAME="${project_name}")
target_compile_definitions(${project_name} PRIVATE XPROJECT_VERSION="${project_version}")
target_compile_definitions(${project_name} PRIVATE XARCH_BIT="${arch_bit}")
target_compile_definitions(${project_name} PRIVATE XCOMPILER="${CMAKE_HOST_SYSTEM_PROCESSOR}")  # i386/i686/x86_64/unknown  Win: AMD64
target_compile_definitions(${project_name} PRIVATE XCOMPILER_ID="${CMAKE_CXX_COMPILER_ID}")     # Clang/GCC/MSVC or GNU
target_compile_definitions(${project_name} PRIVATE XQT_INSTALL_PATH="${qt_binray_dir}")         #
target_compile_definitions(${project_name} PRIVATE UNICODE _UNICODE)

if(MSVC)
     # 设置默认启动 project
     set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${project_name})
     # Fix: Visual Studio 2022 生成的目标文件的输出目录, 移除掉 Release/Debug 的多一层文件夹
     set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${trage_runtime_output_dir}")
     set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${trage_runtime_output_dir}")
     set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${trage_runtime_output_dir}")
     set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_runtime_output_dir}")
     set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${trage_archive_output_dir}")
     set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${trage_archive_output_dir}")
     set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${trage_archive_output_dir}")
     set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_archive_output_dir}")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")             # 开启 MSVC 终端上的中文不乱码
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    target_compile_definitions(${project_name} PRIVATE
        WIN32_LEAN_AND_MEAN                                      # Header files containing only the common APIs
        _CRT_SECURE_NO_WARNINGS                                  # Unsafe functions such as strcpy, scanf, etc. are allowed)
    )
endif()

# 获取 CPU 核心数量，开启 IDE 多核编译
include(ProcessorCount)
ProcessorCount(N)
message("Number of available processor cores: ${N}")

if(WIN32)
    if(MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    elseif(MINGW)
        set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${N}")
    endif()
elseif(UNIX)
    add_compile_options(-j${N})
endif()

#******************************************* copy OpenSSL and qt base .dll *******************************************************
if(APPLE)
    file(COPY "/usr/local/Cellar/openssl@3/3.1.1/lib/libcrypto.3.dylib" DESTINATION "${EXECUTABLE_OUTPUT_PATH}")
    file(COPY "/usr/local/Cellar/openssl@3/3.1.1/lib/libssl.3.dylib" DESTINATION "${exe_output_dir}")
    # add_custom_command(TARGET ${project_name} POST_BUILD COMMAND "${windeployqt}" "${EXECUTABLE_OUTPUT_PATH}/${project_name}.app" "-verbose=1")
    add_custom_command(TARGET ${project_name} POST_BUILD COMMAND "${windeployqt}" "${EXECUTABLE_OUTPUT_PATH}/${project_name}.app" "-dmg")
elseif(UNIX)
elseif(WIN32)

    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        if(${is_x64bit})
            set(ssl_dir "C:/Qt/Tools/OpenSSLv3/Win_x64/bin")
            set(ssl_file "libssl-3-x64.dll" "libcrypto-3-x64.dll")
        else()
            message("Waring: Using Qt6, Only using x64 OpenSSLv3!")
        endif()
    else()
        if(${is_x64bit})
            set(ssl_dir "C:/Qt/Tools/OpenSSL/Win_x64/bin")
            set(ssl_file "libssl-1_1-x64.dll" "libcrypto-1_1-x64.dll")
        else()
            set(ssl_dir "C:/Qt/Tools/OpenSSL/Win_x86/bin")
            set(ssl_file "libssl-1_1.dll" "libcrypto-1_1.dll")
        endif()
    endif()

    foreach(it ${ssl_file})
        file(COPY "${ssl_dir}/${it}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
        message("OpenSSL dll it: ${ssl_dir}/${it}")
    endforeach()

    add_custom_command(TARGET ${project_name} POST_BUILD COMMAND "${windeployqt}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${project_name}.exe" --no-opengl-sw --no-translations)
endif()
#****************************************************** 稚拙 Ununtu 22.04 的 .deb *********************************************************************

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
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
