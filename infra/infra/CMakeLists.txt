cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_INFRA_COMMON_TARGET_NAME "machiavelli-infra-common")

project("Machiavelli Infra Common library")

add_library(${MACHIAVELLI_INFRA_COMMON_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/abstractfactory.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/concurrentautoincrement.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/clicommand.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/clicommandparser.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/concurrentqueue.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/functionalerror.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/functionalerrorcategory.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/noncopyable.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/observable.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/sockets.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/subject.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/tcpclient.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/tcpserver.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/thing.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/threadpool.h"
	"${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/clicommandparser.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/functionalerrorcategory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/sockets.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/subject.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/tcpclient.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/tcpserver.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/threadpool.cpp")

set_target_properties(${MACHIAVELLI_INFRA_COMMON_TARGET_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED 14)

target_include_directories(${MACHIAVELLI_INFRA_COMMON_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

if (WIN32)
    target_link_libraries(${MACHIAVELLI_INFRA_COMMON_TARGET_NAME}
        "wsock32"
        "ws2_32")
endif()