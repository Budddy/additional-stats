file(GLOB LIB_EXST_SRC src/* exst/*)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
add_library(lib_exst STATIC ${LIB_EXST_SRC})
