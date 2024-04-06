#include <rest_rpc.hpp>
using namespace rest_rpc;
using namespace rpc_service;

struct dummy {
	int add(rpc_conn conn, int a, int b) { return a + b; }
};

int main() {
	rpc_server server(9000, std::thread::hardware_concurrency());

	dummy d;
	server.register_handler("add", &dummy::add, &d);

	server.run();
}

