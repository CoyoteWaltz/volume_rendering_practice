/**
 * new opengl toturial demo
*/
#include "include/libs.h"

// 非得这么写 declaration
void window_resize_callback(GLFWwindow *window, const int width, const int height);
void padding_viewport(const int width, const int height, const int padding);

void window_resize_callback(GLFWwindow *window, const int width, const int height)
{
    std::cout << "resizeing!" << std::endl;
    glViewport(0, 0, width, height);
    // padding_viewport(width, height, 10);
}

void padding_viewport(const int width, const int height, const int padding = 10)
{
    glViewport(padding, padding, width - padding, height - padding);
}

void handle_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // hit space change color
        glClearColor(1.f, 1.f, 1.f, 1.f);
    }
}

int main(int argc, char **argv)
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit())
    {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return -1;
    }

    // Setting window properties  opengl 4.1 on mac
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 开启 core profile

    const unsigned int width = 640;
    const unsigned int height = 480;

    // Actually create the window
    GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL window", NULL, NULL);
    if (!window)
    {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, window_resize_callback);

    // swap 的频率 vsync
    // the number of screen updates to wait from the time
    // glfwSwapBuffers was called before swapping the buffers and returning.
    // This is sometimes called vertical synchronization,
    // vertical retrace synchronization or just vsync.
    // 翻译一下
    glfwSwapInterval(1);

    // padding_viewport(width, height);
    glViewport(0, 0, width, height);

    // start GLEW extension handler
    // glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // version as a string
    std::cout << ">>> Renderer: " << renderer << std::endl;
    std::cout << ">>> OpenGL version supported " << version << std::endl;

    // actually 这些 vertices 只有 position 属性
    // 如果有其他属性 那么在 attrib point 的时候就要 计算其他步长之类的计算
    float vertices[] = {
        // 如果要画一个长方形
        // // 第一个三角形的三个坐标 右下三角
        // -0.5f, -0.5f, 0.0f,
        // 0.5f, -0.5f, 0.0f,
        // 0.5f, 0.5f, 0.0f,
        // // 第二个 左上三角
        // // 0.5f, 0.5f, 0.0f,   // 右上角重复的点
        // // -0.5f, -0.5f, 0.0f, // 左下角重复的点
        // -0.5f, 0.5f, 0.0f // 左上角
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    // index of a triangle
    unsigned int indices[] = {
        // 0, 1, 2, // 组成右下三角形
        // 2, 3, 0  // 左上三角形
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };           // 必须是 unsigned

    {
        // unsigned int buffer;

        // unsigned int VAO;

        // unsigned int IBO; // index buffer or EBO element buffer
        // glGenVertexArrays(1, &VAO);

        // glGenBuffers(1, &buffer); // 生成 1 个 buffer 给他个 point
        // glBindVertexArray(VAO);
        // 选择这个 buffer 就是 binding 操作 指明类型
        // glBindBuffer(GL_ARRAY_BUFFER, buffer);
        // 接着给他 一点数据 和大小 usage GL_STATIC_DRAW 修改一次数据用好多次_画画用
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // glGenBuffers(1, &IBO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // opengl 还不知道数据是如何 layout
        // index: index of the attribs, size: 每个 attrib 的数据大小 4 default,
        // type: 数据类型, normalized: 是否需要被标准化到 0 1,
        // stride: the amount of bytes between each vertex
        // pointer: offset of the first component of the first generic vertex attribute(byte)
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        // 注意这里的 attrib index 也会被用在 vertex shader 中 对应的 shader 会取得对应的 attrib 做相应的计算
        // 这一行又 link 了 vao

        // enable!!
        // glEnableVertexAttribArray(0);
        VertexBuffer vb(vertices, 4 * 3 * sizeof(float));
        IndexBuffer ib(indices, 6);
        VertexArray va;
        VertexBufferLayout layout;
        layout.push<float>(3); // layout 3D xyz => 3
        va.add_buffer(vb, layout);

        // ShaderProgramSource source = parse_shader("../resource/shaders/Basic.shader");
        // unsigned int shader = create_shader(source.vertex_shader_source, source.fragment_shader_source);
        // GLCALL(glUseProgram(shader));

        float r = 0.224;
        float g = 0.612;
        float b = 0.940;
        float a = 1.0;
        float increment = 0.05;
        Shader shader("../resource/shaders/Basic.shader");
        shader.bind();
        // location of the uniform variable according to the name in shader
        // GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
        // ASSERT(location != -1); // 没找到 就是 -1 但不会影响整个代码 shader 不编译
        // 设定值
        // GLCALL(glUniform4f(location, r, g, b, a));
        shader.set_unifroms4f("u_Color", r, g, b, a);

        // glDrawArrays(GL_TRIANGLES, 0, 3);

        // Wireframe mode
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // GLCALL(glUseProgram(0)); // 如果所有的 bind 操作给的是 0 就是 unbind。。。
        // 需要在 loop 中 重新 bind
        // 画不同 obj 的时候 我们有必要每次都重新绑定参数 告诉 opengl
        ib.unbind();
        va.unbind();
        vb.unbind();
        shader.unbind();

        Renderer renderer;

        while (!glfwWindowShouldClose(window))
        {
            handle_input(window);

            // rendering something
            // glClearColor(0.3f, 0.4f, 0.1f, 1.f);
            // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            // GLCALL(glUniform4f(location, r, g, b, a));
            shader.bind();
            shader.set_unifroms4f("u_Color", r, g, b, a);

            if (r > 1.0f)
            {
                increment = -0.02f;
            }
            else if (r < 0)
            {
                increment = 0.02f;
            }
            r += increment;
            // g += 0.001;
            // b += 0.001;

            glClear(GL_COLOR_BUFFER_BIT);

            // drawing
            // call draw 三角形 从 第一个数据顶点开始 需要 render 的 size 3 个
            // gl 其实知道 context
            // glUseProgram(shader);
            // glBindVertexArray(VAO);
            // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            // glDrawArrays(GL_TRIANGLES, 0, 3);

            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
            // gl_clear_error();                              // 清除下面方法之外的所有 error
            // glDrawElements(GL_TRIANGLES, 6, GL_INT, NULL); // 这种错误 只会得到个 黑屏 GL_INVALID_ENUM
            // // gl_check_error();
            // ASSERT(gl_log_call());

            renderer.draw(va, ib, shader);
            // va.bind();
            // vb.bind();
            // ib.bind();
            // GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

            // 为什么 unsigned

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // glDebugMessageCallback()

        // close GL context and any other GLFW resources
        // glDeleteVertexArrays(1, &VAO);
        // glDeleteBuffers(1, &buffer);
        // glDeleteProgram(shader);
        shader.unbind();
    }

    glfwTerminate();
    return 0;
}
