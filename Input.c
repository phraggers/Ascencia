
#include "Ascencia.h"

void Input_HandleEvents()
{
    SDL_Event Event;

    // Keys: set WasDown state
    for (int i = 0; i < ASC_KEYCOUNT; i++)
    {
        if (State->Input.Keys[i].Key) // check only recently used keys
        {
            if (State->Input.Keys[i].IsDown && !State->Input.Keys[i].WasDown)
            {
                State->Input.Keys[i].WasDown = 1;
                ASC_Log(LOGLEVEL_DEBUG, "INPUT: Key[%d] IsDown[%d] WasDown[%d]", State->Input.Keys[i].Key, 
                        State->Input.Keys[i].IsDown, State->Input.Keys[i].WasDown);
            }

            else if (!State->Input.Keys[i].IsDown && State->Input.Keys[i].WasDown)
            {
                State->Input.Keys[i].WasDown = 0;
                ASC_Log(LOGLEVEL_DEBUG, "INPUT: Key[%d] IsDown[%d] WasDown[%d]", State->Input.Keys[i].Key,
                        State->Input.Keys[i].IsDown, State->Input.Keys[i].WasDown);
            }

            else if (!State->Input.Keys[i].IsDown && !State->Input.Keys[i].WasDown)
            {
                State->Input.Keys[i].Key = 0; // clear non-recent key
                ASC_Log(LOGLEVEL_DEBUG, "INPUT: Key[%d] IsDown[%d] WasDown[%d]", State->Input.Keys[i].Key,
                        State->Input.Keys[i].IsDown, State->Input.Keys[i].WasDown);
            }
        }
    }

    // Mouse: set WasDown states
    if (State->Input.Mouse.Left.IsDown && !State->Input.Mouse.Left.WasDown)
    {
        State->Input.Mouse.Left.WasDown = 1;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[LMB] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.Left.IsDown, State->Input.Mouse.Left.WasDown);
    }
    else if (!State->Input.Mouse.Left.IsDown && State->Input.Mouse.Left.WasDown)
    {
        State->Input.Mouse.Left.WasDown = 0;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[LMB] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.Left.IsDown, State->Input.Mouse.Left.WasDown);
    }

    if (State->Input.Mouse.Right.IsDown && !State->Input.Mouse.Right.WasDown)
    {
        State->Input.Mouse.Right.WasDown = 1;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[RMB] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.Right.IsDown, State->Input.Mouse.Right.WasDown);
    }
    else if (!State->Input.Mouse.Right.IsDown && State->Input.Mouse.Right.WasDown)
    {
        State->Input.Mouse.Right.WasDown = 0;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[RMB] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.Right.IsDown, State->Input.Mouse.Right.WasDown);
    }

    if (State->Input.Mouse.Middle.IsDown && !State->Input.Mouse.Middle.WasDown)
    {
        State->Input.Mouse.Middle.WasDown = 1;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[MMB] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.Middle.IsDown, State->Input.Mouse.Middle.WasDown);
    }
    else if (!State->Input.Mouse.Middle.IsDown && State->Input.Mouse.Middle.WasDown)
    {
        State->Input.Mouse.Middle.WasDown = 0;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[MMB] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.Middle.IsDown, State->Input.Mouse.Middle.WasDown);
    }

    if (State->Input.Mouse.X1.IsDown && !State->Input.Mouse.X1.WasDown)
    {
        State->Input.Mouse.X1.WasDown = 1;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[X1] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.X1.IsDown, State->Input.Mouse.X1.WasDown);

    }
    else if (!State->Input.Mouse.X1.IsDown && State->Input.Mouse.X1.WasDown)
    {
        State->Input.Mouse.X1.WasDown = 0;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[X1] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.X1.IsDown, State->Input.Mouse.X1.WasDown);
    }

    if (State->Input.Mouse.X2.IsDown && !State->Input.Mouse.X2.WasDown)
    {
        State->Input.Mouse.X2.WasDown = 1;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[X2] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.X2.IsDown, State->Input.Mouse.X2.WasDown);
    }
    else if (!State->Input.Mouse.X2.IsDown && State->Input.Mouse.X2.WasDown)
    {
        State->Input.Mouse.X2.WasDown = 0;
        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse[X2] IsDown[%d] WasDown[%d]",
                State->Input.Mouse.X2.IsDown, State->Input.Mouse.X2.WasDown);
    }

    State->Input.Mouse.MouseWheel = MW_NONE;

    // Controllers State
    {
        for (int i = 0; i < ASC_CONTROLLERS; i++)
        {
            if (State->Input.Controllers[i].Connected)
            {
                if (SDL_GameControllerGetAttached(State->Input.Controllers[i].Controller) == SDL_FALSE)
                {
                    SDL_GameControllerClose(State->Input.Controllers[i].Controller);
                    SDL_memset(&State->Input.Controllers[i], 0, sizeof(ASC_Controller));
                    ASC_Log(LOGLEVEL_INFO, "INPUT: Controller[%d] Disconnected", i);
                    break;
                }

                // Get Axis
                State->Input.Controllers[i].LeftX = SDL_GameControllerGetAxis(State->Input.Controllers[i].Controller, SDL_CONTROLLER_AXIS_LEFTX);
                State->Input.Controllers[i].LeftY = SDL_GameControllerGetAxis(State->Input.Controllers[i].Controller, SDL_CONTROLLER_AXIS_LEFTY);
                State->Input.Controllers[i].RightX = SDL_GameControllerGetAxis(State->Input.Controllers[i].Controller, SDL_CONTROLLER_AXIS_RIGHTX);
                State->Input.Controllers[i].RightY = SDL_GameControllerGetAxis(State->Input.Controllers[i].Controller, SDL_CONTROLLER_AXIS_RIGHTY);
                State->Input.Controllers[i].LeftTrigger = SDL_GameControllerGetAxis(State->Input.Controllers[i].Controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                State->Input.Controllers[i].RightTrigger = SDL_GameControllerGetAxis(State->Input.Controllers[i].Controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

                // Set WasDown States
                if (State->Input.Controllers[i].A.IsDown && !State->Input.Controllers[i].A.WasDown)
                {
                    State->Input.Controllers[i].A.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[A] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].A.IsDown, State->Input.Controllers[i].A.WasDown);
                }
                else if (!State->Input.Controllers[i].A.IsDown && State->Input.Controllers[i].A.WasDown)
                {
                    State->Input.Controllers[i].A.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[A] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].A.IsDown, State->Input.Controllers[i].A.WasDown);
                }

                if (State->Input.Controllers[i].B.IsDown && !State->Input.Controllers[i].B.WasDown)
                {
                    State->Input.Controllers[i].B.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[B] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].B.IsDown, State->Input.Controllers[i].B.WasDown);
                }
                else if (!State->Input.Controllers[i].B.IsDown && State->Input.Controllers[i].B.WasDown)
                {
                    State->Input.Controllers[i].B.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[B] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].B.IsDown, State->Input.Controllers[i].B.WasDown);
                }

                if (State->Input.Controllers[i].X.IsDown && !State->Input.Controllers[i].X.WasDown)
                {
                    State->Input.Controllers[i].X.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[X] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].X.IsDown, State->Input.Controllers[i].X.WasDown);
                }
                else if (!State->Input.Controllers[i].X.IsDown && State->Input.Controllers[i].X.WasDown)
                {
                    State->Input.Controllers[i].X.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[X] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].X.IsDown, State->Input.Controllers[i].X.WasDown);
                }

                if (State->Input.Controllers[i].Y.IsDown && !State->Input.Controllers[i].Y.WasDown)
                {
                    State->Input.Controllers[i].Y.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Y] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Y.IsDown, State->Input.Controllers[i].Y.WasDown);
                }
                else if (!State->Input.Controllers[i].Y.IsDown && State->Input.Controllers[i].Y.WasDown)
                {
                    State->Input.Controllers[i].Y.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Y] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Y.IsDown, State->Input.Controllers[i].Y.WasDown);
                }

                if (State->Input.Controllers[i].Back.IsDown && !State->Input.Controllers[i].Back.WasDown)
                {
                    State->Input.Controllers[i].Back.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Back] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Back.IsDown, State->Input.Controllers[i].Back.WasDown);
                }
                else if (!State->Input.Controllers[i].Back.IsDown && State->Input.Controllers[i].Back.WasDown)
                {
                    State->Input.Controllers[i].Back.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Back] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Back.IsDown, State->Input.Controllers[i].Back.WasDown);
                }

                if (State->Input.Controllers[i].Guide.IsDown && !State->Input.Controllers[i].Guide.WasDown)
                {
                    State->Input.Controllers[i].Guide.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Guide] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Guide.IsDown, State->Input.Controllers[i].Guide.WasDown);
                }
                else if (!State->Input.Controllers[i].Guide.IsDown && State->Input.Controllers[i].Guide.WasDown)
                {
                    State->Input.Controllers[i].Guide.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Guide] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Guide.IsDown, State->Input.Controllers[i].Guide.WasDown);
                }

                if (State->Input.Controllers[i].Start.IsDown && !State->Input.Controllers[i].Start.WasDown)
                {
                    State->Input.Controllers[i].Start.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Start] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Start.IsDown, State->Input.Controllers[i].Start.WasDown);
                }
                else if (!State->Input.Controllers[i].Start.IsDown && State->Input.Controllers[i].Start.WasDown)
                {
                    State->Input.Controllers[i].Start.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Start] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Start.IsDown, State->Input.Controllers[i].Start.WasDown);
                }

                if (State->Input.Controllers[i].LeftStick.IsDown && !State->Input.Controllers[i].LeftStick.WasDown)
                {
                    State->Input.Controllers[i].LeftStick.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[LeftStick] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].LeftStick.IsDown, State->Input.Controllers[i].LeftStick.WasDown);
                }
                else if (!State->Input.Controllers[i].LeftStick.IsDown && State->Input.Controllers[i].LeftStick.WasDown)
                {
                    State->Input.Controllers[i].LeftStick.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[LeftStick] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].LeftStick.IsDown, State->Input.Controllers[i].LeftStick.WasDown);
                }

                if (State->Input.Controllers[i].RightStick.IsDown && !State->Input.Controllers[i].RightStick.WasDown)
                {
                    State->Input.Controllers[i].RightStick.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[RightStick] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].RightStick.IsDown, State->Input.Controllers[i].RightStick.WasDown);
                }
                else if (!State->Input.Controllers[i].RightStick.IsDown && State->Input.Controllers[i].RightStick.WasDown)
                {
                    State->Input.Controllers[i].RightStick.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[RightStick] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].RightStick.IsDown, State->Input.Controllers[i].RightStick.WasDown);
                }

                if (State->Input.Controllers[i].LeftShoulder.IsDown && !State->Input.Controllers[i].LeftShoulder.WasDown)
                {
                    State->Input.Controllers[i].LeftShoulder.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[LeftShoulder] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].LeftShoulder.IsDown, State->Input.Controllers[i].LeftShoulder.WasDown);
                }
                else if (!State->Input.Controllers[i].LeftShoulder.IsDown && State->Input.Controllers[i].LeftShoulder.WasDown)
                {
                    State->Input.Controllers[i].LeftShoulder.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[LeftShoulder] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].LeftShoulder.IsDown, State->Input.Controllers[i].LeftShoulder.WasDown);
                }

                if (State->Input.Controllers[i].RightShoulder.IsDown && !State->Input.Controllers[i].RightShoulder.WasDown)
                {
                    State->Input.Controllers[i].RightShoulder.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[RightShoulder] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].RightShoulder.IsDown, State->Input.Controllers[i].RightShoulder.WasDown);
                }
                else if (!State->Input.Controllers[i].RightShoulder.IsDown && State->Input.Controllers[i].RightShoulder.WasDown)
                {
                    State->Input.Controllers[i].RightShoulder.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[RightShoulder] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].RightShoulder.IsDown, State->Input.Controllers[i].RightShoulder.WasDown);
                }

                if (State->Input.Controllers[i].DPadUp.IsDown && !State->Input.Controllers[i].DPadUp.WasDown)
                {
                    State->Input.Controllers[i].DPadUp.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadUp] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadUp.IsDown, State->Input.Controllers[i].DPadUp.WasDown);
                }
                else if (!State->Input.Controllers[i].DPadUp.IsDown && State->Input.Controllers[i].DPadUp.WasDown)
                {
                    State->Input.Controllers[i].DPadUp.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadUp] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadUp.IsDown, State->Input.Controllers[i].DPadUp.WasDown);
                }

                if (State->Input.Controllers[i].DPadDown.IsDown && !State->Input.Controllers[i].DPadDown.WasDown)
                {
                    State->Input.Controllers[i].DPadDown.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadDown] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadDown.IsDown, State->Input.Controllers[i].DPadDown.WasDown);
                }
                else if (!State->Input.Controllers[i].DPadDown.IsDown && State->Input.Controllers[i].DPadDown.WasDown)
                {
                    State->Input.Controllers[i].DPadDown.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadDown] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadDown.IsDown, State->Input.Controllers[i].DPadDown.WasDown);
                }

                if (State->Input.Controllers[i].DPadLeft.IsDown && !State->Input.Controllers[i].DPadLeft.WasDown)
                {
                    State->Input.Controllers[i].DPadLeft.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadLeft] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadLeft.IsDown, State->Input.Controllers[i].DPadLeft.WasDown);
                }
                else if (!State->Input.Controllers[i].DPadLeft.IsDown && State->Input.Controllers[i].DPadLeft.WasDown)
                {
                    State->Input.Controllers[i].DPadLeft.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadLeft] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadLeft.IsDown, State->Input.Controllers[i].DPadLeft.WasDown);
                }

                if (State->Input.Controllers[i].DPadRight.IsDown && !State->Input.Controllers[i].DPadRight.WasDown)
                {
                    State->Input.Controllers[i].DPadRight.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadRight] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadRight.IsDown, State->Input.Controllers[i].DPadRight.WasDown);
                }
                else if (!State->Input.Controllers[i].DPadRight.IsDown && State->Input.Controllers[i].DPadRight.WasDown)
                {
                    State->Input.Controllers[i].DPadRight.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadRight] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].DPadRight.IsDown, State->Input.Controllers[i].DPadRight.WasDown);
                }

                if (State->Input.Controllers[i].Misc1.IsDown && !State->Input.Controllers[i].Misc1.WasDown)
                {
                    State->Input.Controllers[i].Misc1.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Misc1] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Misc1.IsDown, State->Input.Controllers[i].Misc1.WasDown);
                }
                else if (!State->Input.Controllers[i].Misc1.IsDown && State->Input.Controllers[i].Misc1.WasDown)
                {
                    State->Input.Controllers[i].Misc1.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Misc1] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Misc1.IsDown, State->Input.Controllers[i].Misc1.WasDown);
                }

                if (State->Input.Controllers[i].Paddle1.IsDown && !State->Input.Controllers[i].Paddle1.WasDown)
                {
                    State->Input.Controllers[i].Paddle1.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle1] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle1.IsDown, State->Input.Controllers[i].Paddle1.WasDown);
                }
                else if (!State->Input.Controllers[i].Paddle1.IsDown && State->Input.Controllers[i].Paddle1.WasDown)
                {
                    State->Input.Controllers[i].Paddle1.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle1] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle1.IsDown, State->Input.Controllers[i].Paddle1.WasDown);
                }

                if (State->Input.Controllers[i].Paddle2.IsDown && !State->Input.Controllers[i].Paddle2.WasDown)
                {
                    State->Input.Controllers[i].Paddle2.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle2] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle2.IsDown, State->Input.Controllers[i].Paddle2.WasDown);
                }
                else if (!State->Input.Controllers[i].Paddle2.IsDown && State->Input.Controllers[i].Paddle2.WasDown)
                {
                    State->Input.Controllers[i].Paddle2.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle2] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle2.IsDown, State->Input.Controllers[i].Paddle2.WasDown);
                }

                if (State->Input.Controllers[i].Paddle3.IsDown && !State->Input.Controllers[i].Paddle3.WasDown)
                {
                    State->Input.Controllers[i].Paddle3.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle3] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle3.IsDown, State->Input.Controllers[i].Paddle3.WasDown);
                }
                else if (!State->Input.Controllers[i].Paddle3.IsDown && State->Input.Controllers[i].Paddle3.WasDown)
                {
                    State->Input.Controllers[i].Paddle3.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle3] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle3.IsDown, State->Input.Controllers[i].Paddle3.WasDown);
                }

                if (State->Input.Controllers[i].Paddle4.IsDown && !State->Input.Controllers[i].Paddle4.WasDown)
                {
                    State->Input.Controllers[i].Paddle4.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle4] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle4.IsDown, State->Input.Controllers[i].Paddle4.WasDown);
                }
                else if (!State->Input.Controllers[i].Paddle4.IsDown && State->Input.Controllers[i].Paddle4.WasDown)
                {
                    State->Input.Controllers[i].Paddle4.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle4] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].Paddle4.IsDown, State->Input.Controllers[i].Paddle4.WasDown);
                }

                if (State->Input.Controllers[i].TouchPad.IsDown && !State->Input.Controllers[i].TouchPad.WasDown)
                {
                    State->Input.Controllers[i].TouchPad.WasDown = 1;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[TouchPad] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].TouchPad.IsDown, State->Input.Controllers[i].TouchPad.WasDown);
                }
                else if (!State->Input.Controllers[i].TouchPad.IsDown && State->Input.Controllers[i].TouchPad.WasDown)
                {
                    State->Input.Controllers[i].TouchPad.WasDown = 0;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[TouchPad] IsDown[%d] WasDown[%d]",
                            i, State->Input.Controllers[i].TouchPad.IsDown, State->Input.Controllers[i].TouchPad.WasDown);
                }

                // Set IsDown States
                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_A))
                {
                    if (!State->Input.Controllers[i].A.IsDown)
                    {
                        State->Input.Controllers[i].A.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[A] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].A.IsDown, State->Input.Controllers[i].A.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].A.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_B))
                {
                    if (!State->Input.Controllers[i].B.IsDown)
                    {
                        State->Input.Controllers[i].B.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[B] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].B.IsDown, State->Input.Controllers[i].B.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].B.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_X))
                {
                    if (!State->Input.Controllers[i].X.IsDown)
                    {
                        State->Input.Controllers[i].X.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[X] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].X.IsDown, State->Input.Controllers[i].X.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].X.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_Y))
                {
                    if (!State->Input.Controllers[i].Y.IsDown)
                    {
                        State->Input.Controllers[i].Y.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Y] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Y.IsDown, State->Input.Controllers[i].Y.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Y.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_BACK))
                {
                    if (!State->Input.Controllers[i].Back.IsDown)
                    {
                        State->Input.Controllers[i].Back.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Back] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Back.IsDown, State->Input.Controllers[i].Back.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Back.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_GUIDE))
                {
                    if (!State->Input.Controllers[i].Guide.IsDown)
                    {
                        State->Input.Controllers[i].Guide.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Guide] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Guide.IsDown, State->Input.Controllers[i].Guide.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Guide.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_START))
                {
                    if (!State->Input.Controllers[i].Start.IsDown)
                    {
                        State->Input.Controllers[i].Start.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Start] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Start.IsDown, State->Input.Controllers[i].Start.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Start.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_LEFTSTICK))
                {
                    if (!State->Input.Controllers[i].LeftStick.IsDown)
                    {
                        State->Input.Controllers[i].LeftStick.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[LeftStick] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].LeftStick.IsDown, State->Input.Controllers[i].LeftStick.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].LeftStick.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_RIGHTSTICK))
                {
                    if (!State->Input.Controllers[i].RightStick.IsDown)
                    {
                        State->Input.Controllers[i].RightStick.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[RightStick] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].RightStick.IsDown, State->Input.Controllers[i].RightStick.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].RightStick.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
                {
                    if (!State->Input.Controllers[i].LeftShoulder.IsDown)
                    {
                        State->Input.Controllers[i].LeftShoulder.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[LeftShoulder] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].LeftShoulder.IsDown, State->Input.Controllers[i].LeftShoulder.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].LeftShoulder.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller, 
                                                SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
                {
                    if (!State->Input.Controllers[i].RightShoulder.IsDown)
                    {
                        State->Input.Controllers[i].RightShoulder.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[RightShoulder] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].RightShoulder.IsDown, State->Input.Controllers[i].RightShoulder.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].RightShoulder.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_DPAD_UP))
                {
                    if (!State->Input.Controllers[i].DPadUp.IsDown)
                    {
                        State->Input.Controllers[i].DPadUp.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadUp] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].DPadUp.IsDown, State->Input.Controllers[i].DPadUp.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].DPadUp.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_DPAD_DOWN))
                {
                    if (!State->Input.Controllers[i].DPadDown.IsDown)
                    {
                        State->Input.Controllers[i].DPadDown.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadDown] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].DPadDown.IsDown, State->Input.Controllers[i].DPadDown.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].DPadDown.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_DPAD_LEFT))
                {
                    if (!State->Input.Controllers[i].DPadLeft.IsDown)
                    {
                        State->Input.Controllers[i].DPadLeft.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadLeft] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].DPadLeft.IsDown, State->Input.Controllers[i].DPadLeft.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].DPadLeft.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                {
                    if (!State->Input.Controllers[i].DPadRight.IsDown)
                    {
                        State->Input.Controllers[i].DPadRight.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[DPadRight] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].DPadRight.IsDown, State->Input.Controllers[i].DPadRight.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].DPadRight.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_MISC1))
                {
                    if (!State->Input.Controllers[i].Misc1.IsDown)
                    {
                        State->Input.Controllers[i].Misc1.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Misc1] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Misc1.IsDown, State->Input.Controllers[i].Misc1.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Misc1.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_PADDLE1))
                {
                    if (!State->Input.Controllers[i].Paddle1.IsDown)
                    {
                        State->Input.Controllers[i].Paddle1.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle1] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Paddle1.IsDown, State->Input.Controllers[i].Paddle1.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Paddle1.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_PADDLE2))
                {
                    if (!State->Input.Controllers[i].Paddle2.IsDown)
                    {
                        State->Input.Controllers[i].Paddle2.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle2] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Paddle2.IsDown, State->Input.Controllers[i].Paddle2.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Paddle2.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_PADDLE3))
                {
                    if (!State->Input.Controllers[i].Paddle3.IsDown)
                    {
                        State->Input.Controllers[i].Paddle3.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle3] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Paddle3.IsDown, State->Input.Controllers[i].Paddle3.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Paddle3.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_PADDLE4))
                {
                    if (!State->Input.Controllers[i].Paddle4.IsDown)
                    {
                        State->Input.Controllers[i].Paddle4.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[Paddle4] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].Paddle4.IsDown, State->Input.Controllers[i].Paddle4.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].Paddle4.IsDown = 0;
                }

                if (SDL_GameControllerGetButton(State->Input.Controllers[i].Controller,
                                                SDL_CONTROLLER_BUTTON_TOUCHPAD))
                {
                    if (!State->Input.Controllers[i].TouchPad.IsDown)
                    {
                        State->Input.Controllers[i].TouchPad.IsDown = 1;
                        ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Btn[TouchPad] IsDown[%d] WasDown[%d]",
                                i, State->Input.Controllers[i].TouchPad.IsDown, State->Input.Controllers[i].TouchPad.WasDown);
                    }
                }
                else
                {
                    State->Input.Controllers[i].TouchPad.IsDown = 0;
                }
            }
        }
    }

    if (SDL_NumJoysticks() > 0)
    {
        SDL_JoystickUpdate();
    }

    while (SDL_PollEvent(&Event))
    {
        switch (Event.type)
        {
            case SDL_QUIT:
            {
                State->Running = 0;
                ASC_Log(LOGLEVEL_DEBUG, "EVENT: Quit");
            } break;

            case SDL_WINDOWEVENT:
            {
                Window_Resize();

                switch (Event.window.event)
                {
                    case SDL_WINDOWEVENT_SHOWN:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Shown");
                    } break;

                    case SDL_WINDOWEVENT_HIDDEN:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Hidden");
                    } break;

                    case SDL_WINDOWEVENT_EXPOSED:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Exposed");
                        State->Window.Exposed = 1;
                        State->Window.Minimized = 0;
                    } break;

                    case SDL_WINDOWEVENT_MOVED:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Moved");
                    } break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Size Changed");
                    } break;

                    case SDL_WINDOWEVENT_MINIMIZED:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Minimized");
                        State->Window.Exposed = 0;
                        State->Window.Minimized = 1;
                    } break;

                    case SDL_WINDOWEVENT_MAXIMIZED:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Maximized");
                        State->Window.Maximized = 1;
                    } break;

                    case SDL_WINDOWEVENT_RESTORED:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Restored");
                        State->Window.Minimized = 0;
                        State->Window.Maximized = 0;
                    } break;

                    case SDL_WINDOWEVENT_ENTER:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Mouse Enter");
                        State->Window.MouseFocus = 1;
                    } break;

                    case SDL_WINDOWEVENT_LEAVE:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Mouse Leave");
                        State->Window.MouseFocus = 0;
                    } break;

                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window KB Focus Gained");
                        State->Window.KeyboardFocus = 1;
                    } break;

                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window KB Focus Lost");
                        State->Window.KeyboardFocus = 0;
                    } break;

                    case SDL_WINDOWEVENT_CLOSE:
                    {
                        ASC_Log(LOGLEVEL_DEBUG, "EVENT: Window Close");
                        State->Running = 0;
                    } break;
                }
            } break;

            case SDL_KEYDOWN:
            {
                bool KeyFound = 0;

                for (int i = 0; i < ASC_KEYCOUNT; i++)
                {
                    if (State->Input.Keys[i].Key == Event.key.keysym.sym)
                    {
                        KeyFound = 1;
                        State->Input.Keys[i].IsDown = 1;
                        State->Input.Keys[i].TextDown = 1;
                    }
                }

                if (!KeyFound)
                {
                    for (int i = 0; i < ASC_KEYCOUNT; i++)
                    {
                        if (State->Input.Keys[i].Key == 0)
                        {
                            KeyFound = 1;
                            State->Input.Keys[i].Key = Event.key.keysym.sym;
                            State->Input.Keys[i].IsDown = 1;
                            State->Input.Keys[i].TextDown = 1;
                            break;
                        }
                    }
                }

                if (!KeyFound)
                {
                    ASC_Log(LOGLEVEL_ERROR, "INPUT: Ran out of Key slots!");
                }

            } break;

            case SDL_KEYUP:
            {
                bool KeyFound = 0;

                for (int i = 0; i < ASC_KEYCOUNT; i++)
                {
                    if (State->Input.Keys[i].Key == Event.key.keysym.sym)
                    {
                        KeyFound = 1;
                        State->Input.Keys[i].IsDown = 0;
                        State->Input.Keys[i].TextDown = 0;
                    }
                }

                if (!KeyFound)
                {
                    ASC_Log(LOGLEVEL_ERROR, "INPUT: Got keyup message but key wasnt down! Key[%d]",
                            Event.key.keysym.sym);
                }

            } break;

            case SDL_TEXTEDITING:
            case SDL_TEXTINPUT:
            {
                ASC_Log(LOGLEVEL_DEBUG, "EVENT: TextEdit Event (%s)", Event.edit.text);
            } break;

            case SDL_MOUSEWHEEL:
            {
                if (Event.wheel.y > 0)
                {
                    State->Input.Mouse.MouseWheel = MW_UP;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: MouseWheel UP");
                }
                else if (Event.wheel.y < 0)
                {
                    State->Input.Mouse.MouseWheel = MW_DOWN;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: MouseWheel DOWN");
                }
                else if (Event.wheel.x < 0)
                {
                    State->Input.Mouse.MouseWheel = MW_LEFT;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: MouseWheel LEFT");
                }
                else if (Event.wheel.x > 0)
                {
                    State->Input.Mouse.MouseWheel = MW_RIGHT;
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: MouseWheel RIGHT");
                }
            } break;

            case SDL_MOUSEMOTION:
            {
                if (State->Window.MouseFocus)
                {
                    SDL_GetMouseState(&State->Input.Mouse.X, &State->Input.Mouse.Y);
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Mouse X[%d] Y[%d]", State->Input.Mouse.X, State->Input.Mouse.Y);
                }
            } break;

            case SDL_MOUSEBUTTONDOWN:
            {
                if (Event.button.button == SDL_BUTTON_LEFT)
                    State->Input.Mouse.Left.IsDown = 1;

                if (Event.button.button == SDL_BUTTON_RIGHT)
                    State->Input.Mouse.Right.IsDown = 1;

                if (Event.button.button == SDL_BUTTON_MIDDLE)
                    State->Input.Mouse.Middle.IsDown = 1;

                if (Event.button.button == SDL_BUTTON_X1)
                    State->Input.Mouse.X1.IsDown = 1;

                if (Event.button.button == SDL_BUTTON_X2)
                    State->Input.Mouse.X2.IsDown = 1;

            } break;

            case SDL_MOUSEBUTTONUP:
            {
                if (Event.button.button == SDL_BUTTON_LEFT)
                    State->Input.Mouse.Left.IsDown = 0;

                if (Event.button.button == SDL_BUTTON_RIGHT)
                    State->Input.Mouse.Right.IsDown = 0;

                if (Event.button.button == SDL_BUTTON_MIDDLE)
                    State->Input.Mouse.Middle.IsDown = 0;

                if (Event.button.button == SDL_BUTTON_X1)
                    State->Input.Mouse.X1.IsDown = 0;

                if (Event.button.button == SDL_BUTTON_X2)
                    State->Input.Mouse.X2.IsDown = 0;
            } break;

            case SDL_JOYDEVICEADDED:
            {
                bool SlotFound = 0;

                for (int i = 0; i < ASC_CONTROLLERS; i++)
                {
                    if (!State->Input.Controllers[i].Controller &&
                        !State->Input.Controllers[i].Connected)
                    {
                        SlotFound = 1;

                        if (SDL_IsGameController(Event.cdevice.which))
                        {
                            State->Input.Controllers[i].Controller = SDL_GameControllerOpen(Event.cdevice.which);
                            State->Input.Controllers[i].ID = Event.cdevice.which;
                            State->Input.Controllers[i].Connected = 1;
                            ASC_Log(LOGLEVEL_INFO, "INPUT: Controller[%d] Connected", i);

                            // ensure no doubles
                            for (int j = 0; j < ASC_CONTROLLERS; j++)
                            {
                                if (j != i && 
                                    State->Input.Controllers[i].Controller == State->Input.Controllers[j].Controller)
                                {
                                    SDL_GameControllerClose(State->Input.Controllers[j].Controller);
                                    SDL_memset(&State->Input.Controllers[j], 0, sizeof(ASC_Controller));
                                    ASC_Log(LOGLEVEL_INFO, "INPUT: Controller[%d] Duplicate Removed", j);
                                }
                            }

                            break;
                        }

                        else
                        {
                            ASC_Log(LOGLEVEL_ERROR, "INPUT: Gamepad not recognized.");
                            break;
                        }
                    }
                }

                if (!SlotFound)
                {
                    ASC_Log(LOGLEVEL_ERROR, "INPUT: Out of controller slots");
                }
                
            } break;

            case SDL_JOYDEVICEREMOVED:
            {
                for (int i = 0; i < ASC_CONTROLLERS; i++)
                {
                    if (State->Input.Controllers[i].ID == Event.cdevice.which)
                    {
                        if (State->Input.Controllers[i].Connected)
                        {
                            SDL_GameControllerClose(State->Input.Controllers[i].Controller);
                            SDL_memset(&State->Input.Controllers[i], 0, sizeof(ASC_Controller));
                            ASC_Log(LOGLEVEL_INFO, " INPUT: Controller[%d] Disconnected", i);
                            break;
                        }
                    }
                }
            } break;

            case SDL_JOYAXISMOTION:
            case SDL_JOYBALLMOTION:
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
            case SDL_CONTROLLERAXISMOTION:
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
            {
                if (Event.jaxis.value >= ASC_DEADZONE)
                {
                    ASC_Log(LOGLEVEL_DEBUG, "INPUT: Controller[%d] Axis[%d] Value[%d]", Event.jdevice.which, Event.jaxis.axis, Event.jaxis.value);
                }
            } break;

            default:
            {
                ASC_Log(LOGLEVEL_DEBUG, "EVENT: Unhandled Event[%u]", Event.type);
            } break;

        } // switch event.type
    } // while poll event
}

bool Input_KeySingle(SDL_Keycode _Key)
{
    int KeyFound = -1;

    for (int i = 0; i < ASC_KEYCOUNT; i++)
    {
        if (State->Input.Keys[i].Key == _Key)
        {
            KeyFound = i;
            break;
        }
    }

    if (KeyFound < 0)
    {
        return 0;
    }

    if (State->Input.Keys[KeyFound].WasDown)
    {
        return 0;
    }

    if (!State->Input.Keys[KeyFound].WasDown && State->Input.Keys[KeyFound].IsDown)
    {
        return 1;
    }

    return 0;
}

bool Input_KeyDown(SDL_Keycode _Key)
{
    int KeyFound = -1;

    for (int i = 0; i < ASC_KEYCOUNT; i++)
    {
        if (State->Input.Keys[i].Key == _Key)
        {
            KeyFound = i;
            break;
        }
    }

    if (KeyFound < 0)
    {
        return 0;
    }

    return State->Input.Keys[KeyFound].IsDown;
}

bool Input_KeyRepeat(SDL_Keycode _Key)
{
    int KeyFound = -1;

    for (int i = 0; i < ASC_KEYCOUNT; i++)
    {
        if (State->Input.Keys[i].Key == _Key)
        {
            KeyFound = i;
            break;
        }
    }

    if (KeyFound < 0)
    {
        return 0;
    }

    if (State->Input.Keys[KeyFound].TextDown)
    {
        State->Input.Keys[KeyFound].TextDown = 0;
        return 1;
    }

    return 0;
}

bool Input_KeyUp(SDL_Keycode _Key)
{
    int KeyFound = -1;

    for (int i = 0; i < ASC_KEYCOUNT; i++)
    {
        if (State->Input.Keys[i].Key == _Key)
        {
            KeyFound = i;
            break;
        }
    }

    if (KeyFound < 0)
    {
        return 0;
    }

    if (!State->Input.Keys[KeyFound].IsDown && State->Input.Keys[KeyFound].WasDown)
    {
        return 1;
    }

    return 0;
}

bool Input_MouseSingle(Uint8 _Button)
{
    if (_Button == SDL_BUTTON_LEFT)
    {
        if (State->Input.Mouse.Left.WasDown)
        {
            return 0;
        }

        if (!State->Input.Mouse.Left.WasDown && State->Input.Mouse.Left.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_BUTTON_RIGHT)
    {
        if (State->Input.Mouse.Right.WasDown)
        {
            return 0;
        }

        if (!State->Input.Mouse.Right.WasDown && State->Input.Mouse.Right.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_BUTTON_MIDDLE)
    {
        if (State->Input.Mouse.Middle.WasDown)
        {
            return 0;
        }

        if (!State->Input.Mouse.Middle.WasDown && State->Input.Mouse.Middle.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_BUTTON_X1)
    {
        if (State->Input.Mouse.X1.WasDown)
        {
            return 0;
        }

        if (!State->Input.Mouse.X1.WasDown && State->Input.Mouse.X1.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_BUTTON_X2)
    {
        if (State->Input.Mouse.X2.WasDown)
        {
            return 0;
        }

        if (!State->Input.Mouse.X2.WasDown && State->Input.Mouse.X2.IsDown)
        {
            return 1;
        }
    }

    return 0;
}

bool Input_MouseDown(Uint8 _Button)
{
    if (_Button == SDL_BUTTON_LEFT)
    {
        return State->Input.Mouse.Left.IsDown;
    }

    else if (_Button == SDL_BUTTON_RIGHT)
    {
        return State->Input.Mouse.Right.IsDown;
    }

    else if (_Button == SDL_BUTTON_MIDDLE)
    {
        return State->Input.Mouse.Middle.IsDown;
    }

    else if (_Button == SDL_BUTTON_X1)
    {
        return State->Input.Mouse.X1.IsDown;
    }

    else if (_Button == SDL_BUTTON_X2)
    {
        return State->Input.Mouse.X2.IsDown;
    }

    return 0;
}

bool Input_MouseUp(Uint8 _Button)
{
    if (_Button == SDL_BUTTON_LEFT)
    {
        if (!State->Input.Mouse.Left.IsDown && State->Input.Mouse.Left.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_BUTTON_RIGHT)
    {
        if (!State->Input.Mouse.Right.IsDown && State->Input.Mouse.Right.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_BUTTON_MIDDLE)
    {
        if (!State->Input.Mouse.Middle.IsDown && State->Input.Mouse.Middle.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_BUTTON_X1)
    {
        if (!State->Input.Mouse.X1.IsDown && State->Input.Mouse.X1.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_BUTTON_X2)
    {
        if (!State->Input.Mouse.X2.IsDown && State->Input.Mouse.X2.WasDown)
        {
            return 1;
        }

        return 0;
    }

    return 0;
}

bool Input_MouseWheel(ASC_MOUSEWHEEL _Direction)
{
    if (State->Input.Mouse.MouseWheel == _Direction) return 1;
    else return 0;
}

bool Input_ControllerBtnSingle(int _Controller, SDL_GameControllerButton _Button)
{
    if (!State->Input.Controllers[_Controller].Connected) return 0;

    if (_Button == SDL_CONTROLLER_BUTTON_A)
    {
        if (State->Input.Controllers[_Controller].A.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].A.WasDown && 
            State->Input.Controllers[_Controller].A.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_B)
    {
        if (State->Input.Controllers[_Controller].B.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].B.WasDown &&
            State->Input.Controllers[_Controller].B.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_X)
    {
        if (State->Input.Controllers[_Controller].X.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].X.WasDown &&
            State->Input.Controllers[_Controller].X.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_Y)
    {
        if (State->Input.Controllers[_Controller].Y.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Y.WasDown &&
            State->Input.Controllers[_Controller].Y.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_BACK)
    {
        if (State->Input.Controllers[_Controller].Back.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Back.WasDown &&
            State->Input.Controllers[_Controller].Back.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_GUIDE)
    {
        if (State->Input.Controllers[_Controller].Guide.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Guide.WasDown &&
            State->Input.Controllers[_Controller].Guide.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_START)
    {
        if (State->Input.Controllers[_Controller].Start.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Start.WasDown &&
            State->Input.Controllers[_Controller].Start.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
    {
        if (State->Input.Controllers[_Controller].LeftStick.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].LeftStick.WasDown &&
            State->Input.Controllers[_Controller].LeftStick.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
    {
        if (State->Input.Controllers[_Controller].RightStick.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].RightStick.WasDown &&
            State->Input.Controllers[_Controller].RightStick.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
    {
        if (State->Input.Controllers[_Controller].LeftShoulder.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].LeftShoulder.WasDown &&
            State->Input.Controllers[_Controller].LeftShoulder.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
    {
        if (State->Input.Controllers[_Controller].RightShoulder.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].RightShoulder.WasDown &&
            State->Input.Controllers[_Controller].RightShoulder.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_UP)
    {
        if (State->Input.Controllers[_Controller].DPadUp.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].DPadUp.WasDown &&
            State->Input.Controllers[_Controller].DPadUp.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
    {
        if (State->Input.Controllers[_Controller].DPadDown.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].DPadDown.WasDown &&
            State->Input.Controllers[_Controller].DPadDown.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
    {
        if (State->Input.Controllers[_Controller].DPadLeft.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].DPadLeft.WasDown &&
            State->Input.Controllers[_Controller].DPadLeft.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
    {
        if (State->Input.Controllers[_Controller].DPadRight.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].DPadRight.WasDown &&
            State->Input.Controllers[_Controller].DPadRight.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_MISC1)
    {
        if (State->Input.Controllers[_Controller].Misc1.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Misc1.WasDown &&
            State->Input.Controllers[_Controller].Misc1.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE1)
    {
        if (State->Input.Controllers[_Controller].Paddle1.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Paddle1.WasDown &&
            State->Input.Controllers[_Controller].Paddle1.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE2)
    {
        if (State->Input.Controllers[_Controller].Paddle2.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Paddle2.WasDown &&
            State->Input.Controllers[_Controller].Paddle2.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE3)
    {
        if (State->Input.Controllers[_Controller].Paddle3.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Paddle3.WasDown &&
            State->Input.Controllers[_Controller].Paddle3.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE4)
    {
        if (State->Input.Controllers[_Controller].Paddle4.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].Paddle4.WasDown &&
            State->Input.Controllers[_Controller].Paddle4.IsDown)
        {
            return 1;
        }
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_TOUCHPAD)
    {
        if (State->Input.Controllers[_Controller].TouchPad.WasDown)
        {
            return 0;
        }

        if (!State->Input.Controllers[_Controller].TouchPad.WasDown &&
            State->Input.Controllers[_Controller].TouchPad.IsDown)
        {
            return 1;
        }
    }

    return 0;
}

bool Input_ControllerBtnDown(int _Controller, SDL_GameControllerButton _Button)
{
    if (!State->Input.Controllers[_Controller].Connected) return 0;

    if (_Button == SDL_CONTROLLER_BUTTON_A)
    {
        return State->Input.Controllers[_Controller].A.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_B)
    {
        return State->Input.Controllers[_Controller].B.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_X)
    {
        return State->Input.Controllers[_Controller].X.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_Y)
    {
        return State->Input.Controllers[_Controller].Y.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_BACK)
    {
        return State->Input.Controllers[_Controller].Back.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_GUIDE)
    {
        return State->Input.Controllers[_Controller].Guide.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_START)
    {
        return State->Input.Controllers[_Controller].Start.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
    {
        return State->Input.Controllers[_Controller].LeftStick.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
    {
        return State->Input.Controllers[_Controller].RightStick.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
    {
        return State->Input.Controllers[_Controller].LeftShoulder.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
    {
        return State->Input.Controllers[_Controller].RightShoulder.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_UP)
    {
        return State->Input.Controllers[_Controller].DPadUp.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
    {
        return State->Input.Controllers[_Controller].DPadDown.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
    {
        return State->Input.Controllers[_Controller].DPadLeft.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
    {
        return State->Input.Controllers[_Controller].DPadRight.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_MISC1)
    {
        return State->Input.Controllers[_Controller].Misc1.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE1)
    {
        return State->Input.Controllers[_Controller].Paddle1.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE2)
    {
        return State->Input.Controllers[_Controller].Paddle2.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE3)
    {
        return State->Input.Controllers[_Controller].Paddle3.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE4)
    {
        return State->Input.Controllers[_Controller].Paddle4.IsDown;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_TOUCHPAD)
    {
        return State->Input.Controllers[_Controller].TouchPad.IsDown;
    }

    return 0;
}

bool Input_ControllerBtnUp(int _Controller, SDL_GameControllerButton _Button)
{
    if (!State->Input.Controllers[_Controller].Connected) return 0;

    if (_Button == SDL_CONTROLLER_BUTTON_A)
    {
        if (!State->Input.Controllers[_Controller].A.IsDown &&
            State->Input.Controllers[_Controller].A.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_B)
    {
        if (!State->Input.Controllers[_Controller].B.IsDown &&
            State->Input.Controllers[_Controller].B.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_X)
    {
        if (!State->Input.Controllers[_Controller].X.IsDown &&
            State->Input.Controllers[_Controller].X.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_Y)
    {
        if (!State->Input.Controllers[_Controller].Y.IsDown &&
            State->Input.Controllers[_Controller].Y.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_BACK)
    {
        if (!State->Input.Controllers[_Controller].Back.IsDown &&
            State->Input.Controllers[_Controller].Back.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_GUIDE)
    {
        if (!State->Input.Controllers[_Controller].Guide.IsDown &&
            State->Input.Controllers[_Controller].Guide.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_START)
    {
        if (!State->Input.Controllers[_Controller].Start.IsDown &&
            State->Input.Controllers[_Controller].Start.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_LEFTSTICK)
    {
        if (!State->Input.Controllers[_Controller].LeftStick.IsDown &&
            State->Input.Controllers[_Controller].LeftStick.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_RIGHTSTICK)
    {
        if (!State->Input.Controllers[_Controller].RightStick.IsDown &&
            State->Input.Controllers[_Controller].RightStick.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
    {
        if (!State->Input.Controllers[_Controller].LeftShoulder.IsDown &&
            State->Input.Controllers[_Controller].LeftShoulder.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
    {
        if (!State->Input.Controllers[_Controller].RightShoulder.IsDown &&
            State->Input.Controllers[_Controller].RightShoulder.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_UP)
    {
        if (!State->Input.Controllers[_Controller].DPadUp.IsDown &&
            State->Input.Controllers[_Controller].DPadUp.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
    {
        if (!State->Input.Controllers[_Controller].DPadDown.IsDown &&
            State->Input.Controllers[_Controller].DPadDown.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
    {
        if (!State->Input.Controllers[_Controller].DPadLeft.IsDown &&
            State->Input.Controllers[_Controller].DPadLeft.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
    {
        if (!State->Input.Controllers[_Controller].DPadRight.IsDown &&
            State->Input.Controllers[_Controller].DPadRight.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_MISC1)
    {
        if (!State->Input.Controllers[_Controller].Misc1.IsDown &&
            State->Input.Controllers[_Controller].Misc1.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE1)
    {
        if (!State->Input.Controllers[_Controller].Paddle1.IsDown &&
            State->Input.Controllers[_Controller].Paddle1.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE2)
    {
        if (!State->Input.Controllers[_Controller].Paddle2.IsDown &&
            State->Input.Controllers[_Controller].Paddle2.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE3)
    {
        if (!State->Input.Controllers[_Controller].Paddle3.IsDown &&
            State->Input.Controllers[_Controller].Paddle3.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_PADDLE4)
    {
        if (!State->Input.Controllers[_Controller].Paddle4.IsDown &&
            State->Input.Controllers[_Controller].Paddle4.WasDown)
        {
            return 1;
        }

        return 0;
    }

    else if (_Button == SDL_CONTROLLER_BUTTON_TOUCHPAD)
    {
        if (!State->Input.Controllers[_Controller].TouchPad.IsDown &&
            State->Input.Controllers[_Controller].TouchPad.WasDown)
        {
            return 1;
        }

        return 0;
    }

    return 0;
}

i16 Input_ControllerAxisRaw(int _Controller, SDL_GameControllerAxis _Axis)
{
    if (!State->Input.Controllers[_Controller].Connected) return 0;

    i16 Result = 0;

    if (_Axis == SDL_CONTROLLER_AXIS_LEFTX)
        Result = State->Input.Controllers[_Controller].LeftX;
    if (_Axis == SDL_CONTROLLER_AXIS_LEFTY)
        Result = State->Input.Controllers[_Controller].LeftY;
    if (_Axis == SDL_CONTROLLER_AXIS_RIGHTX)
        Result = State->Input.Controllers[_Controller].RightX;
    if (_Axis == SDL_CONTROLLER_AXIS_RIGHTY)
        Result = State->Input.Controllers[_Controller].RightY;
    if (_Axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
        Result = State->Input.Controllers[_Controller].LeftTrigger;
    if (_Axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
        Result = State->Input.Controllers[_Controller].RightTrigger;

    return Result;
}

i16 Input_ControllerAxisDZ(int _Controller, SDL_GameControllerAxis _Axis)
{
    if (!State->Input.Controllers[_Controller].Connected) return 0;

    i16 Result = 0;

    if (_Axis == SDL_CONTROLLER_AXIS_LEFTX)
        Result = State->Input.Controllers[_Controller].LeftX;
    if (_Axis == SDL_CONTROLLER_AXIS_LEFTY)
        Result = State->Input.Controllers[_Controller].LeftY;
    if (_Axis == SDL_CONTROLLER_AXIS_RIGHTX)
        Result = State->Input.Controllers[_Controller].RightX;
    if (_Axis == SDL_CONTROLLER_AXIS_RIGHTY)
        Result = State->Input.Controllers[_Controller].RightY;
    if (_Axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
        Result = State->Input.Controllers[_Controller].LeftTrigger;
    if (_Axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
        Result = State->Input.Controllers[_Controller].RightTrigger;

    i16 DZ = ASC_DEADZONE;
    if (Result <= -DZ) return Result;
    if (Result >= DZ) return Result;

    return 0;
}