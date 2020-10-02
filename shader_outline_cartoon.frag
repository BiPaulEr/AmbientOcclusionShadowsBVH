varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

void main (void) {
  vec3 color = vec3(0.0, 0.0, 0.0);
  gl_FragColor = vec4(color, 1.0);
}
