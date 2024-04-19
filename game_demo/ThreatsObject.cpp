
#include "ThreatsObject.h"
#include "windows.h"

ThreatsObject::ThreatsObject()
{

	map_x_ = 0;
	map_y_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	on_land = false;
	come_back_time = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	frame_ = 0;
	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	type_move_ = STATIC_THREAT;
	check_collision = 0;
	blood = 0;
	val = 12;

}

ThreatsObject::~ThreatsObject()
{

}


bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}
	return ret;
}

SDL_Rect ThreatsObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}

void ThreatsObject::RemoveBullet(const int& idx)
{
	int size = bullet_list.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = bullet_list.at(idx);
		bullet_list.erase(bullet_list.begin() + idx);

		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void ThreatsObject::set_clips()
{
	for(int i = 0 ; i < 8; i++)
	{
		frame_clip_[i].x = i* width_frame_;
		frame_clip_[i].y = 0;
		frame_clip_[i].w = width_frame_;
		frame_clip_[i].h = height_frame_;
	}
}

void ThreatsObject::Show(SDL_Renderer* des)
{
	if (come_back_time == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		frame_++;

		if (frame_ >= 8) 
		{
			frame_ = 0;
		}

		SDL_Rect* currentClip = &frame_clip_[frame_];
		SDL_Rect rendQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
		SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
	}
}

void ThreatsObject::DoPlayer(Map& gmap)
{
	if (come_back_time == 0)
	{
		x_val_ = 0;
		y_val_ += 0.8;

		if (y_val_ >= THREATS_MAX_FALLSPEED)
		{
			y_val_ = THREATS_MAX_FALLSPEED;
		}


		if (input_type_.left_ == 1)
		{
			x_val_ -= threat_val;
			status = 1;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += threat_val;
			status = 2;
		}
		else {
			status = 0;
		}

		ChecktoMap(gmap);
	}
	else if (come_back_time > 0)
	{
		come_back_time--;
		if (come_back_time == 0)
		{
			InitThreats();
		}

	}
}
void ThreatsObject::InitThreats() 
{
	x_val_ = 0;
	y_val_ = 0;

	if (x_pos_ > 256)
	{
		x_pos_ -= 256;
		animation_a_ -= 256;
		animation_b_ -= 256;
	}
	else
	{
		x_pos_ = 0;
	}
	y_pos_ = 0;
	come_back_time = 0;
	input_type_.left_ = 0;
}

void ThreatsObject::ChecktoMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	check_collision = 0;
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


			if (v2 != BLANK_TILE && v2 != 23 || v4 != BLANK_TILE && v4 != 23)
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;
				check_collision = 2;
			}

		}
		else if (x_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v3 = map_data.tile[y2][x1];
			if (v1 != BLANK_TILE && v1 != 23 || v3 != BLANK_TILE && v3 != 23)
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
				check_collision = 1;
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
			

				if (v3 != BLANK_TILE && v3 != 23 || v4 != BLANK_TILE && v4 != 23)
				{
					y_pos_ = (y2)*TILE_SIZE;
					y_pos_ -= (height_frame_ + 1);
					y_val_ = 0;
					on_land = true;
					
			}
		}
		else if (y_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v2 = map_data.tile[y1][x2];
			
			
				if (v1 != BLANK_TILE && v1 != 23 || v2 != BLANK_TILE && v2 != 23)
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
			come_back_time = 50;
		}

}

void ThreatsObject::ImpMoveType(SDL_Renderer* screen)
{
	if (type_move_ == STATIC_THREAT) {
		if (input_type_.left_ == 1)
		{
			LoadImg("img//threat_level1.png", screen);
		}
		else if (input_type_.right_ == 1)
		{
			LoadImg("img//threat_level2.png", screen);
		}
	}
	else
	{
		if (on_land == true)
		{
			if (x_pos_ > animation_b_)
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				LoadImg("img//threat//threat_left.png", screen);
			}
			else if (x_pos_ < animation_a_)
			{
				input_type_.right_ = 1;
				input_type_.left_ = 0;
				LoadImg("img//threat//threat_right.png", screen);
			}
			else if (check_collision != 0) {
				if (check_collision == 1) {
					input_type_.right_ = 1;
					input_type_.left_ = 0;
					LoadImg("img//threat//threat_right.png", screen);
				}
				else {
					input_type_.left_ = 1;
					input_type_.right_ = 0;
					LoadImg("img//threat//threat_left.png", screen);
				}
			}
		}
		else 
		{
			if (input_type_.left_ == 1)
			{
				LoadImg("img//threat//threat_left.png", screen);
			}
			else {
				LoadImg("img//threat//threat_right.png", screen);
			}
		}
	}
}

void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
	if (p_bullet != NULL && ((player_Rect.x - rect_.x) > 0  || (player_Rect.x - rect_.x) < 256 ))
	{
		p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
		p_bullet->LoadImgBullet(screen);
		p_bullet->set_is_move(true);

		float angle = atan2(player_Rect.y - rect_.y + 20, player_Rect.x - rect_.x);

		int dx = static_cast<int>(cos(angle) * val); // Vận tốc của viên đạn
		int dy = static_cast<int>(sin(angle) * val);

		if (input_type_.left_ == 1) {
			p_bullet->SetRect(rect_.x - 20 -  60, rect_.y + 10);
			p_bullet->set_x_val(dx);
			p_bullet->set_y_val(dy);
		}
		else if (input_type_.right_ == 1) {
			p_bullet->SetRect(rect_.x + 60 + 20, rect_.y + 10);
			p_bullet->set_x_val(dx);
			p_bullet->set_y_val(dy);
		}
		else {
			p_bullet->SetRect(rect_.x - 20 - 60, rect_.y + 10);
			p_bullet->set_x_val(dx);
			p_bullet->set_y_val(dy);
		}

		bullet_list.push_back(p_bullet);
	}
}

void ThreatsObject::makeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	
	for (int i = 0; i < bullet_list.size(); i++) 
	{
		BulletObject* p_bullet = bullet_list.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move())
			{

				p_bullet->get_rect_x(rect_.x);
				p_bullet->return_player_val(player_x_val);
				p_bullet->HandleMove(x_limit, y_limit);
				p_bullet->Setclip();
				p_bullet->Show(screen);
			}
			else
			{
				p_bullet->return_player_val(player_x_val);
				float angle;
				if (player_Rect.x > rect_.x) {
					angle = atan2(player_Rect.y - rect_.y + 30, player_Rect.x - rect_.x);
				}
				else {
					angle = atan2(player_Rect.y - rect_.y + 30, player_Rect.x - rect_.x + 20);
				}
					int dx = static_cast<int>(cos(angle) * val); // Vận tốc của viên đạn
					int dy = static_cast<int>(sin(angle) * val);
					//set duoc phep ban

				p_bullet->set_is_move(true);

				if (dx < 0) {
					p_bullet->SetRect(this->rect_.x - 20, rect_.y + 10);
					
				}
				else {
					p_bullet->SetRect(this->rect_.x + 64 + 20, rect_.y + 10);
				}
				
					p_bullet->set_x_val(dx);
					p_bullet->set_y_val(dy);
			}
		}
	}
}
void ThreatsObject::tracking_player(const Map& map_data, const float& xp, const float& yp)
{
	if (abs(xp - x_pos_) < (8 + level_) * 64)
	{
		int x1 = (x_pos_ + x_val_ + 1) / TILE_SIZE;
		int x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;
		int y1 = (y_pos_ + 1) / TILE_SIZE;
		int y2 = (y_pos_ + TILE_SIZE - 1) / TILE_SIZE;

		if (xp < x_pos_ - 10)
		{
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			
			if (map_data.tile[y2][x1 - 1] == 0)
			{
				
			}
			else {
				
				y_val_ -= 1;
			}
			if (map_data.tile[y2-1][x2-1] != 0 && map_data.tile[y1][x1 - 1] != 0)
			{
				x_pos_ += 64;
				y_pos_ -= 64;

				input_type_.left_ = 0;
				input_type_.right_ = 1;

				y_val_ -= 2;
			}
			

		}
		else if(xp > x_pos_ + 10)
		{
			input_type_.left_ = 0;
			input_type_.right_ = 1;

			if (map_data.tile[y2][x2 + 1] == 0)
			{

			}
			else {

				y_val_ -= 1;
			}
			if (map_data.tile[y2 - 1][x1 + 1] != 0 && map_data.tile[y1][x2 + 1] != 0)
			{
				x_pos_ -= 64;
				y_pos_ -= 64;

				input_type_.left_ = 1;
				input_type_.right_ = 0;

				y_val_ -= 2;
			}
		}
		else {
			input_type_.left_ = 0;
			input_type_.right_ = 0;
		}
	}
	else
	{
		input_type_.left_ = 0;
		input_type_.right_ = 0;
	}
}

void ThreatsObject::Show_blood(SDL_Renderer* des)
{
	SDL_Rect r;
	r.x = rect_.x + 6;
	r.y = rect_.y - 5;
	r.w = 50 / (max_blood) * blood;
	r.h = 5;

	SDL_SetRenderDrawColor(des, 255, 0, 0, 255);
	SDL_RenderFillRect(des, &r);
	SDL_Rect rr;
	rr.x = rect_.x + 6 + r.w;
	rr.y = rect_.y - 5;
	rr.w = 50 - r.w;
	rr.h = 5;

	SDL_SetRenderDrawColor(des, 100, 100, 100, 255);
	SDL_RenderFillRect(des, &rr);
}