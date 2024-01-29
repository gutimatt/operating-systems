cmake_minimum_required(VERSION 3.21)
project(GutierrezLoadBalancer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -pthread)

add_executable(GutierrezLoadBalancer User.c InstanceHost.c LoadBalancer.c)
