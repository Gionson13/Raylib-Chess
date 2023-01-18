#pragma once

#include "../ScreenManagement/Screen.h"
#include "../Utils/BannerAnimation.h"
#include "../Particles/ParticleEmitter.h"
#include "../Board.h"

#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include <vector>

// TODO: Polish 



namespace GameScreen
{
    Screen GetScreen();


    void Load();
    void Unload();

    void Update(float dt);
    void Render();

    void RenderStartTransition(float time);
    bool IsStartTransitionDone(float time);

    void RenderEndTransition(float time);
    bool IsEndTransitionDone(float time);

    void OnResize(int width, int height);

    void DrawBoard();
    void DrawPiece(int x, int y, PieceType type, PieceColor color);
} // namespace GameScreen
