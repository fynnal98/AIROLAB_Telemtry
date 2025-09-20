add_library(libcore SHARED )

target_link_libraries(${Core}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
)