#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 screenCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    screenCoord = (vec2(aPos.x, aPos.y) + 1.0) / 2.0;
}


#shader fragment
#version 330 core
in vec2 screenCoord;

uniform vec2 screenSize;
uniform sampler2D randomMap;
uniform sampler2D envMap;

out vec4 FragColor;

void main()
{
    // 这个颜色有点意思 在 vs 中把 pos 的 x y 对应转换成了
    // 屏幕的坐标 0-1 范围 直接给到颜色的 R G B 中两个值去做 interploate
    // 比较好看呢
    FragColor = vec4((screenCoord.x + screenCoord.y) / 2, screenCoord.x, screenCoord.y, 1.0);
}