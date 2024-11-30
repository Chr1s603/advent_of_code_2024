file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    string(REGEX REPLACE "_test$" "" DAY ${TEST_NAME})
    set(INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../data/${DAY}.txt")
    set(NAMESPACE "${DAY}")
    set(HEADER_NAME "${CMAKE_CURRENT_SOURCE_DIR}/../src/${DAY}/${DAY}.hpp")

    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} "${CMAKE_CURRENT_SOURCE_DIR}/../src/${DAY}/${DAY}.cpp")
    target_link_libraries(${TEST_NAME} PRIVATE gtest gtest_main)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    target_compile_definitions(${TEST_NAME} PRIVATE
            INPUT_FILE_PATH="${INPUT_FILE}"
            NAMESPACE_NAME=${NAMESPACE}
            HEADER_NAME="${HEADER_NAME}"
    )
endforeach()
