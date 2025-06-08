set(TARGET_NAME "Hazel")


file(GLOB_RECURSE HAZEL_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "src/*.cpp"

)
file(GLOB_RECURSE HAZEL_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "include/*.h"
        "src/*.h"
)
add_library(${TARGET_NAME} SHARED
        ${HAZEL_SOURCES}
        ${HAZEL_HEADERS}

)

#设置路径
target_include_directories(Hazel PUBLIC
       ${CMAKE_CURRENT_SOURCE_DIR}/include
       ${CMAKE_CURRENT_SOURCE_DIR}/src
)
# Set export symbols for the library  编译时 加上宏
target_compile_definitions(${TARGET_NAME} PRIVATE HZ_BUILD_DLL)

# set platform symbol
if (WIN32)
    target_compile_definitions(${TARGET_NAME} PUBLIC HZ_PLATFORM_WINDOWS)
else ()
    message(FATAL_ERROR "Unknown Platform: HZ only supports Windows!")
endif ()

# Add HZ_DEBUG in Debug mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${TARGET_NAME} PUBLIC HZ_DEBUG)
endif ()

# Set include directories
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set output directories for the library
set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  # DLL files
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"  # Shared library files
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"  # Static library files
)
