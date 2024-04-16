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

std::string get_current_time_str() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);
    std::stringstream ss;
    ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S"); // 格式化为YYYY-MM-DD HH:MM:SS
    return ss.str();
}

int main() {
  rpc_server server(9000, std::thread::hardware_concurrency(), 3600);
  server.set_network_err_callback(
      [](std::shared_ptr<connection> conn, std::string reason) {
        std::cout << "remote client address: " << conn->remote_address()
                  << " networking error, reason: " << reason << "\n";
      });

  bool stop = false;
  // 持续不断的发布，每次发布都会使得订阅者收到对应的信息
  std::thread thd([&server, &stop] {
    person p{1, "tom", 20};
    while (!stop) {
      server.publish("主题：Hello", "hello, world!");    // 不需要token
      server.publish_by_token("主题：时间", 
                            "048a796c8a3c6a6b7bd1223bf2c8cee05232e927b521984ba417cb2fca6df9d1", 
                            get_current_time_str());
      server.publish_by_token("主题：人物", 
                            "c8a3c6a6b7bd1223bf2c8cee05232e927b521984ba417cb2fca6df9d1048a796", p);
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  });

  server.run();
  thd.join();
}