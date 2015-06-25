#######################################################################
# Setting Target_Name as current folder name
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)



set  (SOURCE_FILES		
						MainWindow.cpp 
						GLRectangle.cpp
						GLRenderTexture.cpp
						GLImageWidget.cpp
						AppMain.cpp )

set  (HEADER_FILES		
						MainWindow.h
						GLRectangle.h
						GLRenderTexture.h
						GLImageWidget.h)

set  (FORM_FILES		 
						MainWindow.ui)


qt5_wrap_ui( HEADER_UI_FILES ${FORM_FILES} )

source_group ("Form Files" FILES ${FORM_FILES})
source_group ("Form Header Files" FILES ${HEADER_UI_FILES})
source_group ("PDR Source" FILES ${PDR_SOURCE_FILES})

include_directories (${CMAKE_SOURCE_DIR})


add_executable(
  ${TARGET_NAME}
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${HEADER_UI_FILES}
  ${PDR_SOURCE_FILES}
)

target_link_libraries (	
	${TARGET_NAME} 
	Qt5::OpenGL
	Qt5::Widgets
)



