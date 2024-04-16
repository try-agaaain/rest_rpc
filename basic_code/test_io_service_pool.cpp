#include "rest_rpc/io_service_pool.h"
#include <iostream>
#include <thread>

void test_io_service_pool_creation() {
    std::cout << "Test: io_service_pool creation with valid size" << std::endl;
    try {
        rest_rpc::rpc_service::io_service_pool pool(2);
        std::cout << "Passed: No exceptions thrown for valid size" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed: Exception thrown with message: " << e.what() << std::endl;
    }
}

void test_io_service_pool_zero_size() {
    std::cout << "Test: io_service_pool creation with zero size" << std::endl;
    try {
        rest_rpc::rpc_service::io_service_pool pool(0);
        std::cerr << "Failed: No exception thrown for zero size" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Passed: Expected exception thrown with message: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed: Unexpected exception type thrown with message: " << e.what() << std::endl;
    }
}

void test_io_service_pool_run_and_stop() {
    std::cout << "Test: io_service_pool run and stop" << std::endl;
    try {
        rest_rpc::rpc_service::io_service_pool pool(2);
        pool.run();
        std::cout << "Running: io_service_pool is running" << std::endl;
        pool.stop();
        std::cout << "Stopped: io_service_pool has stopped" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed: Exception thrown with message: " << e.what() << std::endl;
    }
}
std::atomic<bool> task_executed(false);
void exmaple_task() {
    task_executed = true;
    std::cout << "Task executed on io_service" << std::endl;
}

void test_io_service_pool_usage() {
    std::cout << "Test: io_service_pool usage" << std::endl;
    try {
        rest_rpc::rpc_service::io_service_pool pool(2);
        auto& io_service = pool.get_io_service();

        // 将任务投递到io_service
        asio::post(io_service, exmaple_task);

        // 运行所有io_service
        std::thread pool_thread([&pool]() { pool.run(); });

        // 给任务一些时间来执行
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // 停止io_service_pool
        pool.stop();
        pool_thread.join();

        if (task_executed) {
            std::cout << "Passed: Task was executed" << std::endl;
        } else {
            std::cerr << "Failed: Task was not executed" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed: Exception thrown with message: " << e.what() << std::endl;
    }
}
// g++ -g -I../include -I../thirdparty/asio ./test_io_service_pool.cpp -o bin/test_io_service_pool
int main() {
    test_io_service_pool_creation();
    test_io_service_pool_zero_size();
    // test_io_service_pool_run_and_stop();
    test_io_service_pool_usage();
    return 0;
}