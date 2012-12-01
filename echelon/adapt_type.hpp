//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_ADAPT_TYPE_HPP
#define ECHELON_ADAPT_TYPE_HPP

#include <echelon/type.hpp>
#include <echelon/type_traits_fwd.hpp>
#include <echelon/static_type_layout.hpp>

#include <cstddef>
#include <utility>
#include <tuple>

#include <boost/mpl/bool.hpp>

#include <boost/preprocessor/seq/transform.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>

#include <boost/preprocessor/stringize.hpp>

#define ECHELON_TUPLE_TO_VARIADIC_(...) __VA_ARGS__

#define ECHELON_TUPLE_TO_VARIADIC(ARG) ECHELON_TUPLE_TO_VARIADIC_ ARG


#define ECHELON_CREATE_ID_OFFSET_PAIR(d,data,value) \
  id_offset_pair(BOOST_PP_STRINGIZE(value) , offsetof(data,value))

#define ECHELON_PROCESS_MEMBER_LIST(TYPE_NAME,MEMBERS)               \
    { ECHELON_TUPLE_TO_VARIADIC(                                     \
          BOOST_PP_SEQ_TO_TUPLE(                                     \
               BOOST_PP_SEQ_TRANSFORM(ECHELON_CREATE_ID_OFFSET_PAIR, \
                                      TYPE_NAME,MEMBERS))) }

#define ECHELON_GET_MEMBER_TYPE(d,data,value) \
    decltype(data::value)

#define ECHELON_GET_MEMBER_TYPES(TYPE_NAME,MEMBERS)                      \
    ECHELON_TUPLE_TO_VARIADIC(                                           \
              BOOST_PP_SEQ_TO_TUPLE(                                     \
                   BOOST_PP_SEQ_TRANSFORM(ECHELON_GET_MEMBER_TYPE,       \
                                          TYPE_NAME,MEMBERS)))

#define ECHELON_ADAPT_COMPOUND_TYPE(TYPE_NAME,MEMBERS)                                    \
    namespace echelon                                                               \
    {                                                                               \
    template<>                                                                      \
    struct static_type_layout<TYPE_NAME>                                            \
    {                                                                               \
     static constexpr static_type_layout_category category =                        \
         static_type_layout_category::generic;                                      \
     typedef std::tuple<ECHELON_GET_MEMBER_TYPES(TYPE_NAME,MEMBERS)> member_types;  \
     static const char* name() { return BOOST_PP_STRINGIZE(TYPE_NAME); }            \
     static constexpr std::size_t size = sizeof(TYPE_NAME);                         \
     static const id_offset_pair* members()                                         \
     {                                                                              \
         static constexpr id_offset_pair members_[] =                               \
             ECHELON_PROCESS_MEMBER_LIST(TYPE_NAME,MEMBERS);                        \
         return members_;                                                           \
     }                                                                              \
     static constexpr std::size_t num_members =                                     \
             std::tuple_size<member_types>::value;                                  \
    };                                                                              \
                                                                                    \
    template<>                                                                      \
    struct is_hdf5_type<TYPE_NAME> : boost::mpl::true_                              \
    {};                                                                             \
    }



#define ECHELON_CREATE_ID_SIZE_PAIR__(d,data,TYPE,NAME) \
    id_size_pair(BOOST_PP_STRINGIZE(NAME) , sizeof(TYPE))

#define ECHELON_CREATE_ID_SIZE_PAIR_(d,data,...) \
    ECHELON_CREATE_ID_SIZE_PAIR__(d,data,__VA_ARGS__)

#define ECHELON_CREATE_ID_SIZE_PAIR(d,data,value) \
    ECHELON_CREATE_ID_SIZE_PAIR_(d,data,ECHELON_TUPLE_TO_VARIADIC(value))

#define ECHELON_PROCESS_PACKED_MEMBER_LIST(TYPE_NAME,MEMBERS)        \
    { ECHELON_TUPLE_TO_VARIADIC(                                     \
          BOOST_PP_SEQ_TO_TUPLE(                                     \
               BOOST_PP_SEQ_TRANSFORM(ECHELON_CREATE_ID_SIZE_PAIR,   \
                                      TYPE_NAME,MEMBERS))) }

#define ECHELON_GET_PACKED_MEMBER_TYPE__(d,data,TYPE,NAME) \
    TYPE

#define ECHELON_GET_PACKED_MEMBER_TYPE_(d,data,...) \
    ECHELON_GET_PACKED_MEMBER_TYPE__(d,data,__VA_ARGS__)

#define ECHELON_GET_PACKED_MEMBER_TYPE(d,data,value) \
    ECHELON_GET_PACKED_MEMBER_TYPE_(d,data,ECHELON_TUPLE_TO_VARIADIC(value))

#define ECHELON_GET_PACKED_MEMBER_TYPES(MEMBERS)                             \
    ECHELON_TUPLE_TO_VARIADIC(                                               \
              BOOST_PP_SEQ_TO_TUPLE(                                         \
                   BOOST_PP_SEQ_TRANSFORM(ECHELON_GET_PACKED_MEMBER_TYPE,    \
                                          _,MEMBERS)))

#define ECHELON_ADAPT_PACKED_COMPOUND_TYPE(TYPE_NAME,MEMBERS)                             \
    namespace echelon                                                               \
    {                                                                               \
    template<>                                                                      \
    struct static_type_layout<TYPE_NAME>                                            \
    {                                                                               \
     static constexpr static_type_layout_category category =                        \
         static_type_layout_category::packed;                                       \
     typedef std::tuple<ECHELON_GET_PACKED_MEMBER_TYPES(MEMBERS)> member_types;     \
     static const char* name() { return BOOST_PP_STRINGIZE(TYPE_NAME); }            \
     static constexpr std::size_t size = sizeof(TYPE_NAME);                         \
     static const id_size_pair* members()                                           \
     {                                                                              \
         static constexpr id_size_pair members_[] =                                 \
             ECHELON_PROCESS_PACKED_MEMBER_LIST(TYPE_NAME,MEMBERS);                 \
         return members_;                                                           \
     }                                                                              \
     static constexpr std::size_t num_members =                                     \
             std::tuple_size<member_types>::value;                                  \
    };                                                                              \
                                                                                    \
    template<>                                                                      \
    struct is_hdf5_type<TYPE_NAME> : boost::mpl::true_                              \
    {};                                                                             \
    }


#endif
