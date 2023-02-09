#pragma once

#include <Difu/ScreenManagement/Screen.h>
#include <raylib.h>

#ifndef GUI_FILE_DIALOG_STATE_DEF
#define GUI_FILE_DIALOG_STATE_DEF
// Gui file dialog context data
typedef struct {

    // Window management variables
    bool windowActive;
    Rectangle windowBounds;
    Vector2 panOffset;
    bool dragMode;
    bool supportDrag;

    // UI variables
    bool dirPathEditMode;
    char dirPathText[1024];

    int filesListScrollIndex;
    bool filesListEditMode;
    int filesListActive;

    bool fileNameEditMode;
    char fileNameText[1024];
    bool SelectFilePressed;
    bool CancelFilePressed;
    int fileTypeActive;
    int itemFocused;

    // Custom state variables
    FilePathList dirFiles;
    char filterExt[256];
    char dirPathTextCopy[1024];
    char fileNameTextCopy[1024];

    int prevFilesListActive;

    bool saveFileMode;

} GuiFileDialogState;

#endif

namespace MenuScreen
{
	Screen GetScreen();

	void Load();
	void Unload();

	void Update(float dt);
	void Render();

	void OnResize(int width, int height);

	void RenderStartTransition(float time);
	bool IsStartTransitionDone(float time);

	void RenderEndTransition(float time);
	bool IsEndTransitionDone(float time);
} // namespace MenuScreen
