#######################################################################
# Setting Target_Name as current folder name
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)


set  (SOURCE_FILES	affine-rectif-parallel-lines.cpp )

set  (HEADER_FILES	)

source_group ("Common Source" FILES ${COMMON_SOURCE_FILES})

add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${COMMON_SOURCE_FILES}
)

target_link_libraries (	
	${TARGET_NAME} 
	Qt5::Widgets
)



