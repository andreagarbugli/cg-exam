# Set the project name
project (gl3w)

# Add a library with the above sources
add_library(${PROJECT_NAME} STATIC src/gl3w.c)
add_library(sub::gl3w ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        )