cmake_minimum_required(VERSION 2.8)
project(House_in_the_Forest)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SOURCES 	src/main.cpp
		src/buildSnowman.cpp
		src/model.cpp
		src/objectLoader.cpp
		src/texture.cpp
		src/object.cpp
		src/heightmap.cpp
		src/camera.cpp
		src/sky.cpp
		src/forest.cpp
		src/environment.cpp

		include/buildSnowman.hpp
		include/model.hpp
		include/objectLoader.hpp
		include/texture.hpp
		include/object.hpp
		include/heightmap.hpp
		include/camera.hpp
		include/sky.hpp
		include/forest.hpp
		include/environment.hpp)

add_executable(House_in_the_Forest ${SOURCES})

FIND_PATH( SOIL_INCLUDE_DIRS SOIL.h PATH_SUFFIXES SOIL )
FIND_LIBRARY( SOIL_LIBRARIES SOIL)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} /include)

target_link_libraries(House_in_the_Forest ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SOIL_LIBRARIES})