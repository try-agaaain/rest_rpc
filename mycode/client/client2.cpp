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
//客户端调用加法的rpc服务
int main() {

    rpc_client client2;
    client2.connect("127.0.0.1", 9000);
    int count = 0;
    while (true) {
        std::string info = "The " + std::to_string(count) + "-th hello, world!";
        client2.publish("key", info);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        count += 1;
    }
    client2.run();
}