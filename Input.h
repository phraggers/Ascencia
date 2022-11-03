#ifndef _ASC_INPUT_H_
#define _ASC_INPUT_H_

#define ASC_KEYCOUNT 16
#define ASC_DEADZONE 3200
#define ASC_CONTROLLERS 4

typedef struct 
{
	SDL_Keycode Key;
	bool IsDown;
	bool WasDown;
	bool TextDown;
} ASC_KeyState;

typedef struct
{
	bool IsDown;
	bool WasDown;
} ASC_BtnState;

typedef enum
{
	MW_NONE,
	MW_UP,
	MW_DOWN,
	MW_LEFT,
	MW_RIGHT
} ASC_MOUSEWHEEL;

typedef struct
{
	int X;
	int Y;
	ASC_BtnState Left; // SDL_BUTTON_LEFT
	ASC_BtnState Right; // SDL_BUTTON_RIGHT
	ASC_BtnState Middle; // SDL_BUTTON_MIDDLE
	ASC_BtnState X1; // SDL_BUTTON_X1
	ASC_BtnState X2; // SDL_BUTTON_X2
	ASC_MOUSEWHEEL MouseWheel; // enum ASC_MOUSEWHEEL
} ASC_Mouse;

typedef struct
{
	SDL_GameController* Controller;
	SDL_JoystickID ID;
	bool Connected;
	ASC_BtnState A;
	ASC_BtnState B;
	ASC_BtnState X;
	ASC_BtnState Y;
	ASC_BtnState Back;
	ASC_BtnState Guide;
	ASC_BtnState Start;
	ASC_BtnState LeftStick;
	ASC_BtnState RightStick;
	ASC_BtnState LeftShoulder;
	ASC_BtnState RightShoulder;
	ASC_BtnState DPadUp;
	ASC_BtnState DPadDown;
	ASC_BtnState DPadLeft;
	ASC_BtnState DPadRight;
	ASC_BtnState Misc1;
	ASC_BtnState Paddle1;
	ASC_BtnState Paddle2;
	ASC_BtnState Paddle3;
	ASC_BtnState Paddle4;
	ASC_BtnState TouchPad;
	i16 LeftX;
	i16 LeftY;
	i16 RightX;
	i16 RightY;
	i16 LeftTrigger;
	i16 RightTrigger;
} ASC_Controller;

typedef struct
{
	ASC_KeyState Keys[ASC_KEYCOUNT];
	ASC_Mouse Mouse;
	ASC_Controller Controllers[ASC_CONTROLLERS];
} ASC_Input;

void Input_HandleEvents(void);

bool Input_KeySingle(SDL_Keycode _Key);
bool Input_KeyDown(SDL_Keycode _Key);
bool Input_KeyRepeat(SDL_Keycode _Key);
bool Input_KeyUp(SDL_Keycode _Key);

bool Input_MouseSingle(Uint8 _Button);
bool Input_MouseDown(Uint8 _Button);
bool Input_MouseUp(Uint8 _Button);
bool Input_MouseWheel(ASC_MOUSEWHEEL _Direction);

bool Input_ControllerBtnSingle(int _Controller, SDL_GameControllerButton _Button);
bool Input_ControllerBtnDown(int _Controller, SDL_GameControllerButton _Button);
bool Input_ControllerBtnUp(int _Controller, SDL_GameControllerButton _Button);
i16 Input_ControllerAxisRaw(int _Controller, SDL_GameControllerAxis _Axis);
i16 Input_ControllerAxisDZ(int _Controller, SDL_GameControllerAxis _Axis);

#endif //_ASC_INPUT_H_