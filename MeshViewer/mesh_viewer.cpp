#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"

#include "view_settings.h"

#include <iostream>

#include "model_loader.h"
#include "half_edge.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_btn_callback(GLFWwindow *window, int button, int action, int mods);

ViewSettings vs;

int main()
{
    // Mesh loading
    string model_path = "/Users/River/Desktop/Coding/Xcode/MeshViewer/MeshViewer/meshes/bunny.m";
    ModelLoader3F m_loader(model_path);
    Model3F* mesh = m_loader.load();
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(vs.SCR_WIDTH, vs.SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_btn_callback);
    
    // uncomment this line to hide cursor when the window is activated.
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader model_shader(
        "/Users/River/Desktop/Coding/Xcode/MeshViewer/MeshViewer/shaders/model_shader.vs",
        "/Users/River/Desktop/Coding/Xcode/MeshViewer/MeshViewer/shaders/model_shader.fs"
     );

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    // Include two attributes
    vector<ModelAttr<3>> attr_types = { ModelAttr<3>(AttrType::POINT), ModelAttr<3>(AttrType::NORMAL) };
    vector<float> points = mesh->getVertexArray(attr_types);
    vector<unsigned int> elements = mesh->getElementArray();

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points.size(), &points[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements.size(), &elements[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    /*
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    
    
    // wireframe related
    Shader wireframe_shader(
        "/Users/River/Desktop/Coding/Xcode/MeshViewer/MeshViewer/shaders/wireframe.vs",
        "/Users/River/Desktop/Coding/Xcode/MeshViewer/MeshViewer/shaders/wireframe.fs"
     );
    
    vector<float> line_array = mesh->getEdgeArray();
    
    unsigned int wf_VBO, wf_VAO;
    glGenVertexArrays(1, &wf_VAO);
    glGenBuffers(1, &wf_VBO);
    
    glBindVertexArray(wf_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, wf_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * line_array.size(), &line_array[0], GL_STATIC_DRAW);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindVertexArray(VAO);
        
        float currentFrame = glfwGetTime();
        vs.deltaTime = currentFrame - vs.lastFrame;
        vs.lastFrame = currentFrame;
        
        model_shader.use();
                        
        glm::mat4 view;
        view = glm::lookAt(vs.cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), vs.cameraUp);
        model_shader.setMat4("view", view);
        
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(vs.fov), (float)vs.SCR_WIDTH / (float)vs.SCR_HEIGHT, 0.1f, 100.0f);
        model_shader.setMat4("projection", projection);
        
        
        glm::mat4  model = glm::mat4(1.0f);
        // The sequence of apply is: tranlate -> scale
        
        // Compute scaling factor
        Vec3F side_lengths = mesh->getSideLengths();
        if (vs.moveToOrigin)
            side_lengths = side_lengths / 2;
        float max_length = numeric_limits<float>::min();
        for (int i = 0; i < 3; ++i) {
            if (abs(side_lengths[i]) > max_length)
                max_length = abs(side_lengths[i]);
        }
        float scale_factor = 0.2f / max_length;
        model = glm::scale(model, glm::vec3(scale_factor, scale_factor, scale_factor));
        if (vs.moveToOrigin) {
            Point3F center = mesh->getCenter();
            model = glm::translate(model, glm::vec3(-center.x(), -center.y(), -center.z()));
        }
        
        
        
        //float angle = 20.0f;
        //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.0f));
        
        model_shader.setMat4("model", model);
        model_shader.setVec3("lightPos", vs.lightPos);
        model_shader.setVec3("lightColor", vs.lightColor);
        model_shader.setVec3("objectColor", vs.objectColor);
        model_shader.setVec3("viewPos", vs.viewPos);
    
        glDrawElements(GL_TRIANGLES, GLsizei(elements.size()), GL_UNSIGNED_INT, (void*)0);
        
        
        // Draw wireframe
        wireframe_shader.use();
        glEnable(GL_LINE_WIDTH);
        glLineWidth(10.0f);
        wireframe_shader.setMat4("view", view);
        wireframe_shader.setMat4("projection", projection);
        wireframe_shader.setMat4("model", model);
        wireframe_shader.setVec3("lineColor", vs.wireframe_color);
        glBindVertexArray(wf_VAO);
        glDrawArrays(GL_LINES, 0, GLsizei(line_array.size() / 3));
        glDisable(GL_LINE_WIDTH);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 2.5 * vs.deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        vs.cameraPos += cameraSpeed * vs.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        vs.cameraPos -= cameraSpeed * vs.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        vs.cameraPos -= glm::normalize(glm::cross(vs.cameraFront, vs.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        vs.cameraPos += glm::normalize(glm::cross(vs.cameraFront, vs.cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!vs.leftBtnPressed)
        return;
    
    if (vs.leftBtnPressed && !vs.clickPtSet) {
        vs.prevPt = glm::vec2(xpos, ypos);
        vs.clickPtSet = true;
        return;
    }
    
    float x_offset = vs.prevPt.x - xpos;
    float y_offset = ypos - vs.prevPt.y;
    
    float sensitivity = 0.01;
    
    float dx = x_offset * sensitivity;
    float dy = y_offset * sensitivity;
    
    // Handling camera rotation
    // Handling x direction
    glm::vec3 cam_right = glm::normalize(glm::cross(vs.cameraFront, vs.cameraUp));
    glm::vec3 new_cam_pos = glm::normalize(vs.cameraPos + cam_right * dx) * vs.radius;
    glm::vec3 new_cam_dir = glm::normalize(glm::vec3(0) - new_cam_pos);
    glm::vec3 new_cam_up = vs.cameraUp;
    glm::vec3 new_cam_right = glm::normalize(glm::cross(new_cam_dir, new_cam_up));
    
    // handling y direction
    new_cam_pos = glm::normalize(new_cam_pos + new_cam_up * dy) * vs.radius;
    new_cam_dir = glm::normalize(glm::vec3(0) - new_cam_pos);
    new_cam_up = glm::normalize(glm::cross(new_cam_right, new_cam_dir));
    
    vs.cameraPos = new_cam_pos;
    vs.cameraUp = new_cam_up;
    vs.cameraFront = new_cam_dir;
    
    /*
    vs.angle_x += y_offset * sensitivity;
    
    if (vs.cameraUp.y >= 0.0f)
        vs.angle_y += x_offset * sensitivity;
    else
        vs.angle_y -= x_offset * sensitivity;
    
    if (vs.angle_x > 180.0f)
        vs.angle_x -= 360.0f;
    if (vs.angle_x < -180.0f)
        vs.angle_x += 360.0f;
    if (vs.angle_y > 360.0f)
        vs.angle_y -= 360.0f;
    if (vs.angle_y < -360.0f)
        vs.angle_y += 360.0f;
            
    vs.cameraPos.x = vs.radius * sin(glm::radians(vs.angle_y)) * cos(glm::radians(vs.angle_x));
    vs.cameraPos.y = vs.radius * sin(glm::radians(vs.angle_x));
    vs.cameraPos.z = vs.radius * cos(glm::radians(vs.angle_y)) * cos(glm::radians(vs.angle_x));
    
    // update cameraUp
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    if (vs.angle_x < 90.0f && vs.angle_x >= -90.f) {
        glm::vec3 up_dir = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 camera_dir = vs.cameraPos - cameraTarget;
        glm::vec3 camera_right = glm::cross(up_dir, camera_dir);
        vs.cameraUp = glm::normalize(glm::cross(camera_dir, camera_right));
    } else {
        glm::vec3 up_dir = glm::vec3(0.0f, -1.0f, 0.0f);
        glm::vec3 camera_dir = vs.cameraPos - cameraTarget;
        glm::vec3 camera_right = glm::cross(up_dir, camera_dir);
        vs.cameraUp = glm::normalize(glm::cross(camera_dir, camera_right));
    }*/
    
    vs.lightPos = vs.cameraPos;
    vs.viewPos = vs.cameraPos;
    
    vs.prevPt = glm::vec2(xpos, ypos);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    vs.fov -= (float)yoffset;
    if (vs.fov < 1.0f)
        vs.fov = 1.0f;
    if (vs.fov > 80.0f)
        vs.fov = 80.0f;
}

// glfw: callback of mouse click event
void mouse_btn_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        vs.leftBtnPressed = true;
    }
    else
    {
        vs.leftBtnPressed = false;
        vs.clickPtSet = false;
    }
}
