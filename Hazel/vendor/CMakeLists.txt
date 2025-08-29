set(third_party_folder)

set(BUILD_SHARED_LIBS  OFF CACHE BOOL "" FORCE)

if (NOT TARGET spdlog)
    option(SPOLOG_BUILD_EXAMPLE "" OFF) #Sample code
    option(SPOLOG_INSTALL "" OFF) #unintall
    option(SPOLOG_USE_STD_FORMAT "" OFF)#std::format
    add_subdirectory(spdlog)

#    Put the glm target in the project view of the IDE
#(e.g. Visual Studio / Xcode) into the "virtual folder" of ${third_party_folder}/glm.
    set_target_properties(spdlog PROPERTIES FOLDER ${third_party_folder}/spdlog)
endif ()

if (NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)#
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)#Document generation is prohibited
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw)
endif ()

if (NOT TARGET glm)
#    option(GLM_BUILD_EXAMPLES "" OFF)#
#    option(GLM_BUILD_TESTS "" OFF)
#    option(GLM_BUILD_DOCS "" OFF)#Document generation is prohibited
#    option(GLM_INSTALL "" OFF)
add_library(glm INTERFACE)
#Add a header file search path to the GLM library target
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)
add_library(glm::glm ALIAS glm)
set_target_properties(glm PROPERTIES FOLDER ${third_party_folder}/glm)
endif ()

if(NOT TARGET glad)
    set(glad_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/glad)
    file(GLOB glad_sources CONFIGURE_DEPENDS "${glad_SOURCE_DIR_}/src/*c")
    add_library(glad STATIC ${glad_sources})
    target_include_directories(glad PUBLIC $<BUILD_INTERFACE:${glad_SOURCE_DIR_}>/include)

endif ()

if(NOT TARGET imgui)
    set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imgui)



    file(GLOB imgui_sources CONFIGURE_DEPENDS "${imgui_SOURCE_DIR_}/*.cpp")

    set(IMGUI_IMPL_SRC
            "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.cpp"
            "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.h"
            "${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.cpp"
            "${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.h"
    )


    add_library(imgui STATIC ${imgui_sources}  ${IMGUI_IMPL_SRC})

#
#    target_include_directories(imgui PUBLIC
#            $<BUILD_INTERFACE:${imgui_SOURCE_DIR_}>
#            $<BUILD_INTERFACE:${imgui_SOURCE_DIR_}/backends>
#    )

    target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR_}>)
    target_link_libraries(imgui PUBLIC glfw)

endif()

if(NOT TARGET stb_image)
    add_subdirectory(stb_image)
endif ()

if(NOT TARGET entt)
    add_subdirectory(entt)
endif ()

if(NOT TARGET yaml-cpp)
    # Prefer upstream CMake: static lib, no contrib/tools/tests/install
    set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
    set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
    set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)
    set(YAML_CPP_FORMAT_SOURCE OFF CACHE BOOL "" FORCE)
    add_subdirectory(yaml-cpp)
    set_target_properties(yaml-cpp PROPERTIES FOLDER ${third_party_folder}/yaml-cpp)
endif()

# ImGuizmo
if(NOT TARGET ImGuizmo)
    set(IMGUIZMO_TARGET ImGuizmo)
    if(NOT TARGET imgui)
        message(FATAL_ERROR "ImGuizmo requires imgui target to be defined before adding this subdirectory.")
    endif()
    add_library(${IMGUIZMO_TARGET} STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.cpp
    )
    target_include_directories(${IMGUIZMO_TARGET} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
    )
    # ImGuizmo uses ImGui math operators, define before including imgui.h
    target_compile_definitions(${IMGUIZMO_TARGET} PRIVATE
            IMGUI_DEFINE_MATH_OPERATORS
            "CaptureMouseFromApp()=SetNextFrameWantCaptureMouse(true)"
    )
    target_link_libraries(${IMGUIZMO_TARGET} PUBLIC imgui)
    # Optional: group in IDE
    set_target_properties(${IMGUIZMO_TARGET} PROPERTIES
            FOLDER "${third_party_folder}/ImGuizmo"
    )
endif()
