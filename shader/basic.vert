#version 460 core
in vec2 position;
in vec3 color;
uniform vec2 x;
out vec3 frag_color;
  
void main() {
  vec2 new_pos = position;
  new_pos += x;
  gl_Position = vec4(new_pos, 0.0, 1.0);
  frag_color = color;
}
