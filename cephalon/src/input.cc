#include "input.h"
using namespace cephalon;

SDL_Window* Input::window_;
const Uint8* Input::keyboard_state_;
int Input::relative_x_;
int Input::relative_y_;