#pragma once

#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_	

#include "BaseObject.h"
#include "CommonFunction.h"

class BulletObject : public BaseObject
{
public:
	BulletObject();
	~BulletObject();

	enum BulletType
	{
		SPHERE_BULLET = 50,
		LASER_BULLET = 51,
	};


	void set_x_val(const int& xval) { x_val_ = xval; }
	void set_y_val(const int& yval) { y_val_ = yval; }
	int get_x_val() const { return x_val_; }
	int get_y_val() const { return y_val_; }



	int get_width_frame()const { return width_bullet; }
	int get_height_frame()const { return height_bullet; }

	void get_rect_x(const int& rec_x) { rect_x = rec_x; }
	void get_rect_y(const int& rec_y) { rect_y = rec_y; }

	void set_is_move(const bool& ismove) { is_move_ = ismove; }
	bool get_is_move()const { return is_move_; }
	void set_delete(){ is_delete = false; }
	void set_delete_true(){ is_delete = true; }
	bool return_isdelete()const { return is_delete; }

	void Setclip();
	void HandleMove(const int& x_border, const int& y_border);

	void set_bullet_type(const unsigned int& bullettype_) { bullet_type = bullettype_; }
	unsigned int get_bullet_type()const { return bullet_type; }
	
	void set_bullet_size();

	void LoadImgBullet(SDL_Renderer* des);

	void return_status(int sttus) { status = sttus; }
	int get_status() { return status; }
	void Show(SDL_Renderer * des);

	void return_player_val(const int& xval) { player_x_val = xval; }

private:
	int x_val_;
	int y_val_;
	bool is_move_;

	SDL_Rect clip_frame[3];
	unsigned int bullet_type;

	int width_bullet;
	int height_bullet;
	int status;
	int frame;
	int rect_x;
	int rect_y;
	int player_x_val;
	bool is_delete;
};

#endif
