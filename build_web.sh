#!/bin/sh

em++ \
	obj/Chess/Release/Web/BannerAnimation.o \
	obj/Chess/Release/Web/Board.o \
	obj/Chess/Release/Web/GameScreen.o \
	obj/Chess/Release/Web/KeyFrameAnimation.o \
	obj/Chess/Release/Web/Layer.o \
	obj/Chess/Release/Web/Logger.o \
	obj/Chess/Release/Web/main.o \
	obj/Chess/Release/Web/MenuScreen.o \
	obj/Chess/Release/Web/ParticleEmitter.o \
	obj/Chess/Release/Web/Random.o \
	obj/Chess/Release/Web/ScreenManager.o \
	obj/Chess/Release/Web/SettingsLayer.o \
	obj/Chess/Release/Web/Timer.o \
	obj/Chess/Release/Web/Variables.o \
	obj/Chess/Release/Web/WindowManager.o \
	-o bin/Chess/Release/output.html \
	-IChess/src -IDependencies/Raylib/webassembly/include -IDependencies/fmt/linux/include \
	-LDependencies/Raylib/webassembly/lib -LDependencies/fmt/linux/lib \
	-lraylib -lfmt \
	--preload-file Assets -s USE_GLFW=3 -s TOTAL_MEMORY=134217728 -s FORCE_FILESYSTEM=1 \
	-Os -std=c++17 -DPLATFORM_WEB -sASYNCIFY --shell-file shell.html
