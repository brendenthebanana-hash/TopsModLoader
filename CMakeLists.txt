cmake_minimum_required(VERSION 3.16)
project(tops CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Static libstdc++/libgcc so the .so loads inside the Flatpak runtime.
add_library(tops-core SHARED core/main.cpp)
target_link_options(tops-core PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(tops-core PRIVATE ${CMAKE_DL_LIBS})

add_library(tops-hello SHARED examples/hello/hello.cpp)
target_include_directories(tops-hello PRIVATE api)
target_link_options(tops-hello PRIVATE -static-libstdc++ -static-libgcc)

install(TARGETS tops-core tops-hello LIBRARY DESTINATION lib)
install(FILES api/tops.h DESTINATION include)
