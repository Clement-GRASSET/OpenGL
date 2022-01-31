project(imgui)

cmake_minimum_required(VERSION 3.0.2)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INC_DIR "${PROJECT_SOURCE_DIR}/include/")

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include BEFORE)

set(SRC  
  ${SRC_DIR}/imgui_demo.cpp
  ${SRC_DIR}/imgui_draw.cpp
  ${SRC_DIR}/imgui_impl_glfw.cpp
  ${SRC_DIR}/imgui_impl_opengl3.cpp
  ${SRC_DIR}/imgui_stdlib.cpp
  ${SRC_DIR}/imgui_tables.cpp
  ${SRC_DIR}/imgui_widgets.cpp
  ${SRC_DIR}/imgui.cpp
  ${SRC_DIR}/imgui_user.inl
)

set(HEADER
  ${INC_DIR}/${PROJECT_NAME}/imconfig.h
  ${INC_DIR}/${PROJECT_NAME}/imgui_impl_glfw.h
  ${INC_DIR}/${PROJECT_NAME}/imgui_impl_opengl3_loader.h
  ${INC_DIR}/${PROJECT_NAME}/imgui_impl_opengl3.h  
  ${INC_DIR}/${PROJECT_NAME}/imgui_internal.h
  ${INC_DIR}/${PROJECT_NAME}/imgui_stdlib.h
  ${INC_DIR}/${PROJECT_NAME}/imgui.h
  ${INC_DIR}/${PROJECT_NAME}/imstb_rectpack.h
  ${INC_DIR}/${PROJECT_NAME}/imstb_textedit.h
  ${INC_DIR}/${PROJECT_NAME}/imstb_truetype.h
)

set(RESOURCE_FILES
)

add_library(${PROJECT_NAME} ${SRC} ${HEADER} ${RESOURCE_FILES})
target_include_directories (${PROJECT_NAME}
  PUBLIC ${INC_DIR}
)

install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

