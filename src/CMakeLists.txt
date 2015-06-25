

#######################################################################
########################## SETUP PDR_COMMON ###########################
set (PDR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set (PDR_SOURCE_FILES 
						${PDR_SOURCE_DIR}/PDR.h 
						${PDR_SOURCE_DIR}/PDR.cpp)
include_directories(${PDR_SOURCE_DIR})
#######################################################################

add_subdirectory (pdr-console)
add_subdirectory (pdr-gui)





