#include <sstream>

#ifndef HW2_PARSER_H
#define HW2_PARSER_H

#endif //HW2_PARSER_H

class TokenParser {
private:
    std::function<int()> start_func_; //везде тип возвращаемого значения должен быть void
    std::function<void(std::string&)> string_func_; //для теста поставила свои, чтобы сравнивать объекты
    std::function<void(int&)> digit_func_;
    std::function<std::string()> end_func_;

public:
    TokenParser() {
        start_func_ = nullptr;
        string_func_ = nullptr;
        digit_func_ = nullptr;
        end_func_ = nullptr;
    }

    void SetStartCallback(const std::function<int()> &start) {
        start_func_ = start;
    }

    void SetStringTokenCallback(const std::function<void(std::string&)> &string) {
        string_func_ = string;
    }

    void SetDigitTokenCallback(const std::function<void(int&)> &digit) {
        digit_func_ = digit;
    }

    void SetEndCallback(const std::function<std::string()> &end) {
        end_func_ = end;
    }

    //парсер считывает все токены из строки и возвращает строку с модифицированными токенами, записанными через пробел
    const std::string Parse(const std::string &text) {
        if (start_func_ == nullptr) {
            throw std::runtime_error("Start-функция не была инициализирована");
        }
        if (start_func_() != 5) {
            throw std::runtime_error("Переданная start-функция не сработала");
        } //этот if нужен исключительно для теста, для работы парсера комментируем его
        start_func_();
        std::istringstream iss = std::istringstream(text);
        std::ostringstream oss;
        std::string token;
        while (!iss.eof()) {
            iss >> token;
            bool isnumber = true;
            for (auto c: token) {
                if (!isdigit(c)) {
                    isnumber = false;
                    break;
                }
            }
            if (isnumber) {
                if (digit_func_ == nullptr) {
                    throw std::runtime_error("Функция для обработки чисел не была инициализирована");
                }
                int t = std::stoi(token);
                digit_func_(t);
                oss << t << " ";
            } else {
                if (string_func_ == nullptr) {
                    throw std::runtime_error("Функция для обработки строк не была инициализирована");
                }
                string_func_(token);
                oss << token << " ";
            }
        }
        if (end_func_ == nullptr) {
            throw std::runtime_error("End-функция не была инициализирована");
        }
        if (end_func_() != "Goodbye!") {
            throw std::runtime_error("Переданная end-функция не сработала");
        } //комментриуем этот if для работы парсера
        end_func_();
        return oss.str();
    }
};
