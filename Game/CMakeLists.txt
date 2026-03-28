cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("\n")
message("${PROJECT_NAME} CMake")

if(EMSCRIPTEN)
  set(PROJECT_NAME ${PROJECT_NAME}_web)
  set(TARGET_PARAM )
else()
  set(PROJECT_NAME ${PROJECT_NAME})
  set(TARGET_PARAM WIN32)
endif()


add_executable(${PROJECT_NAME} ${TARGET_PARAM}
	"Main.cpp"
    "Components/RotateComponent.cpp"
    "Components/RotateComponent.hpp"
)
 

# Compile at max warning level + treat warnings as errors
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()



# Add Steamworks
if(ENABLE_STEAMWORKS)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/steam")
    find_package(Steamworks QUIET)
    if(NOT Steamworks_FOUND)
        message("\n")
        message(WARNING "Steamworks package NOT found")
        set(STEAMWORKS_AVAILABLE FALSE)

    else()
        message("\n")
        message("Steamworks package Found")
        set(STEAMWORKS_AVAILABLE TRUE)

    endif()
endif()


target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_NAME})


if(EMSCRIPTEN)
    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
    # set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER_NAME}@/\"")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH -sNO_DISABLE_EXCEPTION_CATCHING --preload-file \"${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER_NAME}@/\"")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake")

    # Additional settings for Emscripten build
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
        LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_SOURCE_DIR}/web"
        #"${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}"
        "${CMAKE_BINARY_DIR}"
    )
        #"$<TARGET_FILE_DIR:${PROJECT_NAME}>/"

    message("binary dir: ${CMAKE_BINARY_DIR}")
    message("source dir: ${CMAKE_SOURCE_DIR}")
    message("project name: ${PROJECT_NAME}")

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()


if(WIN32)
	add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER_NAME}"
        "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/${RESOURCE_FOLDER_NAME}"
	)
endif()


message("\n")

