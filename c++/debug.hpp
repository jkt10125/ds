#include <tuple>
#include <string>
#include <utility>
#include <iostream>
#include <iterator>
#include <type_traits>

bool LINE_AND_NAME = true;

#define SFINAE(x, ...)             \
    template <class, class = void> \
    struct x : std::false_type {}; \
    template <class T>             \
    struct x<T, std::void_t<__VA_ARGS__>> : std::true_type {}

SFINAE(DefaultIO, decltype(std::cout << std::declval<T &>()));
SFINAE(IsTuple, typename std::tuple_size<T>::type);
SFINAE(Iterable, decltype(std::begin(std::declval<T>())));

template <class T>
constexpr char Space(const T &) {
    return (Iterable<T>::value or IsTuple<T>::value) ? '\n' : ' ';
}

template <auto &os>
struct Writer {
    template <class T>
    void Impl(T const &t) const {
        if constexpr (DefaultIO<T>::value) os << t;
        else if constexpr (Iterable<T>::value) {
            int i = 0;
            for (auto &&x : t) ((i++) ? (os << Space(x), Impl(x)) : Impl(x));
        } else if constexpr (IsTuple<T>::value) {
            std::apply([this](auto const &... args) {
                    int i = 0;
                    (((i++) ? (os << ' ', Impl(args)) : Impl(args)), ...);
                }, t);
        }
        else static_assert(IsTuple<T>::value, "No matching type for print");
    }
    template <class F, class... Ts>
    auto &operator()(F const &f, Ts const &... ts) const {
        return Impl(f), ((os << ' ', Impl(ts)), ...), os << '\n', *this;
    }
};

#ifdef DEBUG
    #define debug(args...)                                    \
        {                                                     \
            std::string _s = #args;                           \
            std::replace(_s.begin(), _s.end(), ',', ' ');     \
            std::stringstream _ss(_s);                        \
            std::istream_iterator<std::string> _it(_ss);      \
            if (LINE_AND_NAME) {                              \
                std::cerr << "\x1b[0;33m";                    \
                std::cerr << "Line " << __LINE__ << ":\n";    \
                std::cerr << "\x1b[0m";                       \
            }                                                 \
            err(_it, args);                                   \
        }

    void err(std::istream_iterator<std::string> it) {
        std::ignore = it;
    }

    template <typename T, typename... Args>
    void err(std::istream_iterator<std::string> it, T a, Args... args) {
        if (LINE_AND_NAME)
            std::cerr << "\x1b[0;31m" << *it << ":\n";
        std::cerr << "\x1b[0;32m";
        Writer<std::cerr>{}(a);
        std::cerr << "\x1b[0m";
        err(++it, args...);
    }

    #define ASSERT(...) \
        if (not(__VA_ARGS__)) throw runtime_error(#__VA_ARGS__)

#else
    #define debug(...) 0
    #define ASSERT(...) 0
#endif