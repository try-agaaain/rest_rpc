#include <asio.hpp>
#include <memory>
#include <array>
#include <deque>
#include <vector>

using asio::ip::tcp;

// 假设的RPC头部长度和魔数定义
constexpr size_t HEAD_LEN = 8;
constexpr uint32_t MAGIC_NUM = 0x12345678;
#define MAX_BUF_LEN  1024
#define INIT_BUF_SIZE 1024
// RPC请求类型
enum class request_type { req_res, sub_pub };

// RPC头部结构
struct rpc_header {
  uint32_t magic;
  request_type req_type;
  uint32_t body_len;
  uint64_t req_id;
};

// 消息类型
struct message_type {
  uint64_t req_id;
  request_type req_type;
  std::shared_ptr<std::string> content;
};

class connection : public std::enable_shared_from_this<connection> {
public:
  connection(asio::io_service &io_service)
      : socket_(io_service), body_(INIT_BUF_SIZE) {
  }

  ~connection() {
    close();
  }

  void start() {
    read_head();
  }

  tcp::socket &socket() { return socket_; }

  void close() {
    if (has_closed_) {
      return;
    }

    asio::error_code ignored_ec;
    socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
    socket_.close(ignored_ec);
    has_closed_ = true;
  }

private:
  void read_head() {
    auto self(this->shared_from_this());
    asio::async_read(socket_, asio::buffer(head_, HEAD_LEN),
                     [this, self](asio::error_code ec, std::size_t /*length*/) {
                       if (!ec) {
                         rpc_header *header = reinterpret_cast<rpc_header *>(head_);
                         if (header->magic != MAGIC_NUM) {
                           close();
                           return;
                         }

                         req_id_ = header->req_id;
                         const uint32_t body_len = header->body_len;
                         if (body_len > 0 && body_len < MAX_BUF_LEN) {
                             body_.resize(body_len);
                             read_body(body_len);
                         } else if (body_len == 0) {
                             read_head();
                         } else {
                             close();
                         }
                       } else {
                         close();
                       }
                     });
  }

  void read_body(std::size_t size) {
    auto self(this->shared_from_this());
    asio::async_read(socket_, asio::buffer(body_.data(), size),
                     [this, self](asio::error_code ec, std::size_t /*length*/) {
                       if (!ec) {
                         // 处理RPC请求...
                         read_head(); // 继续读取下一个请求
                       } else {
                         close();
                       }
                     });
  }

  tcp::socket socket_;
  char head_[HEAD_LEN];
  std::vector<char> body_;
  std::uint64_t req_id_;
  bool has_closed_ = false;
};

// 测试代码
int main() {
  asio::io_service io_service;
  tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));
  acceptor.async_accept([&](std::error_code ec, tcp::socket socket) {
    if (!ec) {
      std::make_shared<connection>(io_service)->start();
    }
  });

  io_service.run();
  return 0;
}