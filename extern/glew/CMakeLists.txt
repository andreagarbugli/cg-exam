# Set the project name
project (glew)

# Add a library with the above sources
add_library(${PROJECT_NAME} STATIC src/glew.c)
add_library(sub::glew ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        )