#############
About echelon
#############

echelon is an HDF5-based library for saving and managing scientific data. echelon is written using modern C++ to enable
easy, versatile and save access to your data.

*************
Project goals
*************

.. note::
	This section needs to be extended.

* The usage of the library should be as distraction-free as possible. All boilerplate operations should be automatically
  performed by the library.
* Common use cases should be straightforward.
* The library should be extendable, if appropriate. Especially user-defined data types should be supported.
* The library should be able to read and manipulate every valid HDF5 file,
  which was created by a supported version of the HDF5 library.
* The design of the library should be driven by common concepts rather than special cases.
* The library should permit peak performance for common usage patterns.

*******************
What echelon is not
*******************
echelon is not a low-level C++ wrapper for HDF5. Therefore, not all echelon objects map directly to their HDF5
counterparts. if they have any. While this might make the handling of echelon-produced files with low-level HDF5
libraries more cumbersome, this was a deliberate design decision from the start as many of the advanced features
of this library are hard or even impossible to implement without using much more elaborate structures to express
them within the HDF5 object model. The HDF5 library even predates us in this decision, as for example dimension
scales map to a quite complex set of primitive HDF5 objects. To aid all users which want to use echelon in combination
with low-level HDF5 libraries (or HDF5-enabled software), we will provide a file-scoped option which will disable those
advanced features in the future. It should be noted however, that each echelon file is always a valid HDF5 file
and that we try to map each feature to the HDF5 object model as plain as possible.

If you are actually interested in a low-level wrapper for HDF5, you might have a look at the echelon.HDF5 sub-library,
which is shipped with the core library. While this library is mainly intended for internal usage, it might serve you well.
Unfortunately the library is not documented at all at the point of this writing.
There is an ongoing proposal to document the library properly and
lower some of the advanced features from the core library into echelon.HDF5.

************************************
How can I contribute to the project?
************************************

.. note::
	This section needs to be written.