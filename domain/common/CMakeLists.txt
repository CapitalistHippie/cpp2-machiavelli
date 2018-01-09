cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_DOMAIN_COMMON_TARGET_NAME "machiavelli-domain-common")

project("Machiavelli Domain Common library")

add_library(${MACHIAVELLI_DOMAIN_COMMON_TARGET_NAME}
    STATIC
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/illegalactionevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/gameupdatedevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/choicenecessaryevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/clientconnectedevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/models/clientinfo.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/characterpowerhelper.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/models/player.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/models/game.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/gamecontroller.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/gamestate.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/eventtype.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/eventvisitor.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/event.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/eventbase.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/gamestartedevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/characterchosenevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/gameendedevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/nextturnevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/nextroundevent.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/domain/events/serverdisconnectedevent.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/domain/gamecontroller.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/mach/domain/models/game.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/domain/models/player.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/src/mach/domain/characterpowerhelper.cpp"
	)

set_target_properties(${MACHIAVELLI_DOMAIN_COMMON_TARGET_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED 14)

target_include_directories(${MACHIAVELLI_DOMAIN_COMMON_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(${MACHIAVELLI_DOMAIN_COMMON_TARGET_NAME}
	"machiavelli-infra-common"
	"machiavelli-dal-common")