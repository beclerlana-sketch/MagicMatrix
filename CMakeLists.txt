cmake_minimum_required(VERSION 3.19)
project(ANASLO LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(ANASLO
    WIN32 MACOSX_BUNDLE
    main.cpp
    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui
    ui/CellButton.h
    ui/CellButton.cpp
    ui/MagicMatrixBoard.h
    ui/MagicMatrixBoard.cpp
    core/GameTypes.h
    core/MagicSquareBoard.h
    core/MagicSquareBoard.cpp
    session/GameSession.h
    session/GameSession.cpp
    core/MagicSquareRules.h
    core/MagicSquareRules.cpp
)
target_link_libraries(ANASLO
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS ANASLO
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ANASLO
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# Enable with -DBUILD_TESTING=ON when the Qt Test component is installed.
include(CTest)
if(BUILD_TESTING)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Test)
    qt_add_executable(GameSessionTests
        tests/GameSessionTest.cpp
        session/GameSession.cpp session/GameSession.h
        core/MagicSquareBoard.cpp core/MagicSquareRules.cpp
    )
    target_include_directories(GameSessionTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(GameSessionTests PRIVATE Qt6::Core Qt6::Test)
    add_test(NAME GameSessionTests COMMAND GameSessionTests)

    qt_add_executable(PlayableUiTests
        tests/PlayableUiTest.cpp
        ui/mainwindow.cpp ui/mainwindow.h
        ui/MagicMatrixBoard.cpp ui/MagicMatrixBoard.h
        session/GameSession.cpp session/GameSession.h
        core/MagicSquareBoard.cpp core/MagicSquareRules.cpp
    )
    target_link_libraries(PlayableUiTests PRIVATE Qt6::Widgets Qt6::Test)
    add_test(NAME PlayableUiTests COMMAND PlayableUiTests -platform offscreen)
endif()

