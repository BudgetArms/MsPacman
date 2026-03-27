cmake_minimum_required(VERSION 4.0.0)
project(MsPacman)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_FOLDER "Game")
set(RESOURCE_FOLDER_NAME "Resources")

message("\nRoot CMakeList")


set(STEAMWORKS_ROOT "${CMAKE_SOURCE_DIR}/3rdParty/steamworks")
set(ENABLE_STEAMWORKS ON)

if(ENABLE_STEAMWORKS AND NOT EMSCRIPTEN AND EXISTS STEAMWORKS_ROOT)
    set(ENABLE_STEAMWORKS ON)
else()
    set(ENABLE_STEAMWORKS OFF)
endif()


# Force update submodule, so that it's exactly identical to latest version of the submodule
#[[
execute_process(
  COMMAND git submodule update --remote --merge
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
#]]


add_subdirectory(BudgetArmsEngine)
add_subdirectory(Game)
