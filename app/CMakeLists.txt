cmake_minimum_required(VERSION 3.16)

project(telemetryApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

# Executable
qt_add_executable(apptelemetry
    src/main.cpp
)

# QML Modul
qt_add_qml_module(apptelemetry
    URI Telemetry
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

add_custom_command(TARGET apptelemetry POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config/config.json
        $<TARGET_FILE_DIR:apptelemetry>/config.json
)

# Ressourcen einbinden
qt_add_resources(APP_RESOURCES resources.qrc)
target_sources(apptelemetry PRIVATE ${APP_RESOURCES})

# Properties
set_target_properties(apptelemetry PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# Link Qt libs
target_link_libraries(apptelemetry
    PRIVATE Qt6::Quick core
)

# if(WIN32)
#     add_custom_command(TARGET apptelemetry POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy_if_different
#             $<TARGET_FILE:core>
#             $<TARGET_FILE_DIR:apptelemetry>
#     )
# endif()

# Installation
include(GNUInstallDirs)
install(TARGETS apptelemetry
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_INSTALL_BINDIR})

# if(WIN32)
#     install(FILES $<TARGET_FILE:core> DESTINATION ${CMAKE_INSTALL_BINDIR})
# endif()