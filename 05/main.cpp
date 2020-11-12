#include "serializer.h"

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
};

int main() {

    Data a {56, false, 78};
    Data b {56, true, 89};

    std::ostringstream oss1;
    Serializer ser1(oss1);

    if (ser1.save(a) == Error::CorruptedArchive) {
        throw std::runtime_error("Serialization failed");
    }
    if (ser1.save(b) == Error::CorruptedArchive) {
        throw std::runtime_error("Serialization failed");
    }
    uint64_t tmp1, tmp2;
    std::string tmp3;
    std::string s = oss1.str();
    std::istringstream iss1(s);
    iss1 >> tmp1 >> tmp3 >> tmp2 >> tmp2;
    assert(tmp1 == tmp2);

    assert(tmp3 == "false");
    iss1 >> tmp3;
    assert(tmp3 == "true");

    std::ostringstream oss2;
    Serializer ser2(oss2);
    if (ser2(true, true, false, uint64_t(555), uint64_t(90), false,
            uint64_t(17), true, uint64_t(18)) == Error::CorruptedArchive) {
        throw std::runtime_error("Serialization failed");
    }

    s = oss2.str();
    std::istringstream iss2(s);
    Deserializer des(iss2);
    bool tmp4, tmp5;
    if (des(tmp4, tmp5, tmp4, tmp1, tmp2, tmp5) == Error::CorruptedArchive) {
        throw std::runtime_error("Deserialization failed");
    }
    assert(tmp4 == tmp5);
    assert(tmp4 == false);
    assert(tmp1 == 555);
    assert(tmp2 == 90);

    if (des.load(a) == Error::CorruptedArchive) {
        throw std::runtime_error("Deserialization failed");
    }
    assert(a.a == 17);
    assert(a.b == true);
    assert(a.c == 18);

    return 0;
}