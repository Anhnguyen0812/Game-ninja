
#include "BossObject.h"

BossObject::BossObject() {
	frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0;
	y_pos_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	think_time_ = 0;
	map_x_ = 0;
	map_y_ = 0;
}
BossObject::~BossObject()
{

}

bool BossObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true)
	{
		width_frame_ = rect_.w / FRAME_NUM_32;
		height_frame_ = rect_.h;
	}
	return ret;
}

void BossObject::set_clips()
{
	
		for (int i = 0; i < FRAME_NUM_32; i++)
		{
			frame_clip_[i].x = width_frame_ * i;
			frame_clip_[i].y = 0;
			frame_clip_[i].w = width_frame_;
			frame_clip_[i].h = height_frame_;
 		}

}

void BossObject::Show(SDL_Renderer* des)
{
	if(think_time_ == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;
		frame_++;
		if (frame_ >= 32)
		{
			frame_ = 0;
		}
		SDL_Rect* currentClip = &frame_clip_[frame_];
		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
		if (currentClip != NULL)
		{
			renderQuad.w = currentClip->w;
			renderQuad.h = currentClip->h;
		}
		SDL_RenderCopy(des, p_object_, currentClip, &renderQuad);

	}
}

void BossObject::DoPlayer(Map& map_data)
{
	if (think_time_ == 0)
	{
		x_val_ = 0;
		y_val_ += GRAVITY_SPEED;

		if (y_val_ >= MAX_FALL_SPEED)
		{
			y_val_ = MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1) {
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += PLAYER_SPEED;
		}
		
		CheckToMap(map_data);
		
	}

	if (think_time_ > 0) {
		think_time_--;

		if (think_time_ == 0)
		{
			
			InitPlayer();
		}
	}

}

void BossObject::InitPlayer()
{
	x_val_ = 0;
	y_val_ = 0;

	if (x_pos_ > 256)
	{
		x_pos_ -= 256;
	}
	else
		x_pos_ = 0;
	y_pos_ = 0;
	y_pos_ = 0;
	think_time_ = 0;
	input_type_.left_ = 1;
}

void BossObject::CheckToMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;

	// check horizontal
	int height_min = min(height_frame_, TILE_SIZE);

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;
	y1 = (y_pos_ + 1) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		/* v1 y1   x1
		   v2 y1   x2
		   v3 y2   x1
		   v4 y2   x2
		*/

		if (x_val_ > 0) // moving to right
		{
			int v2 = map_data.tile[y1][x2];
			int v4 = map_data.tile[y2][x2];


			if (v2 != BLANK_TILE && v2 != 4 || v4 != BLANK_TILE && v4 != 4)
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;
				;
			}

		}
		else if (x_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v3 = map_data.tile[y2][x1];
			if (v1 != BLANK_TILE && v1 != 4 || v3 != BLANK_TILE && v3 != 4)
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;

			}
		}

	}

	// vertical

	int width_min = min(width_frame_, TILE_SIZE);
	x1 = (x_pos_ + 1) / TILE_SIZE;
	x2 = (x_pos_ + width_min - 1) / TILE_SIZE;
	y1 = (y_pos_ + y_val_ + 1) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0)
		{
			int v3 = map_data.tile[y2][x1];
			int v4 = map_data.tile[y2][x2];


			if (v3 != BLANK_TILE && v3 != 4 || v4 != BLANK_TILE && v4 != 4)
			{
				y_pos_ = (y2)*TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				y_val_ = 0;


			}
		}
		else if (y_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v2 = map_data.tile[y1][x2];


			if (v1 != BLANK_TILE && v1 != 4 || v2 != BLANK_TILE && v2 != 4)
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}

		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}

	if (y_pos_ > map_data.max_y_)
	{
		think_time_ = 50;
	}
}


void BossObject::InitBullet(SDL_Renderer* screen)
{
	BulletObject* p_bullet = new BulletObject();
	bool ret = p_bullet->LoadImg("img//boss//boss_bullet.png", screen);
	if (ret)
	{
		p_bullet->set_is_move(true);
		p_bullet->SetRect(rect_.x - 50, rect_.y + height_frame_ - 30);
		p_bullet->set_x_val(-15);

		bullet_list_.push_back(p_bullet);
	}
}

void BossObject::MakeBullet(SDL_Renderer* des, const int& x_limit, const const int& y_limit)
{
	if (frame_ == 18)
	{
		InitBullet(des);
	}

	for (int i = 0; i < bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move())
			{
				if (p_bullet->get_is_move())
				{
					p_bullet->HandleMove(x_limit, y_limit);
					p_bullet->Render(des);

				}
				else
				{
					p_bullet->Free();
					bullet_list_.erase(bullet_list_.begin() + i);
				}
			}
		}
	}
}
