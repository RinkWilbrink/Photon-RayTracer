#pragma once
#include "notstd/Array2D.h"
#include "Vector/Vector3.h"
#include "Engine/Input/InputHandler.h"
using namespace RinkWilbrink;

namespace Tmpl8
{
    enum class JobState
    {
        NONE = 0, WORKING = 1, DONE = 2
    };
    struct RayTracingJob
    {
        void SetValues(int posx, int posy, uint32_t _Index, JobState _CurrentJobState)
        {
            posX = posx;
            posY = posy;

            m_Index = _Index;

            m_CurrentJobState = _CurrentJobState;
        }

        int posX = 0;
        int posY = 0;

        uint32_t m_Index = 0;

        JobState m_CurrentJobState = JobState::NONE;
    };

    class Surface;
    class Game
    {
    public:
        void SetTarget(Surface* surface) { screen = surface; }
        void Init();
        void Shutdown();
        void Tick(float deltaTime);
        void MouseUp(int button);       /* implement if you want to detect mouse button presses */
        void MouseDown(int button);     /* implement if you want to detect mouse button presses */
        void MouseMove(int x, int y);   /* implement if you want to detect mouse movement*/
        void KeyUp(int key);            /* implement if you want to handle keys */
        void KeyDown(int key);          /* implement if you want to handle keys */

        /// <summary>Reset Input values that need to be reset each frame.</summary>
        void InputReset() { inputHandler.ResetBools(); } // this gets called after the tick function gets called in template.cpp.

    private:
        /// <summary>This function contains the code that checks if the frame is done rendering, and submits its to the Screen.</summary>
        void CheckRayTracingCompletion();
        void ResetValuesForRayTracing();
        void SetNewScene(uint8_t _sceneIndex);
        void ScreenReset();
        void SetNewSceneValues();
        /// <summary>Display Performance Metrics on Screen.</summary>
        void PerformanceMetrics(const float& _deltaTime);
    
    private:
        Surface* screen{ nullptr };

        Engine::Input::InputHandler inputHandler = Engine::Input::InputHandler();

        notstd::Array2D<vec3> pixels = notstd::Array2D<vec3>(ScreenWidth, ScreenHeight);
    };
}; // namespace Tmpl8