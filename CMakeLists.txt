
#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.22)
project(tauMu)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


# CRY
include(ExternalProject)
ExternalProject_Add(
    CRY-1.7
    URL http://nuclear.llnl.gov/simulation/cry_v1.7.tar.gz
    URL_MD5 85f240bebe81fe0b257e92ef1d390a83
    CONFIGURE_COMMAND ""
    INSTALL_COMMAND ""
    BUILD_IN_SOURCE 1
    TEST_AFTER_INSTALL 1
)

ExternalProject_Get_Property(CRY-1.7 source_dir)
set(CRY_INCLUDE_DIRS "${source_dir}/src")
set(CRY_LIBRARY_DIR "${source_dir}/lib")
set(CRY_LIBRARIES "libCRY.a")
include_directories(${CRY_INCLUDE_DIRS})
link_directories(${CRY_LIBRARY_DIR})
set(LINK_LIBS ${CRY_LIBRARIES} ${LINK_LIBS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(muonlifetime muonlifetime.cc ${sources} ${headers})
target_link_libraries(muonlifetime ${Geant4_LIBRARIES} ${CRY_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
FILE(GLOB RUN_MACROS macros/*.mac)

foreach(_script ${RUN_MACROS})
  # message( ${_script} )
  get_filename_component(shortname ${_script} NAME)
  # message(${shortname})

  configure_file(
    macros/${shortname}
    ${PROJECT_BINARY_DIR}/${shortname}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(tauMu DEPENDS muonlifetime)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS muonlifetime DESTINATION bin)


