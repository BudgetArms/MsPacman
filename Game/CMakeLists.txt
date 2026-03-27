cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("\n")
message("${PROJECT_NAME} CMake")



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


target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_NAME})


if(WIN32)
	add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER_NAME}"
        "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/${RESOURCE_FOLDER_NAME}"
	)
endif()


message("\n")

