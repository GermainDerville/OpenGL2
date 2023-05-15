#version 330 core

in vec3 vFragColor;
in vec2 vFragPosLocal;

out vec3 fFragColor;

vec2 complexSqr(vec2 z){
	return vec2(z.x*z.x-z.y*z.y , 2*z.x*z.y);
}
void main() {
	
		//float alpha = 1.0; 
		//float beta = 10.0;
		//float distance = length(vFragPosLocal);
		// vec2 P = vFragPosLocal;
		//float distance = length(fract(5.0 * P));
		//float distance = length(abs(fract(5.0 * P) * 2.0 - 1.0));
		//float distance = mod(floor(10.0 * vFragPosLocal.x) + floor(10.0 * vFragPosLocal.y), 2.0);
		//float distance = smoothstep(0.3, 0.32, length(fract(5.0 * P) - 0.5));
		// float distance = smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5)));
		// fFragColor = alpha *exp(-1*beta*distance*distance)* vFragColor;
	
	int Nmax = 50000;
	fFragColor = vec3(1,0,0);
	vec2 c = vFragPosLocal;
	vec2 zi= c;
	for(int i=1 ; i<Nmax ; i++){
		zi = complexSqr(zi) + c;
		if(length(zi)>=2.0){
			fFragColor = vec3(float(i) / Nmax);
			break;
		}

	}
	
	
	
};