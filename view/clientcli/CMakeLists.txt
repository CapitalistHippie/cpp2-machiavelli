cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_VIEW_CLIENT_CLI_TARGET_NAME "machiavelli-view-clientcli")

project("Machiavelli View Client CLI")

add_executable(${MACHIAVELLI_VIEW_CLIENT_CLI_TARGET_NAME}
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/connecttoserver.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/connecttoserver.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/statehandler.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/statehandlerbase.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/init.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/clientcli.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/clientcli.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/clientclistate.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerror.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerrorcategory.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerrorcategory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")

target_include_directories(${MACHIAVELLI_VIEW_CLIENT_CLI_TARGET_NAME}
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src")

target_link_libraries(${MACHIAVELLI_VIEW_CLIENT_CLI_TARGET_NAME}
    "machiavelli-infra-common"
    "machiavelli-app-common"
    "machiavelli-app-client")

# set_target_properties(${MACHIAVELLI_VIEW_CLIENT_CLI_TARGET_NAME} PROPERTIES
#     LINK_FLAGS "/WHOLEARCHIVE:machiavelli-app-common")