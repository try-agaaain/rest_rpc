#include <chrono>
#include <fstream>
#include <iostream>
#include <rest_rpc.hpp>

using namespace rest_rpc;
using namespace rest_rpc::rpc_service;

int main() {
    rpc_client client("127.0.0.1", 9000);

    client.connect();
    
    try {
        //int result = client.call<int>("add", 1, 2);
        auto fu = client.async_call<FUTURE>("Add", 1, 2);
        int res = fu.get().as<int>();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unkonw exception" << std::endl;
    }
    
    std::cout << "end" << std::endl;

    //client.run();
}