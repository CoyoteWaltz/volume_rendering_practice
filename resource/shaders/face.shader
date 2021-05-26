#shader vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color; // 其实这个 不要也可以 目前看完全用不到

uniform mat4 u_MVP;

out vec3 v_Position;  // 输出一个 varing 作为 color 给 frag

void main()
{
    v_Position = color;
    gl_Position = u_MVP * vec4(position, 1.0);
}



#shader fragment
#version 410 core

layout(location = 0) out vec3 fragColor;  // out vec3 for color frame buffer

in vec3 v_Position;

void main()
{
    fragColor = v_Position;
}
