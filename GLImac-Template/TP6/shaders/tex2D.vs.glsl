#version 330 core

uniform mat3 uModelMatrix;
uniform sampler2D uTexture;

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 vFragTexture;
out vec2 vFragPosLocal;

void main() {
  vFragTexture = aVertexTexture;
  vec2 transformed = (uModelMatrix * vec3(aVertexPosition, 1)).xy;
  gl_Position = vec4(transformed, 0, 1); //vec4(aVertexPosition, 0, 1); 
  vFragPosLocal = aVertexPosition;
};