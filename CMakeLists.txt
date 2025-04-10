cmake_minimum_required(VERSION 3.10)
project(mkxqimage)

find_package(OpenSSL REQUIRED)

add_executable(mkxq source/mkxqimage.c)
target_compile_options(mkxq PRIVATE -w)
target_link_libraries(mkxq PRIVATE OpenSSL::SSL OpenSSL::Crypto)