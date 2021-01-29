cmake_minimum_required(VERSION 3.5)                                                
add_definitions("-D_AMD64_")	

project(cplug)											
set(CMAKE_C_COMPILER "gcc")			
set(CMAKE_CXX_COMPILER "g++")		
set(CMAKE_OPTION "$ENV{MJOY_OPTION} -Werror -rdynamic")		
 
set(THIRDPARTY_LIB_DIR ${PROJECT_SOURCE_DIR}/thirdparty/lib)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(PRO_DIR ${PROJECT_SOURCE_DIR})
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

if (WIN32)
	set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})		#exe outputdir 
	set(LIBRARY_OUTPUT_PATH ${LIB_DIR})			#lib outputdir
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/Debug)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/Release)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIB_DIR}/Debug)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIB_DIR}/Release)
endif()

add_subdirectory(thirdparty)
add_subdirectory(src)				
add_subdirectory(test)				

