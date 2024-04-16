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
        int count = 0;
        for(;count==0;){
            std::cout << "count: " << count << "add 1 and 2" << std::endl;
            auto fu = client.async_call<FUTURE>("add", 1, 2);
            int res = fu.get().as<int>();
            
            auto fu2 = client.async_call<FUTURE>("add", 1, 2);
            int res2 = fu2.get().as<int>();

            count += 1;
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unkonw exception" << std::endl;
    }
    
    std::cout << "end" << std::endl;

    client.run();
}