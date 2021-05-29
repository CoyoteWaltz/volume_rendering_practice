#shader vertex
#version 410 core

layout(location = 0) in vec3 position;
// layout(location = 1) in vec3 color; // 其实这个 不要也可以 目前看完全用不到

out vec3 v_EntryPoint;
// out vec4 ExitPointCoord;

uniform mat4 u_MVP;

void main()
{
    v_EntryPoint = position;
    gl_Position = u_MVP * vec4(position, 1.0);
    // gl_Position = vec4(position, 1.0);
    // ExitPointCoord = gl_Position;
}




#shader fragment
#version 410 core

layout(location = 0) out vec4 fragColor;

// uniform vec2 u_ScreenSize;
uniform float u_StepSize;

uniform sampler1D u_TransferFunc;
uniform sampler2D u_bfTexture;
// uniform sampler2D u_Texture;
uniform sampler3D u_FaceTexture;

in vec3 v_EntryPoint;
// in vec4 ExitPointCoord;

// #define StepSize .003

void main()
{

    // gl_FragCoord window relative coord
    // 0.5, 0.5 -> width-.5, height-.5
    //-----
    ivec2 tcoord = ivec2(gl_FragCoord.xy);
    vec3 exitPoint = texelFetch(u_bfTexture, tcoord, 0).xyz;  // fetch texture by uv

    vec3 dir=exitPoint-v_EntryPoint;

    // vec3 dx = abs(dir);
    // float eps = .00001f;
    // if (dx.x < eps || dx.y < eps || dx.z < eps) {
        //     discard;
    // }

    float len=length(dir);// the length from front to back is calculated and used to terminate the ray
    vec3 deltaDir=normalize(dir)*u_StepSize;
    float deltaDirLen=length(deltaDir);
    vec3 voxelCoord=v_EntryPoint;
    // vec4 colorAcum=vec4(0.);// The dest color
    // 灰度所以只需要一个灰度值即可
    float indensityAcum = .0f;
    float alphaAcum = .0f;
    /* 定义颜色查找的坐标 */
    float intensity;
    float forwardLen=0.;
    // vec4 colorSample;// The src color
    // float alphaSample;// The src alpha
    // backgroundColor
    // vec4 bgColor=vec4(1.,1.,1.,0.);
    vec4 bgColor=vec4(0.);
    
    for(int i=0;i<16000;i++)
    {
        // 获得体数据中的标量值scaler value
        intensity=texture(u_FaceTexture,voxelCoord).x;
        // 查找传输函数中映射后的值
        if (intensity > 0.15f)
        {
            // modulate the value of colorSample.a
            float enhancedIntensity = 1.0f - exp(-3.f * intensity);
            // colorSample=vec4(vec3(1.0f - exp(-intensity)), enhancedIntensity);
            // colorSample = vec4(enhancedIntensity);

            // accomodate for variable sampling rates base interval defined by mod_compositing.frag
            // front-to-back integration
            float opacityAlpha = (1.-alphaAcum) * enhancedIntensity;
            indensityAcum+=opacityAlpha * enhancedIntensity;
            alphaAcum+=opacityAlpha;
        }

        forwardLen+=deltaDirLen;
        
        // terminate if opacity > 1 or the ray is outside the volume
        if(forwardLen>=len)
        {
            if (alphaAcum > .5) 
            {
                indensityAcum=indensityAcum*alphaAcum + (1.-alphaAcum)*bgColor.r;
            }
            break;
        }
        if(alphaAcum>.93)
        {
            alphaAcum=1.;
            // colorAcum.rgb = vec3(1., 0., 0.);
            break;
        }
        voxelCoord+=deltaDir;
    }
    // fragColor=colorAcum;
    fragColor=vec4(indensityAcum, indensityAcum, indensityAcum, alphaAcum);
    // fragColor = vec4(v_EntryPoint, 1.0);
    // fragColor = vec4(exitPoint, 1.0);
    // fragColor = vec4(dir, 1.0);
}
