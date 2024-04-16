project(basic_code)

include_directories(
        "../thirdparty/msgpack-c/include"
        "../thirdparty/asio")

add_executable(io_service_pool ./io_service_pool.cpp)
add_executable(test_codec ./test_codec.cpp)
