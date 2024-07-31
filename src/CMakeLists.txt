file(GLOB_RECURSE SOURCES "*.cpp")
include_directories(${PROJECT_SOURCE_DIR}/src)
add_library(${libname} ${SOURCES})

file(INSTALL
        ${PROJECT_SOURCE_DIR}/src/jazz/boolean-algebra.h
        ${PROJECT_SOURCE_DIR}/src/jazz/config.h
        DESTINATION ${CMAKE_BINARY_DIR}/include/jazz)

set(JAZZ_ALGEBRA_HEADERS
        jazz/algebra/ptr.h
        jazz/algebra/basic.h
        jazz/algebra/expr.h
        jazz/algebra/flags.h
        jazz/algebra/operations.h
        jazz/algebra/registration.h
        jazz/algebra/class_hierarchy.h
        jazz/algebra/print.h
)

file(INSTALL ${JAZZ_ALGEBRA_HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/include/jazz/algebra)