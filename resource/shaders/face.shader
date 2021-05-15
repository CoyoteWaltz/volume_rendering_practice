#shader vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color; // 其实这个 不要也可以 目前看完全用不到

out vec3 v_Color;  // 输出一个 varing 作为 color 给 frag

uniform mat4 u_MVP;

void main()
{
    v_Color = position;
    gl_Position = u_MVP * vec4(position, 1.0);
}



#shader fragment
#version 410 core

layout(location = 0) out vec4 fragColor;

in vec3 v_Color;

void main()
{
    fragColor = vec4(v_Color, 1.);
    // fragColor = vec4(.61, .2, .4, 1.);
}
