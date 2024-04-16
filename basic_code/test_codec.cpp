#include "rest_rpc/codec.h"
#include <cassert>
#include <iostream>
#include <string>
#include <tuple>

// 枚举类型示例
enum class TestEnum { VALUE1, VALUE2 };

// 测试pack_args函数
void test_pack_args() {
    int a = 42;
    float b = 3.14f;
    std::string c = "test";
    auto packed = rest_rpc::rpc_service::msgpack_codec::pack_args(a, b, c);

    msgpack::unpacked unpacked;
    msgpack::unpack(unpacked, packed.data(), packed.size());
    msgpack::object obj = unpacked.get();
    assert(obj.type == msgpack::type::ARRAY);
    assert(obj.via.array.size == 3);

    std::tuple<int, float, std::string> result;
    obj.convert(result);
    assert(std::get<0>(result) == a);
    assert(std::get<1>(result) == b);
    assert(std::get<2>(result) == c);

    std::cout << "test_pack_args passed." << std::endl;
}

// 测试pack_args_str函数
void test_pack_args_str() {
    TestEnum e = TestEnum::VALUE1;
    int a = 1;
    std::string b = "hello";
    auto packed_str = rest_rpc::rpc_service::msgpack_codec::pack_args_str(e, a, b);

    msgpack::unpacked unpacked;
    msgpack::unpack(unpacked, packed_str.data(), packed_str.size());
    msgpack::object obj = unpacked.get();
    assert(obj.type == msgpack::type::ARRAY);
    assert(obj.via.array.size == 3);

    std::tuple<int, int, std::string> result;
    obj.convert(result);
    assert(std::get<0>(result) == static_cast<int>(e));
    assert(std::get<1>(result) == a);
    assert(std::get<2>(result) == b);

    std::cout << "test_pack_args_str passed." << std::endl;
}

// 测试pack函数
void test_pack() {
    std::string s = "msgpack";
    rest_rpc::rpc_service::msgpack_codec codec;
    auto packed = codec.pack(s);

    msgpack::unpacked unpacked;
    msgpack::unpack(unpacked, packed.data(), packed.size());
    msgpack::object obj = unpacked.get();
    assert(obj.as<std::string>() == s);

    std::cout << "test_pack passed." << std::endl;
}

// 测试unpack函数
void test_unpack() {
    std::string original = "unpack";
    rest_rpc::rpc_service::msgpack_codec codec;
    auto packed = codec.pack(original);

    std::string unpacked = codec.unpack<std::string>(packed.data(), packed.size());
    assert(unpacked == original);

    // 测试异常情况
    bool exception_caught = false;
    try {
        int wrong_type = codec.unpack<int>(packed.data(), packed.size());
    } catch (const std::invalid_argument& e) {
        exception_caught = true;
    }
    assert(exception_caught);

    std::cout << "test_unpack passed." << std::endl;
}

// g++ -g -I../thirdparty/msgpack-c/include -I../include ./test_codec.cpp -o bin/test_codec
int main() {
    test_pack_args();
    test_pack_args_str();
    test_pack();
    test_unpack();
    std::cout << "All unit tests passed." << std::endl;
    return 0;
}