#add_library(Points STATIC Points.cpp)

#add_library(Point STATIC Point.cpp)
add_library(TagUtility STATIC TagUtility.cpp)
add_library(TagStatic STATIC TagStatic.cpp)

#target_include_directories(Point PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TagUtility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TagStatic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})