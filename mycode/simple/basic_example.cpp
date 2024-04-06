#include <msgpack.hpp>
#include <string>
#include <iostream>

// �Զ�������
struct Person {
    std::string name;
    int age;

    MSGPACK_DEFINE(name, age);
};

int main() {
    // ����һ��Person����
    Person p = { "Alice", 25 };

    // ���Person����
    msgpack::sbuffer sbuf; // Simple buffer
    msgpack::pack(sbuf, p);

    // ���������ݾ���sbuf��
    std::cout << "Packed data size: " << sbuf.size() << " bytes." << std::endl;

    // ����ѡ��������õ�����ת��Ϊ16�����ַ�����������ڲ鿴
    for (size_t i = 0; i < sbuf.size(); ++i) {
        printf("%02x ", static_cast<unsigned char>(sbuf.data()[i]));
    }
    std::cout << std::endl;

    // ���
    msgpack::unpacked unp;
    msgpack::unpack(unp, sbuf.data(), sbuf.size());
    msgpack::object obj = unp.get();

    // ��msgpack::objectת��ΪPerson����
    Person unpacked_person;
    obj.convert(unpacked_person);

    std::cout << "Unpacked data: " << unpacked_person.name << ", " << unpacked_person.age << std::endl;

    return 0;
}