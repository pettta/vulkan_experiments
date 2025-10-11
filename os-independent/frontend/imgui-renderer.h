#pragma once

#include <functional>

#include "imgui.h"

class ImGuiBackend {
public:
    virtual ~ImGuiBackend() = default;

    virtual void initializeBackend() = 0;
    virtual void newFrame() = 0;
    virtual void renderDrawData(ImDrawData* drawData) = 0;
    virtual void shutdownBackend() = 0;
};

class ImGuiRenderer {
public:
    explicit ImGuiRenderer(ImGuiBackend& backend);
    ~ImGuiRenderer();

    void initialize();
    ImGuiIO& io();

    void beginFrame();
    void businessLogic(); 
    void endFrame();
    void renderFrame(const std::function<void()>& drawUI);

    void shutdown();

private:
    ImGuiBackend& backend_;
    bool initialized_{false};
};
