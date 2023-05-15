#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;


// Sorties du shader
out vec3 vPosition_fs;
out vec3 vNormal_fs;
out vec2 vTexCoords_fs;

void main() {
     vPosition_fs =  vPosition_vs;
     vNormal_fs = vNormal_vs;
     vTexCoords_fs = vTexCoords;
}