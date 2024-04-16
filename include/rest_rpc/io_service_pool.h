#ifndef REST_RPC_IO_SERVICE_POOL_H_
#define REST_RPC_IO_SERVICE_POOL_H_

#include "use_asio.hpp"
#include <memory>
#include <vector>

namespace rest_rpc {
namespace rpc_service {
class io_service_pool : private asio::noncopyable {
public:
  explicit io_service_pool(std::size_t pool_size) : next_io_service_(0) {
    if (pool_size == 0)
      throw std::runtime_error("io_service_pool size is 0");

    for (std::size_t i = 0; i < pool_size; ++i) {
      io_service_ptr io_service(new asio::io_context);
      work_ptr work(new asio::io_context::work(*io_service));
      io_services_.push_back(io_service);
      work_.push_back(work);
    }
  }
  // mark：在执行完这个函数后，所有的io_service都将开始运行
  void run() {
    std::vector<std::shared_ptr<std::thread>> threads;
    for (std::size_t i = 0; i < io_services_.size(); ++i) {
      threads.emplace_back(std::make_shared<std::thread>(
          [](io_service_ptr svr) { svr->run(); }, io_services_[i]));
    }

    for (std::size_t i = 0; i < threads.size(); ++i)
      threads[i]->join();
  }
  // mark：让所有io_service停止运行
  void stop() {
    for (std::size_t i = 0; i < io_services_.size(); ++i) {
      io_services_[i]->stop();
    }
  }

  asio::io_context &get_io_service() {
    asio::io_context &io_service = *io_services_[next_io_service_];
    ++next_io_service_;
    if (next_io_service_ == io_services_.size())
      next_io_service_ = 0;
    return io_service;
  }

private:
  typedef std::shared_ptr<asio::io_context> io_service_ptr;
  typedef std::shared_ptr<asio::io_context::work> work_ptr;

  /// The pool of io_services.
  std::vector<io_service_ptr> io_services_;

  /// The work that keeps the io_services running.
  std::vector<work_ptr> work_;

  /// The next io_service to use for a connection.
  std::size_t next_io_service_;
};
} // namespace rpc_service
} // namespace rest_rpc

#endif // REST_RPC_IO_SERVICE_POOL_H_