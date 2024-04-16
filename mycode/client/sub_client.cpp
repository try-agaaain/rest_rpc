#include <chrono>
#include <fstream>
#include <iostream>
#include <rest_rpc.hpp>

using namespace rest_rpc;
using namespace rest_rpc::rpc_service;
struct person {
  int id;
  std::string name;
  int age;
  MSGPACK_DEFINE(id, name, age);
};

void test_sub1() {
  rpc_client client;
  client.enable_auto_reconnect();
  client.enable_auto_heartbeat();
  bool r = client.connect("127.0.0.1", 9000);
  if (!r) {
    return;
  }

  client.subscribe("主题：Hello", [](string_view data) { std::cout << data << "\n"; });
  client.subscribe(
      "主题：人物", "c8a3c6a6b7bd1223bf2c8cee05232e927b521984ba417cb2fca6df9d1048a796",
      [](string_view data) {
        msgpack_codec codec;
        person p = codec.unpack<person>(data.data(), data.size());
        std::cout << "人物信息：name=" << p.name << "\n";
      });
  client.subscribe(
      "主题：时间",
      "048a796c8a3c6a6b7bd1223bf2c8cee05232e927b521984ba417cb2fca6df9d1",
      [](string_view data) { std::cout << "时间：" << data << "\n"; });
  client.run();
}

int main(){
    test_sub1();
    return 0;
}