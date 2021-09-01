SET(CURL_TESTS
    h2post
)

FOREACH(test ${CURL_TESTS})
    ADD_EXECUTABLE(${test} ${test}.c common.c)
    TARGET_COMPILE_OPTIONS(${test} PRIVATE
        -Werror -Wall -Wextra
        # Additional warnings
        -Wunreachable-code -Wpointer-arith
        -Wformat=2 -Wno-format-nonliteral
        # Not warnings
        -Wno-missing-field-initializers
        -Wno-unused-parameter)
    TARGET_INCLUDE_DIRECTORIES(${test} PRIVATE
        ${CMAKE_BINARY_DIR}/curl/include)
    TARGET_LINK_LIBRARIES(${test} PUBLIC
        ${CMAKE_BINARY_DIR}/curl/lib/libcurl.so)
    ADD_DEPENDENCIES(${test} CURL)
ENDFOREACH(test)
