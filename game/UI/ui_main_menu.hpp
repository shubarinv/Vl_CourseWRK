//
// Created by vhundef on 27.12.2019.
//

#ifndef PROGONHLANG_UI_MAIN_MENU_HPP
#define PROGONHLANG_UI_MAIN_MENU_HPP


#include <utility>

#include "ui_base.hpp"
#include "ui_button.hpp"

class UI_MainMenu : private virtual UI_Base {
public:
    void show() {
        play_btn->draw(uiManager->getInputManager()->getMouseCoords().x,
                       uiManager->getInputManager()->getMouseCoords().y);
        quit_btn->draw(uiManager->getInputManager()->getMouseCoords().x,
                       uiManager->getInputManager()->getMouseCoords().y);
    }

    char act() {
        if (play_btn->isHover() && uiManager->getInputManager()->getMouseState() & SDL_BUTTON_LMASK) { return 'r'; }
        else if (quit_btn->isHover() &&
                 uiManager->getInputManager()->getMouseState() & SDL_BUTTON_LMASK) { return 'q'; }
        else return 'm';
    }

    uiButton *play_btn;
    uiButton *quit_btn;

    UI_MainMenu(ScreenManager *ui_Manager, SDL_Window *window) : UI_Base(ui_Manager, window) {

            play_btn = new uiButton("Start", uiManager, uiManager->getWindowResolutionX() / 2 - 100,
                                    150, 200, 50, 20);
            quit_btn = new uiButton("Quit", uiManager, uiManager->getWindowResolutionX() / 2 - 100, 250,
                                    200, 50, 20);


    }

    ~UI_MainMenu() {
        delete play_btn;
        delete quit_btn;
    }
};


#endif //PROGONHLANG_UI_MAIN_MENU_HPP
