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
//�ͻ��˵��üӷ���rpc����
int main() {

    rpc_client client1;
    bool r = client1.connect("127.0.0.1", 9000);
    client1.subscribe("publish", [](string_view data) {
        std::cout << "data: " << data << "\n";
        });
    client1.run();
}
