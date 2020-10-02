const vec3 matAlbedo = vec3 (0.0, 1.0,0.5);

uniform vec3 lightPositions[8];
uniform int numberOfLightActive;

varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

uniform bool perVertexShadow;
uniform bool perVertexAO;

const float cartoon_ambient_lighting_coefficient = 0.15;

vec3 calculateCartoonColor(){
	vec3 x = vec3 (gl_ModelViewMatrix * P);       //x
	vec3 n = normalize (gl_NormalMatrix * N);     //n
	vec3 wo = normalize (-x);                      //wo
	vec3 color = matAlbedo;
	for(int i = 0; i < numberOfLightActive; i++){
		vec3 light_pos = (gl_ModelViewMatrix * vec4( lightPositions[i], 1.0 ) ).xyz;
		vec3 wi = normalize (light_pos - x);            //wi
		vec3 wh = normalize (wi + wo);                          //wh
		if( dot(n, wh) >= 0.99 ) color = vec3(1.0, 1.0, 1.0);
	}
	return color;
}

float cartoon_AO_factor(){
	if( abs(C.r) == 0.0 ) return 0.0;
	else if( abs(C.r) <= 0.7 ) return 0.4;
	else return 1.0;
}

void main (void) {
	vec3 color = vec3(0.0, 0.0, 0.0);
	if(perVertexShadow == true && perVertexAO == false){
		if(C.a > 0.0) 	color = calculateCartoonColor();
	}else if(perVertexShadow == false && perVertexAO == false){
		color = calculateCartoonColor();
	}else if(perVertexShadow == true && perVertexAO == true){
		if(C.a > 0.0){
			color = calculateCartoonColor();
			color = color * (1.0 - cartoon_AO_factor() * cartoon_ambient_lighting_coefficient );
		}
	}else if(perVertexShadow == false && perVertexAO == true){
		color = calculateCartoonColor();
		color = color * (1.0 - cartoon_AO_factor() * cartoon_ambient_lighting_coefficient );
	}
	gl_FragColor = vec4(color, 1.0);
}
