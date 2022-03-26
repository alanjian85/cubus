project(external)

add_library(external noise/noise.c)
target_include_directories(external INTERFACE noise)