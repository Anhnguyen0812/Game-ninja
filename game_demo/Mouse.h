#pragma once

#ifndef MOUSE_H
#define MOUSE_H

class  Mouse: public BaseObject
{
public:
	Mouse();
	~ Mouse();

	void update_mouse() {
		SDL_GetMouseState(&rect_.x, &rect_.y);
	}


private:
	
};

#endif // !MOUSE_H
