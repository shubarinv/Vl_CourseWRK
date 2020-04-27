//
// Created by vhundef on 24.12.2019.
//

#ifndef PROGONHLANG_UI_MANAGER_HPP
#define PROGONHLANG_UI_MANAGER_HPP


#include <SDL_ttf.h>
#include <iostream>
#include "input_manager.hpp"

class ScreenManager {
private:
    TTF_Font *font{};
    SDL_Renderer *renderer;
    int fontSize = {};
    int windowResolutionX{0};
    int windowResolutionY{0};
    InputManager *inputManager;
    SDL_Window *window{};
    struct twoInt {
        int a;
        int b;
    };
public:
    std::string fontName = "Roboto-Medium";

    [[nodiscard]] SDL_Window *getWindow() const {
        return window;
    }

    [[nodiscard]] int getWindowResolutionX() const {
        return windowResolutionX;
    }

    [[nodiscard]] int getWindowResolutionY() const {
        return windowResolutionY;
    }

    [[nodiscard]] SDL_Renderer *getRenderer() const {
        return renderer;
    }

    [[nodiscard]] int getFontSize() const {
        return fontSize;
    }

    [[nodiscard]] InputManager *getInputManager() const {
        return inputManager;
    }

public:
    ScreenManager(SDL_Surface *pSurface, SDL_Renderer *pRenderer, SDL_Window *win, InputManager *pManager) {
        if (TTF_Init() == -1) {
            std::string error = TTF_GetError();
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s", error.c_str());
            throw std::runtime_error("ScreenManager->TTF_OpenFont: Attempt to open font was unsuccessful");
        }

        font = TTF_OpenFont((fontName + ".ttf").c_str(), 16);
        if (!font) {
            std::string error = TTF_GetError();
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s", error.c_str());
            throw std::runtime_error("ScreenManager->TTF_OpenFont: Attempt to open font was unsuccessful");
        }
        renderer = pRenderer;
        window = win;
        inputManager = pManager;
        SDL_GetWindowSize(win, &windowResolutionX, &windowResolutionY);
    }

    void changeFontSize(int size) {
        TTF_CloseFont(font);
        font = TTF_OpenFont((fontName + ".ttf").c_str(), size);
        if (!font) {
            std::string error = TTF_GetError();
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s", error.c_str());
            throw std::runtime_error("ScreenManager->changeFontSize: Attempt to change font size was unsuccessful");
        }
        fontSize = size;
    }

    /**@brief печатает текст
 * @param text текст который нужно напечатать
 * @param x местоположение по оси X
 * @param y Местоположение по оси Y
 * @param color Цвет текста
 * @param font_size размер текста
 * @param centred Должен ли тескст быть по центру указанных координат
 */
    void printText(const std::string &text, int x, int y, SDL_Color color = {0, 0, 0},
                   int font_size = 16, bool centred = false) {
        changeFontSize(font_size);
        auto surfaceMessage = TTF_RenderUTF8_Solid(font, text.c_str(),
                                                   color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
        auto Message = SDL_CreateTextureFromSurface(renderer,
                                                    surfaceMessage); //now you can convert it into a texture
        SDL_Rect Message_rect; //create a rect
        if (centred) {
            Message_rect.x = x - getTextSize(text, font_size).a / 2;  //controls the rect's x coordinate
            Message_rect.y = y - getTextSize(text, font_size).b / 2; // controls the rect's y coordinate
        } else {
            Message_rect.x = x;  //controls the rect's x coordinate
            Message_rect.y = y; // controls the rect's y coordinate
        }
        TTF_SizeUTF8(font, text.c_str(), &Message_rect.w, &Message_rect.h);
        SDL_RenderCopy(renderer, Message, nullptr,
                       &Message_rect); //you put the screenManager's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
        //  delete Message;

    }

    static int rgbToHex(SDL_Color rgb) {///< @brief used to convert SDL_Color to hex
        return rgb.r * rgb.g * rgb.b;
    }

    twoInt getTextSize(const std::string &_string, int _fontSize) {
        changeFontSize(_fontSize);
        twoInt widthAndHeight{};
        TTF_SizeUTF8(font, _string.c_str(), &widthAndHeight.a, &widthAndHeight.b);
        return widthAndHeight;
    }

    /**
* Загружает изображение в текстуру для рендерера
* @param file Путь к изображению
* @param ren Рендерер, на который эту текстуру можно будет отрисовать
* @return Возвращает текстуру, либо nullptr в случае ошибки.
*/
    SDL_Texture *loadTexture(const std::string &file) {
        SDL_Texture *texture = IMG_LoadTexture(renderer, file.c_str());
        if (!texture) {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" << "!! " << IMG_GetError()
                      << " !!\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl
                      << "USING NoTexture instead" << std::endl;
            //throw (std::runtime_error("ScreenManager.loadTexture() failed to load texture"));
            return loadTexture("noTex.png");
        }
        return texture;
    }

    /**
* Отобразить SDL_Texture на SDL_Renderer на координатах x, y, с масштабированием
* @param tex Текстура для отображения
* @param ren Рендерер
* @param x Координаты
* @param y
* @param w Фактический размер при отрисовке
* @param h
*/
    void renderTexture(SDL_Texture *tex, int x, int y, int w, int h) {
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        dst.w = w;
        dst.h = h;
        SDL_RenderCopy(renderer, tex, nullptr, &dst);
    }

/**
* Отрисовать SDL_Texture на SDL_Renderer на координатах x, y, без масштабирования
* @param tex Текстура
* @param ren Рендерер
* @param x Координаты
* @param y
*/
    void renderTexture(SDL_Texture *tex, int x, int y) {
        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        renderTexture(tex, x, y, w, h);
    }

    static twoInt getTextureSize(SDL_Texture *tex) {
        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        return {w, h};
    }
};


#endif //PROGONHLANG_UI_MANAGER_HPP
