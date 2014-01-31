//  Copyright (c) 2013-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_HDF5_LINK_HPP
#define ECHELON_HDF5_LINK_HPP

#include <echelon/hdf5/object.hpp>
#include <string>

namespace echelon
{
namespace hdf5
{
/** \brief A link (directed edge) between two HDF5 objects.
 */
class link
{
public:
    link(object origin_, std::string name_);

    /** \brief Returns the destination of the link.
     *
     *  The function will open the corresponding object if necessary
     *  and will perform any other operation, which is associated with the link.
     *
     *  \return the destination of the link.
     */
    object destination() const;

    /** \brief The object, which acts as the origin of the link.
     */
    object origin() const;

    /** \brief The name of the link.
     */
    const std::string& name() const;

private:
    object origin_;
    std::string name_;
};
}
}

#endif
