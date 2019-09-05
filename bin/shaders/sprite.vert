#version 330

layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aTexCoord;

out vec2 textureCoords;

uniform mat4 model;

void main(void){

    gl_Position = model * vec4(aPos, 0.0, 1.0);
    textureCoords = vec2((aPos.x+1.0)/2.0, 1 - (aPos.y+1.0)/2.0);
//    textureCoords = aTexCoord;
}