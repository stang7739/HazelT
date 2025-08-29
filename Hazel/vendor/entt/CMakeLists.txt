cmake_minimum_required(VERSION 3.15)

project(entt-header-only LANGUAGES CXX)

add_library(entt INTERFACE)
target_include_directories(entt INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_library(entt::entt ALIAS entt)

# Optional: organize in IDE virtual folder when included from parent
if(DEFINED third_party_folder)
    set_target_properties(entt PROPERTIES FOLDER "${third_party_folder}/entt")
endif()
