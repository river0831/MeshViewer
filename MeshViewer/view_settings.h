//
//  view_controls.h
//  MeshViewer
//
//  Created by Jiang Jie on 10/6/23.
//

#ifndef view_settings_h
#define view_settings_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ViewSettings {
public:
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    const unsigned int SCR_CENTER_X = SCR_WIDTH / 2;
    const unsigned int SCR_CENTER_Y = SCR_HEIGHT / 2;

    // Mouse control related
    bool firstMouse = true;
    float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch =  0.0f;
    float lastX =  800.0f / 2.0;
    float lastY =  600.0 / 2.0;
    float fov   =  45.0f;
    
    // delta time track
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Default camera view
    float radius = 1.0f;
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  radius);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    // Angle rotation
    float angle_x = 0.0f;
    float angle_y = 0.0f;

    // Button event
    bool leftBtnPressed = false;
    bool clickPtSet = false;
    glm::vec2 prevPt = glm::vec2(0.0f, 0.0f);
    
    // Lighting settings
    glm::vec3 lightPos = cameraPos; // light position. Initialized to the camera position.
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 viewPos = cameraPos;
    
    // Wireframe settings
    glm::vec3 wireframe_color = glm::vec3(0.0f, 0.0f, 0.0f);
    
    
    // Model controls
    bool moveToOrigin = true;
};


#endif /* view_controls_h */
