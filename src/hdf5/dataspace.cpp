#include <echelon/hdf5/dataspace.hpp>

#include <utility>
#include <algorithm>
#include <functional>

namespace echelon
{
namespace hdf5
{

hyperslab_block::hyperslab_block(std::vector<hsize_t> start_,
                                 std::vector<hsize_t> opposite_)
:start_(start_),dims_(opposite_.size())
{
    std::transform(begin(opposite_),end(opposite_),
                   begin(start_),begin(dims_),
                   std::minus<hsize_t>());
}

const std::vector<hsize_t>& hyperslab_block::start()const
{
    return start_;
}

const std::vector<hsize_t>& hyperslab_block::dims()const
{
    return dims_;
}

dataspace::dataspace()
: dataspace_id_(H5Screate(H5S_SCALAR))
{
}

dataspace::dataspace(hid_t dataspace_id_)
: dataspace_id_(dataspace_id_)
{
    H5Iinc_ref(dataspace_id_);
}

dataspace::dataspace(const std::vector<hsize_t>& dims)
: dataspace_id_(H5Screate_simple(dims.size(), dims.data(), 0))
{
}

dataspace::dataspace(const std::vector<hsize_t>& dims,
                     const std::vector<hsize_t>& max_dims)
: dataspace_id_(H5Screate_simple(dims.size(), dims.data(), max_dims.data()))
{
}

dataspace::~dataspace()
{
    if (id() > -1)
        H5Sclose(id());
}

dataspace::dataspace(const dataspace& other)
:dataspace_id_(other.dataspace_id_)
{
    H5Iinc_ref(dataspace_id_);
}

dataspace::dataspace(dataspace&& other)
:dataspace_id_(other.dataspace_id_)
{
    other.dataspace_id_ = -1;
}

dataspace& dataspace::operator=(const dataspace& other)
{
    using std::swap;

    dataspace temp(other);
    swap(*this,temp);

    return *this;
}

dataspace& dataspace::operator=(dataspace&& other)
{
    using std::swap;

    swap(dataspace_id_,other.dataspace_id_);

    return *this;
}

hssize_t dataspace::select_npoints()const
{
    return H5Sget_select_npoints(id());
}

void dataspace::select_elements(H5S_seloper_t op, std::size_t num_elements,
                                const std::vector<hsize_t>& coord)
{
    H5Sselect_elements(id(), op, num_elements, coord.data());
}

void dataspace::select_hyperslab(H5S_seloper_t op,
                                 const std::vector<hsize_t>& start,
                                 const std::vector<hsize_t>& stride,
                                 const std::vector<hsize_t>& count,
                                 const std::vector<hsize_t>& block)
{
    H5Sselect_hyperslab(id(), op, start.data(), stride.data(), count.data(),
                        block.data());
}

void dataspace::select_hyperslab(H5S_seloper_t op,
                                 const std::vector<hsize_t>& start,
                                 const std::vector<hsize_t>& stride,
                                 const std::vector<hsize_t>& count)
{
    H5Sselect_hyperslab(id(), op, start.data(), stride.data(), count.data(),0);
}

void dataspace::select_hyperslab(H5S_seloper_t op,
                                 const std::vector<hsize_t>& start,
                                 const std::vector<hsize_t>& count)
{
    H5Sselect_hyperslab(id(), op, start.data(), 0, count.data(), 0);
}

hssize_t dataspace::get_select_hyperslab_nblocks()const
{
    return H5Sget_select_hyper_nblocks(id());
}

std::vector<hyperslab_block> dataspace::get_select_hyperslab_blocks()const
{
    int rank = H5Sget_simple_extent_ndims(id());
    hssize_t num_of_blocks = get_select_hyperslab_nblocks();

    std::vector<hsize_t> buffer(2*num_of_blocks*rank);

    H5Sget_select_hyper_blocklist(id(),0,num_of_blocks,buffer.data());

    std::vector<hyperslab_block> blocks;

    for(auto iter = begin(buffer); iter != end(buffer); iter += 6)
    {
        std::vector<hsize_t> start(iter,iter + 3);
        std::vector<hsize_t> opposite(iter + 3,iter + 6);

        blocks.push_back(hyperslab_block(start,opposite));
    }

    return blocks;
}

std::vector<hsize_t> dataspace::get_simple_extent_dims() const
{
    std::vector<hsize_t> dims(H5Sget_simple_extent_ndims(id()));

    H5Sget_simple_extent_dims(id(), dims.data(), 0);

    return dims;
}

hid_t dataspace::id() const
{
    return dataspace_id_;
}

}
}
