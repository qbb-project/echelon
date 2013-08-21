#ifndef ECHELON_GRID_HPP
#define ECHELON_GRID_HPP

#include <echelon/multi_array.hpp>
#include <echelon/dataset.hpp>
#include <echelon/not_implemented_exception.hpp>
#include <echelon/container_adaption.hpp>

#include <vector>
#include <string>
#include <utility>

namespace echelon
{

/** \brief Multidimensional grid.
 *
 *  \tparam DimensionScaleValueType value type of the dimension scales
 *  \tparam DataValueType value type of the grid
 * 
 *  \note This class is experimental and currently not fully implemented and documented. Use it with care.
 */
template<typename DimensionScaleValueType,typename DataValueType = DimensionScaleValueType>
class grid
{
public:
    class dimension_scale
    {
    public:
        dimension_scale(std::string name,
                        std::vector<DimensionScaleValueType> values)
        :name{std::move(name)},values(std::move(values))
        {
        }
        
        std::string name;
        std::vector<DimensionScaleValueType> values;
    };
    
    using dimension_scale_type = dimension_scale;
        
    explicit grid(const std::vector<dimension_scale_type>& dimension_scales_,
                  const DataValueType& value_ = DataValueType())
    :dimension_scales_(dimension_scales_),
     data_(get_sizes_from_scales(dimension_scales_),
           value_)
    {
    }
    
    const std::vector<dimension_scale_type>& dimension_scales()const
    {
        return dimension_scales_;
    }
    
    std::vector<dimension_scale_type>& dimension_scales()
    {
        return dimension_scales_;
    }
    
    const multi_array<DataValueType>& values()const
    {
        return data_;
    }
    
    multi_array<DataValueType>& values()
    {
        return data_;
    }
    
    const DataValueType* data()const
    {
        return data_.data();
    }
    
    DataValueType* data()
    {
        return data_.data();
    }
    
    /** \brief Reshapes the grid.
     *
     *  \param new_shape new shape of the grid
     */
    void reshape(const std::vector<std::size_t>& new_shape)
    {
        for(std::size_t i = 0;i < new_shape.size();++i)
        {
            dimension_scales_[i].values.resize(new_shape[i]);
        }
        
        data_.reshape(new_shape);
    }
private:
    static std::vector<std::size_t> get_sizes_from_scales(const std::vector<dimension_scale_type>& scales)
    {
        std::vector<std::size_t> sizes;
        
        for(const auto& scale : scales)
        {
            sizes.push_back(scale.values.size());
        }
        
        return sizes;
    }
    
    std::vector<dimension_scale_type> dimension_scales_; 
    multi_array<DataValueType> data_;
};

/** \brief Writes the content of a data source into the dataset.
*
*  The shape of the data source must match the shape of the dataset.
*
*  \param sink  the dataset, which is used as a sink
*  \param source the data source
*/
template<typename DimensionScaleValueType,typename DataValueType>
inline void operator<<=(dataset& sink,
                        const grid<DimensionScaleValueType,DataValueType>& source)
{
    std::size_t count = 0;
    
    for(const auto& scale : source.dimension_scales())
    {
        sink.dimensions[count].relabel(scale.name);
        auto ds = sink.dimensions[count].attach_dimension_scale<DimensionScaleValueType>(scale.name);
        ds <<= scale.values;
        ++count;
    }
    
    sink <<= source.values();
}

/** \brief Reads the content of the dataset into a data sink.
*
*  \param sink the data sink
*  \param source the dataset, which is used as a source
*/
template<typename DimensionScaleValueType,typename DataValueType>
inline void operator<<=(grid<DimensionScaleValueType,DataValueType>& sink,const dataset& source)
{
    throw not_implemented_exception("Reading grids from HDF5 files is currently not implemented.");
    
    for(const auto& dimension : source.dimensions)
    {
        
    }
    
    sink.values() <<= source;
}
}

#endif