#include "format.h"

#include <iostream>

int main() {
    std::string f1 = "{0} + {1} = {2}";
    assert(format(f1, 5, 6, 11) == "5 + 6 = 11");

    try {
        format(f1, 5, 6);
    } catch (const format_exc &re) {
        std::string s = re.what();
        assert(s == "Not enough arguments");
    }

    try {
        format(f1, 5, 6, 11, 12);
    } catch (const format_exc &re) {
        std::string s = re.what();
        assert(s == "Incorrect string format");
    }

    try {
        format("{error} {0} {1}", 5, 6, 7);
    } catch (const format_exc &re) {
        std::string s = re.what();
        assert(s == "Incorrect string format");
    }

    std::string f2 = "Today is {3}. Current time is {0}:{1}. Current year is {2}.";
    assert(format(f2, 19, 30, 2020, "Tuesday") == "Today is Tuesday. Current time is 19:30. Current year is 2020.");

    std::string f3 = "{1} / {2} = {0}";
    assert(format(f3, "0.(3)", 1, 3) == "1 / 3 = 0.(3)");

    std::ostringstream oss;
    oss << format("{1}{0}{0}", '+', 'C');
    assert(oss.str() == "C++");

    return 0;
}
