
set(TARGET_NAME "Hazelnet")

add_executable(${TARGET_NAME} src/HazelnutApp.cpp)
## Set source files (all .cpp files in the src directory)
file(GLOB_RECURSE SANDBOX_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
file(GLOB_RECURSE SANDBOX_HANDLES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

target_sources(${TARGET_NAME} PRIVATE
        ${SANDBOX_SOURCES}
        ${SANDBOX_HANDLES}

)

target_link_libraries(${TARGET_NAME} PRIVATE Hazel)
target_compile_options(${TARGET_NAME} PRIVATE /utf-8)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_SCAN_FOR_MODULES 1)


# Set the output directory for the executable
set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
