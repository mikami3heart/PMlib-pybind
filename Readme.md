# PMlib - Performance Monitor library

* Copyright (c) 2012-2024 RIKEN Center for Computational Science (R-CCS). All rights reserved.
* Copyright (c) 2016-2024 Research Institute for Information Technology (RIIT), Kyushu University. All rights reserved.
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN. All rights reserved.

## OUTLINE

This library records the statistics information of run-time performance and the trace information of a user code and reports its summary.
PMlib can be used for both serial and parallel environments including hybrid(OpenMP & MPI) code.
PMlib integrates PAPI HWPC(hardware performance counter) APIs and Power APIs internally, and produces HPC oriented statistics reports as controlled by the environment variables.

## SOFTWARE REQUIREMENT
- Linux OS or UNIX OS
- Compilers for C/C++/Fortran
- Cmake
- MPI library  (optional)
- PAPI library (optional)
- Power API library (optional)
- OTF library (optional)
- Python3

Although MPI and PAPI libraries are optional, they provide the essential multiprocessing capability
and the hardware performance measurement capability, so are strongly recommended to be included in PMlib
building process.
Currently, PMlib APIs for Python are maintained in separate repository at
https://github.com/mikami3heart/PMlib-pybind


## PMLIB PACKAGE INGREDIENTS
~~~
ChangeLog         History of development
CMakeLists.txt    Cmake setup file
License.txt       License to apply
Readme.md         This document, including the description of build
cmake/            Modules of cmake
doc/              Document
  Doxyfile        Configuration file to generate a doxygen file
  scripts/        Shell script collections for installation/execution
  src_advanced/   Advanced example programs calling PMlib APIs
  src_tutorial/   Example tutorial programs for beginners
  tutorial/       Tutorial documents
example/          Example source codes
include/          PMlib Header files
src/              PMlib Source codes
src_pybind/       PMlib Python interface
src_papi_ext/     PMlib Extension of PAPI interface
src_power_ext/    PMlib Extension of Power API interface
src_otf_ext/      PMlib Extension of Open Tracer Format interface
~~~

## HOW TO BUILD

Before starting the build, it is recommended to check if PMlib is already
installed on the system by the system administrator and is available through
some package manager such as Module Environment or Spack.
These package managers often provide easier method to access and use PMlib,
without needing the build process by users.
If PMlib is not found on your system, build and install PMlib as explained
in the following sections. Building PMlib is fairly straight forward.

For building PMlib, set two shell variables first.
The shell variable PACKAGE\_DIR should point to the PMlib source files.
Git clone the PMlib repository in ${PACKAGE\_DIR}.
The shell variable PMLIB\_DIR should point to the installation destination.

```
    PACKAGE_DIR=${HOME}/pmlib/PMlib-develop
    PMLIB_DIR=${HOME}/pmlib/usr_local_pmlib
```
These two variables can point to anywhere, although somewhere under
${HOME} is generally recommended.

### Build commands

The structure of the build commands is as simple as below.
Execute cmake to produce Makefiles and make.
The only part which needs careful setup is the options to cmake command,
which will be explained next.
In this example, the build process goes under the subdirectory
${PACKAGE\_DIR}/BUILD, and the built files will be installed in
${PMLIB\_DIR}/lib and ${PMLIB\_DIR}/include.

~~~
$ cd $PACKAGE_DIR
$ mkdir BUILD
$ cd BUILD
$ cmake [options] ../
$ # If building python interface with PyBind11, then edit two files
$ # as shown below before doing make
$ make
$ make install
~~~

~~~
$ # edit two files
$ sed -i 's/strip/echo/g'   src_pybind/CMakeFiles/pyPerfMonitor.dir/build.make
$ sed -i 's/-DCMAKE_INSTALL_DO_STRIP=1//g' src_pybind/Makefile
~~~


### Cmake options

`-D INSTALL_DIR=`_directory_

>  Specify the installation destination directory.
PMlib libraries will be installed under _directory_/lib and the header files under _directory_/include.
It is recommended to set a shell variable PMLIB\_DIR as described above and use it for _directory_,
i.e. `-D INSTALL_DIR=${PMLIB_DIR}`.
The default _directory_ is /usr/local/PMlib, which usually requires privilege.

`-D with_MPI=` {no | yes}

>  Setting this option "yes" specifies building PMlib with MPI library.
If you link your application with MPI, then PMlib also has to be built `with_MPI=yes`.
The default is no.

`-D enable_OPENMP=` {no | yes}

> Setting this option "yes" enables measuring OpenMP threads. The default is no.

`-D with_PAPI=` {no | yes |_directory_}

>  Setting this option "yes" or _directory_ specifies building PMlib with PAPI library
for automated hardware counter measurement.
`-Dwith_PAPI=yes` option can detect the system provided default PAPI library in usual cases.
However in certain cases, users have to specify the appropriate PAPI _directory_ explicitly.
An example of this is the cross-compiling environment.
In the cross-compiling environment, there can be multiple PAPI libraries on the system,
one for the current platform and another for the target platform.
Another example of this is when the OS provided PAPI library is obsolete for some reason,
and the appropriate PAPI is installed under some alternative directory.
The default is no.

`-D with_POWER=` {no | yes | _directory_}

>  This option is exclusively available on supercomputer Fugaku and on FX1000 systems.
Power API developed by Sandia National Laboratory is integrated into PMlib.
Specifying `-Dwith_POWER=yes` should detect the correct path on these systems.
In case not, specify this option with _directory_ pointing to the installed Power API library.
The default is no.

`-D with_OTF=` {no | _directory_}

>  This option is for linkling OTF (Open Trace Format) library with PMlib.
Specify this option with the _directory_ pointing to the installed OTF library.
The default is no.

`-D enable_PreciseTimer=` {no| yes}

> This option enables the precise timer for high resolution measurement.
Precise timers are currently supported for supercomputer Fugaku, Fujitsu FX1000, Intel Xeon servers.
Setting this option "yes" is equivalent to adding the C++ compiler option `-D CMAKE_CXX_FLAGS="-DUSE\_PRECISE\_TIMER", and adding the extended libraries`
The default is no.

`-D with_example=` {no| yes}

>  This option turns on compiling example codes. Default is no.

`-D enable_Fortran=` {no | yes}

> This option turns on compiling a fortran example code. Default is no.

`-D CMAKE_CXX_FLAGS="C++ compiler flags"`

>  This option can be used for specifying C++ compiler flags in string format.

`-D CMAKE_C_FLAGS="C compiler flags"`

>  This option can be used for specifying C compiler flags in string format.
  Note that C compiler is necessary only if example codes are compiled, i.e. `-D with_example=yes`.

`-D CMAKE_Fortran_FLAGS="Fortran compiler flags"`

>  This option can be used for specifying Fortran compiler flags in string format.
  Note that Fortran compiler is necessary only if example codes are compiled, i.e. `-D with_example=yes`.

`-D CMAKE_TOOLCHAIN_FILE="file-name"`

>  This option can be used for specifying a predefined toolchain file "file-name" for a specific platforms.

`-D with_PYTHON=` {no | yes}

> This option turns on building PMlib Python APIs. Default is no.

`-D pybind11_ROOT=` "${PYBIND11_ROOT}" 

> For building PMlib Python APIs, PMlib requires PyBind11 standard templates.
If the previous `with_PYTHON`  option is set `yes`,
then this option must also be set pointing to the PyBind11 top directory.



### Compiler option values

Compiler options can be given as the options to cmake.
The default compiler options are described in `cmake/CompilerOptionSelector.cmake` file.
On some systems the compiler names should be given as the environment variables
such as CXX/CC/F90/FC.
There are Toolchain cmake files which define the compiler names for
several systems. See cmake/Toolchan_${system}.cmake.


### Cmake Examples

#### Supercomputer Fugaku clang mode compiler

##### MPI version with Python APIs enabled, compiling on compute node

~~~
cmake command example for supercomputer Fugaku - compute node clang mode MPI x OpenMP
Note that PYTHON modules can be compiled and linked on compute node only,
i.e. cross compiling Python on login node is not available.

$ CXXFLAGS="--std=c++11 -Nclang -Kocl "
$ CFLAGS="--std=c11 -Nclang -Kocl "
$ FFLAGS="-Kocl "
$ PAPI_DIR=/opt/FJSVxos/devkit/aarch64/rfs/usr
$ PMLIB_DIR=${HOME}/pmlib/usr_local_pmlib
$ PACKAGE_DIR=${HOME}/pmlib/PMlib-9.1
$ cd $PACKAGE_DIR
$ mkdir -p BUILD-MPI
$ cd BUILD-MPI
$ cmake -DINSTALL_DIR=${PMLIB_DIR} \
		-DCMAKE_CXX_FLAGS="${CXXFLAGS} " \
		-DCMAKE_C_FLAGS="${CFLAGS} " \
		-DCMAKE_Fortran_FLAGS="${FFLAGS} " \
		-DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain_fugaku.cmake \
		-Denable_OPENMP=yes \
		-Dwith_MPI=yes \
		-Dwith_PAPI="${PAPI_DIR}" \
		-Dwith_POWER=yes \
		-Dwith_example=yes \
		-Denable_Fortran=yes \
		-Denable_PreciseTimer=yes \
		-Dwith_PYTHON=yes \
		-Dpybind11_ROOT="${PYBIND11_ROOT}" \
		../
$ # If `-Dwith_PYTHON=yes` is set, the next two lines are necessary.
$   sed -i 's/strip/echo/g'   src_pybind/CMakeFiles/pyPerfMonitor.dir/build.make
$   sed -i 's/-DCMAKE_INSTALL_DO_STRIP=1//g' src_pybind/Makefile
$ # endif
$ make
$ make install
~~~
<br>

#### INTEL/GNU/PGI compiler on Intel Xeon server

##### serial version

Single process (non-MPI) with possible OpenMP thread parallel version.  
The possible choice of the compilers and their suggested compiler options are shown below.


~~~sh:preset.sh
Choice of compilers and options:

Intel OneAPI compilers
	compiler command : C++:icpx CC:icx F90:ifort FC:ifort
	C++ compiler options : "-qopenmp -std=c++11 -DUSE_PRECISE_TIMER "
	C   compiler options : "-qopenmp -std=c99"
	Fortran compiler options : "-qopenmp -fpp "

GNU compiler:  
	compiler command : C++:g++ CC:gcc F90:gfortran FC:gfortran
	 C++ compiler options : "fopenmp -std=c++11"
	 C   compiler options : "fopenmp -cpp"
	 Fortran compiler options : "fopenmp -cpp"

PGI compiler:  
	compiler command : C++:pgc++ CC:pgcc F90:pgf90 FC:pgfortran
	 C++ compiler options : "-mp=numa -std=c++11"
	 C   compiler options : "-mp=numa -c99"
	 Fortran compiler options : "-mp=numa -Mpreprocess"
~~~

Above compiler options should be set as CMAKE\_\*\_FLAGS options to cmake command.  

##### INTEL serial version build example

Actual cmake command example for Intel compiler on Intel Xeon platform is shown below.

~~~
cmake command example for Intel OneAPI compiler - SERIAL.

$ source /opt/intel/oneapi/setvars.sh intel64
$ export LANG=C
$ export CC=icx CXX=icpx F90=ifort FC=ifort
$ CXXFLAGS="-qopenmp -std=c++11 -DUSE_PRECISE_TIMER "
$ CFLAGS="-qopenmp -std=c99"
$ FFLAGS="-qopenmp -fpp "
$ PMLIB_DIR=${HOME}/pmlib/usr_local_pmlib
$ PACKAGE_DIR=${HOME}/pmlib/PMlib-9.1
$ cd $PACKAGE_DIR
$ mkdir -p BUILD
$ cd BUILD
$ cmake -DINSTALL_DIR=${PMLIB_DIR} \
	-DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
	-DCMAKE_C_FLAGS="${CFLAGS}" \
	-DCMAKE_Fortran_FLAGS="${FFLAGS}" \
	-Denable_OPENMP=yes \
	-Denable_PreciseTimer=yes \
	-Dwith_PAPI=yes \
	-Dwith_example=yes \
	../
$ make
$ make install
~~~


##### MPI version

MPI with OpenMP hybrid parallel version.

Choose the compilers that support MPI. Note that the command name can be site dependent.
The choice of options are similar to the serial version

~~~sh:preset.sh
Choice of compilers and options:

Intel OneAPI compilers
	MPI compiler command : C++:mpiicpx CC:mpiicx F90:mpiifort FC:mpiifort
	serial compiler command : C++:icpx CC:icx F90:ifort FC:ifort

GNU compiler:  
	MPI compiler command : C++:mpicxx CC:mpicc F90:mpif90 FC:mpif90

PGI compiler:  
	MPI compiler command : C++:mpic++ CC:mpicc F90:mpif90 FC:mpif90
~~~

##### INTEL MPI version build example

Actual cmake command example for Intel compiler on Intel Xeon platform is shown below.
Note that, not only the MPI compiler command, but also the serial compilers are set for Intel cmake.

~~~
cmake command example for Intel OneAPI compiler - MPI x OpenMP

$ source /opt/intel/oneapi/setvars.sh intel64
$ export LANG=C
$ export CXX=mpiicpx CC=mpiicx F90=mpiifort FC=mpiifort
$ export I_MPI_CXX=icpx I_MPI_CC=icx I_MPI_F90=ifort I_MPI_FC=ifort
$ CXXFLAGS="-qopenmp -std=c++11 -DUSE_PRECISE_TIMER "
$ CFLAGS="-qopenmp -std=c99"
$ FFLAGS="-qopenmp -fpp "
$ PMLIB_DIR=${HOME}/pmlib/usr_local_pmlib
$ PACKAGE_DIR=${HOME}/pmlib/PMlib-9.1
$ cd $PACKAGE_DIR
$ mkdir -p BUILD-MPI
$ cd BUILD-MPI
$ cmake -DINSTALL_DIR=${PMLIB_DIR} \
	-DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
	-DCMAKE_C_FLAGS="${CFLAGS}" \
	-DCMAKE_Fortran_FLAGS="${FFLAGS}" \
	-Denable_OPENMP=yes \
	-Denable_PreciseTimer=yes \
	-Dwith_PAPI=yes \
	-Dwith_example=yes \
	../
$ make
$ make install
~~~
<br>



## HOW TO RUN YOUR APPLICATION WITH PMLIB

### DOCUMENTS FOR COMPILING AND RUNNING THE APPLICATION WITH PMLIB

There are several documents explaining how to build and run the application
with PMlib.

~~~
doc/tutorial/Introducing-PMlib-20241114.pdf 
	: overview including how to code, compile and execute the application
	  using PMlib. report example and symbol notation

doc/scripts/
	: compile and run jobscript examples for user applications
	: cmake script examples for PMlib building

doc/Readme.md : How to generate the detail API document with Doxygen
~~~

* If you specify the cmake option `-Denable_example=yes`, the test programs
calling PMlib are compiled and saved as example1, example2,.. 
under the example direcoty.

* More example source programs, compile and run scripts, some logs can be found
under doc/ directory.



### RUN TIME ENVIRONMENT VARIABLES

When running applications linked with PMlib, the following environment variables can be set to the shell.

`PMLIB_REPORT=(BASIC|DETAIL|FULL)`

This environment variable controlls the level of details in the PMlib performance statistics report.
The value BASIC will provide the summary report of the averaged values of the all processes.
The value DETAIL will provide the statistics report for all the processes.
The value FULL will provide the statistics report for all the threads of all the processes.
Note that the amount of the report is decided by the number of processes, the number of threads, the choice of HWPC_CHOOSER.

`HWPC_CHOOSER=(FLOPS|BANDWIDTH|VECTOR|LOADSTORE|CACHE|CYCLE|USER)`

If this environment variable is set, PMlib automatically detects the PAPI based hardware counters. If this environment variable is not set, the HWPC counters are not reported.
To enable this feature, PMlib must be built with PAPI option enabled.

`POWER_CHOOSER=(NODE|NUMA|PARTS|OFF)`

If this environment variable is set, PMlib detects the POWER API supported devices and collect the data from them.
The power consumption information is recorded in rather coarse granularity, and the overhead to collect the data from those devices tends to be heavy.
For the performance measurement that requires precise time resolution, it is recommended to set this value as OFF.
To enable this feature, PMlib must be built with Power API option enabled.

`BYPASS_PMLIB=YES`

This environment variable disable PMlib measurements. The PMlib APIs return immediately with no effects.

`OTF_TRACING=(off|on|full)`

If this environment variable is set, PMlib automatically generates the Open Trace Format files for post processing.
To enable this feature, PMlib must be built with OTF tracing option enabled.
There will be three type of OTF files.

~~~
  ${OTF_FILENAME}.otf
  ${OTF_FILENAME}.0.def
  ${OTF_FILENAME}.(1|2|..|N).events
~~~

See the next environment variable `OTF_FILENAME`. If the value is "off" or not defined, the OTF files are not produced. If the value is "on", the OTF files will contain the timer information only. If the value is "full", the OTF files will contain the counter information as well as the timer information. Remark that `OTF_TRACING=full` may yield the heavy overhead, if the measuring sections are repeated many times.

`OTF_FILENAME="some file name"`

The value of `${OTF_FILENAME}` is used to prefix the OTF file names if the value of previous `${OTF_TRACING}` has been set to "on" or "full". If this environment variable is not set, the default value of `"pmlib_optional_otf_files"` is used to prefix the OTF file names.


### Remark on PAPI interface

Note that the default PAPI library provided with distro Operating System may not be best built for the
specific CPU type. In such case, users may choose to build PAPI library using the latest distribution
for their environment. The latest package and the build instructions are available from PAPI home page.

Typically, the Linux PAPI installation on Intel Xeon processors can be as simple as the example below.

~~~
$ ./configure --prefix=${PREFIX} CC=icc F77=ifort
$ make
$ make install-all
~~~


### Remark on Max_nthreads parameter

Maximum number of the measuable threads per process can be configured. The default is 48.
It is recommended to set this parameter accrding to the actual CPU hardware configuration.
Change the value of `const int Max_nthreads` in `~/include/pmlib_papi.h`, and build.


## CONTRIBUTORS

* Kenji     Ono      *keno@{cc.kyushu-u.ac, riken}.jp*
* Kazunori  Mikami   kazunori.mikami@riken.jp
* Soichiro  Suzuki
* Syoyo     Fujita
