//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_FUNCTION_OPTIONS_HPP
#define ECHELON_FUNCTION_OPTIONS_HPP

#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/has_key.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/algorithm/transformation/push_back.hpp>
#include <boost/fusion/container/map/convert.hpp>

#include <cstddef>
#include <utility>
#include <type_traits>

namespace echelon
{
namespace utility
{

template <typename Tag, typename ArgType>
using tagged_argument = boost::fusion::pair<Tag, ArgType>;

template <typename Tag, typename OptionType>
class keyword
{
public:
    typedef Tag tag_type;

    template <typename Argument>
    tagged_argument<Tag, OptionType> operator=(Argument arg) const
    {
        static_assert(std::is_convertible<Argument, OptionType>::value,
                      "Invalid option type");

        return tagged_argument<Tag, OptionType>(arg);
    }
};

namespace detail
{

template <typename Tag, typename... Options>
struct tag_count;

template <typename Tag>
struct tag_count<Tag> : std::integral_constant<std::size_t, 0>
{
};

template <typename Tag, typename HeadTag, typename HeadOptionType,
          typename... Tail>
struct tag_count<Tag, tagged_argument<HeadTag, HeadOptionType>,
                 Tail...> : tag_count<Tag, Tail...>
{
};

template <typename Tag, typename HeadOptionType, typename... Tail>
struct tag_count<
    Tag, tagged_argument<Tag, HeadOptionType>,
    Tail...> : std::integral_constant<std::size_t,
                                      tag_count<Tag, Tail...>::value + 1>
{
};

class null_parser
{
public:
    template <typename Options>
    auto operator()(const Options&) const -> boost::fusion::map<>
    {
        return boost::fusion::map<>();
    }

    template <typename... Options>
    void verify(const Options&...) const
    {
        static_assert(sizeof...(Options) == 0, "invalid option");
    }
};

template <typename Tag, typename Options, typename DefaultValue,
          typename = typename std::enable_if<boost::fusion::result_of::has_key<
              Options, Tag>::type::value>::type>
inline auto get_option_value_or(const Options& options, const DefaultValue&)
    -> decltype(tagged_argument<Tag, DefaultValue>(
          boost::fusion::at_key<Tag>(options)))
{
    return tagged_argument<Tag, DefaultValue>(
        boost::fusion::at_key<Tag>(options));
}

template <
    typename Tag, typename Options, typename DefaultValue,
    typename = typename std::enable_if<
        !boost::fusion::result_of::has_key<Options, Tag>::type::value>::type>
inline auto get_option_value_or(const Options&,
                                const DefaultValue& default_value)
    -> decltype(tagged_argument<Tag, DefaultValue>(default_value))
{
    return tagged_argument<Tag, DefaultValue>(default_value);
}

template <typename Tag, typename OptionType, typename ContinuationParser>
class option_parser
{
public:
    option_parser(OptionType default_value_,
                  ContinuationParser continuation_parser_)
    : default_value_(std::move(default_value_)),
      continuation_parser_(std::move(continuation_parser_))
    {
    }

    template <typename Options>
    auto operator()(const Options& options) const
        -> decltype(boost::fusion::push_back(
              std::declval<ContinuationParser>()(options),
              get_option_value_or<Tag>(options, std::declval<OptionType>())))
    {
        return boost::fusion::push_back(
            continuation_parser_(options),
            get_option_value_or<Tag>(options, default_value_));
    }

    void verify() const
    {
        continuation_parser_.verify();
    }

    template <typename Head, typename... Tail>
    void verify(const Head&, const Tail&... tail) const
    {
        static_assert(tag_count<Tag, Head, Tail...>::value < 2,
                      "option is ambiguous");

        continuation_parser_.verify(tail...);
    }

private:
    OptionType default_value_;
    ContinuationParser continuation_parser_;
};
}

template <typename RootParser = detail::null_parser>
class options_description
{
public:
    explicit options_description(RootParser root_parser_ = RootParser())
    : root_parser_(std::move(root_parser_))
    {
    }

    template <typename Tag, typename OptionValue, typename DefaultValue>
    auto operator()(keyword<Tag, OptionValue>,
                    const DefaultValue& default_value) const
        -> decltype(options_description<
              detail::option_parser<Tag, OptionValue, RootParser>>(
              detail::option_parser<Tag, OptionValue, RootParser>(
                  default_value, std::declval<RootParser>())))
    {
        return options_description<
            detail::option_parser<Tag, OptionValue, RootParser>>(
            detail::option_parser<Tag, OptionValue, RootParser>(default_value,
                                                                root_parser_));
    }

    template <typename... Options>
    auto parse(Options... options) -> decltype(boost::fusion::as_map(
        std::declval<RootParser>()(boost::fusion::map<Options...>(options...))))
    {
        root_parser_.verify(options...);

        auto parsed_options = boost::fusion::as_map(
            root_parser_(boost::fusion::map<Options...>(options...)));

        return parsed_options;
    }

private:
    RootParser root_parser_;
};

template <typename Tag, typename OptionType, typename OptionMap>
inline auto get_option_value(keyword<Tag, OptionType>,
                             const OptionMap& option_map)
    -> decltype(boost::fusion::at_key<Tag>(option_map))
{
    return boost::fusion::at_key<Tag>(option_map);
}
}
}

#define ECHELON_DECLARE_KEYWORD(OPTION_TYPE, IDENTIFIER)                       \
    namespace tag                                                              \
    {                                                                          \
    struct IDENTIFIER                                                          \
    {                                                                          \
    };                                                                         \
    }                                                                          \
                                                                               \
    constexpr ::echelon::utility::keyword<tag::IDENTIFIER, OPTION_TYPE>        \
    _##IDENTIFIER{};

#endif
