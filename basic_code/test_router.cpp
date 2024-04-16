#include "rest_rpc/const_vars.h"
#include "rest_rpc/router.h"
#include "rest_rpc/md5.hpp"
#include <memory>
#include <iostream>
#include <cassert>

void example_func(std::weak_ptr<rest_rpc::rpc_service::connection> conn, const std::string& param) {
    // 示例函数，仅打印参数
    std::cout << "example_func called with param: " << param << std::endl;
}

void should_register_function_and_route_successfully() {
    rest_rpc::rpc_service::router r;
    r.register_handler("example_func", example_func);
    
    auto result = r.route(rest_rpc::MD5::MD5Hash32("example_func"), "test_param", std::weak_ptr<rest_rpc::rpc_service::connection>());
    
    assert(result.ec == rest_rpc::rpc_service::router_error::ok);
    assert(!result.result.empty()); // 您可能还需要对result的内容进行更精确的测试
    std::cout << "Test [should_register_function_and_route_successfully] passed." << std::endl;
}

void should_return_no_such_function_for_unregistered_functions() {
    rest_rpc::rpc_service::router r;
    uint32_t key = rest_rpc::MD5::MD5Hash32("unregistered_func");
    auto result = r.route(key, "", std::weak_ptr<rest_rpc::rpc_service::connection>());

    assert(result.ec == rest_rpc::rpc_service::router_error::no_such_function);
    std::cout << "Test [should_return_no_such_function_for_unregistered_functions] passed." << std::endl;
}

// g++ -g -I../include -I../thirdparty/asio -I../thirdparty/msgpack-c/include ./test_router.cpp -o bin/test_router
int main() {
    should_register_function_and_route_successfully();
    should_return_no_such_function_for_unregistered_functions();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}