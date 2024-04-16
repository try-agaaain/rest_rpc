#include "doctest/doctest.h"
#include <string>
#include <vector>
#include <map>
#include "doctest/doctest.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <rest_rpc.hpp>
#include <thread>

// 枚举类型示例
enum class Color { RED, GREEN, BLUE };

// 自定义结构体示例
struct Person {
  std::string name;
  int age;

  MSGPACK_DEFINE(name, age); // MSGPACK_DEFINE宏用于定义序列化和反序列化
};

// 为了能够序列化和反序列化Person结构体，需要提供自定义类型的序列化和反序列化函数
namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
  namespace adaptor {
  template <>
  struct convert<Person> {
    msgpack::object const &operator()(msgpack::object const &o, Person &p) const {
      if (o.type != msgpack::type::ARRAY || o.via.array.size != 2) {
        throw msgpack::type_error();
      }
      p.name = o.via.array.ptr[0].as<std::string>();
      p.age = o.via.array.ptr[1].as<int>();
      return o;
    }
  };

  template <>
  struct pack<Person> {
    template <typename Stream>
    packer<Stream> &operator()(msgpack::packer<Stream> &o, Person const &p) const {
      o.pack_array(2);
      o.pack(p.name);
      o.pack(p.age);
      return o;
    }
  };
  } // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
} // namespace msgpack

// 测试函数
TEST_CASE("test_msgpack_codec"){
  rest_rpc::rpc_service::msgpack_codec codec;

  // 测试基本数据类型
  int num = 42;
  auto num_buf = codec.pack(num);
  int unpacked_num = codec.unpack<int>(num_buf.data(), num_buf.size());
  assert(num == unpacked_num);

  // 测试字符串
  std::string str = "Hello, msgpack!";
  auto str_buf = codec.pack(str);
  std::string unpacked_str = codec.unpack<std::string>(str_buf.data(), str_buf.size());
  assert(str == unpacked_str);

  // 测试枚举
  Color color = Color::RED;
  auto color_buf = codec.pack_args_str(color);
  int unpacked_color = codec.unpack<int>(color_buf.data(), color_buf.size());
  assert(static_cast<int>(color) == unpacked_color);

  // 测试容器
  std::vector<int> vec = {1, 2, 3};
  auto vec_buf = codec.pack(vec);
  std::vector<int> unpacked_vec = codec.unpack<std::vector<int>>(vec_buf.data(), vec_buf.size());
  assert(vec == unpacked_vec);

  // 测试自定义结构体
  Person person = {"Alice", 30};
  auto person_buf = codec.pack(person);
  Person unpacked_person = codec.unpack<Person>(person_buf.data(), person_buf.size());
  assert(person.name == unpacked_person.name && person.age == unpacked_person.age);

  std::cout << "All tests passed!" << std::endl;
}

// int main() {
//   try {
//     test_msgpack_codec();
//   } catch (const std::exception &e) {
//     std::cerr << "Exception: " << e.what() << std::endl;
//     return 1;
//   }
//   return 0;
// }