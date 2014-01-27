//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_GROUP_HPP
#define ECHELON_GROUP_HPP

#include <echelon/object.hpp>
#include <echelon/type.hpp>
#include <echelon/attribute_repository.hpp>
#include <echelon/dataset.hpp>
#include <echelon/scalar_dataset.hpp>
#include <echelon/object_reference.hpp>
#include <echelon/link.hpp>

#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/type_factory.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

namespace echelon
{

class file;

/** \brief Additional options for the dataset creation
 *
 */
class dataset_options
{
public:
    /** \brief Enables/Disables auto-chunking
     *
     *  \param value state of the auto-chunking algorithm
     *
     *  \return *this
     */
    dataset_options& auto_chunking(bool value)
    {
        auto_chunking_ = value;

        return *this;
    }

    /** \brief Sets the gzip compression level (0 - 9) of the dataset
     *
     *  \param value compression level (0 - 9)
     *
     *  \return *this
     */
    dataset_options& compression_level(int value)
    {
        compression_level_ = value;

        return *this;
    }

    /** \brief Enables/disables the shuffle filter
     */
    dataset_options& shuffle_filter(bool value)
    {
        shuffle_filter_ = value;

        return *this;
    }

    /** \brief Sets the chunk shape of the dataset
     *
     *  \param value chunk shape
     *
     *  \return *this
     */
    dataset_options& chunk_shape(std::vector<hsize_t> value)
    {
        chunk_shape_ = std::move(value);

        return *this;
    }

    /** \brief auto-chunking option
     */
    bool auto_chunking() const
    {
        return auto_chunking_;
    }

    /** \brief gzip compression level
     */
    int compression_level() const
    {
        return compression_level_;
    }

    /** \brief shuffle filter option
     */
    bool shuffle_filter() const
    {
        return shuffle_filter_;
    }

    /** \brief chunk shape
     */
    const std::vector<hsize_t>& chunk_shape() const
    {
        return chunk_shape_;
    }

private:
    bool auto_chunking_ = false;
    int compression_level_ = -1;
    bool shuffle_filter_ = false;
    std::vector<hsize_t> chunk_shape_ = {};
};

/** \brief A handle to an HDF5 group object.
 *
 */
class group
{
public:
    /** \brief Type of the underlying HDF5 low-level handle
     */
    using native_handle_type = hdf5::group;

    friend class file;

    explicit group(native_handle_type native_handle_);

    /** \brief Creates a new HDF5 group within this group.
     *
     *  \param name name of the new group
     *  \return a handle to the new group
     */
    group create_group(const std::string& name);

    /** \brief Creates a new HDF5 dataset within this group.
     *
     *  \param name name of the new dataset
     *  \param datatype value type of the new dataset
     *  \param dims shape of the new dataset
     *  \param options additional dataset creation options
     *         keyword             |          semantic
     *         --------------------|-----------------------------------------
     *         _compression_level  | level of the deflate compression (0 - 9)
     *         _chunk_shape        | shape of a dataset chunk
     *
     *  \return a handle to the new dataset
     */
    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims, const dataset_options& options = {});

    /** \brief Creates a new HDF5 dataset within this group.
     *
     *  \param name name of the new dataset
     *  \param dims shape of the new dataset
     *  \param options additional dataset creation options
     *         keyword             |          semantic
     *         --------------------|-----------------------------------------
     *         _compression_level  | level of the deflate compression (0 - 9)
     *         _chunk_shape        | shape of a dataset chunk
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \return a handle to the new dataset
     */
    template <typename T>
    dataset create_dataset(const std::string& name, const std::vector<hsize_t>& dims,
                           const dataset_options& options = {})
    {
        return create_dataset(name, type(hdf5::get_hdf5_type<T>()), dims, options);
    }

    /** \brief Creates a new HDF5 scalar dataset within this group.
     *
     *  \param name name of the new dataset
     *  \param datatype value type of the new dataset
     *
     *  \return a handle to the new scalar dataset
     */
    scalar_dataset create_scalar_dataset(const std::string& name, const type& datatype);

    /** \brief Creates a new HDF5 scalar dataset within this group.
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
        return create_scalar_dataset(name, type(hdf5::get_hdf5_type<T>()));
    }

    /** \brief Creates a new HDF5 scalar dataset within this group and
     *initializes it with a given value.
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
        scalar_dataset ds = create_scalar_dataset<T>(name);

        ds <<= value;

        return ds;
    }

    /** \brief Accessor function for this group.
     *
     *  \param name name of the requested object
     *
     *  \return a handle to the requested object
     */
    object operator[](const std::string& name) const;

    /** \brief Removes the specified object from the group.
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
     *         keyword             |          semantic
     *         --------------------|-----------------------------------------
     *         _compression_level  | level of the deflate compression (0 - 9)
     *         _chunk_shape        | shape of a dataset chunk
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested dataset, if it is already existing, or a new
     *          dataset otherwise
     */
    dataset require_dataset(const std::string& name, const type& datatype,
                            const std::vector<hsize_t>& dims, const dataset_options& options = {});

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
     *         keyword             |          semantic
     *         --------------------|-----------------------------------------
     *         _compression_level  | level of the deflate compression (0 - 9)
     *         _chunk_shape        | shape of a dataset chunk
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
                            const dataset_options& options = {})
    {
        return require_dataset(name, type(hdf5::get_hdf5_type<T>()), dims, options);
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
        return require_scalar_dataset(name, type(hdf5::get_hdf5_type<T>()));
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
        return require_scalar_dataset(name, value);
    }

    /** \brief Iterates over every link within this group.
     *
     *  \param op function, which is applied to every link
     *
     */
    void iterate_links(const std::function<void(const link&)>& op) const;

    /** \brief Visits every link, which is reachable from this group
     *
     *  \param visitor visitor, which is applied to every visited link
     *
     */
    void visit_links(const std::function<void(const link&)>& visitor) const;

    /** \brief Visits every object, which is reachable from this group
     *         (including this group)
     *
     *  \param visitor visitor, which is applied to every visited object
     *
     */
    void visit_objects(const std::function<void(const object&)>& visitor) const;

    /** \brief A HDF5 object reference to this group.
     */
    object_reference ref() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    native_handle_type native_handle() const;

private:
    friend class constructor_access;
    friend class object;

    enum class creation_mode
    {
        open,
        create
    };

    explicit group(const file& loc, const std::string& name = "/");

    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims, int comp_level, bool auto_chunking,
                           bool shuffle_filter, const std::vector<hsize_t> chunk_shape);

    hdf5::group group_handle_;

public:
    /** \brief The attributes, which are attached to the group.
     */
    attribute_repository<group> attributes() const;
};
}

#endif
