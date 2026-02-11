#include "Camera3D.h"
#include <cmath>
#include <GLFW/glfw3.h>

Camera3D::Camera3D(GLFWwindow* win, float width, float height)
    : window_(win), width_(static_cast<int>(width)), height_(static_cast<int>(height))
{
}

void Camera3D::setWindowSize(int w, int h)
{
    width_ = w;
    height_ = h;
}

void Camera3D::cursorPosCallback(double xpos, double ypos)
{
    if (firstMouse_)
    {
        lastX_ = static_cast<float>(xpos);
        lastY_ = static_cast<float>(ypos);
        firstMouse_ = false;
        return;
    }

    float xoffset = static_cast<float>(xpos) - lastX_;
    float yoffset = lastY_ - static_cast<float>(ypos); // reversed since y-coordinates go from top to bottom
    lastX_ = static_cast<float>(xpos);
    lastY_ = static_cast<float>(ypos);

    xoffset *= sensitivity_;
    yoffset *= sensitivity_;

    if (orbitMode_)
    {
        if (rotating_)
        {
            orbitYaw_ += xoffset * 0.5f;
            orbitPitch_ += yoffset * 0.5f;
            if (orbitPitch_ > 89.0f) orbitPitch_ = 89.0f;
            if (orbitPitch_ < -89.0f) orbitPitch_ = -89.0f;
        }
    }
    else
    {
        // first-person style
        yaw_ += xoffset;
        pitch_ += yoffset;
        if (pitch_ > 89.0f) pitch_ = 89.0f;
        if (pitch_ < -89.0f) pitch_ = -89.0f;
    }
}

void Camera3D::mouseButtonCallback(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        rotating_ = (action == GLFW_PRESS);
        if (action == GLFW_PRESS)
        {
            firstMouse_ = true; // reset delta on press
        }
    }
}

void Camera3D::scrollCallback(double xoffset, double yoffset)
{
    if (orbitMode_)
    {
        orbitRadius_ -= static_cast<float>(yoffset) * 20.0f;
        if (orbitRadius_ < 50.0f) orbitRadius_ = 50.0f;
        if (orbitRadius_ > 2000.0f) orbitRadius_ = 2000.0f;
    }
    else
    {
        posZ_ -= static_cast<float>(yoffset) * 20.0f;
        if (posZ_ < 1.0f) posZ_ = 1.0f;
    }
}

void Camera3D::update(float deltaTime)
{
    // For this minimal camera, we simply compute derived position if in orbit mode.
    if (orbitMode_)
    {
        float radYaw = orbitYaw_ * 3.14159265f / 180.0f;
        float radPitch = orbitPitch_ * 3.14159265f / 180.0f;
        posX_ = orbitRadius_ * std::cos(radPitch) * std::cos(radYaw);
        posY_ = orbitRadius_ * std::sin(radPitch);
        posZ_ = orbitRadius_ * std::cos(radPitch) * std::sin(radYaw);
    }
    else
    {
        // In first-person we don't move automatically; movement could be added.
    }

    // Here we would normally upload view/proj matrices to shaders; for safety we do nothing.
    (void)deltaTime;
}

void Camera3D::toggleMode()
{
    orbitMode_ = !orbitMode_;
}
