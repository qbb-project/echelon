//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_FILE_HPP
#define ECHELON_FILE_HPP

#include <echelon/group.hpp>
#include <echelon/hdf5/file.hpp>

#include <echelon/keywords.hpp>

#include <string>
#include <memory>

namespace echelon
{

/** \brief A handle to an HDF5 file object.
 *
 *  It can be used to access all properties of an HDF5 file.
 *  In addition it implements the same interface as group to cover the symmetry between
 *  a file (or more precise it's root group) and any other group.
 */
class file
{
public:
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

    //file interface

    /** \brief Creates a new HDF5 file using the given path and creation mode.
     *
     *  \param path path of the new HDF5 file
     *  \param mode creation mode, which is used during the file creation.
     */
    file(const std::string& path, create_mode mode);

    /** \brief Opens a pre-existing HDF5 file using the given path and open mode.
     *
     *  \param path path of the pre-existing HDF5 file
     *  \param mode open mode, which is used to open the file
     */
    file(const std::string& path, open_mode mode);

    explicit file(hdf5::file file_wrapper_);

    /** \brief Returns the root group corresponding to this file.
     *
     *  \deprecated This function is deprecated in favor to file's group interface
     *              and will be removed in the near future.
     */
    group& root_group();

    //group interface

    /** \brief Creates a new HDF5 group within the root group.
     *
     *  \param name name of the new group
     *  \return a handle to the new group
     */
    group create_group(const std::string& name);

    /** \brief Creates a new HDF5 dataset within the root group.
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
     *  \tparam Options options type list
     *
     *  \return a handle to the new dataset
     */
    template<typename... Options>
    dataset create_dataset(const std::string& name, const type& datatype,
                           const std::vector<hsize_t>& dims,
                           Options... options)
    {
        return root_group_.create_dataset(name,datatype,dims,options...);
    }

    /** \brief Creates a new HDF5 dataset within the root group.
     *
     *  \param name name of the new dataset
     *  \param dims shape of the new dataset
     *  \param options additional dataset creation options
     *         keyword             |          semantic
     *         --------------------|-----------------------------------------
     *         _compression_level  | level of the deflate compression (0 - 9)
     *         _chunk_shape        | shape of a dataset chunk
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *  \tparam Options options type list
     *
     *  \return a handle to the new dataset
     */
    template<typename T,typename... Options>
    dataset create_dataset(const std::string& name,
                           const std::vector<hsize_t>& dims,
                           Options... options)
    {
        return root_group_.create_dataset<T>(name,dims,options...);
    }

    /** \brief Creates a new HDF5 scalar dataset within the root group.
     *
     *  \param name name of the new dataset
     *  \param datatype value type of the new dataset
     *
     *  \return a handle to the new scalar dataset
     */
    scalar_dataset create_scalar_dataset(const std::string& name, const type& datatype);


    /** \brief Creates a new HDF5 scalar dataset within the root group.
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
        return root_group_.create_scalar_dataset<T>(name);
    }

    /** \brief Creates a new HDF5 scalar dataset within the root group and initializes it with a given value.
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
        return root_group_.create_scalar_dataset(name,value);
    }

    /** \brief Accessor function for the root group.
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
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has terminated.
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
     *  \tparam Options options type list
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be fulfilled.
     *
     *  \return the requested dataset, if it is already existing, or a new dataset otherwise
     */
    template<typename... Options>
    dataset require_dataset(const std::string& name, const type& datatype,
                            const std::vector<hsize_t>& dims,
                            Options... options)
    {
        return root_group_.require_dataset(name,datatype,dims,options...);
    }

    /** \brief Returns the requested dataset, if it already exists, otherwise a new dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has terminated.
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
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *  \tparam Options options type list
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be fulfilled.
     *
     *  \return the requested dataset, if it is already existing, or a new dataset otherwise
     */
    template<typename T,typename... Options>
    dataset require_dataset(const std::string& name,
                           const std::vector<hsize_t>& dims,
                           Options... options)
    {
        return root_group_.require_dataset<T>(name,dims,options...);
    }

    /** \brief Returns the requested scalar dataset, if it already exists, otherwise a new scalar dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has terminated.
     *
     *  If the scalar dataset already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *  \param datatype value type of the new dataset
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be fulfilled.
     *
     *  \return the requested scalar dataset, if it is already existing, or a new scalar dataset otherwise
     */
    scalar_dataset require_scalar_dataset(const std::string& name, const type& datatype);

    /** \brief Returns the requested scalar dataset, if it already exists, otherwise the scalar dataset is created.
     *
     *  The new dataset is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has terminated.
     *
     *  If the scalar dataset already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be fulfilled.
     *
     *  \return the requested scalar dataset, if it is already existing, or a new scalar dataset otherwise
     */
    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name)
    {
        return root_group_.require_scalar_dataset<T>(name);
    }

    /** \brief Returns the requested scalar dataset, if it already exists, otherwise a new scalar dataset is created.
     *
     *  The new dataset is created using the given parameters and is initialized with the given value.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has terminated.
     *
     *  If the scalar dataset already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested dataset
     *  \param value value, which should be used to intialize the dataset
     *
     *  \tparam T C++ type, which should be used to determine the dataset's value type
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be fulfilled.
     *
     *  \return the requested scalar dataset, if it is already existing, or a new scalar dataset otherwise
     */
    template<typename T>
    scalar_dataset require_scalar_dataset(const std::string& name,const T& value)
    {
        return root_group_.require_scalar_dataset(name,value);
    }

    /** \brief Iterates over every object within the root group.
     *
     *  \param op function, which is applied to every object
     *
     */
    void iterate(const std::function<void(const object&)>& op);

    /** \brief A HDF5 object reference to the root group.
     */
    object_reference ref()const;

    //general interface

    /** \brief The ID, which corresponds to the underlying HDF5 object.
     */
    hid_t id() const noexcept;
private:
    hdf5::file file_wrapper_;
    group root_group_;
public:
    /** \brief The attributes, which are attached to the root group.
     */
    attribute_repository<group> attributes;
};

}

#endif
