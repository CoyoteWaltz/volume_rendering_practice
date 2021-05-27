#shader vertex
#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color; // 其实这个 不要也可以 目前看完全用不到

out vec3 v_EntryPoint;
// out vec4 ExitPointCoord;

uniform mat4 u_MVP;

void main()
{
    v_EntryPoint = color;
    gl_Position = u_MVP * vec4(position, 1.0);
    // gl_Position = vec4(position, 1.0);
    // ExitPointCoord = gl_Position;
}




#shader fragment
#version 410 core

layout(location = 0) out vec4 fragColor;

uniform vec2 u_ScreenSize;

uniform sampler1D u_TransferFunc;
uniform sampler2D u_bfTexture;
// uniform sampler2D u_Texture;
uniform sampler3D u_FaceTexture;

in vec3 v_EntryPoint;
// in vec4 ExitPointCoord;

#define StepSize .005

void main()
{

    // gl_FragCoord window relative coord
    // 0.5, 0.5 -> width-.5, height-.5
    //-----
    ivec2 tcoord = ivec2(gl_FragCoord.xy);
    vec3 exitPoint = texelFetch(u_bfTexture, tcoord, 0).xyz;  // fetch texture by uv

    vec3 dir=exitPoint-v_EntryPoint;

    vec3 dx = abs(dir);
    // float eps = .00001f;
    // if (dx.x < eps || dx.y < eps || dx.z < eps) {
        //     fragColor = vec4(1., 0., 0.,1.);
        //     discard;
        //     // return;
    // }

    float len=length(dir);// the length from front to back is calculated and used to terminate the ray
    vec3 deltaDir=normalize(dir)*StepSize;
    float deltaDirLen=length(deltaDir);
    vec3 voxelCoord=v_EntryPoint;
    // vec3 voxelCoord=vec3(v_EntryPoint.x / 2., v_EntryPoint.y, v_EntryPoint.z);
    vec4 colorAcum=vec4(0.);// The dest color
    float alphaAcum=0.;// The dest alpha for blending
    /* 定义颜色查找的坐标 */
    float intensity;
    float forwardLen=0.;
    vec4 colorSample;// The src color
    float alphaSample;// The src alpha
    // backgroundColor
    vec4 bgColor=vec4(1.,1.,1.,0.);
    // vec4 bgColor=vec4(0.);
    
    for(int i=0;i<1600;i++)
    {
        // 获得体数据中的标量值scaler value
        intensity=texture(u_FaceTexture,voxelCoord).x;
        // 查找传输函数中映射后的值
        // 依赖性纹理读取
        colorSample=texture(u_TransferFunc,intensity);

        // modulate the value of colorSample.a
        // front-to-back integration
        if(colorSample.a>0.){
            // accomodate for variable sampling rates (base interval defined by mod_compositing.frag)
            colorSample.a=1.-pow(1.-colorSample.a,StepSize * 200.f);// 放大了好多的 alpha optical correction
            float opacityAlpha = (1.-colorAcum.a) * colorSample.a;
            colorAcum.rgb+=colorSample.rgb * opacityAlpha;
            colorAcum.a+=opacityAlpha;
        }
        forwardLen+=deltaDirLen;
        
        // terminate if opacity > 1 or the ray is outside the volume
        if(forwardLen>=len){
            colorAcum.rgb=colorAcum.rgb*colorAcum.a + (1-colorAcum.a)*bgColor.rgb;
            break;
        }
        if(colorAcum.a>.93)
        {
            colorAcum.a=1.;
            break;
        }
        voxelCoord+=deltaDir;

    }
    fragColor=colorAcum;
    // for debug
    // fragColor = vec4(v_EntryPoint, 1.0);
    // fragColor = vec4(exitPoint, 1.0);
    // fragColor = vec4(dir, 1.0);
}
