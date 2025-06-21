set(third_party_folder)

set(BUILD_SHARED_LIBS  OFF CACHE BOOL "" FORCE)

if (NOT TARGET spdlog)
    option(SPOLOG_BUILD_EXAMPLE "" OFF) #Sample code
    option(SPOLOG_INSTALL "" OFF) #unintall
    option(SPOLOG_USE_STD_FORMAT "" OFF)#std::format
    add_subdirectory(spdlog)

#    Put the glm target in the project view of the IDE
#(e.g. Visual Studio / Xcode) into the "virtual folder" of ${third_party_folder}/glm.
#    set_target_properties(spdlog PROPERTIES FOLDER ${third_party_folder}/spdlog)
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

