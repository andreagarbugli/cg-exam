#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord0;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    Normal = mat3(transpose(inverse(model))) * vNormal;
    TexCoord0 = vTexCoord;
    FragPos = vec3(model * vec4(vPosition, 1.0));
    gl_Position = projection * view * model *  vec4(vPosition, 1.0);

}