#include <iostream>
#include <asio.hpp>
#include <memory>
#include <vector>
#include <thread>

class io_service_pool {
public:
  explicit io_service_pool(std::size_t pool_size) : next_io_service_(0) {
    if (pool_size == 0)
      throw std::runtime_error("io_service_pool size is 0");

    for (std::size_t i = 0; i < pool_size; ++i) {
      auto io_service = std::make_shared<asio::io_context>();
      io_services_.push_back(io_service);
      workers_.push_back(std::make_shared<asio::io_context::work>(*io_service));
    }
  }

  void run() {
    for (auto& io_service : io_services_) {
      threads_.emplace_back([io_service] { io_service->run(); });
    }
  }

  void stop() {
    for (auto& io_service : io_services_) {
      io_service->stop();
    }
  }

  asio::io_context& get_io_service() {
    auto& io_service = *io_services_[next_io_service_];
    next_io_service_ = (next_io_service_ + 1) % io_services_.size();
    return io_service;
  }

  ~io_service_pool() {
    for (auto& thread : threads_) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

private:
  std::vector<std::shared_ptr<asio::io_context>> io_services_;
  std::vector<std::shared_ptr<asio::io_context::work>> workers_;
  std::vector<std::thread> threads_;
  std::size_t next_io_service_;
};

// 创建一个全局互斥锁
std::mutex cout_mutex;

// 修改example_task函数，使其接受一个参数
void example_task(int i) {
  // 锁定互斥锁
  std::lock_guard<std::mutex> guard(cout_mutex);
  // 现在我们可以安全地写入标准输出
  std::cout << "Running example task " << i << " on thread " << std::this_thread::get_id() << std::endl;
  // 当guard对象被销毁时，互斥锁会自动解锁
}

int test1() {
  try {
    io_service_pool pool(4); // 创建一个包含4个io_context的池
    // 启动所有io_context
    pool.run();
    // 分配任务到io_service
    for (int i = 0; i < 5; ++i) {
      // 使用std::bind绑定i作为参数传递给example_task
      asio::post(pool.get_io_service(), std::bind(example_task, i));
    }

    // 等待所有任务完成
    pool.stop();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

int test2() {
  try {
    io_service_pool pool(4); // 创建一个包含4个io_context的池

    // 启动所有io_context
    pool.run();

    std::vector<std::future<void>> futures;

    // 分配任务到io_service
    for (int i = 0; i < 10; ++i) {
      // 使用std::async而不是asio::post来启动任务，并获取future对象
      futures.push_back(std::async(std::launch::async, std::bind(example_task, i)));
    }

    // 等待所有任务完成
    for (auto& future : futures) {
      future.wait();
    }

    // 停止io_service_pool
    pool.stop();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

// g++ -g ./io_service_pool.cpp -I../thirdparty/asio -o bin/io_service_pool
int main(){
  test1();
  test2();
}