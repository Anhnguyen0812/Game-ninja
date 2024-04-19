
#include "BulletObject.h"

BulletObject::BulletObject()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = true;
	frame = 0;

	bullet_type = SPHERE_BULLET;

	width_bullet = 0;
	height_bullet = 0;
	is_delete = true;
}

BulletObject::~BulletObject() 
{

}

void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
	
	rect_.x += x_val_ - player_x_val;
	rect_.y += y_val_;

	if (rect_.x > x_border || rect_.x < 0 || rect_.y > y_border || rect_.y < 0)
	{
		is_move_ = false;
	}
}

void BulletObject::Setclip()
{
	if (width_bullet > 0 && height_bullet > 0)
	{
		clip_frame[0].x = 0;
		clip_frame[0].y = 0;
		clip_frame[0].w = width_bullet;
		clip_frame[0].h = height_bullet;

		clip_frame[1].x = width_bullet;
		clip_frame[1].y = 0;
		clip_frame[1].w = width_bullet;
		clip_frame[1].h = height_bullet;

		clip_frame[2].x = 2 * width_bullet;
		clip_frame[2].y = 0;
		clip_frame[2].w = width_bullet;
		clip_frame[2].h = height_bullet;

	}
}

void BulletObject::Show(SDL_Renderer* des)
{
	frame++;

	if (frame >= 3) {
		frame = 0;
	}
	
		SDL_Rect* current_clip = &clip_frame[frame];
		SDL_Rect renderQuad = { rect_.x, rect_.y, width_bullet, height_bullet };
		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

}

void BulletObject::LoadImgBullet(SDL_Renderer* des) {

	if (bullet_type == LASER_BULLET)
	{
		LoadImg("img//laser_bullet.png", des);
		width_bullet = 30;
		height_bullet = 30;
	} 
	else
	{
		if (get_status() == 1) {
			LoadImg("img//shuriken.png", des);
		}else 
			if (get_status() == 2) {
			LoadImg("img//shuriken_left.png", des);
		}
		width_bullet = 30;
		height_bullet = 30;
	}

}
