/**
 * -d directory of multi-files --tff transfer function data -f single data file
*/
#include "include/libs.h"
#include <bitset>

#include <string>

#define X_axis 0
#define Y_axis 1
#define Z_axis 2

double lastTime = glfwGetTime();
int frames = 0;

int g_angle_horizontal = 0; // y
int g_angle_vertical = 90;  // x

bool update_mvp = false;

glm::vec3 g_eye_pos(0.0f, 0.0f, 2.0f); // z 2.0 init

void horizontal_rotate(int angle = 1)
{
    g_angle_horizontal = (g_angle_horizontal + angle) % 360;
}

void vertical_rotate(int angle = 1)
{
    g_angle_vertical = (g_angle_vertical + angle) % 360;
}

void move_eye(float length = 0.05, int axis = Z_axis)
{
    glm::vec3 direction(0.f, 0.f, 0.f);
    direction[axis] = length;
    g_eye_pos += direction;
}

// 非得这么写 declaration
void window_resize_callback(GLFWwindow *window, const int width, const int height);
void padding_viewport(const int width, const int height, const int padding);

void window_resize_callback(GLFWwindow *window, const int width, const int height)
{
    std::cout << "resizeing to " << width << "*" << height << std::endl;
    glViewport(0, 0, width, height);
    // padding_viewport(width, height, 10);
}

void padding_viewport(const int width, const int height, const int padding = 10)
{
    glViewport(padding, padding, width - padding, height - padding);
}

void handle_input(GLFWwindow *window)
{
    int rotate_delta_deg = 3;
    update_mvp = true;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        // hit space change color
        // glClearColor(1.f, 1.f, 1.f, 1.f);
        std::cout << "归零!" << std::endl;
        g_angle_horizontal = 0;
        g_angle_vertical = 0;
        g_eye_pos = glm::vec3(0.0f, 0.0f, 2.0f); // z 2.0 init
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        std::cout << "on key left press!" << std::endl;
        horizontal_rotate(rotate_delta_deg);
        std::cout << "current angle on y axis: " << g_angle_horizontal << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        std::cout << "on key right press!" << std::endl;
        horizontal_rotate(-rotate_delta_deg);
        std::cout << "current angle on y axis: " << g_angle_horizontal << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        std::cout << "on key up press!" << std::endl;
        vertical_rotate(-rotate_delta_deg);
        std::cout << "current angle on x axis: " << g_angle_vertical << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        std::cout << "on key down press!" << std::endl;
        vertical_rotate(rotate_delta_deg);
        std::cout << "current angle on x axis: " << g_angle_vertical << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        std::cout << "go forward along z(negative)!" << std::endl;
        move_eye(-.05, Z_axis);
        std::cout << "current angle on x axis: " << g_angle_vertical << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        std::cout << "go forward along z!" << std::endl;
        move_eye(.05, Z_axis);
        std::cout << "current angle on x axis: " << g_angle_vertical << std::endl;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        std::cout << "go forward along x(negative)!" << std::endl;
        move_eye(-.05, X_axis);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        std::cout << "go forward along x!" << std::endl;
        move_eye(.05, X_axis);
    }
    else
    {
        update_mvp = false;
    }
}

float angle2radian(const float &ang)
{
    return ang * glm::pi<float>() / 180.0;
}

void show_fps(GLFWwindow *window)
{
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    frames++;
    if (delta >= 1.0)
    {
        // If last cout was more than 1 sec ago
        std::cout << "million-second per frame: " << 1000.0 / double(frames) << std::endl;

        double fps = double(frames) / delta;

        std::stringstream ss;
        ss << " [" << fps << " FPS]";

        glfwSetWindowTitle(window, ss.str().c_str());

        frames = 0;
        lastTime = currentTime;
    }
}

glm::mat4 getMVP(int width, int height)
{

    // 来个投影矩阵 正交变换矩阵 6 个参数 left right bottom top narZ farZ 关于 viewport plane 的
    // 给定这些参数 就能创建一个 正交矩阵 game101 C4 学过哦
    glm::mat4 proj = glm::mat4(1.f);
    glm::mat4 view = glm::mat4(1.f);

    //  transform the box
    // proj = glm::perspective(.9f, (float)width / height, .1f, 200.f);
    // fovy aspect near far
    proj = glm::perspective(glm::radians(45.f), float(width) / float(height), 0.1f, 400.f);

    // glm::quat q(1.f, 2.f, 3);  // w x y z!!
    view = glm::lookAt(g_eye_pos,
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 model = glm::mat4(1.f);
    // y 轴 旋转
    model *= glm::rotate(glm::mat4(1.f), glm::radians(float(g_angle_horizontal)), glm::vec3(0.0f, 1.0f, 0.0f));
    // x 轴 旋转
    model *= glm::rotate(glm::mat4(1.f), glm::radians(float(g_angle_vertical)), glm::vec3(1.0f, 0.f, 0.0f));
    // to make the "head256.raw" i.e. the volume data stand up.
    model *= glm::rotate(glm::mat4(1.f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // x axis 90 deg
    // model *= glm::rotate(glm::mat4(1.f), angle2radian(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // z axis 45 deg
    model *= glm::translate(glm::mat4(1.f), glm::vec3(-0.5f, -0.5f, -0.5f));
    // notice the multiplication order: reverse order of transform

    // proj = glm::ortho(-200.f, 200.f, -150.f, 150.f, -1.f, 1.f);
    // view 矩阵 移动 camera 实际上是对物体做 reverse 的移动
    // camera 向左 50 => 物体向右 50 对一个 I 做 translate 变换
    // view = glm::translate(glm::mat4(1.f), glm::vec3(50.f, 0.f, 0.f));
    // model = glm::translate(glm::mat4(1.f), glm::vec3(-80.f, 20.f, 0.f));
    // glm::mat4 model = glm::rotate(glm::mat4(1.f), 0.22f, glm::vec3(1, 1, 0));
    glm::mat4 mvp = proj * view * model;

    return mvp;
}

int main(int argc, char **argv)
{
    bool render_face = true;
    if (argc >= 2)
    {
        // for (size_t i = 0; i < argc; i++)
        // {
        //     std::cout << "III---" << i << " > " << argv[i] << std::endl;
        // }
        std::string face_str(argv[1]);
        if (face_str.compare("noface") == 0)
        {
            std::cout << "III---noface" << std::endl;
            render_face = false;
        }
    }

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit())
    {
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        return -1;
    }

    // Setting window properties  opengl 4.1 on mac
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // APPLE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 开启 core profile

    unsigned int width = 600;
    unsigned int height = 600;

    // Actually create the window
    GLFWwindow *window = glfwCreateWindow(width, height, "volume rendering", NULL, NULL);
    if (!window)
    {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, window_resize_callback);

    // swap 的频率 vsync
    // the number of screen updates to wait from the time
    // glfwSwapBuffers was called before swapping the buffers and returning.
    // This is sometimes called vertical synchronization,
    // vertical retrace synchronization or just vsync.
    // 翻译一下
    // glfwSwapInterval(1);

    const std::string textures_path = "../resource/textures/";
    const std::string shaders_path = "../resource/shaders/";

    // padding_viewport(width, height);
    // glViewport(0, 0, width, height);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // version as a string
    std::cout << ">>> Renderer: " << renderer << std::endl;
    std::cout << ">>> OpenGL version supported " << version << std::endl;

    // actually 这些 vertices 只有 position 属性
    // 如果有其他属性 那么在 attrib point 的时候就要 计算其他步长之类的计算
    // 如果要再传入一个 xyz 给 location 需要 double 一下数据...
    // float vertices[24] = {
    //     0.0, 0.0, 0.0,
    //     0.0, 0.0, 1.0,
    //     0.0, 1.0, 0.0,
    //     0.0, 1.0, 1.0,
    //     1.0, 0.0, 0.0,
    //     1.0, 0.0, 1.0,
    //     1.0, 1.0, 0.0,
    //     1.0, 1.0, 1.0};
    float vertices[48] = {
        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 1.0, 1.0, 0.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    // draw the six faces of the boundbox by drawwing triangles
    // draw it contra-clockwise
    // front: 1 5 7 3
    // back: 0 2 6 4
    // left: 0 1 3 2
    // right:7 5 4 6
    // up: 2 3 7 6
    // down: 1 0 4 5
    unsigned int indices[36] = {
        // counter-clockwise for cull face
        1, 5, 7,
        7, 3, 1,
        0, 2, 6,
        6, 4, 0,
        0, 1, 3,
        3, 2, 0,
        7, 5, 4,
        4, 6, 7,
        2, 3, 7,
        7, 6, 2,
        1, 0, 4,
        4, 5, 1};

    float vertices2[] = {
        //World					//Color
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  //v0
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   //v1
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,    //v2
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   //v3
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //v4
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,  //v5
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,   //v6
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  //v7
    };

    unsigned int indices2[36] = {
        0, 1, 2, 0, 2, 3, //front
        4, 7, 6, 4, 6, 5, //back
        4, 0, 3, 4, 3, 7, //left
        1, 5, 6, 1, 6, 2, //right
        3, 2, 6, 3, 6, 7, //top
        4, 5, 1, 4, 1, 0, //bottom
    };

    {
        // gl 如何 blend 获取 GL_SRC_ALPHA 在这之上的 alpha' = 1 - alpha
        // glBlendFunc: src -> dest 新的颜色 -> 目标的颜色 然后加在原始的颜色上
        // 取 src 的 alpha 用 1 减去这个值 得到目标颜色的 alpha
        // 最后求和
        // GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        // glBlendFunc src 默认是 1 dest 默认是 0
        // src 永远乘 1 不变 dest 直接乘 0 没用了 也就是丢弃 原来的 dest 用 src 去覆盖
        // glBlendEquation(GL_ADD); // 默认的混合方式就是 add 将 src 加到 dest 上
        // GLCALL(glEnable(GL_BLEND));
        // call deconstructor
        VertexBuffer vb(vertices, sizeof(vertices));
        IndexBuffer ib(indices, 36);
        VertexArray va;
        VertexBufferLayout layout;
        layout.push<float>(3); // layout 3D xyz => 3
        layout.push<float>(3); // layout 3D color => 3 the same to xyz
        // layout.push<float>(2); // layout 3D texture coord => 2
        va.add_buffer(vb, layout);

        glm::mat4 mvp = getMVP(width, height);
        Shader face_shader(shaders_path + "face.shader");
        // Shader raycast_shader(shaders_path + "raycast_tff.shader");
        Shader raycast_shader(shaders_path + "raycast_gray.shader");
        face_shader.bind();

        face_shader.set_unifroms_mat4f("u_MVP", mvp);
        raycast_shader.bind();
        raycast_shader.set_unifroms_mat4f("u_MVP", mvp);
        // face_shader.set_unifroms2f("u_ScreenSize", float(width), float(height));

        Texture1D transfer_function("../resource/textures/tff.dat");
        // Texture2D bf_texture("../resource/textures/ttt.png", true);
        Texture3D face_texture(render_face);

        // 需要在 loop 中 重新 bind
        // 画不同 obj 的时候 我们有必要每次都重新绑定参数 告诉 opengl
        ib.unbind();
        va.unbind();
        vb.unbind();
        face_shader.unbind();
        raycast_shader.unbind();
        // glViewport(0, 0, width, height);

        Renderer renderer;
        FrameBuffer frame(width, height);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        while (!glfwWindowShouldClose(window))
        {
            // glViewport(100, 0, width, height);
            glViewport(0, 0, width * 2, height * 2);

            handle_input(window);
            if (update_mvp)
            {
                std::cout << "new mvp" << std::endl;
                mvp = getMVP(width, height);
                update_mvp = false;
            }

            /************************* draw back face to buffer */
            face_shader.bind();
            // 要先 bind 才能 set uniform
            renderer.clear();
            frame.bind();

            face_shader.set_unifroms_mat4f("u_MVP", mvp);
            renderer.draw(va, ib, face_shader, true, GL_FRONT, false);
            frame.unbind();
            face_shader.unbind();

            /************************* draw cube for volume */
            raycast_shader.bind();
            renderer.clear();

            raycast_shader.set_unifroms2f("u_ScreenSize", float(width), float(height));
            raycast_shader.set_unifroms_mat4f("u_MVP", mvp);

            // set texture uniforms
            transfer_function.bind(0);
            frame.bind_color_buffer_to_texture(1);
            // bf_texture.bind(1);
            face_texture.bind(2);
            raycast_shader.set_unifroms1i("u_TransferFunc", 0);
            raycast_shader.set_unifroms1i("u_bfTexture", 1); // texture => slot 0
            raycast_shader.set_unifroms1i("u_FaceTexture", 2);

            renderer.draw(va, ib, raycast_shader, true, GL_BACK, true);

            raycast_shader.unbind();
            face_texture.unbind();

            /************************* end drawing  */

            // window idle stuff
            glfwSwapBuffers(window);
            glfwPollEvents();
            show_fps(window);
        }
        // close GL context and any other GLFW resources
        face_shader.unbind();
        raycast_shader.unbind();
    }

    glfwTerminate();
    return 0;
}
