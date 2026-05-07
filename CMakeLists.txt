project("SimpleVoxel")

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_SOURCE_FILES
        src/Application.cpp
        src/Utils.cpp
        src/Utils.h
        src/Shader.cpp
        src/Shader.h
        src/VertexBuffer.cpp
        src/VertexBuffer.h
        src/IndexBuffer.cpp
        src/IndexBuffer.h
        src/VertexBufferLayout.h
        src/VertexArray.cpp
        src/VertexArray.h
        src/test/Test.h
        src/Texture.cpp
        src/Texture.h
        src/stb/stb_image.cpp
        src/MarchingCube.cpp
        src/MarchingCube.h
        src/test/TestMarchingCubes.cpp
        src/test/TestMarchingCubes.h
)

include_directories(src)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

# Add Resouce directory
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/resources
        COMMENT "Copying resouces into binaryy directory"
)
add_dependencies(${PROJECT_NAME} copy_resources)


# Add GLFW
add_subdirectory(dependencies/glfw-3.4 "${CMAKE_CURRENT_BINARY_DIR}/glfw_build")
include_directories(dependencies/glfw-3.4/include)

# Add GLM
add_subdirectory(dependencies/glm-master "${CMAKE_CURRENT_BINARY_DIR}/glm_build")
include_directories(dependencies/glm-master)

# Add GLAD
add_library(glad STATIC
        dependencies/glad/src/glad.c
)
target_include_directories(glad PUBLIC
        dependencies/glad/include
)

# Add IMGui
add_library(imgui STATIC
        dependencies/imgui/imgui.cpp
        dependencies/imgui/imgui_draw.cpp
        dependencies/imgui/imgui_tables.cpp
        dependencies/imgui/imgui_widgets.cpp
        dependencies/imgui/imgui_demo.cpp
        dependencies/imgui/backends/imgui_impl_glfw.cpp
        dependencies/imgui/backends/imgui_impl_opengl3.cpp

)

target_include_directories(imgui PUBLIC
        dependencies/imgui
        dependencies/imgui/backends
)

# Link all thrid party libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE glm::glm
        glfw
        glad
        imgui
)