file(GLOB_RECURSE FILES 
    "*Driver.cpp"
)

message(STATUS "${FILES}")

foreach(FILE ${FILES})
    get_filename_component(FILE_NAME_EXT "${FILE}" NAME)

    string(REGEX MATCH "^(.*)\\.[^.]*$" dummy ${FILE_NAME_EXT})

    set(FILE_NAME ${CMAKE_MATCH_1})

    message(STATUS ${FILE_NAME})

    add_executable(${PROJECT_NAME}_${FILE_NAME}
        "${FILE_NAME}.cpp"
    )

    target_link_libraries(${PROJECT_NAME}_${FILE_NAME}
        PUBLIC
            ${PROJECT_NAME}
    )
endforeach()
