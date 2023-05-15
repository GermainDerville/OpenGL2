#version 330 core

// Matrices de transformations re�ues en uniform
uniform mat4 uMVPMatrix;    //Matrice de transformation Position projet�e
uniform mat4 uMVMatrix;     //Matrice de transformation Position
uniform mat4 uNormalMatrix; //Matrice de transformation de la normale

// Entr�es du shader
in vec3 vPosition_vs; // Position du sommet transform� dans l'espace View
in vec3 vNormal_vs;   // Normale du sommet transform� dans l'espace View
in vec2 vTexCoords;   // Coordonn�es de texture du sommet

out vec3 fFragColor; // La couleur du fragment renvoy�e

void main() {

	fFragColor = normalize(vNormal_vs);
	
};