#pragma once

// class Screen
// {
// public:
//     virtual void Load() = 0;
//     virtual void Unload() = 0;
//
//     virtual void Update(float dt) = 0;
//     virtual void Render() = 0;
//     
//     virtual bool IsEndTransitionDone(float time) { return true; }
//     virtual bool IsStartTransitionDone(float time) { return true; }
//     
//     virtual void RenderStartTransition(float time) {}
//     virtual void RenderEndTransition(float time) {}
//
// };
//


struct Screen
{
    // Mandatory
    void(*LoadFunction)();
    void(*UnloadFunction)();
    void(*UpdateFunction)(float);
    void(*RenderFunction)();

    // Optional
    bool(*IsStartTransitionDoneFunction)(float) = nullptr;
    bool(*IsEndTransitionDoneFunction)(float) = nullptr;
    void(*RenderStartTransitionFunction)(float) = nullptr;
    void(*RenderEndTransitionFunction)(float) = nullptr;
};
