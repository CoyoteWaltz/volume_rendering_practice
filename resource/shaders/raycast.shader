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
    ExitPointCoord = gl_Position;
}




#shader fragment
#version 410 core

layout(location = 0) out vec4 fragColor;

uniform vec2 u_ScreenSize;

uniform sampler1D u_TransferFunc;
uniform sampler2D u_bfTexture;
// uniform sampler2D u_Texture;
uniform sampler3D u_FaceTexture;

in vec3 EntryPoint;
in vec4 ExitPointCoord;

float StepSize = .003;

void main()
{
    // 很诡异 先跳过...
    // vec3 exitPoint=texture(u_bfTexture,gl_FragCoord.st/u_ScreenSize).xyz;

    vec2 exitFragCoord = (ExitPointCoord.xy / ExitPointCoord.w + 1.0)/2.0;
    vec3 exitPoint = texture(u_bfTexture, exitFragCoord / 2.).xyz;
    if (exitPoint == EntryPoint) {
        fragColor = vec4(1.);

        discard;
        return;

    }
    fragColor = vec4(0.);
    return;
    ///-----

    vec3 dir=exitPoint-EntryPoint;
    float len=length(dir);// the length from front to back is calculated and used to terminate the ray
    vec3 deltaDir=normalize(dir)*StepSize;
    float deltaDirLen=length(deltaDir);
    vec3 voxelCoord=EntryPoint;
    // x 方向少了一半...
    // vec3 voxelCoord=vec3(EntryPoint.x / 2., EntryPoint.y, EntryPoint.z);
    vec4 colorAcum=vec4(0.);// The dest color
    float alphaAcum=0.;// The  dest alpha for blending
    /* 定义颜色查找的坐标 */
    float intensity;
    float lengthAcum=0.;
    vec4 colorSample;// The src color
    float alphaSample;// The src alpha
    // backgroundColor
    vec4 bgColor=vec4(1.,1.,1.,0.);
    
    for(int i=0;i<1600;i++)
    {
        // 获得体数据中的标量值scaler value
        intensity=texture(u_FaceTexture,voxelCoord).x;
        // 查找传输函数中映射后的值
        // 依赖性纹理读取
        // colorSample=texture(u_TransferFunc,intensity);
        colorSample=vec4(intensity);
        // modulate the value of colorSample.a
        // front-to-back integration
        if(colorSample.a>0.){
            // accomodate for variable sampling rates (base interval defined by mod_compositing.frag)
            colorSample.a=1.-pow(1.-colorSample.a,StepSize*200.f);// 放大了好多的 alpha
            colorAcum.rgb+=(1.-colorAcum.a)*colorSample.rgb*colorSample.a;
            colorAcum.a+=(1.-colorAcum.a)*colorSample.a;
        }
        voxelCoord+=deltaDir;
        lengthAcum+=deltaDirLen;
        
        // terminate if opacity > 1 or the ray is outside the volume
        if(lengthAcum>=len){
            colorAcum.rgb=colorAcum.rgb*colorAcum.a+(1-colorAcum.a)*bgColor.rgb;
            break;
        }
        else if(colorAcum.a>.99)
        {
            colorAcum.a=1.;
            break;
        }
    }
    fragColor=colorAcum;
    // fragColor = vec4(EntryPoint, 1.0);
    // fragColor = vec4(exitPoint, 1.0);
}
