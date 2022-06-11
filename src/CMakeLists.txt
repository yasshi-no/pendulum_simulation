add_library(pendulum STATIC pendulum.cpp)
target_include_directories(pendulum PUBLIC ../include/)
target_compile_features(pendulum PRIVATE cxx_std_11)

add_library(gauss_elimination STATIC gauss_elimination.cpp)
target_include_directories(gauss_elimination PUBLIC ../include/)
target_compile_features(gauss_elimination PRIVATE cxx_std_11)