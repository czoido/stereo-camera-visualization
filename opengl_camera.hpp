//
//  opengl_camera.hpp
//  stereo-visualization
//
//  Created by Carlos Zoido on 09/01/2019.
//

#ifndef opengl_camera_hpp
#define opengl_camera_hpp

#include <stdio.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROTATE_LEFT,
    ROTATE_RIGHT,
    UP,
    DOWN
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  0.5f;
const float SENSITIVITY =  0.05f;
const float ZOOM        =  45.0f;

class OpenGLCamera
{
public:
    OpenGLCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH);
    
    OpenGLCamera(const OpenGLCamera& camera);
    ~OpenGLCamera(void);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);
    glm::mat4 GetViewMatrix() const;
    float getZoom() const {return Zoom;};
    glm::vec3 getPosition() const {return Position;};
    glm::vec3 getFront() const {return Front;};
    glm::vec3 getUp() const {return Up;};
    glm::vec3 getRight() const {return Right;};
    glm::vec3 getWorldUp() const {return WorldUp;};

private:
    void updateCameraVectors();
    void printPos() const;
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    const glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};

#endif /* opengl_camera_hpp */
