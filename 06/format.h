#ifndef HW6_FORMAT_H
#define HW6_FORMAT_H

#endif //HW6_FORMAT_H

#include <string>
#include <sstream>

static int n = 0;

int get_num(const std::string &s)
{
    int res;
    try {
        res = std::stoi(s);
    } catch (std::invalid_argument&) {
        n = 0;
        throw std::runtime_error("Incorrect string format");
    } catch (std::out_of_range&) {
        n = 0;
        throw std::runtime_error("Number out of range");
    }
    return res;
}

std::string res_check(const std::string &s)
{
    for (auto c: s) {
        if (c == '{' || c == '}') {
            n = 0;
            throw std::runtime_error("Not enough arguments");
        }
    }
    return s;
}

template <typename T>
std::string process(const std::string &s, T&& arg)
{
    int i = 0;
    bool find = false;
    int cur_num = n;
    std::ostringstream res{};
    while (i < s.length()) {
        if (s[i] == '{') {
            i++;
            std::string cur_str{};
            while (s[i] != '}') {
                cur_str += s[i];
                i++;
            }
            cur_num = get_num(cur_str);
            if (n == cur_num) {
                res << arg;
                find = true;
            } else {
                res << ("{" + cur_str + "}");
            }
        } else {
            res << s[i];
        }
        i++;
    }
    if (!find) {
        if (n >= cur_num) {
            n = 0;
            throw std::runtime_error("Too many arguments");
        } else {
            n = 0;
            throw std::runtime_error("Incorrect string format");
        }
    }
    n++;
    return res.str();
}

template <typename T>
std::string format(const std::string &s, T&& arg)
{
    std::string res = res_check(process(s, arg));
    n = 0;
    return res;
}

template <typename T, typename... ArgsT>
std::string format(const std::string &s, T&& arg, ArgsT&&... args)
{
    return res_check(format(process(s, arg), std::forward<ArgsT>(args)...));
}
