
set(TARGET_NAME "sandBox")

add_executable(${TARGET_NAME} src/SandBoxApp.cpp)
## Set source files (all .cpp files in the src directory)
file(GLOB_RECURSE SANDBOX_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/SandBox2D.cpp"
)

target_sources(${TARGET_NAME} PRIVATE
        ${SANDBOX_SOURCES}

)

target_link_libraries(${TARGET_NAME} PRIVATE Hazel)
target_compile_options(${TARGET_NAME} PRIVATE /utf-8)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_SCAN_FOR_MODULES 1)


# Set the output directory for the executable to ensure it can find the DLL
set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
