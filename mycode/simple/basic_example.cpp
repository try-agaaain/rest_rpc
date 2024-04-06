#include <msgpack.hpp>
#include <string>
#include <iostream>

// 自定义类型
struct Person {
    std::string name;
    int age;

    MSGPACK_DEFINE(name, age);
};

int main() {
    // 创建一个Person对象
    Person p = { "Alice", 25 };

    // 打包Person对象
    msgpack::sbuffer sbuf; // Simple buffer
    msgpack::pack(sbuf, p);

    // 打包后的数据就在sbuf中
    std::cout << "Packed data size: " << sbuf.size() << " bytes." << std::endl;

    // 【可选】将打包好的数据转换为16进制字符串输出，便于查看
    for (size_t i = 0; i < sbuf.size(); ++i) {
        printf("%02x ", static_cast<unsigned char>(sbuf.data()[i]));
    }
    std::cout << std::endl;

    // 解包
    msgpack::unpacked unp;
    msgpack::unpack(unp, sbuf.data(), sbuf.size());
    msgpack::object obj = unp.get();

    // 将msgpack::object转换为Person对象
    Person unpacked_person;
    obj.convert(unpacked_person);

    std::cout << "Unpacked data: " << unpacked_person.name << ", " << unpacked_person.age << std::endl;

    return 0;
}