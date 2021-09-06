SET(CURL_TESTS
    h2post
    issue7630
)

FOREACH(test ${CURL_TESTS})
    IF("${test}" MATCHES "^h2")
        ADD_EXECUTABLE(${test} ${test}.c common.c)
    ELSE()
        ADD_EXECUTABLE(${test} ${test}.c)
    ENDIF()

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
