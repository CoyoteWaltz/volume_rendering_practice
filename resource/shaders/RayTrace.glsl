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
#define RAYTRACE_MAX 100000.
#define PI 3.1415926535
#define MATERIAL_LAMBERTIAN 0//
#define MATERIAL_METALLIC 1
#define MATERIAL_DIELECTRIC 2
#define MATERIAL_PBR 3

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

vec3 random_in_unit_sphere()
{
    // vec3 p;
    //
    // do
    // {
        // 	p = 2.0 * rand3() - vec3(1, 1, 1);
    // }while(dot(p, p)>=1.0);
    // return p;
    
    vec3 p;
    
    float theta=rand()*2.*PI;
    float phi=rand()*PI;
    p.y=cos(phi);
    p.x=sin(phi)*cos(theta);
    p.z=sin(phi)*sin(theta);
    
    return p;
}

// 记录是否命中
struct HitRecord
{
    float t;// 方程的 t
    vec3 position;
    vec3 normal;
    
    int materialType;// 材质
    int material;//  材质 index
};

// 来个 ray
struct Ray{
    vec3 origin;
    vec3 direction;
};

Ray createRay(vec3 origin,vec3 direction){
    Ray ray;
    ray.origin=origin;
    ray.direction=direction;
    return ray;
}

struct Lambertian
{
    vec3 albedo;// 反射率
};

Lambertian createLambertian(vec3 albedo)
{
    Lambertian lambertian;
    
    lambertian.albedo=albedo;
    
    return lambertian;
}

struct Metallic
{
    vec3 albedo;
    float roughness;
};

Metallic createMetallic(vec3 albedo,float roughness)
{
    Metallic metallic;
    
    metallic.albedo=albedo;
    metallic.roughness=roughness;
    
    return metallic;
}

struct Dielectric
{
    vec3 albedo;
    float roughness;
    float ior;// 折射率
};

Dielectric createDielectric(vec3 albedo,float roughness,float ior)
{
    Dielectric dielectric;
    
    dielectric.albedo=albedo;
    dielectric.roughness=roughness;
    dielectric.ior=ior;
    
    return dielectric;
}

float schlick(float cosine,float ior)
{
    float r0=(1-ior)/(1+ior);
    r0=r0*r0;
    return r0+(1-r0)*pow((1-cosine),5);
}

vec3 reflect(in vec3 incident,in vec3 normal)
{
    return incident-2*dot(normal,incident)*normal;
}

bool refract(vec3 v,vec3 n,float ni_over_nt,out vec3 refracted)
{
    vec3 uv=normalize(v);
    float dt=dot(uv,n);
    float discriminant=1.-ni_over_nt*ni_over_nt*(1.-dt*dt);
    if(discriminant>0)
    {
        refracted=ni_over_nt*(uv-n*dt)-n*sqrt(discriminant);
        return true;
    }
    else
    return false;
}

bool DielectricScatter2(in Dielectric dielectric,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=dielectric.albedo;
    vec3 reflected=reflect(incident.direction,hitRecord.normal);
    
    vec3 outward_normal;
    float ni_over_nt;
    float cosine;
    if(dot(incident.direction,hitRecord.normal)>0.)// hit from inside
    {
        outward_normal=-hitRecord.normal;
        ni_over_nt=dielectric.ior;
        cosine=dot(incident.direction,hitRecord.normal)/length(incident.direction);// incident angle
    }
    else// hit from outside
    {
        outward_normal=hitRecord.normal;
        ni_over_nt=1./dielectric.ior;
        cosine=-dot(incident.direction,hitRecord.normal)/length(incident.direction);// incident angle
    }
    
    float reflect_prob;
    vec3 refracted;
    if(refract(incident.direction,outward_normal,ni_over_nt,refracted))
    {
        reflect_prob=schlick(cosine,dielectric.ior);
    }
    else
    {
        reflect_prob=1.;
    }
    
    if(rand()<reflect_prob)
    {
        scattered=Ray(hitRecord.position,refracted);
    }
    else
    {
        scattered=Ray(hitRecord.position,refracted);
    }
    
    return true;
}

bool DielectricScatter(in Dielectric dielectric,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    //return DielectricScatter1(dielectric, incident, hitRecord, scattered, attenuation);
    return DielectricScatter2(dielectric,incident,hitRecord,scattered,attenuation);
}

// abstract camera
struct Camera{
    vec3 left_lower_corner;// 视平面左下角
    vec3 horizontal;// 水平长度 h, 0, 0
    vec3 vertical;// 垂直长度 0, v, 0
    vec3 origin;// camera origin
};

Camera createCamera(vec3 left_lower_corner,vec3 horizontal,vec3 vertical,vec3 origin){
    Camera cam;
    
    cam.left_lower_corner=left_lower_corner;
    cam.horizontal=horizontal;
    cam.vertical=vertical;
    cam.origin=origin;
    
    return cam;
}

// object
struct Sphere{
    vec3 center;
    float radius;
    int materialType;// add Material Type
    int material;// add Material Index
};

Sphere createSphere(vec3 c,float r,int materialType,int material){
    Sphere s;
    s.center=c;
    s.radius=r;
    s.materialType=materialType;
    s.material=material;
    return s;
}

struct World
{
    int objectCount;
    Sphere objects[10];
};

World createWorld()
{
    World world;
    
    world.objectCount=4;
    world.objects[0]=createSphere(vec3(0.,0.,-3.),.35,MATERIAL_LAMBERTIAN,0);
    world.objects[1]=createSphere(vec3(.9,0.,-1.),.15,MATERIAL_METALLIC,1);
    world.objects[2]=createSphere(vec3(-.7,0.,-1.),.15,MATERIAL_DIELECTRIC,2);
    world.objects[3]=createSphere(vec3(0.,-100.5,-1.),100.,MATERIAL_LAMBERTIAN,3);
    
    return world;
}

struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float transparent;
};

bool LambertianScatter(in Lambertian lambertian,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=lambertian.albedo;
    
    scattered.origin=hitRecord.position;
    scattered.direction=hitRecord.normal+random_in_unit_sphere();
    
    return true;
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

// uniform Camera camera;// cpu 传入 uniform
// uniform World world;
uniform vec2 screenSize;
uniform sampler2D u_envMap;

in vec2 screenCoord;
out vec4 FragColor;

World world;
Camera camera;
Lambertian lambertMaterials[4];
Metallic metallicMaterials[4];
Dielectric dielectricMaterials[4];

void InitScene()
{
    world=createWorld();
    camera=createCamera(vec3(-2.,-1.,-1.),vec3(4.,0.,0.),vec3(0.,2.,0.),vec3(0.,0.,0.));
    
    lambertMaterials[0]=createLambertian(vec3(0.898, 0.4196, 0.4196));
    lambertMaterials[1]=createLambertian(vec3(.5,.7,.5));
    lambertMaterials[2]=createLambertian(vec3(0.1922, 0.1922, 0.5529));
    lambertMaterials[3]=createLambertian(vec3(0.851, 0.7333, 0.2157));
    
    metallicMaterials[0]=createMetallic(vec3(.7,.5,.5),0.);
    metallicMaterials[1]=createMetallic(vec3(.5,.7,.5),.1);
    metallicMaterials[2]=createMetallic(vec3(.5,.5,.7),.2);
    metallicMaterials[3]=createMetallic(vec3(.7,.7,.7),.3);
    
    dielectricMaterials[0]=createDielectric(vec3(1.,1.,1.),0.,1.5);
    dielectricMaterials[1]=createDielectric(vec3(1.,1.,1.),.1,1.5);
    dielectricMaterials[2]=createDielectric(vec3(1.,1.,1.),.2,1.5);
    dielectricMaterials[3]=createDielectric(vec3(1.,1.,1.),.3,1.5);
}

bool MetallicScatter(in Metallic metallic,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=metallic.albedo;
    
    scattered.origin=hitRecord.position;
    scattered.direction=reflect(incident.direction,hitRecord.normal);
    
    return dot(scattered.direction,hitRecord.normal)>0.;
}

bool MaterialScatter(in int materialType,in int material,in Ray incident,in HitRecord hitRecord,out Ray scatter,out vec3 attenuation)
{
    if(materialType==MATERIAL_LAMBERTIAN){
        return LambertianScatter(lambertMaterials[material],incident,hitRecord,scatter,attenuation);
    }
    else if(materialType==MATERIAL_METALLIC){
        return MetallicScatter(metallicMaterials[material],incident,hitRecord,scatter,attenuation);
    }
    else if(materialType==MATERIAL_DIELECTRIC){
        return DielectricScatter(dielectricMaterials[material],incident,hitRecord,scatter,attenuation);
    }
    else{
        return false;
    }
}

bool DielectricScatter1(in Dielectric dielectric,in Ray incident,in HitRecord hitRecord,out Ray scattered,out vec3 attenuation)
{
    attenuation=dielectric.albedo;
    vec3 reflected=reflect(incident.direction,hitRecord.normal);
    
    vec3 outward_normal;
    float ni_over_nt;
    if(dot(incident.direction,hitRecord.normal)>0.)// hit from inside
    {
        outward_normal=-hitRecord.normal;
        ni_over_nt=dielectric.ior;
    }
    else// hit from outside
    {
        outward_normal=hitRecord.normal;
        ni_over_nt=1./dielectric.ior;
    }
    
    vec3 refracted;
    if(refract(incident.direction,outward_normal,ni_over_nt,refracted))
    {
        scattered=Ray(hitRecord.position,refracted);
        
        return true;
    }
    else
    {
        scattered=Ray(hitRecord.position,reflected);
        
        return false;
    }
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
            hitRecord.normal=(hitRecord.position-sphere.center)/sphere.radius;
            hitRecord.materialType=sphere.materialType;
            hitRecord.material=sphere.material;
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

// vec3 WorldTrace(World world,Ray ray){
    //     // 颜色插值
    //     // float hit=RayHitSphere(sphere,ray);
    //     HitRecord record;
    //     if(WorldHit(world,ray,0.,RAYTRACE_MAX,record)){
        //         // if(hit>0.)
        //         // vec3 norm=normalize(RayGetPointAt(ray,record.normal)-sphere.center);
        //         // return vec3(.451,.6941,.2902);
        //         // return.5*(vec3(norm.x+1.,norm.y+1.,norm.z+1.));
        //         return.5*(record.normal+1.);
    //     }
    //     vec3 down_color=vec3(.9333,.6784,.5098);
    //     vec3 up_color=vec3(.1725,.6588,.7804);
    //     vec3 unit_direction=normalize(ray.direction);// 归一化方向 y
    //     float t=.5*(unit_direction.y+1.);// 将 y 缩放到 0-1 y 越大看到 up_color 越多 向上看
    
    //     return(1.-t)*down_color+t*up_color;
    //     // return (1.-t)*down_color+t*t*t*up_color;
// }

vec3 GetEnvironmentColor(World world,Ray ray)
{
    vec3 down_color=vec3(.9333,.6784,.5098);
    vec3 up_color=vec3(.1725,.6588,.7804);
    vec3 unit_direction=normalize(ray.direction);// 归一化方向 y
    float t=.5*(unit_direction.y+1.);// 将 y 缩放到 0-1 y 越大看到 up_color 越多 向上看
    
    return(1.-t)*down_color+t*up_color;
    // return (1.-t)*down_color+t*t*t*up_color;
    
    // float phi=acos(unit_direction.y)/PI;
    // float theta=(atan(unit_direction.x,unit_direction.z)+(PI/2.))/PI;
    // return texture(u_envMap,vec2(theta,phi)).xyz;
}

vec3 WorldTrace(World world,Ray ray,int depth){
    // 颜色插值
    // float hit=RayHitSphere(sphere,ray);
    HitRecord record;
    vec3 current_attenuation=vec3(1.,1.,1.);
    vec3 bgColor=vec3(0.,0.,0.);
    Ray current_ray=ray;
    while(depth>0){
        depth--;
        
        if(WorldHit(world,current_ray,.001,RAYTRACE_MAX,record)){
            vec3 attenuation;
            Ray scatter_ray;
            
            // 根据不同材质获取对应的 反射率
            if(!MaterialScatter(record.materialType,record.material,current_ray,record,scatter_ray,attenuation)){
                break;
            }
            
            // attenuation=vec3(.5);// 衰减系数
            // scatter_ray.origin=record.position;
            // 生成漫反射随机采样的 current_ray 继续 world hit
            // scatter_ray.direction=record.normal+random_in_unit_sphere();
            
            current_attenuation*=attenuation;
            current_ray=scatter_ray;
            // return.5*(record.normal+1.);
        }else{
            // return current_attenuation*GetEnvironmentColor(world,ray);
            bgColor=GetEnvironmentColor(world,ray);
            break;
        }
    }
    return current_attenuation*bgColor;
}

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
    InitScene();
    int sampleCount=10;
    int depth = 5;
    vec3 color=vec3(0.,0.,0.);
    for(int s=0;s<sampleCount;s++)
    {
        // Ray ray=createRay(camera.origin,
        //     camera.left_lower_corner+u*camera.horizontal+v*camera.vertical);
        // 随机对 采样点在 screen 上的像素点进行采样 除以 screensize why
        Ray ray=CameraGetRay(camera,screenCoord+rand2()/screenSize);
        
        color+=WorldTrace(world,ray, depth);
    }
    
    color/=sampleCount;
    
    FragColor.xyz=color;
    FragColor.w=1.;
}
