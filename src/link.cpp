#include <echelon/link.hpp>
#include <utility>
namespace echelon
{

link::link(object origin_,std::string name_)
:origin_{std::move(origin_)},name_{std::move(name_)}
{
}

object link::destination()const
{
    return object(hdf5::object(origin_.id(),name()));
}

object link::origin()const
{
    return origin_;
}

const std::string& link::name()const
{
    return name_;
}

}
