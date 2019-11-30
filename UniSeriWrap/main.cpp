#include <iostream>

#include "nlohmannjsonseriobj.hpp"
#include "uniseriobjwrapper.hpp"

struct Point
{
    int x;
    int y;
};

template <class T>
void toSeriObj(usw::UniSeriObjWrapper<T>& seriObjWrapper, const Point& obj)
{
    seriObjWrapper.set("x", obj.x);
    seriObjWrapper.set("y", obj.y);
}

template <class T>
void fromSeriObj(const usw::UniSeriObjWrapper<T>& seriObjWrapper, Point& obj)
{
    seriObjWrapper.fill("x", obj.x);
    seriObjWrapper.fill("y", obj.y);
}

struct Line
{
    Point first;
    Point second;
};

template <class T>
void toSeriObj(usw::UniSeriObjWrapper<T>& seriObjWrapper, const Line& obj)
{
    seriObjWrapper.set("first", obj.first);
    seriObjWrapper.set("second", obj.second);
}

template <class T>
void fromSeriObj(const usw::UniSeriObjWrapper<T>& seriObjWrapper, Line& obj)
{
    seriObjWrapper.fill("first", obj.first);
    seriObjWrapper.fill("second", obj.second);
}

int main()
{
    std::cout << "Serialize:" << std::endl;
    usw::UniSeriObjWrapper<usw::NlohmannJsonSeriObj<>> serializerWrapper;

    serializerWrapper.set("hello", 5.5);
    serializerWrapper.set("salut", 5);
    serializerWrapper.set("bonjour", 5L);
    serializerWrapper.set("ok", true);
    serializerWrapper.set("non", "oui");

    serializerWrapper.set("myPoint", Point{1, 2});

    serializerWrapper.set("myLine", Line{Point{1, 2}, Point{3, 4}});

    std::cout << serializerWrapper.serializeToString() << std::endl;

    std::cout << std::endl;
    std::cout << "Deserialize:" << std::endl;

    usw::UniSeriObjWrapper<usw::NlohmannJsonSeriObj<>> otherSerializerWrapper;

    int i;
    double d;
    std::string s;
    Point p;
    Line l;

    otherSerializerWrapper.deserializeFromString(
        R"~({"int":159,"double":5.3,"string":"strval","point":{"x":10,"y":20},"line":{"first":{"x":100,"y":200},"second":{"x":300,"y":400}}})~");

    otherSerializerWrapper.fill("int", i);
    otherSerializerWrapper.fill("double", d);
    otherSerializerWrapper.fill("string", s);
    otherSerializerWrapper.fill("point", p);
    otherSerializerWrapper.fill("line", l);

    std::cout << i << " " << d << " " << s << std::endl;
    std::cout << p.x << " " << p.y << std::endl;
    std::cout << l.first.x << " " << l.first.y << " " << l.second.x << " " << l.second.y << std::endl;

    return 0;
}
