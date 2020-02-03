//
// Created by vhund on 02.02.2020.
//

#ifndef VL_COURSEWORK_FIELD_HPP
#define VL_COURSEWORK_FIELD_HPP


#include <list>
#include <vector>
#include "screenManager.hpp"
#include "Object.hpp"
#include "game.hpp"

class Field {
	struct twoInt {
		int a{0}, b{0};
	};
private:
	ScreenManager *screenManager{};
	std::vector<Object> left{};
	std::vector<Object> right{};
public:
	explicit Field(ScreenManager *pScreenManager = nullptr) {
		if (pScreenManager == nullptr) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Field::Field() got empty screenManager");
			throw (std::runtime_error("Field::Field() pScreenManager is NULL"));
		}
		screenManager = pScreenManager;
		left.emplace_back(10, "Sprites/Weights.png", screenManager);
	}

	twoInt countSidesWeight() {
		int leftWeight{0};
		int rightWeight{0};
		for (const Object &obj:left) {
			leftWeight += obj.getWeight();
		}
		for (const Object &obj:right) {
			rightWeight += obj.getWeight();
		}
		return {leftWeight, rightWeight};
	}

	void redraw() {
		for (auto &obj:left) {
			obj.redraw();
		}
		for (auto &obj:right) {
			obj.redraw();
		}

	}

	void checkForGrab() {
		twoInt mouseLocation;
		mouseLocation.a = screenManager->getInputManager()->getMouseCoords().x;
		mouseLocation.b = screenManager->getInputManager()->getMouseCoords().y;
		if (screenManager->getInputManager()->getMouseState() & SDL_BUTTON_LMASK) {
			std::cout << "\n--------------\nLMB Pressed (" << SDL_GetTicks() << ")" << std::endl;
			for (auto &i : left) {
				if (i.getGrabbed())return;
			}
			for (auto &i : right) {
				if (i.getGrabbed())return;
			}
			std::cout << "There is no grabbed OBJ" << std::endl;
			for (auto &i : left) {
				if (i.checkCollision({mouseLocation.a, mouseLocation.b})) {
					return;
				}
			}
			for (auto &i : right) {
				if (i.checkCollision({mouseLocation.a, mouseLocation.b})) {
					return;
				}
			}
		} else {
			for (auto &i : left) {
				if (i.getGrabbed()) {i.setIsGrabbed(false); return; }
			}
			for (auto &i : right) {
				if (i.getGrabbed()) {i.setIsGrabbed(false); return; }
			}
		}
	}


};


#endif //VL_COURSEWORK_FIELD_HPP
