add_executable(denormals denormals.cpp)

if (NOT MSVC)
    add_executable(sse-avx-mix sse-avx-mix.cpp)
endif (NOT MSVC)
