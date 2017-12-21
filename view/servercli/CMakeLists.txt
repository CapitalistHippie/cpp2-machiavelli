cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_SERVER_CLI_TARGET_NAME "machiavelli-server-cli")

project("Machiavelli Server CLI")

add_executable(${MACHIAVELLI_SERVER_CLI_TARGET_NAME}
    "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/servercli.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/mach/view/servercli.h")

target_include_directories(${MACHIAVELLI_SERVER_CLI_TARGET_NAME}
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src")

target_link_libraries(${MACHIAVELLI_SERVER_CLI_TARGET_NAME}
    "machiavelli-infra"
    "machiavelli-server")
