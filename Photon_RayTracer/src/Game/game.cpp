#include "precomp.h"
#include "Game\game.h"
#include "Template\surface.h"

#include "RayTracer/Camera/Camera.h"
#include "RayTracer/Colour/Colour.h"
#include "RayTracer/Scene/Scene.h"

#include "RayTracer/LightSource/LightSource.h"

/* Scenes:
* 1. A lot of random spheres, a performance test
* 2. displaying material types, and coloured metals and dielectrics
* 3. displaying a different sky colour, like an evening sky colour gradient.
* 4. This scene is ridicoulus and is disabled by default (enable the pre processor statement below that defines EnableMassiveScene to test it),
     This scenes adds a lot of time to the initial loading because it needs to construct the BVH for the scene, 
     but it's a cool performance test.
*/
#if false
#define EnableMassiveScene
#endif

/* Multi-Threading
 * 
pros:
 * It utilizes cores on your cpu that other wouldn't be used, these cores can handle a lot of instructions per second and not using all
 * cores available requires a single core to do all the work, when this can quite easily be split up, thus increasing runtime performance.

cons:
 * It's not an easy concept to grasp at first, and multi-threading requires you to put great thought into how you handle data,
 * Thread safety is very important because if more then one thread is changing the same piece of data at the same time, 
 * it can then result in incorrect or corrupt data.

 * From my conclusion, the extra performance gained by Multi-Threading massivly outweights the added difficuly of Multi-Threading.
 * It has also been a great learning experience, since Multi-Threading is becomming more and more industry standard
 * and even a requirement for certain job applications within the gaming industry.

 * This can be prevented by using atomic operators (which happens to be the approach I chose),
 * The reason I chose this approach was because it wasn't that hard to understand and implement, it's also the approach that got recommended to me
 * by another student who had implemented this approach before I did, 
 * an atomic operator pauses all other threads for (ussualy one cycle) when changing its data like addition or subtraction.

 * another option I saw online was shared pointers but shared pointers are not recommended because 
 * they can massivly slow down you program.
*/
#if true
#define MULTITHREADING
#include <thread>
#include <mutex>
#include <condition_variable>
#endif

// Game

namespace Tmpl8
{
    // Image
    const int ImageWidth  = 512;
    const int ImageHeight = 512;
    const float ImageRatio = ImageWidth / ImageHeight;

    const int MaxSamplesPerImage = 1000;
    const int max_depth = 20;

    // Camera
    Camera camera;

    // Anti Aliasing
    int m_AntiAliasingCount = 0;

    // Lower Resolution Rendering
    bool enableLowResRendering = false;

    // Scene stuff
    Scene scene1 = Scene();

    //BVH
    const bool UseBVH = true;

    // Switch Scene
    uint8_t SceneIndex = 0;
    std::vector<HittableList> scenes = std::vector<HittableList>();

    // Multi Threading
    const int TileSize = 16;
    const int TileCount_X = ImageWidth / TileSize;
    const int TileCount_Y = (ImageHeight / TileSize);

#ifdef MULTITHREADING
    const int ThreadCount = std::thread::hardware_concurrency() - 1;
#else
    const int ThreadCount = 1;
#endif
    notstd::Array<std::thread> threads = notstd::Array<std::thread>(ThreadCount);

    // Screen Buffer
    Colour* ScreenBuffer = nullptr;

    const int JobsSize = TileCount_X * TileCount_Y;
    notstd::Array<RayTracingJob> jobs = notstd::Array<RayTracingJob>(JobsSize);

    // Tile based Specific Variables.
    std::atomic<int> JobsToDo;

    //------------------------------Functionality------------------------------\\

    // Input
    void Game::MouseUp(int button) { inputHandler.MouseUp(button); }
    void Game::MouseDown(int button) { inputHandler.MouseDown(button); }
    void Game::MouseMove(int x, int y) { inputHandler.MouseMovement(x, y); }
    void Game::KeyUp(int key) { inputHandler.KeyUp(key); }
    void Game::KeyDown(int key) { inputHandler.KeyDown(key); }

    float Reflectance(float _CoSine, float _RefIdx)
    {
        /* Schlick's approximation for Reflectance.
         * https://en.wikipedia.org/wiki/Schlick%27s_approximation
        */
        float r0 = (1.0f - _RefIdx) / (1.0f + _RefIdx);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * powf((1.0f - _CoSine), 5);
    }

    Colour RayColour(const Ray& _Ray, const HittableList& _world, int _depth)
    {
        HitRecord rec;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (_depth <= 0)
        {
            return Colour(0, 0, 0);
        }
        if (_world.hit(_Ray, Epsilon, infinity, rec))
        {
            // handle materials here
            Colour albedo = rec.m_pMaterial->m_Albedo;
            float lambertian = rec.m_pMaterial->a_Lambertian;
            float metal = rec.m_pMaterial->a_Metal;
            float specular = rec.m_pMaterial->a_Specular;
            float dielectric = rec.m_pMaterial->a_Dielectric;

            // Set Colours
            Colour attenuation = Colour::Zero();
            Colour shadowColour = Colour::Zero();
            Colour materialColour = albedo * 0.3f;

            // Calculate the angle between normal and light source
            vec3 lightDirection = (_world.pointLight.m_Position - rec.m_HitPoint).Normalized();
            float lDOTn = Dot(rec.m_Normal, lightDirection);
            Ray shadowRay = Ray(rec.m_HitPoint, lightDirection, 0.0f);

            // Check lambertian code stuff here
            if (lambertian > Epsilon)
            {
                if (metal > Epsilon || dielectric > Epsilon)
                {
                    attenuation += albedo * lambertian * 0.1f;
                }
                else
                {
                    if (_world.hit(shadowRay, Epsilon, infinity, rec))
                    {
                        shadowColour = Colour::Zero();
                    }
                    else
                    {
                        shadowColour = Colour::One() - lambertian;
                        shadowColour = _world.pointLight.m_Intesity * (1.0f / lightDirection.sqrLenght())
                                     * _world.pointLight.m_Colour * Max(lDOTn, 0.0f);
                    }
                    attenuation += albedo * lambertian * (0.3f * shadowColour);
                }
            }

            /* Sources:
             * For the Phong Model found below I used the following link.
             * https://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_2_Phong_Mirrors_and_Shadows.shtml
            */
            if (specular > Epsilon)
            {
                vec3 L = (_world.pointLight.m_Position - rec.m_HitPoint).Normalized();
                vec3 N = (rec.m_Normal);

                vec3 V = _Ray.m_Direction;
                vec3 R = L - 2.0f * Dot(L, N) * N;
                float dot = Dot(V, R);
                if (dot > 0.0f)
                {
                    float spec = powf(dot, 20.0f) * specular;
                    attenuation += (spec * _world.pointLight.m_Colour) * spec;
                }
            }

            if (metal > Epsilon)
            {
                // Get the reflected vector
                vec3 reflected = Reflect(UnitVector(_Ray.m_Direction), rec.m_Normal).Normalized();
                // Get the colour of the reflection
                Colour reflectedColour = RayColour(Ray(rec.m_HitPoint, reflected, 0.0f), _world, _depth - 1);
                // Add colour to attenuation for the final colour.
                attenuation += (reflectedColour * _world.pointLight.m_Intesity) * metal;
            }

            /* Sources:
            * Refractions and Glass:
            * https://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_3_Refractions_and_Beers_Law.shtml
            * 
            */
            if (dielectric > Epsilon)
            {
                vec3 rayDirection;
                Colour refractedColour;

                vec3 unitDirection = UnitVector(_Ray.m_Direction);
                float refractionRatio = rec.front_face ? (1.0f / dielectric) : dielectric;
                float cosTheta = fmin(Dot(-unitDirection, rec.m_Normal), 1.0f);
                float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
                bool refractOrReflect = refractionRatio * sinTheta > 1.0f;

                // Add random-ness to the Dielectric
                if (refractOrReflect || Reflectance(cosTheta, refractionRatio) > random_float())
                {
                    rayDirection = Reflect(unitDirection, rec.m_Normal);
                }
                else
                {
                    rayDirection = Refract(unitDirection, rec.m_Normal, refractionRatio);
                }

                // return the Ray 
                attenuation += RayColour(Ray(rec.m_HitPoint, rayDirection, 0.0f), _world, _depth - 1);
            }
            // Return the final Colour
            return attenuation;
        }

        vec3 unitDirection = UnitVector(_Ray.m_Direction);
        float t = 0.5f * (unitDirection.y + 1.0f);
        // Return Sky Colour
        return (1.0f - t) * _world.m_LowerSkyColour + t * _world.m_UpperSkyColour * _world.pointLight.m_Intesity;
    }

    bool CanTraceRays = true;
    std::mutex MutexThing;
    std::condition_variable iterationLock;
    void RenderSection(int tileIndex)
    {
        while (CanTraceRays)
        {
            int currentJobIndex = JobsToDo.fetch_sub(1);
            HittableList& world = scenes[SceneIndex];

            if (currentJobIndex < 0)
            {
                // when no tiles are available, lock thread until new frame starts for new AA Iteration.
                std::unique_lock<std::mutex> lock(MutexThing);
                iterationLock.wait(lock);
                continue;
            }

            RayTracingJob* currentjob = &jobs[currentJobIndex];
            currentjob->m_CurrentJobState = JobState::WORKING;

            float random = random_float();
            float tempX = 1.0f / (ImageWidth - 1);
            float tempY = 1.0f / (ImageHeight - 1);
            for (uint32_t y = currentjob->posY; y < (currentjob->posY + TileSize); y++)
            {
                for (uint32_t x = currentjob->posX; x < (currentjob->posX + TileSize); x++)
                {
                    float u = (x + random) * tempX;
                    float v = (y + random) * tempY;
                    Ray r = camera.GetRay(u, v);

                    // Write Pixel data to Screen Buffer.
                    Colour col = RayColour(r, world, max_depth);
                    ScreenBuffer[x * ImageHeight + y] += col;

                    if (enableLowResRendering)
                    {
                        ScreenBuffer[(x) * ImageHeight + (y + 1)] += col;
                        ScreenBuffer[(x + 1) * ImageHeight + (y)] += col;
                        ScreenBuffer[(x + 1) * ImageHeight + (y + 1)] += col;
                        x += 1;
                    }
                }
                if (enableLowResRendering)
                {
                    y += 1;
                }
            }

            // Set Variables
            currentjob->m_CurrentJobState = JobState::DONE;
        }
    }

    void Game::Init()
    {
        printf("Loading...\n");

        // Set value of JobsToDo Atomic Variable
        JobsToDo.store(JobsSize - 1);

        scenes.push_back(scene1.CreateRandomScene(UseBVH, 283)); // 283 = Scene with ~80.000 Spheres
        scenes[0].SetCameraValues(Point3(-2, 5, -2), Point3(5, 4, 5), vec3(0, 1, 0), 60.0f, ImageRatio, 10.0f, 0.0f);

        scenes.push_back(scene1.ColouredMaterialsShowCase(UseBVH));
        scenes[1].SetCameraValues(Point3(0, 2, -10), Point3(0, 1, 0), vec3(0, 1, 0), 60.0f, ImageRatio, 10.0f, 0.0f);
        
        scenes.push_back(scene1.DielectricUpClose(UseBVH, true));
        scenes[2].SetCameraValues(Point3(0, 0, -10), Point3(0, 1, 0), vec3(0, 1, 0), 30.0f, ImageRatio, 10.0f, 0.0f);
        scenes[2].SetAthmosSphereValues(Colour(1.0f, 0.1f, 0.1f), Colour(0.5f, 0.7f, 0.9f));
        
        scenes.push_back(scene1.DielectricUpClose(UseBVH, false));
        scenes[3].SetCameraValues(Point3(0, 0, -10), Point3(0, 1, 0), vec3(0, 1, 0), 25.0f, ImageRatio, 10.0f, 0.0f);
        scenes[3].SetAthmosSphereValues(Colour(0.9f, 0.1f, 0.1f), Colour(0.5f, 0.7f, 0.9f));

#ifdef EnableMassiveScene
        // Insane number of spheres here, kinda not necesary and adds a lot of loading time at the start but its cool non the less.
        scenes.push_back(scene1.CreateRandomScene(UseBVH, 600)); // 283 = Scene with ~80.000 Spheres
        scenes[4].SetCameraValues(Point3(-2, 5, -2), Point3(5, 4, 5), vec3(0, 1, 0), 60.0f, ImageRatio, 10.0f, 0.0f);
#endif
        
        // Initialize ScreenBuffer.
        ScreenBuffer = new Colour[ImageWidth * ImageHeight];
        // Set the whole screen to black
        memset(ScreenBuffer, 0.1f, ImageWidth * ImageHeight * sizeof(Colour));

        // Set Default Values
        SetNewSceneValues();

        printf("Loading Completed!\n");

        // Set Values for each Tile(Tile location, size etc).
        for (size_t y = 0; y < TileCount_Y; y++)
        {
            for (size_t x = 0; x < TileCount_X; x++)
            {
                int i = x + TileCount_X * y;
                jobs[i].SetValues(x * TileSize, y * TileSize, i, JobState::NONE);
            }
        }

        // Start the Threads to Render the Tiles.
        for (uint8_t i = 0; i < threads.get_size(); i++)
        {
            threads[i] = std::thread(RenderSection, i);
        }
    }

    void Game::Shutdown()
    {
        JobsToDo.store(0);
        CanTraceRays = false;
        m_AntiAliasingCount = MaxSamplesPerImage;

        for (uint16_t i = 0; i < threads.get_size(); i++)
        {
            threads[i].join();
        }
    }

    // RT Performance
    int RT_FPS = 0;
    int RT_FPS_Counter = 0;
    float RT_FrameTime = 0.0f;
    timer RT_Timer; bool RT_TimerRunning = false;
    float RT_FPS_Timer = 0.0f;

    // Main Thread Performance
    int MT_FPS = 0;
    int MT_FPS_Counter = 0;
    float MT_FPS_Timer = 0.0f;

    void Game::Tick(float deltaTime)
    {
#pragma region Update perfromance Metrics Variables

        // Set Timer to 0;
        if (RT_TimerRunning == false) { RT_Timer.reset(); RT_TimerRunning = true; }

        if (m_AntiAliasingCount < MaxSamplesPerImage)
        {
            if (RT_FPS_Timer < 1.0f)
            {
                RT_FPS_Timer += deltaTime;
            }
            else
            {
                RT_FPS_Timer = 0.0f;
                RT_FPS = RT_FPS_Counter;
                RT_FPS_Counter = 0;
            }
        }

        if (MT_FPS_Timer < 1.0f)
        {
            MT_FPS_Timer += deltaTime;
        }
        else
        {
            MT_FPS_Timer = 0.0f;
            MT_FPS = MT_FPS_Counter;
            MT_FPS_Counter = 0;
        }
#pragma endregion

        if (inputHandler.GetKeyDown(KEYCODE_C))
        {
            system("CLS");
        }
        if (inputHandler.GetKeyDown(KEYCODE_R))
        {
            SetNewSceneValues();
            ScreenReset();
        }
        if (inputHandler.GetKeyDown(KEYCODE_L))
        {
            if (enableLowResRendering)
            {
                enableLowResRendering = false;
            }
            else
            {
                enableLowResRendering = true;
            }

            ScreenReset();
        }

        if (inputHandler.GetKeyDown(KEYCODE_UP))
        {
            ScreenReset();
            camera.MoveCamera(Point3(5.0f * deltaTime, 0, 0));
        }
        if (inputHandler.GetKeyDown(KEYCODE_DOWN))
        {
            ScreenReset();
            camera.MoveCamera(Point3(-5.0f * deltaTime, 0, 0));
        }
        if (inputHandler.GetKeyDown(KEYCODE_RIGHT))
        {
            ScreenReset();
            camera.MoveCamera(Point3(0, 0, 5.0f * deltaTime));
        }
        if (inputHandler.GetKeyDown(KEYCODE_LEFT))
        {
            ScreenReset();
            camera.MoveCamera(Point3(0, 0, -5.0f * deltaTime));
        }
        if (inputHandler.GetKeyDown(KEYCODE_SPACE))
        {
            ScreenReset();
            camera.MoveCamera(Point3(0, 5.0f * deltaTime, 0));
        }
        if (inputHandler.GetKeyDown(KEYCODE_LCTRL))
        {
            ScreenReset();
            camera.MoveCamera(Point3(0, -5.0f * deltaTime, 0));
        }

#pragma region KeyBinds For Scene Switching
        if (inputHandler.GetKeyDown(KEYCODE_1))
        { SetNewScene(0); }
        if (inputHandler.GetKeyDown(KEYCODE_2))
        { SetNewScene(1); }
        if (inputHandler.GetKeyDown(KEYCODE_3))
        { SetNewScene(2); }
        if (inputHandler.GetKeyDown(KEYCODE_4))
        { SetNewScene(3); }
        if (inputHandler.GetKeyDown(KEYCODE_5))
        { SetNewScene(4); }
        if (inputHandler.GetKeyDown(KEYCODE_6))
        { SetNewScene(5); }
        if (inputHandler.GetKeyDown(KEYCODE_7))
        { SetNewScene(6); }
        if (inputHandler.GetKeyDown(KEYCODE_8))
        { SetNewScene(7); }
        if (inputHandler.GetKeyDown(KEYCODE_9))
        { SetNewScene(8); }
        if (inputHandler.GetKeyDown(KEYCODE_0))
        { SetNewScene(9); }
#pragma endregion

        // Rendering
        CheckRayTracingCompletion();
        
        // Display Performance Metrics in the Screen.
        PerformanceMetrics(deltaTime);

        // Display keybinds on the screen.
        for (uint16_t y = 420; y < 504; y++)
        {
            for (uint16_t x = 515; x < 700; x++)
            {
                screen->Plot(x, y, 0x111111);
            }
        }

        const int TextColour = 0xffffff;
        screen->Print("Keybinds:", 520, 425, TextColour);
        screen->Print("Number Keys: select a Scene", 520, 440, TextColour);
        screen->Print("L: Enable Low Res Rendering", 520, 450, TextColour);
        screen->Print("C: Clear the Console", 520, 460, TextColour);
        screen->Print("Keypad U: X+ | Keypad D: X-", 520, 475, TextColour);
        screen->Print("Keypad L: Z- | Keypad R: Z+", 520, 485, TextColour);
        screen->Print("LCTRL   : Y- | SPACE   : Y+", 520, 495, TextColour);

        // Update FPS
        MT_FPS_Counter += 1;
    }

    void Game::CheckRayTracingCompletion()
    {
        bool isWorking = false;
        if (CanTraceRays)
        {
            isWorking = true;
            for (uint16_t i = 0; i < jobs.get_size(); i++)
            {
                if (jobs[i].m_CurrentJobState != JobState::DONE)
                {
                    isWorking = false;
                }
            }
        }
        if (isWorking)
        {
            m_AntiAliasingCount++;

            // Draw Pixels to the Screen from the ScreenBuffer.
            for (uint16_t y = 0; y < ImageHeight; y++)
            {
                for (uint16_t x = 0; x < ImageWidth; x++)
                {
                    // Draw the screen
                    screen->Plot((-x) + (ImageWidth - 1), (-y) + (ImageHeight - 1), 
                        write_colour(ScreenBuffer[x * ImageHeight + y], m_AntiAliasingCount));
                }
            }

            ResetValuesForRayTracing();
        }
    }

#ifdef _DEBUG
    int DebugTextColour = 0xff0000;
#endif
    void Game::PerformanceMetrics(const float& _deltaTime)
    {
        // Background (prevent showing pixels from previous frame).
        const vec2 pos = vec2(ImageWidth + 20, 10);
        const vec2 pos2 = vec2(pos.x + 200, pos.y + 150);
        const int TextColour = 0xffffff;

        for (uint16_t y = pos.y - 1; y < pos2.y + 1; y++)
        {
            for (uint16_t x = pos.x - 1; x < pos2.x + 1; x++)
            {
                screen->Plot(x, y, 0x111111);
            }
        }

        // General Things
        const int LineOffset = 5;

        const int Line1YPos = 85;
        screen->Line(pos.x + LineOffset, Line1YPos, pos2.x - LineOffset, Line1YPos, TextColour);
        const int Line2YPos = 140;
        screen->Line(pos.x + LineOffset, Line2YPos, pos2.x - LineOffset, Line2YPos, TextColour);

        screen->Line(ImageWidth, 0, ImageWidth, ScreenHeight, 0xffaaff);

        // RT Image Stats

        char resBuffer[25];
        std::snprintf(resBuffer, sizeof(resBuffer), "RT Resolution: %i x %i", ImageWidth, ImageHeight);
        screen->Print(resBuffer, pos.x + 10, 20, TextColour);

        char jobsCountBuffer[26];
        std::snprintf(jobsCountBuffer, sizeof(jobsCountBuffer), "RT Tiles To Render: %u", static_cast<uint32_t>(jobs.get_size()));
        screen->Print(jobsCountBuffer, pos.x + 10, 30, TextColour);

        // Performance Metrics

        char RT_FrameTimeBuffer[25];
        std::snprintf(RT_FrameTimeBuffer, sizeof(RT_FrameTimeBuffer), "RT FrameTime: %f s", RT_FrameTime);
        screen->Print(RT_FrameTimeBuffer, pos.x + 10, 50, TextColour);

        char fpsbuffer[12];
        std::snprintf(fpsbuffer, sizeof(fpsbuffer), "RT FPS: %i", RT_FPS);
        screen->Print(fpsbuffer, pos.x + 10, 60, TextColour);

        char RT_SampleCountBuffer[30];
        std::snprintf(RT_SampleCountBuffer, sizeof(RT_SampleCountBuffer), "Current Sample Count: %i", m_AntiAliasingCount);
        screen->Print(RT_SampleCountBuffer, pos.x + 10, 70, TextColour);

        if (UseBVH == true) 
        { screen->Print("BVH: Enabled", pos.x + 10, 95, TextColour); }
        else 
        { screen->Print("BVH: Disabled", pos.x + 10, 95, TextColour); }

        //SceneIndex
        char sceneIndexBuffer[22];
        std::snprintf(sceneIndexBuffer, sizeof(sceneIndexBuffer), "Scene: %i of %i", SceneIndex + 1, scenes.size());
        screen->Print(sceneIndexBuffer, pos.x + 10, 105, TextColour);

        char objCountbuffer[22];
        std::snprintf(objCountbuffer, sizeof(objCountbuffer), "OBJECT COUNT: %i", scenes[SceneIndex].ObjectCount);
        screen->Print(objCountbuffer, pos.x + 10, 115, TextColour);

        if (enableLowResRendering)
        {
            screen->Print("Low Res Rendering: Enabled", pos.x + 10, 125, TextColour);
        }
        else
        {
            screen->Print("Low Res Rendering: Disabled", pos.x + 10, 125, TextColour);
        }

        // Main Thread Performance Metrics
        char mtfpsbuffer[15];
        std::snprintf(mtfpsbuffer, sizeof(mtfpsbuffer), "MT FPS: %i", MT_FPS);
        screen->Print(mtfpsbuffer, pos.x + 10, 150, TextColour);

        // Tile State Grid
        for (uint16_t y = 0; y < (TileCount_Y); y++)
        {
            for (uint16_t x = 0; x < (TileCount_X); x++)
            {
                int colour = 0xffffff;
                if (jobs[x * TileCount_Y + y].m_CurrentJobState == JobState::DONE)
                {
                    colour = 0x00ff00;
                }
                else
                {
                    colour = 0xff0000;
                }

                int X = (-x) + (TileCount_X - 1);
                int Y = (-y) + (TileCount_Y - 1);
                screen->Plot(pos2.x + 20 +(X * 2), pos.y + (Y * 2), colour);
            }
        }

#ifdef _DEBUG
        screen->Print("DEBUG BUILD", ImageWidth + 4, 2, DebugTextColour);
#endif
    }

#pragma region Reset Values Functions

    void Game::SetNewScene(uint8_t _SceneIndex)
    {
        // Prevent reseting the scene, unless the scene has finished, then it can be reset by pressing the scene key.
        if (_SceneIndex == SceneIndex && CanTraceRays == true) { return; }
        if (_SceneIndex >= scenes.size()) { return; }
        
        SceneIndex = _SceneIndex;

        // Reset Screen and Ray Tracing.
        ScreenReset();
        SetNewSceneValues();
        ResetValuesForRayTracing();
    }

    void Game::ResetValuesForRayTracing()
    {
        for (uint16_t i = 0; i < jobs.get_size(); i++)
        {
            jobs[i].m_CurrentJobState = JobState::NONE;
        }

        // Update Performance Metrics
        RT_FrameTime = RT_Timer.elapsed() * 0.001f;
        RT_Timer.reset();
        RT_FPS_Counter++;

        if (m_AntiAliasingCount >= MaxSamplesPerImage)
        {
            CanTraceRays = false;
        }
        else
        {
            // Reset Lock on Threads
            JobsToDo.store(JobsSize - 1);
            iterationLock.notify_all();
        }
    }

    // Reset the Screen and all the variables needed for that to happen.
    void Game::ScreenReset()
    {
        CanTraceRays = true;
        m_AntiAliasingCount = 0;

        memset(ScreenBuffer, 0.0f, ImageWidth * ImageHeight * sizeof(Colour));
        ResetValuesForRayTracing();
    }

    /// <summary>Set the values for when a new Scene is loaded.</summary>
    void Game::SetNewSceneValues()
    {
        // Initialize Level
        HittableList& world = scenes[SceneIndex];

        // Initialize the camera
        camera = Camera(world.m_CamPos, world.m_CamLookAt, world.m_CamUp, world.m_CamFOV, ImageRatio, world.m_CamAperture, world.m_CamDistanceToFocus, 0.0f, 0.0f);
    }

#pragma endregion

}; // namespace Tmpl8