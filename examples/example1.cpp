#include <echelon/file.hpp>
#include <echelon/group.hpp>
#include <echelon/dataset.hpp>
#include <echelon/scalar_dataset.hpp>
#include <echelon/attribute.hpp>

#include <echelon/multi_array_adaptor.hpp>
#include <echelon/multi_array.hpp>

#include <echelon/object_reference.hpp>

#include <iostream>

using namespace echelon;

int main()
{
    {
        file my_file("test.hdf5", file::create_mode::truncate);

        auto root = my_file.root_group();

        auto foo = root.add_group("test").add_group("foo");

        auto ds = root.add_group("bar").add_dataset<double>("my_data",{ 10, 10 });

        multi_array<double> arr({ 10 , 10 });

        std::vector<float> v(100);

        for(std::size_t i = 0;i < 10;++i)
        {
            for(std::size_t j = 0;j < 10;++j)
            {
                arr(i,j) = i + j;
                v[i*10 + j] = i;
            }
        }

        ds = arr;
        arr = ds;

        auto version = ds.attributes.add<unsigned int>("version");
        auto desc = ds.attributes.add<std::string>("desc");
        version = 2;
        desc = "this is an attribute";

        std::string desc_copy = desc;
        std::cout << desc_copy << std::endl;

        auto ds2 = foo.add_dataset<float>("my_data",{ 10 , 10 });

        ds2 = multi_array_adaptor<std::vector<float>>(v,{ 10 , 10 });
        multi_array_adaptor<std::vector<float>>(v,{ 10 , 10 }) = ds2;

        auto time = foo.attributes.add<double>("time");
        time = 1.0;

        auto ds3 = root.add_group("strings").
                        add_dataset<std::string>("my_first_strings", { 2 } );
        multi_array<std::string> sa({ 2 });
        sa(0) = "Hello";
        sa(1) = "World";

        ds3 = sa;

        auto ds4 = root.add_group("references").
                        add_dataset<object_reference>("refs", { 2 } );
        object_reference ref1 = ds2.ref();
        object_reference ref2 = foo.ref();

        std::vector<object_reference> rr = { ref1 , ref2 };

        ds4 = rr;

        auto ref_attr = ds2.attributes.add<object_reference>("self");
        ref_attr = ref1;

        std::vector<float> v2(10,10);
        ds2.slice(2,_) = v2;

        auto my_scalar = root.add_scalar_dataset<std::string>("my_scalar");
        my_scalar = "MyScalar";
    }

    {
        file my_file("test.hdf5", file::open_mode::read_only);

        auto root = my_file.root_group();

        group bar = root["bar"];
        dataset ds = bar["my_data"];

        multi_array<double> arr = ds;

        for (std::size_t i = 0; i < 10; ++i)
        {
            for (std::size_t j = 0; j < 10; ++j)
            {
                std::cout << arr(i, j) << " ";
            }

            std::cout << '\n';
        }

        attribute version = ds.attributes["version"];
        attribute desc = ds.attributes["desc"];

        unsigned int version_ = version;
        std::string desc_ = desc;

        std::cout << version_ << " " << desc_ << std::endl;

        group references = root["references"];
        dataset ds4 = references["refs"];

        std::vector<object_reference> refs = ds4;

        dataset ds2 = *refs[0];

        multi_array<float> ds2_ = ds2;

        std::cout << ds2_(2, 4) << std::endl;

        group strings = root["strings"];
        dataset ds3 = strings["my_first_strings"];

        std::vector<std::string> sa = ds3;
        std::cout << sa[0] << std::endl;

        scalar_dataset my_scalar = root["my_scalar"];
        std::string MyScalar = my_scalar;

        std::cout << MyScalar << std::endl;
    }
}
