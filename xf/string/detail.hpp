#pragma once

namespace xf { namespace detail {

/******************************************************************************************/

template <class T, std::size_t N>
constexpr auto string_length(T[N]) {return hana::size_c<N>;}

template <class T>
constexpr auto string_length(T const &t) -> decltype(t.size()) {return t.size();}

/******************************************************************************************/

template <class T, class=void>
struct is_character_t : public std::false_type {};

template <> struct is_character_t<char, void> : public std::true_type {};
template <> struct is_character_t<char16_t, void> : public std::true_type {};
template <> struct is_character_t<char32_t, void> : public std::true_type {};
template <> struct is_character_t<wchar_t, void> : public std::true_type {};

/******************************************************************************************/

// Null terminated character pointer
template <class T>
class string_wrap {
    T const * data;
    std::size_t m_size;
public:
    template<std::size_t N> constexpr string_wrap(const T(&t)[N]) : data(t), m_size(N-1) {}
    constexpr string_wrap(T const *t, std::size_t n) : data(t), m_size(n) {}

    constexpr auto begin() const {return data;}
    constexpr auto end() const {return data + m_size;}
    constexpr auto size() const {return m_size;}
};

// Null terminated character array of length N (actual string is length N-1)
template <class T, std::size_t N>
class array_wrap {
    T const * const data;
public:
    constexpr array_wrap(T const *t) : data(t) {}

    constexpr auto size() const {return hana::size_c<N - 1>;}
    constexpr auto begin() const {return data;}
    constexpr auto end() const {return data + (N - 1);}
};

/******************************************************************************************/

}

template <class T> static constexpr auto is_character = hana::bool_c<detail::is_character_t<T>::value>;

template <class T>
struct container_type_t<T, void_if<(is_character<T>)>> {using type = std::basic_string<T>;};

}
