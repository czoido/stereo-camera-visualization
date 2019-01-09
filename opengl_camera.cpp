//
//  opengl_camera.cpp
//  stereo-visualization
//
//  Created by Carlos Zoido on 09/01/2019.
//

#include "opengl_camera.hpp"

OpenGLCamera::OpenGLCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Position(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(up), Yaw(yaw),Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    updateCameraVectors();
}

OpenGLCamera::OpenGLCamera(const OpenGLCamera& camera) : Position(camera.Position), Front(camera.Front), WorldUp(camera.WorldUp), Yaw(camera.Yaw),Pitch(camera.Pitch), MovementSpeed(camera.MovementSpeed), MouseSensitivity(camera.MouseSensitivity), Zoom(camera.Zoom) {
    updateCameraVectors();
}

OpenGLCamera::~OpenGLCamera(void) {
    
}

glm::mat4 OpenGLCamera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void OpenGLCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD)
        Position += Front * velocity;
    if (direction == Camera_Movement::BACKWARD)
        Position -= Front * velocity;
    if (direction == Camera_Movement::LEFT)
        Position -= Right * velocity;
    if (direction == Camera_Movement::RIGHT)
        Position += Right * velocity;
    if (direction == Camera_Movement::UP)
        Position += Up * velocity;
    if (direction == Camera_Movement::DOWN)
        Position -= Up * velocity;
    
    printPos();
}

void OpenGLCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    Yaw   += xoffset;
    Pitch += yoffset;
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    updateCameraVectors();
}

void OpenGLCamera::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;
}

void OpenGLCamera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

void OpenGLCamera::printPos() const {
    std::cout << "pos: " << Position.x << "," << Position.y  << "," << Position.z << std::endl;
    std::cout << "front: " << Front.x << "," << Front.y  << "," << Front.z << std::endl;
    std::cout << "up: " << WorldUp.x << "," << WorldUp.y  << "," << WorldUp.z << std::endl;
    std::cout << "yaw: " << Yaw << ", pitch: " << Pitch  << std::endl;
}
