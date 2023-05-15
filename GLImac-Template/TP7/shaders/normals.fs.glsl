#version 330 core

// Matrices de transformations reçues en uniform
uniform mat4 uMVPMatrix;    //Matrice de transformation Position projetée
uniform mat4 uMVMatrix;     //Matrice de transformation Position
uniform mat4 uNormalMatrix; //Matrice de transformation de la normale

// Entrées du shader
in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs;   // Normale du sommet transformé dans l'espace View
in vec2 vTexCoords;   // Coordonnées de texture du sommet

out vec3 fFragColor; // La couleur du fragment renvoyée

void main() {

	fFragColor = normalize(vNormal_vs);
	
};