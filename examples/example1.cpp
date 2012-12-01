//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/echelon.hpp>

#include <complex>

#include <iostream>

using namespace echelon;

void print_exception(const std::exception& e, int level =  0)
{
    std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
    try
    {
        std::rethrow_if_nested(e);
    }
    catch(const std::exception& e)
    {
        print_exception(e, level+1);
    }
    catch(...)
    {}
}

int main()
{
    try
    {

    {
        file my_file("test.hdf5", file::create_mode::truncate);

        auto root = my_file.root_group();

        auto foo = root.create_group("test").create_group("foo");

        auto ds = root.create_group("bar").create_dataset<double>("my_data",{ 10, 10 });

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

        ds <<= arr;
        arr <<= ds;

        auto version = ds.attributes.create<unsigned int>("version");
        auto desc = ds.attributes.create<std::string>("desc");
        version <<= 2;
        desc <<= "this is an attribute";

        std::string desc_copy;
        desc_copy <<= desc;
        std::cout << desc_copy << std::endl;

        auto time = foo.attributes.create<double>("time");
        time <<= 1.0;

        auto ds3 = root.create_group("strings").
                        create_dataset<std::string>("my_first_strings", { 2 } );
        multi_array<std::string> sa({ 2 });
        sa(0) = "Hello";
        sa(1) = "World";

        ds3 <<= sa;

        auto ds4 = root.create_group("references").
                        create_dataset<object_reference>("refs", { 2 } );
        object_reference ref1 = ds.ref();
        object_reference ref2 = foo.ref();

        std::vector<object_reference> rr = { ref1 , ref2 };

        ds4 <<= rr;

        auto ref_attr = ds.attributes.create<object_reference>("self");
        ref_attr <<= ref1;

        std::vector<double> v2(10,10);
        ds.slice(2,_) <<= v2;

        std::vector<double> v3(4,9);
        ds.slice(3,range(1,5)) <<= v3;

        std::vector<double> v4(2,8);
        ds.slice(0,range(1,5,2)) <<= v4;

        auto my_scalar = root.create_scalar_dataset<std::string>("my_scalar");
        my_scalar <<= "MyScalar";

        std::complex<double> c(1.0,2.0);
        root.create_scalar_dataset("complex_number",c);

        root.attributes.create("sliteral","test");
    }

    {
        file my_file("test.hdf5", file::open_mode::read_only);

        auto root = my_file.root_group();

        group bar = root["bar"];
        dataset ds = bar["my_data"];

        multi_array<double> arr;
        arr <<= ds;

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

        unsigned int version_;
        version_ <<= version;
        std::string desc_;
        desc_ <<= desc;

        std::cout << version_ << " " << desc_ << std::endl;

        group references = root["references"];
        dataset ds4 = references["refs"];

        std::vector<object_reference> refs;
        refs <<= ds4;

        dataset ds2 = *refs[0];

        multi_array<double> ds2_;
        ds2_ <<= ds2;

        std::cout << ds2_(2, 4) << std::endl;

        group strings = root["strings"];
        dataset ds3 = strings["my_first_strings"];

        std::vector<std::string> sa;
        sa <<= ds3;
        std::cout << sa[0] << std::endl;

        scalar_dataset my_scalar = root["my_scalar"];
        std::string MyScalar;
        MyScalar <<= my_scalar;

        std::cout << MyScalar << std::endl;

        root["foo"];
    }

    }

    catch(const std::exception& e)
    {
        print_exception(e);
    }
}
