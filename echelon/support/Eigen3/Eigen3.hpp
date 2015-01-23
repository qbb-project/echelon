//  Copyright (c) 2015 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_EIGEN3_HPP
#define ECHELON_EIGEN3_HPP

#include <echelon/hdf5/container_adaption.hpp>

#include <Eigen/Dense>
#include <cassert>

namespace echelon
{
namespace hdf5
{

template <typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
inline std::vector<std::size_t>
shape(const Eigen::Matrix<Scalar, Rows, Cols, Options, MaxRows, MaxCols>& container, adl_enabler)
{
    return {static_cast<std::size_t>(container.rows()), static_cast<std::size_t>(container.cols())};
}

template <typename Scalar, int Options, int MaxRows, int MaxCols>
inline void reshape(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Options, MaxRows, MaxCols>& container,
                    const std::vector<std::size_t>& new_shape, adl_enabler)
{
    assert(new_shape.size() == 2);

    container.resize(new_shape[0], new_shape[1]);
}

template <typename Scalar, int Cols, int Options, int MaxRows, int MaxCols>
inline void reshape(Eigen::Matrix<Scalar, Eigen::Dynamic, Cols, Options, MaxRows, MaxCols>& container,
                    const std::vector<std::size_t>& new_shape, adl_enabler)
{
    assert(new_shape.size() == 1);

    container.resize(new_shape[0], Eigen::NoChange);
}

template <typename Scalar, int Rows, int Options, int MaxRows, int MaxCols>
inline void reshape(Eigen::Matrix<Scalar, Rows, Eigen::Dynamic, Options, MaxRows, MaxCols>& container,
                    const std::vector<std::size_t>& new_shape, adl_enabler)
{
    assert(new_shape.size() == 1);

    container.resize(Eigen::NoChange, new_shape[0]);
}

}
}

#endif
