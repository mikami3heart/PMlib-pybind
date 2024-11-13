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
message("src_pybind : Python interface module PMlib-pybind:  ")
message(" ")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)

link_directories(${PROJECT_BINARY_DIR}/src)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11)

if(NOT with_MPI)
pybind11_add_module(pyPerfMonitor SHARED pyPerfMonitor.cpp)
else()
#	elseif(with_MPI STREQUAL "yes")
pybind11_add_module(pyMpiPerfMonitor SHARED pyPerfMonitor.cpp)
endif()


