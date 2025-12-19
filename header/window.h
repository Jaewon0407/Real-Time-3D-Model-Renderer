#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();
    
    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();
    void getFramebufferSize(int& width, int& height) const;
    GLFWwindow* getHandle() const { return m_window; }
    
private:
    GLFWwindow* m_window;
};