cmake_minimum_required(VERSION 3.15)
project(Libusb)

set(CMAKE_CXX_STANDARD 14)
include_directories(/usr/local/Cellar/libusb/1.0.23/include/libusb-1.0)

add_executable(Libusb main.cpp)