
#######################################################################
########################## SETUP PDR_COMMON ###########################
set (COMMON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)
set (COMMON_SOURCE_FILES 
						${COMMON_SOURCE_DIR}/PDR.h 
						${COMMON_SOURCE_DIR}/PDR.cpp
						${COMMON_SOURCE_DIR}/Arpl.h 
						${COMMON_SOURCE_DIR}/Arpl.cpp
						${COMMON_SOURCE_DIR}/MetricRetification.h 
						${COMMON_SOURCE_DIR}/MetricRetification.cpp
						${COMMON_SOURCE_DIR}/Interpolation.h
						)
include_directories(${COMMON_SOURCE_DIR})
#######################################################################



add_subdirectory (pdr-console)
add_subdirectory (pdr-gui)
#add_subdirectory (parallel-lines-vanishing-points-console)
add_subdirectory (affine-rectific-parallel-lines-console)
add_subdirectory (affine-rectific-parallel-lines-gui)
add_subdirectory (affine-rectific-cross-ratio-console)
add_subdirectory (affine-rectific-cross-ratio-gui)
add_subdirectory (metric-rectification-gui)





