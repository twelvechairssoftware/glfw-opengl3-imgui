#include"Camera.h"



Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projection * view;
}


void Camera::Matrix(Shader& shader, const char* uniform)
{
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::scrollCallback(){

}

void Camera::relevelView(){
    if (Position.y < 0.0){
        Position.y = 0.0;
//        Orientation.y = 0.0;
    }
}

void Camera::resetView(){
    Orientation = glm::vec3(0.553, 0.270f, -0.844f);
    Position = glm::vec3(-0.931f, 0.0f, 4.174f);
}


void Camera::Inputs(GLFWwindow* window, ImVec2 glWindowPosition, ImVec2 glWindowSize)
{
    ImVec4 glWindowCoordinates = ImVec4(glWindowPosition.x, glWindowPosition.y, glWindowPosition.x + glWindowSize.x, glWindowPosition.y + glWindowSize.y);
    // Handles key inputs
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        Position += speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        Position += speed * -Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }

    relevelView();

//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//    {
//        Position += speed * Up;
//    }
//    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
//    {
//        Position += speed * -Up;
//    }
//    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//    {
//        speed = 0.4f;
//    }
//    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
//    {
//        speed = 0.1f;
//    }


    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double mouseX;
        double mouseY;

        float padding = 20.0f;
        glWindowCoordinates.x += padding;
        glWindowCoordinates.y += padding;
        glWindowCoordinates.z -= padding;
        glWindowCoordinates.w -= padding;

        glfwGetCursorPos(window, &mouseX, &mouseY);
        if ((glWindowCoordinates.x < mouseX && mouseX < glWindowCoordinates.z) && (glWindowCoordinates.y < mouseY && mouseY < glWindowCoordinates.w)){
            // Hides mouse cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            // Prevents camera from jumping on the first click
            float z = (glWindowCoordinates.x + glWindowCoordinates.z) / 2;
            float w = (glWindowCoordinates.y + glWindowCoordinates.w) / 2;
            if (firstClick)
            {
//                glfwSetCursorPos(window, z, w);
                firstClick = false;
            }


            // Stores the coordinates of the cursor

            // Fetches the coordinates of the cursor

            // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
            // and then "transforms" them into degrees
            float rotX = sensitivity * (float)(mouseY - (glWindowCoordinates.w / 2)) / glWindowCoordinates.z;
            float rotY = sensitivity * (float)(mouseX - (glWindowCoordinates.z / 2)) / glWindowCoordinates.w;

            // Calculates upcoming vertical change in the Orientation
            glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

            // Decides whether the next vertical Orientation is legal or not
            if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
            {
                Orientation = newOrientation;
            }

            // Rotates the Orientation left and right
            Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
        }

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
//        float z = (glWindowCoordinates.x + glWindowCoordinates.z) / 2;
//        float w = (glWindowCoordinates.y + glWindowCoordinates.w) / 2;
//        glfwSetCursorPos(window, z, w);

    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }
}