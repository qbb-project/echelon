//  Copyright (c) 2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_SINK_ADAPTORS_HPP
#define ECHELON_SINK_ADAPTORS_HPP

#include <echelon/hdf5/container_adaption.hpp>

namespace echelon
{

template <typename ResizeableSink>
class auto_reshaper
{
public:
    explicit auto_reshaper(ResizeableSink& underlying_sink_) : underlying_sink_{&underlying_sink_}
    {
    }

    template <typename Source>
    friend void operator<<=(auto_reshaper<ResizeableSink> sink, const Source& source)
    {
        auto shape = source.shape();
        
        std::vector<std::size_t> shape_;
        shape_.reserve(shape.size());
        
        for(auto value : shape)
        {
            shape_.push_back(value);
        }
        
        hdf5::reshape_adl(*sink.underlying_sink_, shape_);

        *sink.underlying_sink_ <<= source;
    }

private:
    ResizeableSink* underlying_sink_;
};

/** \brief Add an adaptor to the sink which automatically reshapes the sink if needed.
 *
 *  Example:
 * 
 *  \code{.cpp}
 *  
 *  std::vector<double> my_array;
 * 
 *  echelon::auto_reshape(my_array) <<= my_dataset;
 * 
 *  \endcode
 * 
 *  \tparam ResizeableSink type of the sink
 *
 *  \param sink the adapted sink
 * 
 *  \returns the corresponding sink adaptor.

 */
template <typename ResizeableSink>
auto_reshaper<ResizeableSink> auto_reshape(ResizeableSink& sink)
{
    return auto_reshaper<ResizeableSink>(sink);
}
}

#endif