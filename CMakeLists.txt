cmake_minimum_required(VERSION 3.19)
project(MagicMatrix LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(MagicMatrix
    WIN32 MACOSX_BUNDLE
    main.cpp
    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui
    ui/CellButton.h
    ui/CellButton.cpp
    ui/MagicMatrixBoard.h
    ui/MagicMatrixBoard.cpp
)
target_link_libraries(MagicMatrix
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS MagicMatrix
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MagicMatrix
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
