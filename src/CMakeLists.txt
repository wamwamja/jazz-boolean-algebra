file(GLOB_RECURSE SOURCES "*.cpp")
add_library(${libname} ${SOURCES})

set(JAZZ_PUBLIC_HEADERS
        jazz/boolean-algebra.h
        jazz/config.h
        jazz/ptr.h
        jazz/basic.h
        jazz/expr.h
        jazz/flags.h
        jazz/operations.h
        jazz/registration.h
        jazz/class_hierarchy.h
        jazz/print.h
)

file(INSTALL ${JAZZ_PUBLIC_HEADERS} DESTINATION ${CMAKE_BINARY_DIR}/include/jazz)