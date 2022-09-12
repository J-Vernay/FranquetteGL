cmake_minimum_required(VERSION 3.13)

project(franquettegl)

file(GLOB_RECURSE franquettegl_HEADERS CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE franquettegl_SRC CONFIGURE_DEPENDS src/**.c src/**.h)
file(GLOB_RECURSE franquettegl_tests_SRC CONFIGURE_DEPENDS tests/*.c tests/*.h)
file(GLOB_RECURSE franquettegl_demo_SRC CONFIGURE_DEPENDS demo/*.c demo/*.h)

add_library(franquettegl ${franquettegl_HEADERS} ${franquettegl_SRC})
target_include_directories(franquettegl PUBLIC include)

add_executable(franquettegl_tests ${franquettegl_tests_SRC})
target_link_libraries(franquettegl_tests PRIVATE franquettegl)

add_executable(franquettegl_demo ${franquettegl_demo_SRC})
target_link_libraries(franquettegl_demo PRIVATE franquettegl)

install(TARGETS franquettegl RUNTIME DESTINATION bin)
