add_executable(minimal1_exec minimal1.cpp)
target_link_libraries(minimal1_exec riffcpp)
add_test(minimal1 minimal1_exec)

add_executable(minimal2_exec minimal2.cpp)
target_link_libraries(minimal2_exec riffcpp)
add_test(minimal2 minimal2_exec)

add_executable(test1_exec test1.cpp)
target_link_libraries(test1_exec riffcpp)
add_test(test1 test1_exec)

add_executable(test2_exec test2.cpp)
target_link_libraries(test2_exec riffcpp)
add_test(test2 test2_exec)

configure_file(minimal.riff minimal.riff COPYONLY)
configure_file(minimal_2.riff minimal_2.riff COPYONLY)
configure_file(test.riff test.riff COPYONLY)
configure_file(test_2.riff test_2.riff COPYONLY)