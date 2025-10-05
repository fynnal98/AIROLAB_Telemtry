cmake_minimum_required(VERSION 3.16)

project(telemetryApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt finden
find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

# -----------------------------------------------------------------------------
# Executable
# -----------------------------------------------------------------------------
qt_add_executable(apptelemetry
    src/main.cpp
    src/Datagram.cpp
    src/TelemetryDataHandler.cpp
    include/TelemetryDataHandler.h
)

# -----------------------------------------------------------------------------
# QML Modul
# -----------------------------------------------------------------------------
qt_add_qml_module(apptelemetry
    URI Telemetry
    VERSION 1.0
    QML_FILES
        qml/Main.qml
)

# -----------------------------------------------------------------------------
# Ressourcen
# -----------------------------------------------------------------------------
qt_add_resources(APP_RESOURCES resources.qrc)
target_sources(apptelemetry PRIVATE ${APP_RESOURCES})

# -----------------------------------------------------------------------------
# Includes
# -----------------------------------------------------------------------------
# core/include einbinden, falls core nicht über install/export kommt
target_include_directories(apptelemetry
    PRIVATE
        ${CMAKE_SOURCE_DIR}/libCore/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -----------------------------------------------------------------------------
# Link-Libraries
# -----------------------------------------------------------------------------
target_link_libraries(apptelemetry
    PRIVATE
        Qt6::Quick
        core           # deine Boost-/Asio-basierte Core-Bibliothek
)

# -----------------------------------------------------------------------------
# Config-Datei kopieren
# -----------------------------------------------------------------------------
add_custom_command(TARGET apptelemetry POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/config/config.json
        $<TARGET_FILE_DIR:apptelemetry>/config.json
)

# -----------------------------------------------------------------------------
# Windows: DLLs aus Qt-Verzeichnis mitkopieren (optional, aber praktisch)
# -----------------------------------------------------------------------------
if(WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/../cmake/QtDeployHelpers.cmake OPTIONAL)
endif()

# -----------------------------------------------------------------------------
# Properties
# -----------------------------------------------------------------------------
set_target_properties(apptelemetry PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# -----------------------------------------------------------------------------
# Installation
# -----------------------------------------------------------------------------
include(GNUInstallDirs)
install(TARGETS apptelemetry
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_INSTALL_BINDIR})
