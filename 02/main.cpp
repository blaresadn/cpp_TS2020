#include "parser.h"

int start()
{
    return 5;
}

void sum(int &n)
{
    int sum = 0;
    while (n) {
        sum += n % 10;
        n /= 10;
    }
    n = sum;
}

void len(std::string &s)
{
    s = std::to_string(s.length());
}

std::string end()
{
    return "Goodbye!";
}

int main() {
    TokenParser p;
    p.SetStartCallback(start);
    p.SetDigitTokenCallback(sum);
    p.SetStringTokenCallback(len);
    p.SetEndCallback(end);
    std::string text = p.Parse("123 1day ifu 7 nee6rv");
    if (text != "6 4 3 7 6 ") {
        throw std::runtime_error("Переданные в парсер функции не сработали");
    }

    TokenParser p1;
    p1.SetStartCallback(start);
    p1.SetDigitTokenCallback(sum);
    p1.SetStringTokenCallback(len);
    p1.SetEndCallback(end);
    text = p1.Parse("fuh 48 djf6 55h 8");
    if (text != "3 12 4 3 8 ") {
        throw std::runtime_error("Переданные в парсер функции не сработали");
    }

    return 0;
}