varying vec4 P; // Per-vertex position
varying vec3 N; // Per-vertex normal
varying vec4 C; // Per-vertex color

void main(void) {
    P = gl_Vertex;
    N = gl_Normal;
    C = gl_Color;
    gl_Position = ftransform ();
}
