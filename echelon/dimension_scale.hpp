#ifndef ECHELON_DIMENSION_SCALE_HPP
#define ECHELON_DIMENSION_SCALE_HPP

#include <echelon/hdf5/dimension_scale.hpp>
#include <echelon/data_transfer_broker.hpp>
/*FIXME: dimension_scale should be based directly on dataset,
 *       but we need to eliminate the cyclic dependency first
 */
#include <echelon/slice.hpp> //for shape_adl

#include <string>

namespace echelon
{

class dataset;
class type;

class dimension_scale
{
public:
    dimension_scale(const dataset& associated_dataset,
                    const std::string& dataset_name,
                    const type& datatype,
                    const std::vector<hsize_t>& extent,
                    const std::string& scale_name);

    std::vector<hsize_t> shape()const;

    template<typename T>
    friend void operator<<=(dimension_scale& sink,const T& source)
    {
        auto current_shape = detail::shape_adl(source);

        std::vector<hsize_t> mem_shape(begin(current_shape), end(current_shape));

        hdf5::dataspace mem_space(mem_shape);
        hdf5::dataspace file_space = sink.dim_scale_.get_space();
        hdf5::type datatype = sink.dim_scale_.datatype();

        ::echelon::write(sink.dim_scale_,datatype,mem_space,file_space,source);
    }

    template<typename T>
    friend void operator<<=(T& sink,const dimension_scale& source)
    {
        std::vector<hsize_t> file_shape = source.shape();

        hdf5::dataspace mem_space(file_shape);
        hdf5::dataspace file_space = source.dim_scale_.get_space();
        hdf5::type datatype = source.dim_scale_.datatype();

        ::echelon::read(source.dim_scale_,datatype,mem_space,file_space,sink);
    }

    hdf5::dimension_scale& get_native_handle();
private:
    hdf5::dimension_scale dim_scale_;
};

}

#endif
