#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoord;
out vec3 Normal;
out vec4 FragPos;
out vec3 ModelPos;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec4(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    ModelPos = aPos;
}