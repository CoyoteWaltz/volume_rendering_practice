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
	FragColor = vec4(screenCoord.x, screenCoord.y, 0.2, 1.0);
}