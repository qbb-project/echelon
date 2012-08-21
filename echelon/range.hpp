#ifndef ECHELON_RANGE_HPP
#define ECHELON_RANGE_HPP

#include <hdf5.h>

namespace echelon
{

class range
{
public:
    range(hsize_t base_,hsize_t bound_,hsize_t stride_ = 1)
    :base_(base_),bound_(bound_),stride_(stride_)
    {}

    hsize_t base()const
    {
        return base_;
    }

    hsize_t bound()const
    {
        return bound_;
    }

    hsize_t stride()const
    {
        return stride_;
    }
private:
    hsize_t base_;
    hsize_t bound_;
    hsize_t stride_;
};

}

#endif
