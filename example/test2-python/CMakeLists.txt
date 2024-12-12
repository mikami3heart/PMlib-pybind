##################################################################################
#
# PMlib - Performance Monitor Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2020 RIKEN Center for Computational Science(R-CCS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2020 Research Institute for Information Technology(RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################

message(" ")
message("Python interface Examples test2-python:  ")
message(STATUS "PAPI_DIR            = " ${PAPI_DIR})
message(STATUS "POWER_DIR            = " ${POWER_DIR})
message(STATUS "OTF_DIR             = " ${OTF_DIR})
message(STATUS "with_MPI            = " ${with_MPI})

#message(STATUS "PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR})
#message(STATUS "PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})
#message(STATUS "IMPLICIT_LINK_LIBS = " ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES})
#message(STATUS "CFLAGS              = " ${CMAKE_C_FLAGS})
#message(STATUS "CXXFLAGS            = " ${CMAKE_CXX_FLAGS})
#message(STATUS "FortranFLAGS        = " ${CMAKE_Fortran_FLAGS})

include_directories(${PROJECT_BINARY_DIR}/include ${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_BINARY_DIR}/src)

if(OPT_PAPI)
  include_directories(${PAPI_DIR}/include)
  link_directories(${PROJECT_BINARY_DIR}/src_papi_ext)
  link_directories(${PAPI_DIR}/lib)
  #	link_directories(${PAPI_DIR}/lib64)
endif()

if(OPT_POWER)
  # So far, Power API option is available on supercomputer Fugaku only
  include_directories(${POWER_DIR}/include)
  link_directories(${PROJECT_BINARY_DIR}/src_power_ext)
  link_directories(${POWER_DIR}/lib64)
endif()

if(OPT_OTF)
  include_directories(${OTF_DIR}/include/open-trace-format)
  link_directories(${PROJECT_BINARY_DIR}/src_otf_ext)
  link_directories(${OTF_DIR}/lib)
endif()


### Example programs

#### Test2 : Python BINDING

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 )
pybind11_add_module(test2-pyPMlib SHARED pyPerfMonitor.cpp)


#	add_executable(test2-pyPMlib  ./pyPerfMonitor.cpp)
#	if(with_MPI)
#	else()
#	  add_test(TEST_2 test2-pyPMlib )
#	endif()

