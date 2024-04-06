#include <rest_rpc.hpp>
using namespace rest_rpc;
using namespace rpc_service;

struct dummy {
	int add(rpc_conn conn, int a, int b) { return a + b; }
};

struct person {
    int id;
    std::string name;
    int age;

    MSGPACK_DEFINE(id, name, age);
};

int main() {
    rpc_server server(9000, std::thread::hardware_concurrency());
    std::cout << "server is running..." << std::endl;
    server.register_handler<true>(
        "publish", [&server](rpc_conn conn, std::string key, std::string token,
            std::string val) {
                //CHECK(val == "hello subscriber");
                std::cout << "val: " << val << std::endl;
                server.publish(std::move(key), std::move(val));
        });
    server.run();
}


