//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/echelon.hpp>

#include <echelon/hdf5/precursor/error_handling.hpp>

#include <complex>

#include <iostream>

using namespace echelon;

void print_exception(const std::exception& e, int level = 0)
{
    std::cerr << std::string(level, ' ') << "exception: " << e.what() << '\n';
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (const std::exception& e)
    {
        print_exception(e, level + 1);
    }
    catch (...)
    {
    }
}

const char* translate_minor_error_code(hid_t minor_num)
{

    if (minor_num == H5E_SEEKERROR)
        return "H5E_SEEKERROR";
    else if (minor_num == H5E_READERROR)
        return "H5E_READERROR";
    else if (minor_num == H5E_WRITEERROR)
        return "H5E_WRITEERROR";
    else if (minor_num == H5E_CLOSEERROR)
        return "H5E_CLOSEERROR";
    else if (minor_num == H5E_OVERFLOW)
        return "H5E_OVERFLOW";
    else if (minor_num == H5E_FCNTL)
        return "H5E_FCNTL";
    else if (minor_num == H5E_NOSPACE)
        return "H5E_NOSPACE";
    else if (minor_num == H5E_CANTALLOC)
        return "H5E_CANTALLOC";
    else if (minor_num == H5E_CANTCOPY)
        return "H5E_CANTCOPY";
    else if (minor_num == H5E_CANTFREE)
        return "H5E_CANTFREE";
    else if (minor_num == H5E_ALREADYEXISTS)
        return "H5E_ALREADYEXISTS";
    else if (minor_num == H5E_CANTLOCK)
        return "H5E_CANTLOCK";
    else if (minor_num == H5E_CANTUNLOCK)
        return "H5E_CANTUNLOCK";
    else if (minor_num == H5E_CANTGC)
        return "H5E_CANTGC";
    else if (minor_num == H5E_CANTGETSIZE)
        return "H5E_CANTGETSIZE";
    else if (minor_num == H5E_OBJOPEN)
        return "H5E_OBJOPEN";
    else if (minor_num == H5E_CANTRESTORE)
        return "H5E_CANTRESTORE";
    else if (minor_num == H5E_CANTCOMPUTE)
        return "H5E_CANTCOMPUTE";
    else if (minor_num == H5E_CANTEXTEND)
        return "H5E_CANTEXTEND";
    else if (minor_num == H5E_CANTATTACH)
        return "H5E_CANTATTACH";
    else if (minor_num == H5E_CANTUPDATE)
        return "H5E_CANTUPDATE";
    else if (minor_num == H5E_CANTOPERATE)
        return "H5E_CANTOPERATE";
    else if (minor_num == H5E_CANTINIT)
        return "H5E_CANTINIT";
    else if (minor_num == H5E_ALREADYINIT)
        return "H5E_ALREADYINIT";
    else if (minor_num == H5E_CANTRELEASE)
        return "H5E_CANTRELEASE";
    else if (minor_num == H5E_CANTGET)
        return "H5E_CANTGET";
    else if (minor_num == H5E_CANTSET)
        return "H5E_CANTSET";
    else if (minor_num == H5E_DUPCLASS)
        return "H5E_DUPCLASS";
    else if (minor_num == H5E_SETDISALLOWED)
        return "H5E_SETDISALLOWED";
    else if (minor_num == H5E_CANTMERGE)
        return "H5E_CANTMERGE";
    else if (minor_num == H5E_CANTREVIVE)
        return "H5E_CANTREVIVE";
    else if (minor_num == H5E_CANTSHRINK)
        return "H5E_CANTSHRINK";
    else if (minor_num == H5E_LINKCOUNT)
        return "H5E_LINKCOUNT";
    else if (minor_num == H5E_VERSION)
        return "H5E_VERSION";
    else if (minor_num == H5E_ALIGNMENT)
        return "H5E_ALIGNMENT";
    else if (minor_num == H5E_BADMESG)
        return "H5E_BADMESG";
    else if (minor_num == H5E_CANTDELETE)
        return "H5E_CANTDELETE";
    else if (minor_num == H5E_BADITER)
        return "H5E_BADITER";
    else if (minor_num == H5E_CANTPACK)
        return "H5E_CANTPACK";
    else if (minor_num == H5E_CANTRESET)
        return "H5E_CANTRESET";
    else if (minor_num == H5E_CANTRENAME)
        return "H5E_CANTRENAME";
    else if (minor_num == H5E_SYSERRSTR)
        return "H5E_SYSERRSTR";
    else if (minor_num == H5E_NOFILTER)
        return "H5E_NOFILTER";
    else if (minor_num == H5E_CALLBACK)
        return "H5E_CALLBACK";
    else if (minor_num == H5E_CANAPPLY)
        return "H5E_CANAPPLY";
    else if (minor_num == H5E_SETLOCAL)
        return "H5E_SETLOCAL";
    else if (minor_num == H5E_NOENCODER)
        return "H5E_NOENCODER";
    else if (minor_num == H5E_CANTFILTER)
        return "H5E_CANTFILTER";
    else if (minor_num == H5E_CANTOPENOBJ)
        return "H5E_CANTOPENOBJ";
    else if (minor_num == H5E_CANTCLOSEOBJ)
        return "H5E_CANTCLOSEOBJ";
    else if (minor_num == H5E_COMPLEN)
        return "H5E_COMPLEN";
    else if (minor_num == H5E_PATH)
        return "H5E_PATH";
    else if (minor_num == H5E_NONE_MINOR)
        return "H5E_NONE_MINOR";
    // else if(minor_num ==  H5E_OPENERROR)
    //    return "H5E_OPENERROR"; //only in newer HDF5 versions
    else if (minor_num == H5E_FILEEXISTS)
        return "H5E_FILEEXISTS";
    else if (minor_num == H5E_FILEOPEN)
        return "H5E_FILEOPEN";
    else if (minor_num == H5E_CANTCREATE)
        return "H5E_CANTCREATE";
    else if (minor_num == H5E_CANTOPENFILE)
        return "H5E_CANTOPENFILE";
    else if (minor_num == H5E_CANTCLOSEFILE)
        return "H5E_CANTCLOSEFILE";
    else if (minor_num == H5E_NOTHDF5)
        return "H5E_NOTHDF5";
    else if (minor_num == H5E_BADFILE)
        return "H5E_BADFILE";
    else if (minor_num == H5E_TRUNCATED)
        return "H5E_TRUNCATED";
    else if (minor_num == H5E_MOUNT)
        return "H5E_MOUNT";
    else if (minor_num == H5E_BADATOM)
        return "H5E_BADATOM";
    else if (minor_num == H5E_BADGROUP)
        return "H5E_BADGROUP";
    else if (minor_num == H5E_CANTREGISTER)
        return "H5E_CANTREGISTER";
    else if (minor_num == H5E_CANTINC)
        return "H5E_CANTINC";
    else if (minor_num == H5E_CANTDEC)
        return "H5E_CANTDEC";
    else if (minor_num == H5E_NOIDS)
        return "H5E_NOIDS";
    else if (minor_num == H5E_CANTFLUSH)
        return "H5E_CANTFLUSH";
    else if (minor_num == H5E_CANTSERIALIZE)
        return "H5E_CANTSERIALIZE";
    else if (minor_num == H5E_CANTLOAD)
        return "H5E_CANTLOAD";
    else if (minor_num == H5E_PROTECT)
        return "H5E_PROTECT";
    else if (minor_num == H5E_NOTCACHED)
        return "H5E_NOTCACHED";
    else if (minor_num == H5E_SYSTEM)
        return "H5E_SYSTEM";
    else if (minor_num == H5E_CANTINS)
        return "H5E_CANTINS";
    else if (minor_num == H5E_CANTPROTECT)
        return "H5E_CANTPROTECT";
    else if (minor_num == H5E_CANTUNPROTECT)
        return "H5E_CANTUNPROTECT";
    else if (minor_num == H5E_CANTPIN)
        return "H5E_CANTPIN";
    else if (minor_num == H5E_CANTUNPIN)
        return "H5E_CANTUNPIN";
    else if (minor_num == H5E_CANTMARKDIRTY)
        return "H5E_CANTMARKDIRTY";
    else if (minor_num == H5E_CANTDIRTY)
        return "H5E_CANTDIRTY";
    else if (minor_num == H5E_CANTEXPUNGE)
        return "H5E_CANTEXPUNGE";
    else if (minor_num == H5E_CANTRESIZE)
        return "H5E_CANTRESIZE";
    else if (minor_num == H5E_TRAVERSE)
        return "H5E_TRAVERSE";
    else if (minor_num == H5E_NLINKS)
        return "H5E_NLINKS";
    else if (minor_num == H5E_NOTREGISTERED)
        return "H5E_NOTREGISTERED";
    else if (minor_num == H5E_CANTMOVE)
        return "H5E_CANTMOVE";
    else if (minor_num == H5E_CANTSORT)
        return "H5E_CANTSORT";
    else if (minor_num == H5E_MPI)
        return "H5E_MPI";
    else if (minor_num == H5E_MPIERRSTR)
        return "H5E_MPIERRSTR";
    else if (minor_num == H5E_CANTRECV)
        return "H5E_CANTRECV";
    else if (minor_num == H5E_CANTCLIP)
        return "H5E_CANTCLIP";
    else if (minor_num == H5E_CANTCOUNT)
        return "H5E_CANTCOUNT";
    else if (minor_num == H5E_CANTSELECT)
        return "H5E_CANTSELECT";
    else if (minor_num == H5E_CANTNEXT)
        return "H5E_CANTNEXT";
    else if (minor_num == H5E_BADSELECT)
        return "H5E_BADSELECT";
    else if (minor_num == H5E_CANTCOMPARE)
        return "H5E_CANTCOMPARE";
    else if (minor_num == H5E_UNINITIALIZED)
        return "H5E_UNINITIALIZED";
    else if (minor_num == H5E_UNSUPPORTED)
        return "H5E_UNSUPPORTED";
    else if (minor_num == H5E_BADTYPE)
        return "H5E_BADTYPE";
    else if (minor_num == H5E_BADRANGE)
        return "H5E_BADRANGE";
    else if (minor_num == H5E_BADVALUE)
        return "H5E_BADVALUE";
    else if (minor_num == H5E_NOTFOUND)
        return "H5E_NOTFOUND";
    else if (minor_num == H5E_EXISTS)
        return "H5E_EXISTS";
    else if (minor_num == H5E_CANTENCODE)
        return "H5E_CANTENCODE";
    else if (minor_num == H5E_CANTDECODE)
        return "H5E_CANTDECODE";
    else if (minor_num == H5E_CANTSPLIT)
        return "H5E_CANTSPLIT";
    else if (minor_num == H5E_CANTREDISTRIBUTE)
        return "H5E_CANTREDISTRIBUTE";
    else if (minor_num == H5E_CANTSWAP)
        return "H5E_CANTSWAP";
    else if (minor_num == H5E_CANTINSERT)
        return "H5E_CANTINSERT";
    else if (minor_num == H5E_CANTLIST)
        return "H5E_CANTLIST";
    else if (minor_num == H5E_CANTMODIFY)
        return "H5E_CANTMODIFY";
    else if (minor_num == H5E_CANTREMOVE)
        return "H5E_CANTREMOVE";
    else if (minor_num == H5E_CANTCONVERT)
        return "H5E_CANTCONVERT";
    else if (minor_num == H5E_BADSIZE)
        return "H5E_BADSIZE";
    else
        return "unkown";
}

void print_exception(const hdf5::precursor::hdf5_error& e, int level = 0)
{
    std::cerr << std::string(level, ' ') << "exception: " << e.what() << "  "
              << translate_minor_error_code(e.minor_num()) << '\n';
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (const hdf5::precursor::hdf5_error& e)
    {
        print_exception(e, level + 1);
    }
    catch (...)
    {
    }
}

int main()
{
    try
    {

        {
            file my_file("test.hdf5", file::create_mode::truncate);

            auto foo = my_file.create_group("test").create_group("foo");

            auto ds = my_file.create_group("bar").create_dataset<double>("my_data", {10, 10});

            multi_array<double> arr({10, 10});

            std::vector<float> v(100);

            for (std::size_t i = 0; i < 10; ++i)
            {
                for (std::size_t j = 0; j < 10; ++j)
                {
                    arr(i, j) = i + j;
                    v[i * 10 + j] = i;
                }
            }

            ds <<= arr;
            arr <<= ds;

            ds.dimensions()[0].relabel("x");

            multi_array<double> x_scale_data({10});

            for (std::size_t i = 0; i < 10; ++i)
            {
                x_scale_data(i) = static_cast<double>(i);
            }

            auto x_scale = ds.dimensions()[0].attach_dimension_scale<double>("x");
            x_scale <<= x_scale_data;

            ds.dimensions()[1].attach_dimension_scale<double>("y");

            auto version = ds.attributes().create<unsigned int>("version");
            auto desc = ds.attributes().create<std::string>("desc");
            version <<= 2;
            desc <<= "this is an attribute";

            std::string desc_copy;
            desc_copy <<= desc;
            std::cout << desc_copy << std::endl;

            auto time = foo.attributes().create<double>("time");
            time <<= 1.0;

            auto ds3 = my_file.create_group("strings")
                           .create_dataset<std::string>("my_first_strings", {2});
            multi_array<std::string> sa({2});
            sa(0) = "Hello";
            sa(1) = "World";

            ds3 <<= sa;

            auto ds4 =
                my_file.create_group("references").create_dataset<object_reference>("refs", {2});
            object_reference ref1 = ds.ref();
            object_reference ref2 = foo.ref();

            std::vector<object_reference> rr = {ref1, ref2};

            ds4 <<= rr;

            auto ref_attr = ds.attributes().create<object_reference>("self");
            ref_attr <<= ref1;

            std::vector<double> v2(10, 10);
            ds(2, _) <<= v2;

            std::vector<double> v3(4, 9);
            ds(3, range(1, 5)) <<= v3;

            std::vector<double> v4(2, 8);
            ds(0, range(1, 5, 2)) <<= v4;

            auto my_scalar = my_file.create_scalar_dataset<std::string>("my_scalar");
            my_scalar <<= "MyScalar";

            std::complex<double> c(1.0, 2.0);
            my_file.create_scalar_dataset("complex_number", c);

            my_file.attributes().create("sliteral", "test");
        }

        /*{
            file my_file("/opt/test.hdf5", file::open_mode::read_only);

            group bar = my_file["bar"];
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

            group references = my_file["references"];
            dataset ds4 = references["refs"];

            std::vector<object_reference> refs;
            refs <<= ds4;

            dataset ds2 = *refs[0];

            multi_array<double> ds2_;
            ds2_ <<= ds2;

            std::cout << ds2_(2, 4) << std::endl;

            group strings = my_file["strings"];
            dataset ds3 = strings["my_first_strings"];

            std::vector<std::string> sa;
            sa <<= ds3;
            std::cout << sa[0] << std::endl;

            scalar_dataset my_scalar = my_file["my_scalar"];
            std::string MyScalar;
            MyScalar <<= my_scalar;

            std::cout << MyScalar << std::endl;

            my_file["foo"];
        }*/
    }
    catch (const hdf5::precursor::hdf5_error& e)
    {
        print_exception(e);
    }
    catch (const std::exception& e)
    {
        print_exception(e);
    }
}
