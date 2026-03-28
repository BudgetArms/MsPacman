cmake_minimum_required(VERSION 4.0.0)


project(MsPacman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(PROJECT_FOLDER "Game")
set(RESOURCE_FOLDER_NAME "Resources")

message("\nRoot CMakeList")


# Steamworks
set(STEAMWORKS_ROOT "${CMAKE_SOURCE_DIR}/3rdParty/steamworks")
set(ENABLE_STEAMWORKS ON)

if(ENABLE_STEAMWORKS AND NOT EMSCRIPTEN AND EXISTS STEAMWORKS_ROOT)
    set(ENABLE_STEAMWORKS ON)
else()
    set(ENABLE_STEAMWORKS OFF)
endif()



set(UPDATE_SUBMODULES OFF)

if(UPDATE_SUBMODULES)
    execute_process(
        COMMAND git submodule update --remote --rebase
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE GIT_SUBMODULE_RESULT
    )

    if(NOT GIT_SUBMODULE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to update the submodules")
    endif()
endif()



add_subdirectory(BudgetArmsEngine)
add_subdirectory(Game)
