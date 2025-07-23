#add_library(Points STATIC Points.cpp)
add_library(Point STATIC Point.cpp)
#target_include_directories(Points PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Point PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})