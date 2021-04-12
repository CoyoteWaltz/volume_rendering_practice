#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <sstream> // string stream
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "renderer.h"

struct ShaderProgramSource
{
    std::string vertex_shader_source;
    std::string fragment_shader_source;
};

class Shader
{
private:
    std::string file_path;
    unsigned int renderer_id; // cache for uniforms
    std::unordered_map<std::string, int> map_uniform_name_location;

    int get_uniform_location(const std::string &name);

public:
    Shader(const std::string &file_path);
    ~Shader();

    void bind() const;
    void unbind() const;

    // set unifroms
    void set_unifroms4f(const std::string &name, float v0, float v1, float v2, float v3);

    ShaderProgramSource parse_shader(const std::string &file_path);
    unsigned int compile_shader(unsigned int type, const std::string &source);
    unsigned int create_shader(const std::string &vertex_shader, const std::string &fragment_shader);
};

#endif