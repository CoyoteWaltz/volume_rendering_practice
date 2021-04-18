#shader vertex
#version 330 core
layout(location=0)in vec3 aPos;

out vec2 screenCoord;

void main()
{
    gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.);
    screenCoord=(vec2(aPos.x,aPos.y)+1.)/2.;
}

#shader fragment
#version 330 core

// 来个 ray
struct Ray{
    vec3 origin;
    vec3 direction;
};

// abstract camera
struct Camera{
    vec3 left_lower_corner;// 视平面左下角
    vec3 horizontal;// 水平长度 h, 0, 0
    vec3 vertical;// 垂直长度 0, v, 0
    vec3 origin;// camera origin
};
// object
struct Sphere{
    vec3 center;
    float radius;
};

struct World
{
    int objectCount;
    Sphere objects[10];
};

// 记录是否命中
struct HitRecord
{
    float t;// 方程的 t
    vec3 position;
    vec3 norm;
};

struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float transparent;
};

uint m_u=uint(521288629);
uint m_v=uint(362436069);

uint GetUintCore(inout uint u,inout uint v)
{
    v=uint(uint(36969)*(v&uint(65535))+(v>>16));
    u=uint(uint(18000)*(u&uint(65535))+(u>>16));
    return(v<<16)+u;
}

float GetUniformCore(inout uint u,inout uint v)
{
    // 0 <= u <= 2^32
    uint z=GetUintCore(u,v);
    // The magic number is 1/(2^32 + 1) and so result is positive and less than 1.
    return float(z)/uint(4294967295);
}

float GetUniform()
{
    return GetUniformCore(m_u,m_v);// 会改变 m_v m_u
}

float rand()
{
    return GetUniform();
}

vec2 rand2()
{
    return vec2(rand(),rand());
}

vec3 rand3()
{
    return vec3(rand(),rand(),rand());
}

vec4 rand4()
{
    return vec4(rand(),rand(),rand(),rand());
}

Ray createRay(vec3 origin,vec3 direction){
    Ray ray;
    ray.origin=origin;
    ray.direction=direction;
    return ray;
}
// 光线在某个点的位置
vec3 RayGetPointAt(Ray ray,float t){
    return ray.origin+t*ray.direction;
}

Ray CameraGetRay(Camera camera,vec2 offset)
{
    Ray ray=createRay(camera.origin,
        camera.left_lower_corner+
        offset.x*camera.horizontal+
    offset.y*camera.vertical);
    
    return ray;
}

Sphere createSphere(vec3 c,float r){
    Sphere s;
    s.center=c;
    s.radius=r;
    return s;
}

bool RayHitSphere(Sphere sphere,Ray ray,float t_min,float t_max,inout HitRecord hitRecord){
    vec3 oc=ray.origin-sphere.center;
    // 解方程求 t
    float a=dot(ray.direction,ray.direction);
    float b=dot(oc,ray.direction);
    float c=dot(oc,oc)-sphere.radius*sphere.radius;
    
    float discriminant=b*b-a*c;
    
    bool isHit=false;
    if(discriminant>0.){
        // 分别求两个 t
        float temp=(-b-sqrt(discriminant))/(a);
        if(temp<t_max&&temp>t_min){
            isHit=true;
        }
        if(!isHit){// 第一个 t 没有命中 在求第二个 t
            temp=(-b+sqrt(discriminant))/(a);
            if(temp<t_max&&temp>t_min){
                isHit=true;
            }
        }
        if(isHit){
            hitRecord.t=temp;
            hitRecord.position=RayGetPointAt(ray,hitRecord.t);
            hitRecord.norm=(hitRecord.position-sphere.center)/sphere.radius;
        }
    }
    return isHit;
}

bool WorldHit(World world,Ray ray,float t_min,float t_max,inout HitRecord rec)
{
    HitRecord tempRec;
    float cloestSoFar=t_max;// 记录目前最近点 t
    bool hitSomething=false;
    
    for(int i=0;i<world.objectCount;i++)
    {
        if(RayHitSphere(world.objects[i],ray,t_min,cloestSoFar,tempRec))
        {
            rec=tempRec;
            cloestSoFar=tempRec.t;
            
            hitSomething=true;
        }
    }
    
    return hitSomething;
}

vec3 WorldTrace(World world,Ray ray){
    // 颜色插值
    // float hit=RayHitSphere(sphere,ray);
    HitRecord record;
    if(WorldHit(world,ray,0.,1000000.,record)){
        // if(hit>0.)
        // vec3 norm=normalize(RayGetPointAt(ray,record.norm)-sphere.center);
        // return vec3(.451,.6941,.2902);
        // return.5*(vec3(norm.x+1.,norm.y+1.,norm.z+1.));
        return.5*(record.norm+1.);
    }
    vec3 down_color=vec3(.9333,.6784,.5098);
    vec3 up_color=vec3(.1725,.6588,.7804);
    vec3 unit_direction=normalize(ray.direction);// 归一化方向 y
    float t=.5*(unit_direction.y+1.);// 将 y 缩放到 0-1 y 越大看到 up_color 越多 向上看
    
    return(1.-t)*down_color+t*up_color;
    // return (1.-t)*down_color+t*t*t*up_color;
}

uniform Camera camera;// cpu 传入 uniform
uniform World world;
uniform vec2 screenSize;

in vec2 screenCoord;
out vec4 FragColor;

void main()
{
    // float u=screenCoord.x;
    // float v=screenCoord.y;
    // Sphere s=createSphere(vec3(.001,0.,-.2),.01);
    
    // Ray ray;
    // ray.origin=camera.origin;
    // 光的方向 也就是到 屏幕某个点的方向 u v 其实已经是归一化之后的值
    // 得到 viewport plane 的 x y 加上 左下角这个起始位置 - camera origin
    // ray.direction=camera.left_lower_corner+u*camera.horizontal+v*camera.vertical-camera.origin;
    int sampleCount=10;
    vec3 color=vec3(0.,0.,0.);
    for(int s=0;s<sampleCount;s++)
    {
        // Ray ray=createRay(camera.origin,
        //     camera.left_lower_corner+u*camera.horizontal+v*camera.vertical);
        // 随机对 采样点在 screen 上的像素点进行采样 除以 screensize why
        Ray ray=CameraGetRay(camera,screenCoord+rand2()/screenSize);
        // Ray ray=CameraGetRay(camera,screenCoord);
        
        color+=WorldTrace(world,ray);
    }
    
    color/=sampleCount;
    
    FragColor.xyz=color;
    FragColor.w=1.;
}
