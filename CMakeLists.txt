#----------------------------------------------------------------------------
# Setup the project
#----------------------------------------------------------------------
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(TREC)

find_package(Geant4 REQUIRED)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#----------------------------------------------------------------------------
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# The version number.
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 1)

# Define arch
if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
    set(PROJECT_ARCH_64 TRUE)
else() 
    set(PROJECT_ARCH_64 FALSE) 
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#----------------------------------------------------------------------------
file(GLOB sources
	${PROJECT_SOURCE_DIR}/src/*.cc
	${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB headers
	${PROJECT_SOURCE_DIR}/include/*.hh
	${PROJECT_SOURCE_DIR}/include/*.h])

#----------------------------------------------------------------------------
# Add the library, and link it to the Geant4 libraries
#----------------------------------------------------------------------------
add_library(trec SHARED ${sources})

#----------------------------------------------------------------------------
# Find ROOT variables if the variable GEANT4_USE_ROOT is set
#----------------------------------------------------------------------------
find_package(ROOT REQUIRED)
add_definitions(-DGEANT4_USE_ROOT)
include_directories(${ROOT_INCLUDE_DIR})
target_link_libraries(trec ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Gnu Scientific Library - GSL  
#----------------------------------------------------------------------------
find_package(PkgConfig)
pkg_check_modules(GSL REQUIRED gsl)
if(GSL_FOUND)
 include_directories(${GSL_INCLUDE_DIR})
 target_link_libraries(trec ${GSL_LIBRARIES})
 target_link_libraries(trec "-lccm")
endif()

#----------------------------------------------------------------------------
# pkg-config file (trec.pc) for library
#----------------------------------------------------------------------------

if(PROJECT_ARCH_64)
	set(PROJECT_PKG_CONFIG_LIBDIR "lib64")
else()
	set(PROJECT_PKG_CONFIG_LIBDIR "lib")
endif()

configure_file (
     ${PROJECT_SOURCE_DIR}/trec.pc.in
     ${PROJECT_SOURCE_DIR}/trec.pc
     @ONLY
  )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build CarbonIonRadiography. This is so that we can run the executable
# directly because it relies on these scripts being in the current
# working directory.
#----------------------------------------------------------------------------
set(TREC_SCRIPTS
  trec.pc
  )

foreach(_script ${TREC_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install include files, library file and pkg-config file
#----------------------------------------------------------------------------
install(FILES ${headers} DESTINATION include/libtrec)

if(PROJECT_ARCH_64)
	install(FILES trec.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib64/pkgconfig)
	install(TARGETS trec DESTINATION lib64)
else()
	install(FILES trec.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
	install(TARGETS trec DESTINATION lib)
endif()
