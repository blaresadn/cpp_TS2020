#include <sstream>

#ifndef HW5_SERIALIZER_H
#define HW5_SERIALIZER_H

#endif //HW5_SERIALIZER_H

class Serializer
{
private:
    std::ostream &out_;
    char sep_ = ' ';

public:
    explicit Serializer(std::ostream &out): out_(out)
    {
    }

    template <class T>
    void save(T& object)
    {
        object.serialize(*this);
    }

    void process(bool elem)
    {
        if (elem) {
            out_ << "true" << sep_;
        } else {
            out_ << "false" << sep_;
        }
    }

    void process(uint64_t elem)
    {
        out_ << std::to_string(elem) << sep_;
    }

    template<typename T>
    void process(T elem)
    {
        throw std::runtime_error("Wrong data type");
    }

    template <typename T>
    void operator()(T&& elem)
    {
        process(elem);
    }

    template <typename T, typename... ArgsT>
    void operator()(T&& elem, ArgsT&&... args)
    {
        process(elem);
        (*this).operator()(std::forward<ArgsT>(args)...);
    }
};

class Deserializer
{
private:
    std::istream &inp_;

public:
    explicit Deserializer(std::istream &is): inp_(is)
    {
    }

    template<typename T>
    void process(T&& var)
    {
        std::string cur_elem;
        inp_ >> cur_elem;
        if (std::is_same<bool&, T>::value) {
            if (cur_elem == "true") {
                var = true;
            } else if (cur_elem == "false") {
                var = false;
            } else {
                throw std::runtime_error("Wrong data type");
            }
        } else if (std::is_same<uint64_t&, T>::value) {
            try {
                var = std::stoull(cur_elem);
            } catch (std::exception&) {
                throw std::runtime_error("Wrong data type");
            }
        } else {
            throw std::runtime_error("Wrong data type");
        }
    }

    template<typename T>
    void load(T&& object)
    {
        object.serialize(*this);
    }

    template<typename T>
    void operator()(T&& var)
    {
        process(var);
    }

    template<typename T, typename... ArgsT>
    void operator()(T&& var, ArgsT&&... args)
    {
        process(var);
        (*this).operator()(std::forward<ArgsT>(args)...);
    }
};
