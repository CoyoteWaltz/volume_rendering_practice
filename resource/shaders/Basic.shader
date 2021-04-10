#shader vertex
#version 410 core

layout(location = 0) in vec3 position;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}
#shader fragment
#version 410 core

out vec4 c;

void main()
{
    c = vec4(0.33f, 0.213f, 0.12f, 1.0f);
}