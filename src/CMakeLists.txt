# pendulum
add_library(pendulum STATIC pendulum.cpp ../include/pendulum.hpp)
target_include_directories(pendulum PUBLIC ../include/)
target_compile_features(pendulum PRIVATE cxx_std_11)

# gauss_elimination
add_library(gauss_elimination STATIC gauss_elimination.cpp ../include/gauss_elimination.hpp)
target_include_directories(gauss_elimination PUBLIC ../include/)
target_compile_features(gauss_elimination PRIVATE cxx_std_11)

# application
add_library(application STATIC application.cpp ../include/application.hpp)
target_include_directories(application PUBLIC 
    ../include/
    ${SDL2_INCLUDE_DIRS}
)
target_link_directories(application PUBLIC
${SDL2_LIBDIR}
)
target_link_libraries(application PUBLIC
    ${SDL2_LIBRARIES}
    pendulum
    canvas
)
target_compile_features(application PRIVATE cxx_std_11)

# canvas
add_library(canvas STATIC canvas.cpp ../include/canvas.hpp)
target_include_directories(canvas PUBLIC 
    ../include/
    ${SDL2_INCLUDE_DIRS}
)
target_link_directories(canvas PUBLIC
${SDL2_LIBDIR}
)
target_link_libraries(canvas PUBLIC
    ${SDL2_LIBRARIES}
    pendulum
)
target_compile_features(canvas PRIVATE cxx_std_11)

# canvas_for_pendulum
add_library(canvas_for_pendulum STATIC canvas_for_pendulum.cpp ../include/canvas_for_pendulum.hpp)
target_include_directories(canvas_for_pendulum PUBLIC 
    ../include/
)
target_link_libraries(canvas_for_pendulum PUBLIC
    canvas
)
target_compile_features(canvas_for_pendulum PRIVATE cxx_std_11)
