#shader vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color; // 其实这个 不要也可以 目前看完全用不到

out vec3 v_Color;
out vec3 EntryPoint;
out vec4 ExitPointCoord;


uniform mat4 u_MVP;

void main()
{
    EntryPoint = position;
    v_Color = position;
    gl_Position = u_MVP * vec4(position, 1.0);
    ExitPointCoord = gl_Position;
}
#shader fragment
#version 410 core

layout(location = 0) out vec4 c;
// uniform vec4 u_Color;
// uniform sampler2D u_Texture;
uniform sampler2D u_bfTexture;
uniform vec2 u_ScreenSize;

in vec3 EntryPoint;
in vec4 ExitPointCoord;
in vec3 v_Color;

void main()
{
    // 很诡异 先跳过...
    // vec3 exitPoint=texture(u_bfTexture,gl_FragCoord.st).xyz;
    // vec2 exitFragCoord = (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0;
    // vec3 exitPoint = texture(u_bfTexture, exitFragCoord).xyz;
    // if (exitPoint == EntryPoint) {
    //     c = vec4(1);
    //     return;
    // }

    // c = vec4(0.33f, 0.213f, 0.12f, 1.0f);
    // vec4 textColor = texture(u_Texture, v_texCoord);
    // c = textColor - (u_Color * 0.66);
    // vec4 textColor = texture(u_bfTexture, v_texCoord);
    c = vec4(exitPoint, 1.);
}
