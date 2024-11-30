file(GLOB DAY_DIRECTORIES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

foreach(DAY ${DAY_DIRECTORIES})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DAY}" AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DAY}/main.cpp")
        set(INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../data/${DAY}.txt")
        set(NAMESPACE "${DAY}")
        set(HEADER_NAME "${DAY}.hpp")

        add_executable(${DAY}_exec
                ${DAY}/main.cpp
                ${DAY}/${DAY}.cpp
        )

        target_include_directories(${DAY}_exec PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${DAY})
        target_include_directories(${DAY}_exec PRIVATE ${DAY})

        target_compile_definitions(${DAY}_exec PRIVATE
                INPUT_FILE_PATH="${INPUT_FILE}"
                NAMESPACE_NAME=${NAMESPACE}
                HEADER_NAME="${HEADER_NAME}"
        )
    elseif(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DAY}")
        message(STATUS "Skipping directory '${DAY}' (no main.cpp found).")
    endif()
endforeach()
