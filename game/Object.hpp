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
	twoInt size;
	std::string spriteName{};
	SDL_Rect test{};
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
		SDL_RenderDrawRect(screenManager->getRenderer(), &test);
		SDL_SetRenderDrawColor(screenManager->getRenderer(), 255, 0,0, 255);
		SDL_RenderFillRect(screenManager->getRenderer(), &test);
		updateLocation();
		std::cout<<this<< " is grabbed: "<<getGrabbed()<<std::endl;
	}

	void setIsGrabbed(bool _isGrabbed) {
		std::cout<<this<< " set is grabbed: "<<_isGrabbed<<std::endl;
		isGrabbed = _isGrabbed;
	}

	bool checkCollision(twoInt x_y) {
		if ((x_y.a >= location.a && x_y.a <= location.a+size.a) && (x_y.b >= location.b && x_y.b <= location.b+size.b)) {
			std::cout << "Hover over: " << this << std::endl;
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
			test.x=location.a;
			test.y=location.b;
			std::cout<<"X: "<<test.x<<" Y:"<<test.y<<std::endl;
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

		test.x=100;
		test.y=200;
		test.h=20;
		test.w=20;
		location.a=test.x;
		location.b=test.y;
		size.a=test.w;
		size.b=test.h;
	}


};


#endif //VL_COURSEWORK_OBJECT_HPP
