#include <fmt/core.h>
#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <string>

template <size_t N>
struct S {
    char str[N];

    template <size_t No>
    constexpr auto append(const S<No> o) const;
};

template <size_t Ns, size_t Nt>
constexpr auto
append(const S<Ns> s, const S<Nt> t) {
    constexpr size_t Nout = Ns + Nt - 1;
    std::string str{s.str};
    str.append(t.str);
    S<Nout> result;
    std::copy(str.begin(), str.begin() + Nout, result.str);
    return result;
}

template <size_t N>
template <size_t No>
constexpr auto S<N>::append(const S<No> o) const {
    return ::append(*this, o);
}

int main() {
    constexpr S s0{"happiness"};

    constexpr auto s = s0.append(S{" comes"})
        .append(S{" to"})
        .append(S{" those"})
        .append(S{" who"})
        .append(S{" wait."});

    std::puts(s.str);
}