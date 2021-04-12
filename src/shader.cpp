#include "shader.h"

Shader::Shader(const std::string &file_path)
    : file_path(file_path), renderer_id(0)
{
    ShaderProgramSource source = parse_shader(file_path);
    renderer_id = create_shader(source.vertex_shader_source, source.fragment_shader_source);
}
Shader::~Shader()
{
    GLCALL(glDeleteProgram(renderer_id));
}

void Shader::bind() const
{
    GLCALL(glUseProgram(renderer_id));
}
void Shader::unbind() const
{
    GLCALL(glUseProgram(0));
}

// set unifroms
void Shader::set_unifroms4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

int Shader::get_uniform_location(const std::string &name)
{
    if (map_uniform_name_location.find(name) != map_uniform_name_location.end())
    { // find cache in hash map
        return map_uniform_name_location[name];
    }
    GLCALL(int location = glGetUniformLocation(renderer_id, name.c_str()));
    if (location == -1)
    {
        // not unsigned
        std::cout << "[WARNING::Shader::get_uniform_location::location == -1!] >>> name: "
                  << name
                  << std::endl;
    }
    map_uniform_name_location[name] = location;

    return location;
}

ShaderProgramSource Shader::parse_shader(const std::string &file_path)
{
    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1, // 用作数组 index
    };
    std::fstream stream(file_path);
    // line by line
    std::string line;

    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        { // no position
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[int(type)] << line << '\n';
        }
    }
    return {
        ss[int(ShaderType::VERTEX)].str(),
        ss[int(ShaderType::FRAGMENT)].str()};
}

// unsigned int => GLenum 还是用 cpp type 比较通用
unsigned int Shader::compile_shader(unsigned int type, const std::string &source)
{
    GLCALL(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();          // &source[0]
    GLCALL(glShaderSource(id, 1, &src, NULL)); // 如果 length 是 NULL 会取到字符串的末尾
    GLCALL(glCompileShader(id));
    // error handling
    // iv? int vector(array) so 有两个类型的 result
    int success;
    GLCALL(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    std::string shader_type = type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
    if (success == GL_FALSE)
    {
        int length;
        GLCALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char *log = (char *)alloca(length * sizeof(char));    // alloca 可以 动态的在 stack 上分配内存 cool
        GLCALL(glGetShaderInfoLog(id, length, &length, log)); // 这个 size 为啥还需要个 pointer ...
        std::cout << "[ERROR::COMPILE::SHADER "
                  << shader_type
                  << "] >>> "
                  << log
                  << " | length: "
                  << length << std::endl;
        GLCALL(glDeleteShader(id));
        return 0;
    }
    else
    {
        std::cout << "SUCCESS compile " << shader_type << std::endl;
    }

    return id;
}

// 返回一个 shader id
unsigned int Shader::create_shader(const std::string &vertex_shader, const std::string &fragment_shader)
{
    // 好多的方法去读取 shader 代码
    GLCALL(unsigned int program = glCreateProgram());
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    GLCALL(glAttachShader(program, vs));
    GLCALL(glAttachShader(program, fs));
    GLCALL(glLinkProgram(program)); // 告诉 opengl 我们的 program
    int success;
    GLCALL(glGetProgramiv(program, GL_LINK_STATUS, &success));
    if (success == GL_FALSE)
    {
        int length;
        GLCALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char *log = (char *)alloca(length * sizeof(char)); // alloca 可以 动态的在 stack 上分配内存 cool
        GLCALL(glGetProgramInfoLog(program, length, NULL, log));
        std::cout << "[ERROR::PROGRAM::LINK] >>> " << log
                  << " | length: " << length << std::endl;
    }
    else
    {
        std::cout << "SUCCESS Link program" << std::endl;
    }
    GLCALL(glValidateProgram(program)); // 结果存在 GL_VALIDATE_STATUS
    GLCALL(glGetProgramiv(program, GL_VALIDATE_STATUS, &success));
    if (success == GL_FALSE)
    {
        int length;
        GLCALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
        char *log = (char *)alloca(length * sizeof(char)); // alloca 可以 动态的在 stack 上分配内存 cool
        GLCALL(glGetProgramInfoLog(program, length, NULL, log));
        std::cout << "[ERROR::PROGRAM::VALIDATION] >>> " << log
                  << " | length: " << length << std::endl;
    }
    else
    {
        std::cout << "SUCCESS Validate program" << std::endl;
    }

    // link shader 之后可以 del 了 但是这里是直接删掉 gpu 上的 shader
    // 推荐的做法是 detach 后续会提到
    GLCALL(glDeleteShader(vs));
    GLCALL(glDeleteShader(fs));

    return program;
}