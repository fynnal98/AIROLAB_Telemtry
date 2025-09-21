# core/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(TARGET core)

# Public Headers liegen unter core/include/...
set(CORE_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Wenn du später .cpp ergänzt, trag sie hier ein
set(CORE_SOURCES
    src/Logger.cpp
)

add_library(${TARGET} SHARED
    ${CORE_SOURCES}
)

target_include_directories(${TARGET}
    PUBLIC
        $<BUILD_INTERFACE:${CORE_PUBLIC_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_CURRENT_BINARY_DIR}
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external>
)

# Export-Makro generieren (CORE_EXPORT)
include(GenerateExportHeader)
generate_export_header(${TARGET}
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/core_export.h
    BASE_NAME CORE
)
target_include_directories(${TARGET}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(${TARGET}
    PRIVATE CORE_BUILD
    PUBLIC CORE_DLL
)

set_target_properties(${TARGET} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    SOVERSION 0
)

if(WIN32)
    set_target_properties(${TARGET} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include(GNUInstallDirs)
install(TARGETS ${TARGET}
    EXPORT coreTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
# Export für find_package über dein Build-Verzeichnis
export(EXPORT coreTargets FILE "${CMAKE_CURRENT_BINARY_DIR}/coreTargets.cmake")


# Headers immer installieren
include(GNUInstallDirs)
install(DIRECTORY ${CORE_PUBLIC_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
