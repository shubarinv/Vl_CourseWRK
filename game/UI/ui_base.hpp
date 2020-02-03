//
// Created by vhundef on 24.12.2019.
//

#ifndef PROGONHLANG_UI_BASE_HPP
#define PROGONHLANG_UI_BASE_HPP


#include <string>
#include <map>
#include <utility>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include "../screenManager.hpp"

class UI_Base {
public:
    UI_Base(ScreenManager *ui_Manager, SDL_Window *window) {
        uiManager = ui_Manager;
        win = window;
    }


protected:
    ScreenManager *uiManager{};
    SDL_Window *win{};
};


#endif //PROGONHLANG_UI_BASE_HPP
