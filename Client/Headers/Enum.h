#pragma once

#define VK_MWHEELUP 120
#define VK_MWHEELDOWN -120

enum class RenderGroup { BackGround, Section, BackEffect, Player, Enemy, BackObject, Object, Item, FrontGround, Effect, UI,Fade, End };
enum class LayerGroup { Object, Effect, UI, End };
enum class SoundGroup { BGM, SFX, Soldier, PlayerVoice, EnemyVoice, UI, End };
enum class MouseState { LButton, RButton, WButton, End };

const int WINCX = 1280;
const int WINCY = 720;