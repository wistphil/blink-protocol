project(blinkc LANGUAGES CXX)

add_executable(blinkc blinkc.cpp)
add_executable(blink::blinkc ALIAS blinkc)
target_link_libraries(blinkc PRIVATE blink::blink pantor::inja)
