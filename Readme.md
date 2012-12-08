echelon
=======

echelon is an HDF5-based library for saving and managing scientific data.
echelon is written using modern C++ to enable easy, versatile and save access to your data.

Prerequisites
-------------

- CMake 2.8.0 or above
- HDF5 1.8.0 or above
- Boost 1.50.0 or above
- A C++11 - compatible compiler (tested with gcc 4.7 and clang 3.1)
- Doxygen 1.8.2 or above (optional)

*Warning: echelon does not compile with gcc 4.6 or older versions due to a compiler bug.*

Getting started
---------------

### Installing echelon on Linux

To install echelon on Linux simply change into echelon's root directory
and create a build directory (for example build/).
Afterwards change into your build directory and configure the build system
using
~~~~~~~~~~~~~~~~~{.bash}
cmake ../
~~~~~~~~~~~~~~~~~
Finally type
~~~~~~~~~~~~~~~~~{.bash}
make
~~~~~~~~~~~~~~~~~
to start the compilation process.
After the compilation has finished successfully, you can use
~~~~~~~~~~~~~~~~~{.bash}
make install
~~~~~~~~~~~~~~~~~
to install echelon in /usr/local (an other path can be specified using CMake standard methods), if you want.

To generate the documentation using doxygen just type
~~~~~~~~~~~~~~~~~{.bash}
make doc
~~~~~~~~~~~~~~~~~