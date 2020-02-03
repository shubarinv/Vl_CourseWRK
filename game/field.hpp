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
	SDL_Texture *weight_plate_left{};
	SDL_Texture *weight_plate_right{};
	SDL_Texture *weight_base{};
	int left_Weight{0};
	int right_Weight{0};
public:
	explicit Field(ScreenManager *pScreenManager = nullptr) {
		if (pScreenManager == nullptr) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Field::Field() got empty screenManager");
			throw (std::runtime_error("Field::Field() pScreenManager is NULL"));
		}
		screenManager = pScreenManager;
		left.emplace_back(10, "Sprites/Weights.png", screenManager);
		weight_base = screenManager->loadTexture("Sprites/Weight_base.png");
		weight_plate_left = screenManager->loadTexture("Sprites/Weight_Plate.png");
		weight_plate_right = screenManager->loadTexture("Sprites/Weight_Plate.png");
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
		addObjectsToLists();
		screenManager->renderTexture(weight_base, screenManager->getWindowResolutionX() / 2 -
		                                          ScreenManager::getTextureSize(weight_base).a / 2,
		                             screenManager->getWindowResolutionY() -
		                             ScreenManager::getTextureSize(weight_base).b);
		if (left_Weight > right_Weight) {
			screenManager->renderTexture(weight_plate_left, 60, 150);
			screenManager->renderTexture(weight_plate_left, screenManager->getWindowResolutionX() -
			                                                ScreenManager::getTextureSize(weight_plate_right).a - 60,
			                             50);
		}
		else if(right_Weight>left_Weight){
			screenManager->renderTexture(weight_plate_left, 60, 50);
			screenManager->renderTexture(weight_plate_left, screenManager->getWindowResolutionX() -
			                                                ScreenManager::getTextureSize(weight_plate_right).a - 60,
			                             150);
		}
		else{
			screenManager->renderTexture(weight_plate_left, 60, 100);
			screenManager->renderTexture(weight_plate_left, screenManager->getWindowResolutionX() -
			                                                ScreenManager::getTextureSize(weight_plate_right).a - 60,
			                             100);
		}
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
			//std::cout << "\n--------------\nLMB Pressed (" << SDL_GetTicks() << ")" << std::endl;
			for (auto &i : left) {
				if (i.getGrabbed())return;
			}
			for (auto &i : right) {
				if (i.getGrabbed())return;
			}
			//	std::cout << "There is no grabbed OBJ" << std::endl;
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
				if (i.getGrabbed()) {
					i.setIsGrabbed(false);
					return;
				}
			}
			for (auto &i : right) {
				if (i.getGrabbed()) {
					i.setIsGrabbed(false);
					return;
				}
			}
		}
	}

	void addObjectsToLists() {
		int j = {0};
		for (auto &i : left) {
			if (i.getLocation().a >= screenManager->getWindowResolutionX() / 2 &&
			    i.getLocation().a + i.getSize().a < screenManager->getWindowResolutionX()) {
				right.emplace_back(i);
				left.erase(left.begin() + j);
				std::cout << "Moved " << this << " left-->Right" << std::endl;
			}
			j++;
		}
		j = 0;
		for (auto &i : right) {
			if (i.getLocation().a >= 0 &&
			    i.getLocation().a + i.getSize().a < screenManager->getWindowResolutionX() / 2) {
				left.emplace_back(i);
				right.erase(left.begin() + j);
				std::cout << "Moved " << this << " Right-->Left" << std::endl;
			}
			j++;
		}
		left_Weight = countSidesWeight().a;
		right_Weight = countSidesWeight().b;

	}


};


#endif //VL_COURSEWORK_FIELD_HPP
