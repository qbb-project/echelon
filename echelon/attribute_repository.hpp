//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_ATTRIBUTE_REPOSITORY_HPP
#define ECHELON_ATTRIBUTE_REPOSITORY_HPP

#include <echelon/type.hpp>
#include <echelon/type_factory.hpp>
#include <echelon/attribute.hpp>

#include <echelon/broken_contract_exception.hpp>

#include <memory>
#include <map>
#include <string>
#include <exception>

namespace echelon
{

class non_existing_attribute_exception : public std::exception
{
public:
    non_existing_attribute_exception(std::string what_)
    : what_(std::move(what_))
    {
    }

    ~non_existing_attribute_exception() noexcept
    {
    }

    const char* what() const noexcept override
    {
        return what_.c_str();
    }

private:
    std::string what_;
};

/** \brief Attribute manager, which should be embedded into a parent object,
 *         which supports attributes.
 *
 * \tparam Parent Type of the object into which the attribute repository is
 *                embedded.
 */
template <typename Parent>
class attribute_repository
{
public:
    explicit attribute_repository(const Parent& parent_) : parent_(&parent_)
    {
    }

    /** \brief Creates a new attribute.
     *
     *  \param name name of the new attribute
     *  \param datatype value type of the new attribute
     *
     *  \return a handle to the new attribute
     */
    attribute create(const std::string& name, const type& datatype)
    {
        return attribute(object(*parent_), name, datatype);
    }

    /** \brief Creates a new attribute.
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \param name name of the new attribute
     *
     *  \return a handle to the new attribute
     */
    template <typename T>
    attribute create(const std::string& name)
    {
        return create(name, get_hdf5_type<T>());
    }

    /** \brief Creates a new attribute and initializes it with a given value.
     *
     *  \tparam T C++ type, which should be used to determine the dataset's
     *            value type
     *
     *  \param name name of the new attribute
     *  \param value value, which is used to initialize the attribute
     *
     *  \return a handle to the new attribute
     */
    template <typename T>
    attribute create(const std::string& name, const T& value)
    {
        attribute attr = create<T>(name);

        attr <<= value;

        return attr;
    }

    /** \brief Accessor function for this attribute repository.
     *
     *  \param name name of the requested attribute
     *
     *  \return a handle to the requested attribute
     */
    attribute operator[](const std::string& name) const
    {
        return attribute(object(*parent_), name);
    }

    /** \brief Tests, if an attribute exists.
     *
     *  \param name name of the requested attribute
     *
     *  \return true, if the requested attribute exists, false otherwise
     */
    bool exists(const std::string& name) const
    {
        return hdf5::is_attribute_existing(hdf5::object(parent_->id()), name);
    }

    /** \brief Returns the requested attribute, if it already exists, otherwise
     *         a new attribute is created.
     *
     *  The new attribute is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the attribute already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested attribute
     *  \param datatype value type of the new attribute
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested attribute, if it is already existing, or a new
     *          attribute otherwise
     */
    attribute require(const std::string& name, const type& datatype)
    {
        if (exists(name))
        {
            attribute attr(object(*parent_), name);

            if (attr.datatype() != datatype)
                throw broken_contract_exception(
                    "The required datatype doesn't "
                    "match the datatype of the attribute.");

            return attr;
        }
        else
        {
            return create(name, datatype);
        }
    }

    /** \brief Returns the requested attribute, if it already exists, otherwise
     *         the attribute is created.
     *
     *  The new attribute is created using the given parameters.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the attribute already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested attribute
     *
     *  \tparam T C++ type, which should be used to determine the attribute's
     *            value type
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested attribute, if it is already existing, or a new
     *          attribute otherwise
     */
    template <typename T>
    attribute require(const std::string& name)
    {
        return require(name, get_hdf5_type<T>());
    }

    /** \brief Returns the requested attribute, if it already exists, otherwise
     *         a new attribute is created.
     *
     *  The new attribute is created using the given parameters and is
     *  initialized with the given value.
     *
     *  This method allows the user to make a contract with the library,
     *  that an object with certain properties exists after the method has
     *  terminated.
     *
     *  If the attribute already exists and its datatype differs
     *  from its requested value and an exception is thrown,
     *  since the contract can't be fulfilled.
     *
     *  \param name name of the requested attribute
     *  \param value value, which should be used to intialize the attribute
     *
     *  \tparam T C++ type, which should be used to determine the attribute's
     *            value type
     *
     *  \throws broken_contract_exception is thrown, if the contract can't be
     *                                    fulfilled.
     *
     *  \return the requested attribute, if it is already existing, or a new
     *          attribute otherwise
     */
    template <typename T>
    attribute require(const std::string& name, const T& value)
    {
        type datatype = get_hdf5_type<T>();

        if (exists(name))
        {
            attribute attr(object(*parent_), name);

            if (attr.datatype() != datatype)
                throw broken_contract_exception(
                    "The required datatype doesn't "
                    "match the datatype of the attribute.");

            return attr;
        }
        else
        {
            attribute attr = create(name, datatype);

            attr <<= value;

            return attr;
        }
    }

private:
    const Parent* parent_;
};
}

#endif
