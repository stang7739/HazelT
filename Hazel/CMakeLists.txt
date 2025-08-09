set(TARGET_NAME "Hazel")


file(GLOB_RECURSE HAZEL_SOURCES CONFIGURE_DEPENDS
        "src/*.cpp"

)
file(GLOB_RECURSE HAZEL_HEADERS CONFIGURE_DEPENDS
        "include/*.h"
        "src/*.h"

)
#Compile the files in these directories as if they were the source files
add_library(${TARGET_NAME} STATIC
        ${HAZEL_SOURCES}
        ${HAZEL_HEADERS}

)
add_custom_command(TARGET ${TARGET_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Building static library..."
)

#Set the path
target_include_directories(Hazel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src

)
target_precompile_headers(${TARGET_NAME} PRIVATE src/hzpch.h)

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_subdirectory(vendor)
target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog) # spdLog requires a namespace, see the add_library in the cmake of spdLog
target_link_libraries(${TARGET_NAME} PRIVATE glfw)
target_link_libraries(${TARGET_NAME} PUBLIC glm::glm)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PRIVATE glad)
target_link_libraries(${TARGET_NAME} PRIVATE stb_image)
target_link_libraries(${TARGET_NAME} PUBLIC entt::entt)


# Set symbols for static library - no need for DLL export/import
target_compile_definitions(${TARGET_NAME} PUBLIC HZ_STATIC_LIB)
#target_compile_options(${TARGET_NAME} PRIVATE /utf-8)

target_compile_definitions(${TARGET_NAME} PUBLIC OPENGL)

# set platform symbol
if (WIN32)
    target_compile_definitions(${TARGET_NAME} PUBLIC HZ_PLATFORM_WINDOWS)
else ()
    message(FATAL_ERROR "Unknown Platform: HZ only supports Windows!")
endif ()

# Add HZ_DEBUG in Debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
    target_compile_definitions(${TARGET_NAME} PUBLIC HZ_DEBUG)
    target_compile_definitions(${TARGET_NAME} PUBLIC HZ_ENABLE_ASSERTS)
endif ()

# Set output directories for the library
set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  # DLL files
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  # Shared library files
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"  # Static library files
)
