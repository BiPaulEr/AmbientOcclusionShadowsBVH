varying vec4 P; // Per-vertex position
varying vec3 N; // Per-vertex normal

void main(void) {
    P = gl_Vertex;
    N = gl_Normal;
    gl_Position = ftransform ();
}
