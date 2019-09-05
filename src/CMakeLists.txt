add_executable(cg-exam main.cpp)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(graphics)
add_subdirectory(math)
add_subdirectory(shaders)
add_subdirectory(utils)

target_include_directories(cg-exam PUBLIC
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        )

target_include_directories(cg-exam PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/engine
        ${CMAKE_CURRENT_LIST_DIR}/game

        ${CMAKE_CURRENT_LIST_DIR}/graphics
        ${CMAKE_CURRENT_LIST_DIR}/math
        ${CMAKE_CURRENT_LIST_DIR}/utils
        )

target_link_libraries(cg-exam PUBLIC
        OpenGL::GLX
        ${SDL2_LIBRARY}
        ${SDL2_TTF_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES}
        sub::gl3w
        glm
    )