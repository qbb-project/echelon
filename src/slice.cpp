#include <echelon/slice.hpp>


namespace echelon
{

slice::slice(hdf5::dataset sliced_dataset_,
             const std::vector<std::tuple<hsize_t,hsize_t>>& boundaries)
:sliced_dataset_(sliced_dataset_),selected_dataspace_(sliced_dataset_.get_space())
{
    std::vector<hsize_t> offset;
    std::vector<hsize_t> count;

    for(auto bounds : boundaries)
    {
        assert(std::get<1>(bounds) > std::get<0>(bounds));

        const hsize_t size = std::get<1>(bounds) - std::get<0>(bounds);

        offset.push_back(std::get<0>(bounds));
        count.push_back(size);
    }

    selected_dataspace_.select_hyperslab(H5S_SELECT_SET,offset,count);
}

}
