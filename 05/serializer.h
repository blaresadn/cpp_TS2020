#include <sstream>

#ifndef HW5_SERIALIZER_H
#define HW5_SERIALIZER_H

#endif //HW5_SERIALIZER_H

enum Error
{
    NoError,
    CorruptedArchive
};

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
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    Error process(bool elem)
    {
        if (elem) {
            out_ << "true" << sep_;
        } else {
            out_ << "false" << sep_;
        }
        return Error::NoError;
    }

    Error process(uint64_t elem)
    {
        out_ << elem << sep_;
        return Error::NoError;
    }

    template<typename T>
    Error process(T elem)
    {
        return Error::CorruptedArchive;
    }

    template <typename T>
    Error operator()(T&& elem)
    {
        return process(elem);
    }

    template <typename T, typename... ArgsT>
    Error operator()(T&& elem, ArgsT&&... args)
    {
        Error res = process(elem);
        if (res != Error::NoError) {
            return res;
        }
        return (*this).operator()(std::forward<ArgsT>(args)...);
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
    Error process(T&& var)
    {
        std::string cur_elem;
        inp_ >> cur_elem;
        if (std::is_same<bool&, T>::value) {
            if (cur_elem == "true") {
                var = true;
            } else if (cur_elem == "false") {
                var = false;
            } else {
                return Error::CorruptedArchive;
            }
        } else if (std::is_same<uint64_t&, T>::value) {
            try {
                var = std::stoull(cur_elem);
            } catch (const std::invalid_argument&) {
                return Error::CorruptedArchive;
            } catch (const std::out_of_range&) {
                return Error::CorruptedArchive;
            }
        } else {
            return Error::CorruptedArchive;
        }
        return NoError;
    }

    template<typename T>
    Error load(T&& object)
    {
        return object.serialize(*this);
    }

    template<typename T>
    Error operator()(T&& var)
    {
        return process(var);
    }

    template<typename T, typename... ArgsT>
    Error operator()(T&& var, ArgsT&&... args)
    {
        Error res = process(var);
        if (res != Error::NoError) {
            return res;
        }
        return (*this).operator()(std::forward<ArgsT>(args)...);
    }
};
