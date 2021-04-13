#shader vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord; // 送去 frag  varing

out vec2 v_texCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    v_texCoord = texCoord;
}
#shader fragment
#version 410 core

layout(location = 0) out vec4 c;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

in vec2 v_texCoord;

void main()
{
    // c = vec4(0.33f, 0.213f, 0.12f, 1.0f);
    vec4 textColor = texture(u_Texture, v_texCoord);
    c = textColor - (u_Color * 0.66);
    // c = u_Color;
}