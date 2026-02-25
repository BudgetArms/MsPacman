cmake_minimum_required(VERSION 3.13)
project(MsPacman)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_FOLDER Game)

message("\nRoot CMakeList")


set(STEAMWORKS_SDK_PATH "${CMAKE_SOURCE_DIR}/3rdParty/steamworks")
set(ENABLE_STEAMWORKS OFF) # temp because I cannot be assed to add/remove compile option

set(EMSCRIPTEN OFF)
set(ENABLE_CONSOLE ON)


if(ENABLE_STEAMWORKS AND NOT EMSCRIPTEN)

    message("Using Steamworks")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")

    find_package(Steamworks QUIET)

    if(Steamworks_FOUND)

        target_link_libraries("${TARGET_NAME}" PUBLIC Steamworks::Steamworks)
        target_compile_definitions(${TARGET_NAME} PRIVATE USE_STEAMWORKS)

        add_custom_command(
            TARGET
            ${TARGET_NAME}
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
        )

        add_custom_command(
            TARGET
            ${TARGET_NAME}
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}"
            -E echo "480" > "$<TARGET_FILE_DIR:${TARGET_NAME}>/steam_appid.txt"
            COMMENT "Generating steam_appid.txt"
        )

    else()

        message(WARNING "Steamworks package NOT found")
        set(STEAMWORKS_AVAILABLE FALSE)

    endif()

endif()


# Force updates submodule, so that it's exactly identical to latest version of submodule
#[[
execute_process(
  COMMAND git submodule update --init --remote --force
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
#]]


add_subdirectory(BudgetArmsEngine)
add_subdirectory(Game)
