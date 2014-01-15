.. highlight:: c++

########
Tutorial
########

*************************
Tutorial 1 - Introduction
*************************

File creation and basic write access
====================================

To create an empty file, we simply construct a new :echelon:`file <echelon::file>` handle, specifing the file path and a creation mode.
For this example, we choose the truncate mode, which ensures, that the content of any pre-existing file is discarded.
The code ::

    file hdf5_file("first_file.h5", file::create_mode::truncate);

creates the empty HDF5 file 'first_file.h5' and names the resulting handle hdf5_file.
With the first step done, we can add new objects to the file and store our data.

Before, we will store any data in our newly created file, we discuss one of the main benefits and major concepts of HDF5, the group.
An HDF5 :echelon:`group <echelon::group>`
allows us to save and access our data in a structured way. You can think of a group as an object similar to a folder
in your favorite file system, since both are used to bundle related objects together, even other groups/folders.
If you are familiar with UNIX file systems, you may even notice more similarities. Any new file starts with a group, which
we call the root group and denote with '/'. Since the file and the root group are strongly related, we normally
use the two terms interchangeable. This is reflected by the file class by providing the same interface as a group.

Using ::

    group data = hdf5_file.create_group("data");

we create a new group 'data' within the root group and save the group handle in data.

Now, we will store some test data in our file. To do this, we have to create a new dataset.
A dataset is a rectangular, homogeneous collection of elements and is the primary facility for
data storage in HDF5. A dataset can distribute its elements over an arbitrary number of dimensions.
This is convenient, since many object in science, for example matrices and tensors, extend along several
dimensions and we can map our data structures naturally onto our file structure. ::

    dataset ds = data.create_dataset<double>("ds", { 10 });

will create a new dataset, named 'ds', of ten double elements along a single dimension and stores the
dataset handle in ds.
The final step is to store our data in the dataset, which can be done by using the, hopefully reasonable, syntax ::

    ds <<= v;

In our example v is a std::vector<double>, which contains ten elements. It should be noted, that
the shape and datatype of the data must always match the shape and datatype of the dataset. An exception is thrown otherwise.
While we will restrict ourself to the element type double and the container std::vector in this example, echelon supports
several other types and containers, and can be extended to handle many more. Since this topic is a very complex one,
we will discuss it in its entire at a later point. For now it should be sufficient, that you know, that every primitive
C++ type is supported.

The following code summarizes every step we have done so far, from the creation of our file to the storage of
our data: ::

    #include <echelon/echelon.hpp>
    #include <vector>

    using namespace echelon;

    int main()
    {
        file hdf5_file("first_file.h5", file::create_mode::truncate);
            
        group data = hdf5_file.create_group("data");

        dataset ds = data.create_dataset<double>("ds", { 10 });

        std::vector<double> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        ds <<= v;
    }

Basic read access
=================

By now, we have learned how to store data in our file.
But it would be rather pointless to store data in the file, if we could not load it again.
In this section we will learn how to load the data from the previous example into a container.

As a first step we create a handle ::

    file hdf5_file("first_file.h5", file::open_mode::read_only);

to our old file 'first_file.h5'. The only difference to the first example is, that
we specify an open mode and not a creation mode. Since we only want to read data
in this example, we use the read-only mode.

You may remember, that we stored our dataset within a group during the previous example.
To access the dataset, we have to acquire a handle to the group first.
For this purpose group overloads the square bracket operator,
which takes the name of the object and returns a handle, if the object exists. Otherwise
an exception is thrown.
In our case we use ::

    group data = hdf5_file["data"];

to acquire a group handle to the previously created group 'data'.

The semantic of the above statement is much more complicated then one might expect at first glance.
Since groups can store objects of many different types and the structure of the file is not available at compile-time,
the square bracket operator does not know what kind of handle it should return. Instead it returns an object
handle, which can reference any object. Therefore we call such a handle a polymorphic handle.
To convert it to a meaningful handle, we guess a handle type at compile-time, in our case group,
and assign the object handle to it. echelon then checks at runtime, if our guess does match the real type of the object and throws an exception, if
we guessed wrong. One consequence of this is, that you can't use the auto keyword in this case, since the object handle would not
be converted to any meaningful type.

It is now straightforward to acquire a handle to the dataset ::

    dataset ds = data["ds"];

using the same syntax and to load the data into the std::vector<double> container v using ::

    v <<= ds;

While the container is automatically resized to hold the data if necessary, the value type of the dataset and
the value type of the container must match again. Otherwise an exception is thrown.

The following code summarizes the second example and prints the content of the container
using the standard output stream: ::

    #include <echelon/echelon.hpp>
    #include <vector>
    #include <iostream>

    using namespace echelon;

    int main()
    {
        file hdf5_file("first_file.h5", file::open_mode::read_only);
            
        group data = hdf5_file["data"];

        dataset ds = data["ds"];

        std::vector<double> v;

        v <<= ds;

        for(auto value : v)
            std::cout << value << "  ";
    }


*****************************
Tutorial x - Advanced tidbits
*****************************

Dimensions and dimension scales
===============================

One feature of datasets which wasn't presented so far are :echelon:`dataset dimensions <echelon::dimension>`.
The dimensions are accessed through the, hopefully indisputably named, dataset property :echelon:`dimensions <echelon::dataset::dimensions>`. 
For example, one could give each dimension a descriptive, as shown in the following code sample label ::

    my_dataset.dimensions.relabel("foo");

For details, please refer to the corresponding :echelon:`API documentation <echelon::dataset_dimensions>`.

Another useful feature linked to dataset dimensions are :echelon:`dimension scales <echelon::dimension_scale>` which can be used to add certain meta-data to that dimension. For example, if one has saved a tabulated function in a dataset, one could add the corresponding value for each variable in a dimension scale, keeping this meta-data close to the actual data.

The following code shows how to add a dimension scale 'x' of type double to an already existing dataset ::

    my_dataset.dimensions.attach_dimension_scale<double>("x");

Echelon will automatically assemble the necessary data structures within the file and will free them if the dataset is destroyed. The shape of each dimensions scale is always one-dimensional and
matches the length of the corresponding dataset dimension.

Since dimension scales are in a sense specialized datasets one can use them as is usual for datasets. For example the code ::

    my_dimension_scale <<= x;    

would write the content of the container x into the dimension scale.