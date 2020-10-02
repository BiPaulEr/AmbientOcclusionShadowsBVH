cmake_minimum_required(VERSION 3.0)

project(BaseGL)

add_subdirectory(External)

add_executable (
	BaseGL
	Main.cpp
	Camera.cpp
	Camera.h
	Exception.h
	GLError.h
	GLError.cpp
	GLError.h
	GLProgram.cpp
	GLProgram.h
	GLShader.cpp
	GLShader.h
	LightSource.cpp
	LightSource.h
	Mat4.h
	Mesh.cpp
	Mesh.h
	Ray.cpp
	Ray.h
	Triangle.h
	Vec3.h
	Vec4.h
	BVH.h
	AABB.h
	BVH.cpp
	External/imgui/imconfig.h
	External/imgui/imgui.h
	External/imgui/imgui.cpp
	External/imgui/imgui_demo.cpp
	External/imgui/imgui_draw.cpp
	External/imgui/imgui_impl_freeglut.h
	External/imgui/imgui_impl_freeglut.cpp
	External/imgui/imgui_impl_opengl3.h
	External/imgui/imgui_impl_opengl3.cpp
	External/imgui/imgui_internal.h
	External/imgui/imstb_rectpack.h
	External/imgui/imstb_textedit.h
	External/imgui/imstb_truetype.h
	External/imgui/imgui_widgets.cpp
)

# Copy the shader files in the binary location. 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(TARGET BaseGL 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:BaseGL> ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(BaseGL LINK_PRIVATE glad)

target_link_libraries(BaseGL LINK_PRIVATE glfw)

target_link_libraries(BaseGL LINK_PRIVATE glm)

# Glew
target_include_directories(BaseGL PRIVATE ${PROJECT_SOURCE_DIR}/External/glew/include)
target_link_libraries(BaseGL LINK_PRIVATE ${PROJECT_SOURCE_DIR}/External/glew/lib/Release/x64/glew32s.lib) 
target_link_libraries(BaseGL LINK_PRIVATE ${PROJECT_SOURCE_DIR}/External/glew/lib/Release/x64/glew32.lib) 

#Glut
target_include_directories(BaseGL PRIVATE ${PROJECT_SOURCE_DIR}/External/glut/include)
target_link_libraries(BaseGL LINK_PRIVATE ${PROJECT_SOURCE_DIR}/External/glut/lib/x64/freeglut.lib) 

#imgui
target_include_directories(BaseGL PRIVATE ${PROJECT_SOURCE_DIR}/External/imgui)