add_executable(fuzz_highfive fuzz_parse.cpp)

target_compile_options(fuzz_highfive PRIVATE -fsanitize=fuzzer)
target_link_options(fuzz_highfive PRIVATE -fsanitize=fuzzer)

target_link_libraries(fuzz_highfive HighFive)