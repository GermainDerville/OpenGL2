#version 330 core

uniform vec3 uColor;
uniform sampler2D uTexture;

in vec2 vFragTexture;
in vec2 vFragPosLocal;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
	return vec2(z.x*z.x-z.y*z.y , 2*z.x*z.y);
}
void main() {
	vec4 applyTexture = texture(uTexture, vFragTexture);
	fFragColor = applyTexture.xyz;
	
};