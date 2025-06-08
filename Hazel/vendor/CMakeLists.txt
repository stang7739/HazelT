set(third_party_folder)

set(BUILD_SHARED_LIBS  OFF CACHE BOOL "" FORCE)

if (NOT TARGET spdlog)
    option(SPOLOG_BUILD_EXAMPLE "" OFF)
    option(SPOLOG_INSTALL "" OFF)
    option(SPOLOG_USE_STD_FORMAT "" OFF)
    add_subdirectory(spdlog)
    set_target_properties(spdlog PROPERTIES FOLDER ${third_party_folder}/spdlog)
endif ()