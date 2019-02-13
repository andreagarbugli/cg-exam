add_executable(cg-exercises main.cpp)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(graphics)
add_subdirectory(math)
add_subdirectory(utils)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
    )

target_include_directories(cg-exercises PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/engine
        ${CMAKE_CURRENT_LIST_DIR}/game

        ${CMAKE_CURRENT_LIST_DIR}/graphics
        ${CMAKE_CURRENT_LIST_DIR}/math
        ${CMAKE_CURRENT_LIST_DIR}/utils
        )

target_link_libraries(cg-exercises PUBLIC
        ${SDL2_LIBRARY} 
        ${SDL2_TTF_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES}
        ${OPENGL_LIBRARIES}
        sub::glew glm
    )
