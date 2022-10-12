project(blinkc LANGUAGES CXX)

find_package(inja CONFIG REQUIRED)

add_executable(blinkc blinkc.cpp)
target_link_libraries(blinkc PRIVATE blink::blink pantor::inja)
