//
// Created by vhund on 02.02.2020.
//

#ifndef VL_COURSEWORK_OBJECT_HPP
#define VL_COURSEWORK_OBJECT_HPP


#include <string>
#include <iostream>
#include <SDL_render.h>
#include <SDL_log.h>
#include "screenManager.hpp"
#include "game.hpp"

class Object {
	struct twoInt{
		int a{0},b{0};
	};
private:
	ScreenManager *screenManager{};
	twoInt location;
	SDL_Texture *sprite;
	twoInt size;
public:
	const twoInt &getLocation() const {
		return location;
	}

	const twoInt &getSize() const {
		return size;
	}

private:
	std::string spriteName{};
	int weight{0};
	bool isGrabbed{false};
public:
	[[nodiscard]] int getWeight() const {
		return weight;
	}

	[[nodiscard]] bool getGrabbed() const {
		return isGrabbed;
	}

	void redraw() {
		updateLocation();
		screenManager->renderTexture(sprite,location.a,location.b);
//		std::cout<<this<< " is grabbed: "<<getGrabbed()<<std::endl;
	}

	void setIsGrabbed(bool _isGrabbed) {
	//	std::cout<<this<< " set is grabbed: "<<_isGrabbed<<std::endl;
		isGrabbed = _isGrabbed;
	}

	bool checkCollision(twoInt x_y) {
		if ((x_y.a >= location.a && x_y.a <= location.a+size.a) && (x_y.b >= location.b && x_y.b <= location.b+size.b)) {
	//		std::cout << "Hover over: " << this << std::endl;
			setIsGrabbed(true);
			return true;
		}
		isGrabbed= false;
		return false;
	}

	void updateLocation() {
		if (isGrabbed) {
			location.a = screenManager->getInputManager()->getMouseCoords().x; ///< x
			location.b = screenManager->getInputManager()->getMouseCoords().y; ///< y
		}
	}

	void setWeight(int _weight) {
		if (_weight >= 0)
			weight = _weight;
		else {
			std::cout << "Invalid Weight: _weight is negative" << std::endl;
			weight = 0;
		}
	}

	explicit Object(int _weight = 0, std::string _fileName = "", ScreenManager *pScreenManager = nullptr) {
		if (pScreenManager == nullptr) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Object::Object() got empty screenManager");
			throw (std::runtime_error("Object::Object() pScreenManager is NULL"));
		}
		screenManager = pScreenManager;

		if (_weight >= 0)
			weight = _weight;
		else {
			std::cout << "Invalid Weight: _weight is negative" << std::endl;
			weight = 0;
		}

		spriteName = std::move(_fileName);
		sprite=screenManager->loadTexture(spriteName);
		location.a=100;
		location.b=200;
		size.a=ScreenManager::getTextureSize(sprite).a;
		size.b=ScreenManager::getTextureSize(sprite).a;
	}


};


#endif //VL_COURSEWORK_OBJECT_HPP
