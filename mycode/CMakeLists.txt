project(simple)

include_directories(
        "../thirdparty/msgpack-c/include"
        "../thirdparty/asio")

add_executable(server server/main.cpp)
add_executable(client client/main.cpp)
add_executable(example simple/basic_example.cpp)
add_executable(client2 client/client2.cpp)
add_executable(client1 client/client1.cpp)
add_executable(server1 server/server1.cpp)


target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(client ${Boost_LIBRARIES})
