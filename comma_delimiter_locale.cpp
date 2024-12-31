#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>
#include <locale>

static constexpr const char* INPUT = "75,47,61,53,29";

struct CommaDelimiter : std::ctype<char>
{
    static const mask* make_table()
    {
        // make a copy of the "C" locale table
        static std::vector<mask> v(classic_table(), classic_table() + table_size);
        v[','] |=  space; // comma will be classified as whitespace
        return &v[0];
    }
 
    CommaDelimiter(size_t refs = 0) : ctype(make_table(), false, refs) {}
};

int main() {
    namespace ranges = std::ranges;
    namespace views = std::views;

    auto *delimiter = new CommaDelimiter{};

    std::istringstream stm_input{INPUT};
    stm_input.imbue(std::locale(stm_input.getloc(), delimiter));

    for (const auto& str : views::istream<int>(stm_input)) {
        std::cout << str << '\n';
    }
}