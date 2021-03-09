project(optymalizacja)
add_executable(test_euler test_euler.cpp)
target_link_libraries(test_euler gtest gmock libgraph libgraph_euler)
add_test(NAME test_euler COMMAND test_euler)

