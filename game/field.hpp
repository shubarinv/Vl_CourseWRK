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
	std::vector<Object> objectsOnField{};
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

		objectsOnField.emplace_back(10, "Sprites/Weights.png", screenManager, 10, 200, 40, 40);
		objectsOnField.emplace_back(100, "Sprites/Weights.png", screenManager, 10 + 50, 200, 45, 45);
		objectsOnField.emplace_back(1000, "Sprites/Weights.png", screenManager, 10 + 80, 200, 50, 50);
		objectsOnField.emplace_back(10000, "Sprites/Weights.png", screenManager, 10 + 120, 200, 55, 55);
		objectsOnField.emplace_back(100000, "Sprites/Weights.png", screenManager, 10 + 160, 200, 60, 60);
		objectsOnField.emplace_back(1000000, "Sprites/Weights.png", screenManager, 10 + 200, 200, 65, 65);
		objectsOnField.emplace_back(10000000, "Sprites/Weights.png", screenManager, 10 + 240, 200, 70, 70);
		objectsOnField.emplace_back(100000000, "Sprites/Weights.png", screenManager, 10 + 300, 200, 75, 70);

		weight_base = screenManager->loadTexture("Sprites/Weight_base.png");
		weight_plate_left = screenManager->loadTexture("Sprites/Weight_Plate.png");
		weight_plate_right = screenManager->loadTexture("Sprites/Weight_Plate.png");
	}

	char determinePosition(const Object *obj) {
		int l_offset{0};
		int r_offset{0};
		if (left_Weight > right_Weight)l_offset = 50;
		if (right_Weight > left_Weight)r_offset = 50;
		if ((obj->getLocation().a >= 60 &&
		     obj->getLocation().a + obj->getSize().a < 60 + ScreenManager::getTextureSize(weight_plate_left).a) &&
		    obj->getLocation().b <= screenManager->getWindowResolutionY() / 2 + l_offset) {
			return 'l';
		} else if ((obj->getLocation().a >= screenManager->getWindowResolutionX() -
		                                    ScreenManager::getTextureSize(weight_plate_right).a - 60 &&
		            obj->getLocation().a + obj->getSize().a < screenManager->getWindowResolutionX() - 60) &&
		           obj->getLocation().b <= screenManager->getWindowResolutionY() / 2 + r_offset) {
			return 'r';
		}
		return 'u';
	}


	twoInt countSidesWeight() {
		int leftWeight{0};
		int rightWeight{0};
		for (const Object &obj:objectsOnField) {
			switch (determinePosition(&obj)) {
				case 'l':
					leftWeight += obj.getWeight();
					break;
				case 'r':
					rightWeight += obj.getWeight();
					break;
			}
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
		} else if (right_Weight > left_Weight) {
			screenManager->renderTexture(weight_plate_left, 60, 50);
			screenManager->renderTexture(weight_plate_left, screenManager->getWindowResolutionX() -
			                                                ScreenManager::getTextureSize(weight_plate_right).a - 60,
			                             150);
		} else {
			screenManager->renderTexture(weight_plate_left, 60, 100);
			screenManager->renderTexture(weight_plate_left, screenManager->getWindowResolutionX() -
			                                                ScreenManager::getTextureSize(weight_plate_right).a - 60,
			                             100);
		}
		for (auto &obj:objectsOnField) {
			obj.redraw();
		}

	}

	void checkForGrab() {
		twoInt mouseLocation;
		mouseLocation.a = screenManager->getInputManager()->getMouseCoords().x;
		mouseLocation.b = screenManager->getInputManager()->getMouseCoords().y;
		if (screenManager->getInputManager()->getMouseState() & SDL_BUTTON_LMASK) {
			//std::cout << "\n--------------\nLMB Pressed (" << SDL_GetTicks() << ")" << std::endl;
			for (auto &i : objectsOnField) {
				if (i.getGrabbed())return;
			}
			//	std::cout << "There is no grabbed OBJ" << std::endl;
			for (auto &i : objectsOnField) {
				if (i.checkCollision({mouseLocation.a, mouseLocation.b})) {
					return;
				}
			}
		} else {
			for (auto &i : objectsOnField) {
				if (i.getGrabbed()) {
					i.setIsGrabbed(false);
					return;
				}
			}
		}
	}

	[[deprecated]]void addObjectsToLists() {
		left_Weight = countSidesWeight().a;
		right_Weight = countSidesWeight().b;
	}


};


#endif //VL_COURSEWORK_FIELD_HPP
