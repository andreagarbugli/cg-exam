#version 330

in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D guiTexture;

void main()
{
    FragColor = texture(guiTexture, textureCoords);
}