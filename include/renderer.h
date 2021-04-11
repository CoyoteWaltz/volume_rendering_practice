#ifndef RENDERER_H
#define RENDERER_H

#include "libs.h"

void gl_clear_error();

bool gl_log_call(const char *function, const char *file, int line); 

class Renderer
{
public:
    Renderer(int aa);
    int a;
};

#endif