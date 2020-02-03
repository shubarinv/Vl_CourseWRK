//
// Created by vhundef on 27.12.2019.
//

#ifndef PROGONHLANG_UI_BUTTON_HPP
#define PROGONHLANG_UI_BUTTON_HPP


#include <SDL_rect.h>
#include <string>
#include "../screenManager.hpp"

class uiButton {
private:
    SDL_Rect body{};
    SDL_Color defaultColor{127, 179, 114,255};
    SDL_Color hoverColor{89, 150, 74,255};
    SDL_Color disabledColor{82, 82, 82,255};
    ScreenManager *uiManager{};
    std::string text = "UNSET";
    int textPosX;
    int textPosY;
    bool enabled{true};
    bool hover{false};
    int fontSize{20};
public:
    uiButton(const std::string &string, ScreenManager *_uiManager, int x, int y, int width, int height,
             int _fontSize = 20) {
        if (_uiManager == nullptr) {
            std::string error = "UIButton->Constructor->screenManager is null";
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s", error.c_str());
            throw std::runtime_error("Unable to button screenManager is null");
        }
        uiManager = _uiManager;
        body.x = x;
        body.y = y;
        body.w = width;
        body.h = height;
        fontSize = _fontSize;
        text = string;
        calculateTextPosition();
    }

    [[nodiscard]] bool isEnabled() const {
        return enabled;
    }

    void setEnabled(bool _enabled) {
        uiButton::enabled = _enabled;
    }

    [[nodiscard]] bool isHover() const {
        return hover;
    }

    bool collisionCheck(int x, int y) {
        return x >= body.x && x <= body.x + body.w && y >= body.y && y <= body.y + body.h;
    }

    void calculateTextPosition() {
        int textWidth = uiManager->getTextSize(text, fontSize).a;
        int textHeight = uiManager->getTextSize(text, fontSize).b;
        textPosX = (body.x + body.w / 2) - textWidth / 2;
        textPosY = (body.y + body.h / 2) - textHeight / 2;
    }

    void draw(int x, int y) {
        //SDL_RenderDrawRect(uiManager->getRenderer(), &body);
        hover = collisionCheck(x, y);
        if (enabled) {
            if (hover) {
                SDL_SetRenderDrawColor(uiManager->getRenderer(), hoverColor.r, hoverColor.g, hoverColor.b, 255);
            } else {
                SDL_SetRenderDrawColor(uiManager->getRenderer(), defaultColor.r, defaultColor.g, defaultColor.b, 255);
            }
        } else {
            SDL_SetRenderDrawColor(uiManager->getRenderer(), disabledColor.r, disabledColor.g, disabledColor.b, 255);
        }
        // Render rect
        SDL_RenderFillRect(uiManager->getRenderer(), &body);
        uiManager->printText(text, textPosX, textPosY, {255, 255, 255}, fontSize);
    }

    void setDefaultColor(const SDL_Color &_defaultColor) {
        uiButton::defaultColor = _defaultColor;
    }

    void setHoverColor(const SDL_Color &_hoverColor) {
        uiButton::hoverColor = _hoverColor;
    }

    void setDisabledColor(const SDL_Color &_disabledColor) {
        uiButton::disabledColor = _disabledColor;
    }

};


#endif //PROGONHLANG_UI_BUTTON_HPP
