#shader vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color; // 其实这个 不要也可以 目前看完全用不到

out vec3 EntryPoint;
out vec4 ExitPointCoord;

uniform mat4 u_MVP;

void main()
{
    EntryPoint = color;
    gl_Position = u_MVP * vec4(position, 1.0);
    // gl_Position = vec4(position, 1.0);
    ExitPointCoord = gl_Position;
}




#shader fragment
#version 410 core

// layout(binding = 2) uniform sampler2D lastHit;
// layout(binding = 1) uniform sampler1D transferFunction;
// layout(binding = 3) uniform sampler3D volume;
uniform sampler1D u_TransferFunc;
uniform sampler2D u_bfTexture;
// uniform sampler2D u_Texture;
uniform sampler3D u_FaceTexture;
uniform float h = 0.003f;
uniform vec2 u_ScreenSize;

//in vec3 vTextureCoord;
in vec3 EntryPoint;
in vec4 ExitPointCoord;



layout(location = 0) out vec4 vFragColor;


#define opacityThreshold 0.99

void main(void)
{

    ivec2 tcoord = ivec2(gl_FragCoord.xy);
    vec3 last = texelFetch(u_bfTexture, tcoord, 0).xyz;

    // vec3 last=texture(u_bfTexture,gl_FragCoord.xy).xyz;

    // vec2 exitFragCoord = (ExitPointCoord.xy / ExitPointCoord.w + 1.);
    // vec3 last = texture(u_bfTexture, exitFragCoord / 2.).xyz;

    // vFragColor = vec4(last, 1.);
    // return;

    //Get direction of the ray
    vec3 direction = last.xyz - EntryPoint.xyz;
    float D = length (direction);
    direction = normalize(direction);

    vec4 color = vec4(0.0f);
    color.a = 1.0f;

    vec3 trans = EntryPoint;
    vec3 rayStep = direction * h;

    for(float t =0; t<=D; t += h){
        
        //Sample in the scalar field and the transfer function
        float scalar = texture(u_FaceTexture, trans).x;
        vec4 samp = texture(u_TransferFunc, scalar).rgba;
        // vec4 samp = vec4(scalar);

        //Calculating alpa
        samp.a = 1.0f - exp(-0.5 * samp.a);
        // samp.a=1.-pow(1.-samp.a,h*200.f);// 放大了好多的 alpha


        //Acumulating color and alpha using under operator 
        samp.rgb = samp.rgb * samp.a;

        color.rgb += samp.rgb * color.a;
        color.a *= 1.0f - samp.w;


        //Do early termination of the ray
        if(1.0f - color.w > opacityThreshold) {
            break;
        }

        //Increment ray step
        trans += rayStep;
    }

    color.w = 1.0f - color.w;
    vFragColor = color; //Set the final color
}
