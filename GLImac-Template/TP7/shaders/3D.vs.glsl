#version 330 core

// Matrices de transformations re�ues en uniform
uniform mat4 uMVPMatrix;    //Matrice de transformation Position projet�e
uniform mat4 uMVMatrix;     //Matrice de transformation Position
uniform mat4 uNormalMatrix; //Matrice de transformation de la normale

// Attributs de sommet
layout(location = 0) in vec3 aVertexPosition;  // Position du sommet
layout(location = 1) in vec3 aVertexNormal;    // Normale du sommet
layout(location = 2) in vec2 aVertexTexCoords; // Coordonn�es de texture du sommet

// Sorties du shader
out vec3 vPosition_vs; // Position du sommet transform� dans l'espace View
out vec3 vNormal_vs;   // Normale du sommet transform� dans l'espace View
out vec2 vTexCoords;   // Coordonn�es de texture du sommet

void main() {
    // Passage en coordonn�es homog�nes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);

    // Calcul des valeurs de sortie
    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;

    // Calcul de la position projet�e
    gl_Position = uMVPMatrix * vertexPosition;
}