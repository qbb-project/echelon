###############
Getting started
###############

***********************
How to install echelon?
***********************

Installing echelon on Linux
===========================

To install echelon on Linux, simply change into echelon's root directory
and create a build directory (for example build/).
Afterwards change into your build directory and configure the build system
using

.. code-block:: bash

    cmake ../

Finally type

.. code-block:: bash

    make

to start the compilation process.
After the compilation has finished successfully, you can use

.. code-block:: bash

    make install

to install echelon in /usr/local (an other path can be specified using CMake standard methods), if you want.

To generate the documentation using doxygen just type

.. code-block:: bash

    make doc

***********************************************
How to integrate echelon into your application?
***********************************************

echelon comes with two kinds of CMake integration. The first one treats echelon as an external
dependency via CMake's find_package mechanism and should be prefered, if you have installed
echelon on your computer. The second one treats echelon as a subproject of your CMake-enabled
project and should be prefered, if you use echelon on many different machines and don't want to
install echelon everywhere, or if you want to ship your code with echelon in one package.

If you don't use CMake, I am afraid that you are on your own and should try to convince your
compiler/build system to find echelon for you.

Using echelon as an external dependency
=======================================

After you have build and installed echelon somewhere, you can use echelon's find module
to find and configure the library. Just copy FindEchelon.cmake from cmake/ to some
directory in CMake's module path and use the standard find_package syntax.

Using echelon as a subproject
=============================

To use echelon as a subproject, simply copy its root directory to some reasonable place within your project's
directory hierarchy and integrate echelon's build system into your exisiting build system
by adding ::

    add_subdirectory(echelon's-root-directory)

If used as a subproject, echelon defines the same
variables, as if it would have done in the external dependency case, which makes both approaches quite interchangeable.
The big advantage of the subproject approach is, that echelon is automatically build alongside your project
and you don't have to bother with a second manual compilation.

Manually link against echelon 
=============================

If you use a different build system than CMake (or good old manual compilation), you need to link manually against
the libraries echelon and echelon_hdf5, together with the HDF5 libraries hdf5 and hdf5_hl.
In addition, the compiler's C++11 support has to be enabled.