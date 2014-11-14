#################################
echelon file format specification
#################################

*************
Legacy format
*************

.. warning::
   This version of the format is deprecated and support will be removed prior to the 1.0 release.

This section describes the legacy format of echelon.

file objects
============

File objects present the top-most entity of every echelon file, providing the container for every other content of the file.

Format
------

Each file object is represented by an HDF5 file object which, in addition, contains the following attributes:

======================= ========= =========================================================================================
Name                    Type      Description
======================= ========= =========================================================================================
echelon.library_version string    version of the library, which was used to create this file
echelon.format_version  string    version of the file format (version 1.0)
======================= ========= =========================================================================================

In addition, the file object might contain arbitrary user-defined echelon objects (groups, attributes, datasets, etc.).

group objects
=============

Group objects provide a container which can be used to bundle several echelon objects.

Format
------

Each group object is represented by an HDF5 group object and might contain arbitrary user-defined echelon objects.

attribute objects
=================

Attribute objects present metadata which can be attached to another echelon object.

Format
------

Each attribute object is represented by an HDF5 attribute object.

dataset objects
===============

Datasets present an in-file array.

Format
------

Datasets are represented by an HDF5 group with the name of the dataset, which contains an HDF5 dataset of the
name 'data' in which the data is actually stored and all sub-objects which need to be created. Attributes are attached to the group.
If dimensions and dimension scales are associated with the dataset, these are represented using the native HDF5 facilities.
The dataset which contains the dimension scale's data is stored in a group 'dimensions/dimension<*index-of-the-dimension*>/dimension_scales'.
This group is assembled lazily upon dimension scale creation.

scalar dataset objects
======================

Scalar datasets present a single datum within the file.

Format
------

Scalar datasets are represented by an HDF5 group with the name of the dataset, which contains an HDF5 scalar dataset of the
name 'data' in which the data is actually stored. Attributes are attached to the group.

object references
=================

Object references present a reference to another echelon object.

Format
------

Each object reference is represented by an HDF5 object reference.