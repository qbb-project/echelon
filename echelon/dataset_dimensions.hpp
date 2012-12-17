#ifndef ECHELON_DATASET_DIMENSIONS_HPP
#define ECHELON_DATASET_DIMENSIONS_HPP

#include <echelon/type.hpp>
#include <echelon/type_factory.hpp>

#include <hdf5.h> // hsize_t

#include <vector>
#include <string>
#include <cstddef>

namespace echelon
{

class dataset;

class dimension
{
public:
    dimension(dataset& associated_dataset_,std::size_t index_);

    void attach_dimension_scale(const std::string& name,
                                const type& datatype);

    template<typename T>
    void attach_dimension_scale(const std::string& name)
    {
        attach_dimension_scale(name,get_hdf5_type<T>());
    }

    std::string label()const;
    void relabel(const std::string& new_label);

    hsize_t extend()const;
private:
    dataset* associated_dataset_;
    std::size_t index_;
};

class dataset_dimensions
{
public:
    typedef std::vector<dimension>::iterator iterator;
    typedef std::vector<dimension>::const_iterator const_iterator;

    dataset_dimensions(dataset& associated_dataset_,std::size_t rank_);

    dimension& operator[](std::size_t index)
    {
        return dimensions_[index];
    }

    const dimension& operator[](std::size_t index)const
    {
        return dimensions_[index];
    }

    iterator begin()
    {
        return dimensions_.begin();
    }

    iterator end()
    {
        return dimensions_.end();
    }

    const_iterator begin()const
    {
        return dimensions_.begin();
    }

    const_iterator end()const
    {
        return dimensions_.end();
    }
private:
    std::vector<dimension> dimensions_;
};

}

#endif
