#version 460 core
layout (location = 0) in vec3 aPos; // Atributo de posição (x, y, z)

void main() {
    gl_Position = vec4(aPos, 1.0); // Saída padrão do vertex shader
}