cmake_minimum_required(VERSION 3.0.0)
project(pendulum_simulator CXX)

# SDL2の読み込み
include(C:\\dev_lib\\SDL2-2.0.16-86-mingw\\lib\\cmake\\SDL2\\sdl2-config.cmake) # SDL2の修正したcmakeを読み込む. SDL2に関わる変数が読み込まれる.
message(STATUS SDL2_INCLUDE_DIRS:${SDL2_INCLUDE_DIRS})
message(STATUS SDL2_LIBRARIES:${SDL2_LIBRARIES})

# subdirectoryの追加
add_subdirectory(src)

# 実行ファイルの追加, インクルード, リンク
add_executable(pendulum_simulator main.cpp)
target_include_directories(pendulum_simulator PUBLIC
${SDL2_INCLUDE_DIRS}
)
target_link_directories(pendulum_simulator PUBLIC
${SDL2_LIBDIR}
)
target_link_libraries(pendulum_simulator PUBLIC
    ${SDL2_LIBRARIES}
    pendulum
    gauss_elimination
    application
    canvas
)

# コンパイルオプション
target_compile_options(pendulum_simulator PUBLIC
    $<$<CONFIG:Debug>: -O0 -g -Wall>
    $<$<CONFIG:Release>: -O3 -Wall>
)
target_compile_features(pendulum_simulator PRIVATE cxx_std_11)