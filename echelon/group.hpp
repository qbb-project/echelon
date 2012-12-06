//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_GROUP_HPP
#define ECHELON_GROUP_HPP

#include <echelon/object.hpp>
#include <echelon/type.hpp>
#include <echelon/type_factory.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/creation_mode.hpp>
#include <echelon/attribute_repository.hpp>
#include <echelon/dataset.hpp>
#include <echelon/scalar_dataset.hpp>
#include <echelon/object_reference.hpp>
#include <echelon/utility.hpp>

#include <echelon/broken_contract_exception.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <exception>
#include <functional>

namespace echelon
{

class file;

/** \brief Exception, which is thrown, if a requested object does not exist.
 */
class non_existing_member_exception : public std::exception
{
public:
    /** \brief Creates a new exception with a given error description.
     *
     * \param what_ error description
     */
    non_existing_member_exception(const std::string& what_)
    :what_(what_)
    {}

    /** \brief The destructor
     */
    ~non_existing_member_exception() noexcept {}

    /** \brief An associated error description.
     */
    const char* what()const noexcept
    {
        return what_.c_str();
    }
private:
    std::string what_;
};

/** \brief A handle to an HDF5 group object.
 *
 */
class group
{
public:
    friend class file;

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
     *  \param comp_level compression level, which is applied to the new dataset
     *
     *  \return a handle to the new dataset
     */
    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1);

    /** \brief Creates a new HDF5 dataset within this group.
     *
     *  \param name name of the new dataset
     *  \param dims shape of the new dataset
     *  \param comp_level compression level, which is applied to the new dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \return a handle to the new dataset
     */
    template<typename T>
    dataset create_dataset(const std::string& name,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1)
    {
        return create_dataset(name,get_hdf5_type<T>(),dims,comp_level);
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
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \return a handle to the new scalar dataset
     */
    template<typename T>
    scalar_dataset create_scalar_dataset(const std::string& name)
    {
        return create_scalar_dataset(name,get_hdf5_type<T>());
    }

    /** \brief Creates a new HDF5 scalar dataset within this group and initializes it with a given value.
     *
     *  \param name name of the new dataset
     *  \param value value, which should be used to intialize the dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \return a handle to the new scalar dataset
     */
    template<typename T>
    scalar_dataset create_scalar_dataset(const std::string& name,const T& value)
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
    object operator[](const std::string& name)const;

    /** \brief Returns the requested group, if it already exists, otherwise a new group is created.
     *
     *  \param name name of the requested group
     *
     *  \return the requested group, if it is already existing, or a new group otherwise
     */
    group require_group(const std::string& name);

    /** \brief Returns the requested dataset, if it already exists, otherwise a new dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  \param name name of the requested dataset
     *  \param datatype value type of the new dataset
     *  \param dims shape of the new dataset
     *  \param comp_level compression level, which is applied to the new dataset
     *
     *  \return the requested dataset, if it is already existing, or a new dataset otherwise
     */
    dataset require_dataset(const std::string& name, const type& datatype,
                            const std::vector<hsize_t>& dims,
                            int comp_level = -1);

    /** \brief Returns the requested dataset, if it already exists, otherwise a new dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  \param name name of the requested dataset
     *  \param dims shape of the new dataset
     *  \param comp_level compression level, which is applied to the new dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \return the requested dataset, if it is already existing, or a new dataset otherwise
     */
    template<typename T>
    dataset require_dataset(const std::string& name,
                           const std::vector<hsize_t>& dims,
                           int comp_level = -1)
    {
        return require_dataset(name,get_hdf5_type<T>(),dims,comp_level);
    }

    /** \brief Returns the requested scalar dataset, if it already exists, otherwise a new scalar dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  \param name name of the requested dataset
     *  \param datatype value type of the new dataset
     *
     *  \return the requested scalar dataset, if it is already existing, or a new scalar dataset otherwise
     */
    scalar_dataset require_scalar_dataset(const std::string& name, const type& datatype);

    /** \brief Returns the requested scalar dataset, if it already exists, otherwise the scalar dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  \param name name of the requested dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \return the requested scalar dataset, if it is already existing, or a new scalar dataset otherwise
     */
    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name)
    {
        return require_scalar_dataset(name,get_hdf5_type<T>());
    }

    /** \brief Returns the requested scalar dataset, if it already exists, otherwise a new scalar dataset is created.
     *
     *  The new dataset is created using the given parameters and is initialized with the given value.
     *
     *  \param name name of the requested dataset
     *  \param value value, which should be used to intialize the dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \return the requested scalar dataset, if it is already existing, or a new scalar dataset otherwise
     */
    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name,const T& value)
    {
        type datatype = get_hdf5_type<T>();

        if(exists(object(*this),name) && get_object_type_by_name(object(*this),name) == object_type::scalar_dataset)
        {
            scalar_dataset ds(hdf5::dataset(id(),name,hdf5::default_property_list));

            if(ds.datatype() != datatype)
                throw broken_contract_exception("The required datatype doesn't "
                                                "match the datatype of the dataset.");

            return ds;
        }
        else
        {
            scalar_dataset ds = create_scalar_dataset(name,datatype);

            ds <<= value;

            return ds;
        }
    }

    /** \brief Iterates over every object within this group.
     *
     *  \param op function, which is applied to every object
     *
     */
    void iterate(const std::function<void(const object&)>& op);

    /** \brief A HDF5 object reference to this group.
     */
    object_reference ref()const;

    /** \brief The ID, which corresponds to the underlying HDF5 object.
     */
    hid_t id() const noexcept;

    /** \brief The underlying HDF5 low-level handle.
     */
    const hdf5::group& get_native_handle()const;
private:
    friend class constructor_access;
    friend class object;

    explicit group(const file& loc, const std::string& name = "/");
    group(const object& parent, const std::string& name, creation_mode mode);
    explicit group(hdf5::group group_wrapper_);

    hdf5::group group_wrapper_;

public:
    /** \brief The attributes, which are attached to the group.
     */
    attribute_repository<group> attributes;
};

}

#endif
