#pragma once

#include "../ScreenManagement/Screen.h"
#include <raylib.h>

#ifndef GUI_FILE_DIALOG_STATE
#define GUI_FILE_DIALOG_STATE
typedef struct {
    Vector2 position;
    Vector2 size;

    bool fileDialogActive;

    bool dirPathEditMode;
    char dirPathText[256];

    int filesListScrollIndex;
    bool filesListEditMode;
    int filesListActive;

    bool fileNameEditMode;
    char fileNameText[256];
    bool SelectFilePressed;
    bool CancelFilePressed;
    int fileTypeActive;
    int itemFocused;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required
    char **dirFiles;
    int dirFilesCount;

    char filterExt[256];

    char dirPathTextCopy[256];
    char fileNameTextCopy[256];

    int prevFilesListActive;

    bool saveFileMode;

} GuiFileDialogState;
#endif

class MenuScreen : public Screen
{
public:
    void Load() override;
    void Unload() override;

    void Update(float dt) override;
    void Render() override;

    void RenderStartTransition(float time) override;
    bool IsStartTransitionDone(float time) override;

    void RenderEndTransition(float time) override;
    bool IsEndTransitionDone(float time) override;
private:
    Rectangle startRect;
    Rectangle loadRect;
    GuiFileDialogState fileDialogState;
};
