add_library(pendulum STATIC pendulum.cpp)
target_include_directories(pendulum PUBLIC ../include/)
target_compile_features(pendulum PRIVATE cxx_std_11)