//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_FILE_HPP
#define ECHELON_FILE_HPP

#include <echelon/group.hpp>
#include <echelon/link.hpp>
#include <echelon/attribute_repository.hpp>

#include <echelon/hdf5/file.hpp>

#include <string>
#include <memory>

namespace echelon
{

/** \brief A handle to a file object.
 *
 *  It can be used to access all properties of an echelon file.
 *  In addition it implements the same interface as group to cover the symmetry
 *  between a file (or more precise it's root group) and any other group.
 */
class file
{
public:
    /** \brief Type of the underlying HDF5 low-level handle
     */
    using native_handle_type = hdf5::file;

    /** \brief Initializes the handle with its null state.
     */
    file() = default;
    
    /** Enum containing possible creation modes for a file handle.
     */
    enum class create_mode
    {
        truncate, ///< Truncate any pre-existing file.
        exclusive ///< Raise an error, if the file already exists.
    };

    /** Enum containing possible open modes for a file handle.
     */
    enum class open_mode
    {
        read_only, ///< Open the file in the read-only mode.
        read_write ///< Open the file in the read-write mode.
    };

    // file interface

    /** \brief Creates a new file using the given path and creation mode.
     *
     *  \param path path of the new echelon file
     *  \param mode creation mode, which is used during the file creation.
     */
    file(const std::string& path, create_mode mode);

    /** \brief Opens a pre-existing file using the given path and open
     *         mode.
     *
     *  \param path path of the pre-existing echelon file
     *  \param mode open mode, which is used to open the file
     */
    file(const std::string& path, open_mode mode);

    explicit file(native_handle_type native_handle_);

    // group interface

    /** \brief Creates a new group within the root group.
     *
     *  \param name name of the new group
     *  \return a handle to the new group
     */
    group create_group(const std::string& name);

    /** \brief Creates a new dataset within the root group.
     *
     *  \param name name of the new dataset
     *  \param datatype value type of the new dataset
     *  \param dims shape of the new dataset
     *  \param options additional dataset creation options
     *         keyword            |          semantic
     *         -------------------|-----------------------------------------
     *         compression_level  | level of the deflate compression (0 - 9)
     *         shuffle_filter     | enable/disable the shuffle filter
     *         auto_chunking      | enable/disable auto-chunking
     *         chunk_shape        | shape of a dataset chunk
     *
     *  \return a handle to the new dataset
     */
    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims, const dataset_options& options = dataset_options());

    /** \brief Creates a new dataset within the root group.
     *
     *  If the maximal extent of the dataset is specified any value might be set to echelon::unlimited
     *  to not restrict this dimension.
     * 
     *  \param name name of the new dataset
     *  \param datatype value type of the new dataset
     *  \param dims shape of the new dataset
     *  \param max_dims maximal extent of the dataset (each value might be 'unlimited')
     *  \param options additional dataset creation options
     *         keyword            |          semantic
     *         -------------------|-----------------------------------------
     *         compression_level  | level of the deflate compression (0 - 9)
     *         shuffle_filter     | enable/disable the shuffle filter
     *         auto_chunking      | enable/disable auto-chunking
     *         chunk_shape        | shape of a dataset chunk
     *
     *  \return a handle to the new dataset
     */
    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims, const std::vector<hsize_t>& max_dims,
                           const dataset_options& options = dataset_options());
    
    /** \brief Creates a new dataset within the root group.
     *
     *  \param name name of the new dataset
     *  \param dims shape of the new dataset
     *  \param options additional dataset creation options
     *         keyword            |          semantic
     *         -------------------|-----------------------------------------
     *         compression_level  | level of the deflate compression (0 - 9)
     *         shuffle_filter     | enable/disable the shuffle filter
     *         auto_chunking      | enable/disable auto-chunking
     *         chunk_shape        | shape of a dataset chunk
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \return a handle to the new dataset
     */
    template <typename T>
    dataset create_dataset(const std::string& name, const std::vector<hsize_t>& dims,
                           const dataset_options& options = dataset_options())
    {
        return root_group_.create_dataset<T>(name, dims, options);
    }

    /** \brief Creates a new dataset within the root group.
     *
     *  If the maximal extent of the dataset is specified any value might be set to echelon::unlimited
     *  to not restrict this dimension.
     * 
     *  \param name name of the new dataset
     *  \param dims shape of the new dataset
     *  \param max_dims maximal extent of the dataset (each value might be 'unlimited')
     *  \param options additional dataset creation options
     *         keyword            |          semantic
     *         -------------------|-----------------------------------------
     *         compression_level  | level of the deflate compression (0 - 9)
     *         shuffle_filter     | enable/disable the shuffle filter
     *         auto_chunking      | enable/disable auto-chunking
     *         chunk_shape        | shape of a dataset chunk
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \return a handle to the new dataset
     */
    template <typename T>
    dataset create_dataset(const std::string& name, const std::vector<hsize_t>& dims,
                           const std::vector<hsize_t>& max_dims,
                           const dataset_options& options = dataset_options())
    {
        return create_dataset(name, type(hdf5::get_hdf5_type<T>()), dims, max_dims, options);
    }
    
    /** \brief Creates a new scalar dataset within the root group.
     *
     *  \param name name of the new dataset
     *  \param datatype value type of the new dataset
     *
     *  \return a handle to the new scalar dataset
     */
    scalar_dataset create_scalar_dataset(const std::string& name, const type& datatype);

    /** \brief Creates a new scalar dataset within the root group.
     *
     *  \param name name of the new dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \return a handle to the new scalar dataset
     */
    template <typename T>
    scalar_dataset create_scalar_dataset(const std::string& name)
    {
        return root_group_.create_scalar_dataset<T>(name);
    }

    /** \brief Creates a new scalar dataset within the root group and
     *         initializes it with a given value.
     *
     *  \param name name of the new dataset
     *  \param value value, which should be used to intialize the dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \return a handle to the new scalar dataset
     */
    template <typename T>
    scalar_dataset create_scalar_dataset(const std::string& name, const T& value)
    {
        return root_group_.create_scalar_dataset(name, value);
    }

    /** \brief Accessor function for the root group.
     *
     *  \param name name of the requested object
     *
     *  \return a handle to the requested object
     */
    object operator[](const std::string& name) const;

    /** \brief Removes the specified object from the root group.
     *
     *  \param name name of the removed object
     *
     */
    void remove(const std::string& name) const;

    /** \brief Returns the requested group, if it already exists, otherwise a
     *         new group is created.
     *
     *  \param name name of the requested group
     *
     *  \return the requested group, if it is already existing, or a new group
     *          otherwise
     */
    group require_group(const std::string& name);

    /** \brief Returns the requested dataset, if it already exists, otherwise a
     *         new dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the dataset already exists and its shape or datatype differ
     *  from their requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *  \param datatype value type of the new dataset
     *  \param dims shape of the new dataset
     *  \param options additional dataset creation options
     *         keyword            |          semantic
     *         -------------------|-----------------------------------------
     *         compression_level  | level of the deflate compression (0 - 9)
     *         shuffle_filter     | enable/disable the shuffle filter
     *         auto_chunking      | enable/disable auto-chunking
     *         chunk_shape        | shape of a dataset chunk
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested dataset, if it is already existing, or a new
     *          dataset otherwise
     */
    dataset require_dataset(const std::string& name, const type& datatype,
                            const std::vector<hsize_t>& dims, const dataset_options& options = dataset_options());

    /** \brief Returns the requested dataset, if it already exists, otherwise a
     *         new dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the dataset already exists and its shape or datatype differ
     *  from their requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *  \param dims shape of the new dataset
     *  \param options additional dataset creation options
     *         keyword            |          semantic
     *         -------------------|-----------------------------------------
     *         compression_level  | level of the deflate compression (0 - 9)
     *         shuffle_filter     | enable/disable the shuffle filter
     *         auto_chunking      | enable/disable auto-chunking
     *         chunk_shape        | shape of a dataset chunk
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested dataset, if it is already existing, or a new
     *          dataset otherwise
     */
    template <typename T>
    dataset require_dataset(const std::string& name, const std::vector<hsize_t>& dims,
                            const dataset_options& options = dataset_options())
    {
        return root_group_.require_dataset<T>(name, dims, options);
    }

    /** \brief Returns the requested scalar dataset, if it already exists,
     *         otherwise a new scalar dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the scalar dataset already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *  \param datatype value type of the new dataset
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested scalar dataset, if it is already existing, or a
     *          new scalar dataset otherwise
     */
    scalar_dataset require_scalar_dataset(const std::string& name, const type& datatype);

    /** \brief Returns the requested scalar dataset, if it already exists,
     *         otherwise the scalar dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the scalar dataset already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested scalar dataset, if it is already existing, or a
     *          new scalar dataset otherwise
     */
    template <typename T>
    scalar_dataset require_scalar_dataset(const std::string& name)
    {
        return root_group_.require_scalar_dataset<T>(name);
    }

    /** \brief Returns the requested scalar dataset, if it already exists,
     *         otherwise a new scalar dataset is created.
     *
     *  The new dataset is created using the given parameters and is initialized
     *  with the given value.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the scalar dataset already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *  \param value value, which should be used to intialize the dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested scalar dataset, if it is already existing, or a
     *          new scalar dataset otherwise
     */
    template <typename T>
    scalar_dataset require_scalar_dataset(const std::string& name, const T& value)
    {
        return root_group_.require_scalar_dataset(name, value);
    }

    /** \brief Iterates over every link within the root group.
     *
     *  \param op function, which is applied to every link
     *
     */
    void iterate_links(const std::function<void(const link&)>& op) const;

    /** \brief Visits every link, which is reachable from the root group
     *
     *  \param visitor visitor, which is applied to every visited link
     *
     */
    void visit_links(const std::function<void(const link&)>& visitor) const;

    /** \brief Visits every object, which is reachable from the root group
     *         (including the root group)
     *
     *  \param visitor visitor, which is applied to every visited object
     *
     */
    void visit_objects(const std::function<void(const object&)>& visitor) const;

    /** \brief A object reference to the root group.
     */
    object_reference ref() const;

    // general interface

    /** \brief The underlying HDF5 low-level handle.
     */
    native_handle_type native_handle() const;

    /** \brief Tests the validity of the handle.
     */
    explicit operator bool() const;
private:
    hdf5::file file_handle_;
    group root_group_;

public:
    /** \brief The attributes, which are attached to the root group.
     */
    attribute_repository<group> attributes() const;
};

/** \brief Mounts a file at a specified location.
 *
 *  \param mounted_file file to be mounted
 *  \param mount_point  location at which the file is mounted
 *
 *  \return a handle to the mount point
 *
 *  \relates echelon::file
 */
group mount(const file& mounted_file, const group& mount_point);

/** \brief Mounts a file at a specified location.
 *
 *  \param mounted_file file to be mounted
 *  \param mount_point  location at which the file is mounted
 *
 *  \return a handle to the mount point
 *
 *  \relates echelon::file
 */
group mount(const file& mounted_file, const file& mount_point);

/** \brief Unmounts a previously mounted file
 *
 *  \param mount_point mount point of the file to be unmounted
 *
 *  \relates echelon::file
 */
void unmount(const group& mount_point);
}

#endif
