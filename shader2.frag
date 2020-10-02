// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

// Add here all the values you need to describe the lights or the materials.
// At first used const values, eventually stored in structures.
// Then, use uniform variables and set them from your CPU program using
// the GLProgram methods.

uniform vec3 lightPositions[8];
uniform vec3 lightColors[8];
uniform int numberOfLightActive;

const vec3 matAlbedo = vec3 (0.0, 0.0, 0.0);
const float pi = 3.1415927;
const float ambient_lighting_coefficient = 1.0;

//coefficients for attenuation, aq the coefficient for d^2, al the coefficient for d, ac the constant coefficient, where d means the distance between the vertex and the light source
const float ac = 0.0;
const float al = 1.0;
const float aq = 0.0;

const float ks = 1.0;							//coefficient specular
const float kd = pi;           //coefficient diffusion
const float fd = kd / pi; 		//Lambert BRDF (diffusion); 			//Lambert BRDF (diffusion)
const float s = 0.0;               //shininess

uniform float alpha;         //roughness
uniform float F0;						//Fresnel refraction index, dependent on material

uniform bool microFacet;		//Blinn-Phong BRDF / micro facet BRDF
uniform bool ggx;					//Cook-Torrance micro facet BRDF / GGX micro facet BRDF
uniform bool schlick;			//Approximation of Schlick for GGX micro facet BRDF
uniform bool perVertexShadow;
uniform bool perVertexAO;

varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

float G1Schlick(vec3 w, vec3 n){
	float k = alpha * sqrt(2.0 / pi);
	return float(dot(n, w)) / float((dot(n, w) * (1.0 - k) + k));
}

float G1Smith(vec3 w, float alpha2, vec3 n){
	return float(2.0 * dot(n, w)) / float((dot(n, w) + sqrt(alpha2 + (1.0 - alpha2) * pow(dot(n, w), 2.0))));
}

float microFacetFs(vec3 n, vec3 wi, vec3 wo, vec3 wh){
	float nwh2 = pow(dot(n, wh), 2.0);
	float F = F0 + (1.0 - F0) * pow(1.0 - max(0.0, dot(wi, wh)), 5.0);
	if(ggx == false){
		//Cook-Torrance micro facet mode
		float alpha2 = pow(alpha, 2.0);
		float D = exp( (nwh2 - 1.0) / (alpha2 * nwh2) ) / (pow(nwh2, 2.0) * alpha2 * pi);

		float shading = 2.0 * (dot(n, wh) * dot(n, wi)) / dot(wo, wh);
		float masking = 2.0 * (dot(n, wh) * dot(n, wo)) / dot(wo, wh);
		float G = min(1.0, min(shading, masking));

		return (D * F * G) / (4.0 * dot(n, wi) * dot(n, wo));
	}
  else{
		//GGX micro facet mode
		float alpha2 = pow(alpha, 2.0);
		float D = alpha2 / (pi * pow(1.0 + (alpha2 - 1.0) * nwh2, 2.0));
		float G;
		if(schlick == false) 	G = G1Smith(wi, alpha2, n) * G1Smith(wo, alpha2, n);
		else 								 	G = G1Schlick(wi, n) * G1Schlick(wo, n); 							//approximation of Schlick

		return (D * F * G) / (4.0 * dot(n, wi) * dot(n, wo));
	}
}

vec3 calculateColor(){
	vec3 x = vec3 (gl_ModelViewMatrix * P);       //x
	vec3 n = normalize (gl_NormalMatrix * N);     //n
	vec3 wo = normalize (-x);                      //wo
	vec3 color = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < numberOfLightActive; i++){
	  vec3 light_pos = (gl_ModelViewMatrix * vec4( lightPositions[i], 1.0 ) ).xyz;
		vec3 wi = normalize (light_pos - x);            //wi
		vec3 wh = normalize (wi + wo);                          //wh

		float d = length(light_pos - x);
		float attenuation = 1.0 / (ac + al * d + aq * d * d);
		vec3 Li = lightColors[i];
		float fs;

		if(microFacet == false) fs = ks * pow(dot(n, wh), s);						//Blinn-Phong BRDF (specular)
		else fs = microFacetFs(n, wi, wo, wh);

		float f = fd + fs;

		color += Li * f * max(dot(n, wi), 0.0) * attenuation;
		}
	return color;
}

void main (void) {
	vec3 color = vec3(0.0, 0.0, 0.0);
	if(perVertexShadow == true && perVertexAO == false){
			if(C.a > 0.0) 	color = calculateColor();
	}else if(perVertexShadow == false && perVertexAO == false){
			color = calculateColor();
	}else if(perVertexShadow == true && perVertexAO == true){
			if(C.a > 0.0){
				color = calculateColor();
				color = color * (1.0 - abs(C.r) * ambient_lighting_coefficient );
			}
	}else if(perVertexShadow == false && perVertexAO == true){
			color = calculateColor();
			color = color * (1.0 - abs(C.r) * ambient_lighting_coefficient );
	}
	gl_FragColor = vec4(color, 1.0);
}
