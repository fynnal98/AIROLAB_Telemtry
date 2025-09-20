# core/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Umschalter: heute Header-only, später SHARED
option(CORE_HEADER_ONLY "Build core as header-only interface" ON)

set(TARGET core)

# Public Headers liegen unter core/include/...
set(CORE_PUBLIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Wenn du später .cpp ergänzt, trag sie hier ein
set(CORE_SOURCES
    # src/foo.cpp
    # src/bar.cpp
)

if(CORE_HEADER_ONLY)
    add_library(${TARGET} INTERFACE)

    target_include_directories(${TARGET}
        INTERFACE
            $<BUILD_INTERFACE:${CORE_PUBLIC_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>
    )

else()
    add_library(${TARGET} SHARED
        ${CORE_SOURCES}
    )

    target_include_directories(${TARGET}
        PUBLIC
            $<BUILD_INTERFACE:${CORE_PUBLIC_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>
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
endif()

# Headers immer installieren
include(GNUInstallDirs)
install(DIRECTORY ${CORE_PUBLIC_INCLUDE_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

