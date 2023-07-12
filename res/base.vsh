attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

varying vec4 color;

uniform mat4 M;
uniform mat4 VP;    //P * V

void main()
{
    gl_Position = VP * M * vec4(position, 1.0f);
    color = vec4(position, 1.0f);
}