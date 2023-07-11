attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

varying vec4 color;

void main()
{
    gl_Position = vec4(position, 1.0f);
    color = gl_Position;
}