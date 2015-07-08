

#######################################################################
########################## SETUP PDR_COMMON ###########################
set (PDR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set (PDR_SOURCE_FILES 
						${PDR_SOURCE_DIR}/PDR.h 
						${PDR_SOURCE_DIR}/PDR.cpp)
include_directories(${PDR_SOURCE_DIR})
#######################################################################


#######################################################################
########################## SETUP PDR_COMMON ###########################
set (METRIC_RETIFIC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set (METRIC_RETIFIC_SOURCE_FILES 
						${METRIC_RETIFIC_SOURCE_DIR}/MetricRetification.h 
						${METRIC_RETIFIC_SOURCE_DIR}/MetricRetification.cpp)
include_directories(${METRIC_RETIFIC_SOURCE_DIR})
#######################################################################


#######################################################################
########################## SETUP PDR_COMMON ###########################
set (PLVR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set (PLVR_SOURCE_FILES 
						${PLVR_SOURCE_DIR}/Plvr.h 
						${PLVR_SOURCE_DIR}/Plvr.cpp)
include_directories(${PLVR_SOURCE_DIR})
#######################################################################



add_subdirectory (pdr-console)
add_subdirectory (pdr-gui)
add_subdirectory (parallel-lines-vanishing-points-console)
add_subdirectory (affine-rectific-parallel-lines-gui)





