cmake_minimum_required(VERSION 3.19)
project(AlgoVision LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

qt_add_executable(AlgoVision
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui


)

qt_add_translations(
    TARGETS AlgoVision
    TS_FILES AlgoVision_en_US.ts
)

target_link_libraries(AlgoVision
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS AlgoVision
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET AlgoVision
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
