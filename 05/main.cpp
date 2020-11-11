#include "serializer.h"

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    void serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }
};

int main() {

    Data a {56, false, 78};
    Data b {56, true, 89};

    std::ostringstream oss1;
    Serializer ser1(oss1);

    ser1.save(a);
    ser1.save(b);
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
    ser2(true, true, false, uint64_t(555), uint64_t(90), false, uint64_t(17), true, uint64_t(18));

    s = oss2.str();
    std::istringstream iss2(s);
    Deserializer des(iss2);
    bool tmp4, tmp5;
    des(tmp4, tmp5, tmp4, tmp1, tmp2, tmp5);
    assert(tmp4 == tmp5);
    assert(tmp4 == false);
    assert(tmp1 == 555);
    assert(tmp2 == 90);

    des.load(a);
    assert(a.a == 17);
    assert(a.b == true);
    assert(a.c == 18);

    return 0;
}