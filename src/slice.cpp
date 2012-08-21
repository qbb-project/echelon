#include <echelon/slice.hpp>


namespace echelon
{

slice::slice(hdf5::dataset sliced_dataset_,
             const std::vector<range>& ranges)
:sliced_dataset_(sliced_dataset_),selected_dataspace_(sliced_dataset_.get_space())
{
    std::vector<hsize_t> offset;
    std::vector<hsize_t> count;
    std::vector<hsize_t> stride;

    for(auto r : ranges)
    {
        assert(r.bound() > r.base());

        const hsize_t size = r.bound() - r.base();

        offset.push_back(r.base());
        count.push_back(size/r.stride());
        stride.push_back(r.stride());
    }

    selected_dataspace_.select_hyperslab(H5S_SELECT_SET,offset,stride,count);
}

}
