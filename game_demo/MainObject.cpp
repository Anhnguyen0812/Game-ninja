
#include "MainObject.h"
#include "windows.h"
#include "iostream"


void MainObject::increase_coins()
{
	coins++;
}


MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_RIGHT;
	status = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.up_ = 0;
	input_type_.down_ = 0;
	map_x_ = 0;
	map_y_ = 0;
	come_back_time_ = 0;
	jum_times = 0;
	jum_times_ = 0;
	on_land = false;
	coins = 0;
	collision = true;
	ld_jum = 0;
	first_call_dbjum = false;
	actions = 0;
	start_ = 0;

	blood = 10;
	lift = 3;
	is_throw = false;
}

MainObject::~MainObject()
{

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true) {
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}

	return ret;

}

SDL_Rect MainObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;
	return rect;
}


void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		frame_clip_[0].x = 0;
		frame_clip_[0].y = 0;
		frame_clip_[0].w = width_frame_;
		frame_clip_[0].h = height_frame_;

		frame_clip_[1].x = width_frame_;
		frame_clip_[1].y = 0;
		frame_clip_[1].w = width_frame_;
		frame_clip_[1].h = height_frame_;

		frame_clip_[2].x = 2 * width_frame_;
		frame_clip_[2].y = 0;
		frame_clip_[2].w = width_frame_;
		frame_clip_[2].h = height_frame_;

		frame_clip_[3].x = 3 * width_frame_;
		frame_clip_[3].y = 0;
		frame_clip_[3].w = width_frame_;
		frame_clip_[3].h = height_frame_;

		frame_clip_[4].x = 4 * width_frame_;
		frame_clip_[4].y = 0;
		frame_clip_[4].w = width_frame_;
		frame_clip_[4].h = height_frame_;

		frame_clip_[5].x = 5 * width_frame_;
		frame_clip_[5].y = 0;
		frame_clip_[5].w = width_frame_;
		frame_clip_[5].h = height_frame_;

		frame_clip_[6].x = 6 * width_frame_;
		frame_clip_[6].y = 0;
		frame_clip_[6].w = width_frame_;
		frame_clip_[6].h = height_frame_;

		frame_clip_[7].x = 7 * width_frame_;
		frame_clip_[7].y = 0;
		frame_clip_[7].w = width_frame_;
		frame_clip_[7].h = height_frame_;

	}
}

void MainObject::Show(SDL_Renderer* des)
{
	
	UpdateImgPlayer(des);
	
	/*
	if (input_type_.left_ == 1
		|| input_type_.right_ == 1)
	{
		frame_++;
	}
	else
	{
		frame_ = 0;
	}
	*/

	frame_++;
	if (frame_ >= 8) {
		frame_ = 0;
		if (on_land == false && jum_times == 0) {
			frame_ = 0;
		}
	}

	if (come_back_time_ == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect* current_clip = &frame_clip_[frame_];

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

		SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

	}
	
}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen)
{

	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_d:
		{
			status_ = WALK_RIGHT;
			status = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImgPlayer(screen);
		}
		break;
		case SDLK_a:
		{
			status_ = WALK_LEFT;
			status = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImgPlayer(screen);
		}
		break;
		case SDLK_w:
		{
			input_type_.jump_ = 1;
		}
		break;
		case SDLK_s:
		{
			input_type_.down_ = 1;
		}
		break;
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			status = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImgPlayer(screen);
		}
		break;
		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			status = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImgPlayer(screen);
		}
		break;
		case SDLK_UP:
		{
			input_type_.jump_ = 1;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 1;
		}
		break;
		default:
			break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_d:
		{
			input_type_.right_ = 0;
		}
		break;

		case SDLK_a:
		{
			input_type_.left_ = 0;
		}
		break;

		case SDLK_w:
		{
			input_type_.jump_ = 0;
		}
		break;
		case SDLK_s:
		{
			input_type_.down_ = 0;
		}
		break;
		case SDLK_RIGHT:
		{
			input_type_.right_ = 0;
		}
		break;

		case SDLK_LEFT:
		{
			input_type_.left_ = 0;
		}
		break;

		case SDLK_UP:
		{
			input_type_.jump_ = 0;
		}
		break;
		case SDLK_DOWN:
		{
			input_type_.down_ = 0;
		}
		break;
		default:
			break;
		}
	}
	
	is_throw = false;

	if (events.type == SDL_MOUSEBUTTONDOWN && p_bullet_list.size() < 1) {
		if (events.button.button = SDL_BUTTON_RIGHT ) {

			BulletObject* p_bullet = new BulletObject();
			p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
			p_bullet->return_player_val(x_val_);
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			
			float angle;
			
			if (mouseX - rect_.x > 0) {
				angle = atan2(mouseY - (rect_.y + 20), mouseX - rect_.x - width_frame_);
			}
			else {
				angle = atan2(mouseY - (rect_.y + 20), mouseX - rect_.x);
			}

			int dx = static_cast<int>(cos(angle) * 20); // Vận tốc của viên đạn
			int dy = static_cast<int>(sin(angle) * 20);

			if (dx >= 0) {
				if (input_type_.left_ == 1)
					return;
				status_ = WALK_RIGHT;
			}
			else {
				if (input_type_.right_ == 1)
					return;
				status_ = WALK_LEFT;
			}

			if (status_ == WALK_RIGHT) {
				p_bullet->return_status(status_);
				p_bullet->LoadImgBullet(screen);
				p_bullet->SetRect(this->rect_.x + width_frame_, this->rect_.y + height_frame_ * 0.3);
				p_bullet->set_x_val(dx);
				p_bullet->set_y_val(dy);
			}
			else if (status_ == WALK_LEFT){
				p_bullet->return_status(status_);
				p_bullet->LoadImgBullet(screen);
				p_bullet->SetRect(this->rect_.x, this->rect_.y + height_frame_ * 0.3);
				p_bullet->set_x_val(dx);
				p_bullet->set_y_val(dy);
			}
			//cho phep phi tieu
			p_bullet->set_is_move(true);
			is_throw = true;
			//nap vao vector
			p_bullet_list.push_back(p_bullet);

		}
	}
}

void MainObject::HandleBullet(SDL_Renderer* des, const Map& map_data) 
{

	//duyet vector 
	for (int i = 0; i < p_bullet_list.size(); i++) {
		BulletObject* p_bullet = p_bullet_list.at(i);
		
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move() == true)
			{
				p_bullet->get_rect_x(this->rect_.x);
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->Setclip();
				p_bullet->Show(des);

				if (checkcollision(p_bullet->GetRect(), map_data))
				{
					p_bullet_list.erase(p_bullet_list.begin() + i);

					if (p_bullet != NULL) {
						delete p_bullet;
						p_bullet = NULL;
					}
				}
			
			}
			else {
				//phi tieu bay het man hinh thi xoa
				p_bullet_list.erase(p_bullet_list.begin() + i);
				is_throw = false;
				if (p_bullet != NULL) {
					delete p_bullet;
					p_bullet = NULL;
				}
			}
		}
	}
}

void MainObject::RemoveBullet(const int& idx)
{
	int size = p_bullet_list.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = p_bullet_list.at(idx);
		p_bullet_list.erase(p_bullet_list.begin() + idx);

		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void MainObject::DoPlayer(Map& map_data, Map& map_data2)
{

	if (come_back_time_ == 0)
	{
		x_val_ = 0;
		y_val_ += Gravity_speed;

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
		if (on_land == true) {
			jum_times = 0;
			jum_times_ = 0;
		}
		is_jump = false;
		if (input_type_.jump_ == 1)
		{
			if (on_land == true)
			{
				y_val_ = - PLAYER_JUMP;
				is_jump = true;
			}
			else if (jum_times == 0) {

				if (y_val_ < 0) {
					if (y_val_ > -7)
						y_val_ -= 7;
					y_val_ -= 7;

				}
				else {
					if (y_val_ < 7)
						y_val_ += 7;
					y_val_ = -y_val_ - 7;
				}
				jum_times = 1;
				jum_times_ = 1;
			}
			

			on_land = false;
			input_type_.jump_ = 0;
			
		}

		if (input_type_.down_ == 1)
		{
			y_val_ += 3;
		}
		CenterEntityOnMap(map_data);
		CheckToMap(map_data, map_data2);
//		CenterEntityOnMap(map_data);
	}

	else if (come_back_time_ > 0) {
		actions = 0;
		
		come_back_time_--;
		CenterEntityOnMap(map_data);
	//	CheckToMap(map_data);
		jum_times = 1;
		if (come_back_time_ == 0)
		{
			on_land = false;
			if (x_pos_ > 256)
			{
				x_pos_ -= 256;
			}
			else
			{
				x_pos_ = 0;
			}
		
			y_pos_ = 0;
			x_val_ = 0;
			y_val_ = 0;
		}
	}
	
}

void MainObject::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;	
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;

	}

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);

	if (map_data.start_y_ < 0)
	{
		map_data.start_y_ = 0;
	}
	else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	
	}

	if (start_ != map_data.start_x_) {
		start_ = map_data.start_x_;
		if (status == WALK_LEFT && collision == 0) {
			actions = 1;
		}
		else if (status == WALK_RIGHT && collision == 0) {
			actions = 2;
		}
		else {
			actions = 0;
		}

	}
	else {
		actions = 0;
	}

}

void MainObject::CheckToMap(Map& map_data, Map& map_data2)
{
	collision = 0;
	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	is_die = false;

	// check horizontal
	int height_min = min(height_frame_, TILE_SIZE);

	x1 = (x_pos_ + x_val_ + 10) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 11) / TILE_SIZE;
	y1 = (y_pos_  + 1) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	int v1 = map_data2.tile[y1][x1];
	if (v1 == coins_value) {
		map_data2.tile[y1][x1] = 0;
		increase_coins();
	}
	int v2 = map_data2.tile[y1][x2];
	if (v2 == coins_value) {
		map_data2.tile[y1][x2] = 0;
		increase_coins();
	}
	int v3 = map_data2.tile[y2][x1];
	if (v3 == coins_value) {
		map_data2.tile[y2][x1] = 0;
		increase_coins();
	}
	int v4 = map_data2.tile[y2][x2];
	if (v4 == coins_value) {
		map_data2.tile[y2][x2] = 0;
		increase_coins();
	}

	if (x1 == 9 && map_data.tile[6][11] != 0) {
		for(int j = 10; j <= 13; j++){
		for (int i = 5; i > 0; i--) {
			map_data.tile[6][j + i ] = map_data.tile[6][j+i-1];
		}

		map_data.tile[6][j] = 0;
	}
	}
	
	if (x2 % 20 == 0 && x2 >= 30) {
		for (int i = x2; i <= x2 + 3; i++) {
			for (int j = 5; j < 10; j++) {
				map_data.tile[j][i] = 0;
				if (j == 9) {
					map_data.tile[j][i] = i - x2 + 71;
				}
			}
		}
	}

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
			
				if (v2 != BLANK_TILE && (v2 < 71) || v4 != BLANK_TILE && (v4 < 71))
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ - 10;
					x_val_ = 0;
					collision = 1;
				}

			}
		
		else if (x_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v3 = map_data.tile[y2][x1];
			
			
				if (v1 != BLANK_TILE && (v1 < 71) || v3 != BLANK_TILE && (v3 < 71))
				{
					x_pos_ = (x1 + 1) * TILE_SIZE - 10;
					x_val_ = 0;
					collision = 1;
				}
			
		}
	}
	//vertical
	int width_min = min(width_frame_, TILE_SIZE);
	x1 = (x_pos_ + 10)/ TILE_SIZE;
	x2 = (x_pos_ + width_min - 11) / TILE_SIZE;
	y1 = (y_pos_ + y_val_ + 1) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{

		int v3 = map_data.tile[y2][x1];
		int v4 = map_data.tile[y2][x2];

		if (v3 >= 24 || v4 >= 24)
		{
			if (y_val_ > 1) {
				y_val_ -= 1.8;
			}
			else {
				y_val_ = 0;
			}
			on_land = true;
			if (status_ == WALK_NONE)
			{
				status_ = WALK_RIGHT;
				status = WALK_RIGHT;
			}
			
			if (y_val_ == 0) {
				//come_back_time_ = 1000;
				y_pos_ = 160;
				x_pos_ -= 256;
				is_die = true;
			}
			
			
			PLAYER_SPEED = 3;
		}else
		PLAYER_SPEED = 6;

		if (y_val_ > 0)
		{
			int v3 = map_data.tile[y2][x1];
			int v4 = map_data.tile[y2][x2];
			
				if (v3 != BLANK_TILE && (v3 < 71)  || v4 != BLANK_TILE && (v4 < 71))
				{
					y_pos_ = (y2)*TILE_SIZE;
					y_pos_ -= (height_frame_ + 1);
					y_val_ = 0;
					on_land = true;
					if (status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
						status = WALK_RIGHT;
					}
				
			}
		}
		else if (y_val_ < 0)
		{
			int v1 = map_data.tile[y1][x1];
			int v2 = map_data.tile[y1][x2];
			
					if (v1 != BLANK_TILE && (v1 < 71) || v2 != BLANK_TILE && (v2 < 71))
					{
						y_pos_ = (y1 + 1) * TILE_SIZE;
						y_val_ = 0;
					}
		}
			
		}

	

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	is_newlevel = false;
	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_ - 150)
	{
	//	x_pos_ = map_data.max_x_ - width_frame_ - 1;
		x_pos_ = 0;
		is_newlevel = true;
	}

	if (y_pos_ > map_data.max_y_)
	{
		come_back_time_ = 10;
	}

	if (input_type_.left_ == 0 && input_type_.right_ == 0) {
		status = WALK_NONE;
	}
}

void MainObject::UpdateImgPlayer(SDL_Renderer* des)
{
	if (on_land == true)
	{
		if (status_ == WALK_LEFT)
		{
			if (input_type_.left_ == 1) {
				LoadImg("img//player sprite//player_left.png", des);
			}
			else {
				LoadImg("img//player_stay_left.png", des);
			}
		}
		else if (status_ == WALK_RIGHT)
		{
			if (input_type_.right_ == 1) {
				LoadImg("img//player sprite//player_right.png", des);
			}
			else {
				LoadImg("img//player_stay_right.png", des);
			}
		}
		
	}
	else
	{
		if (status_ == WALK_LEFT)
		{
			if (jum_times_ == 1 && y_val_ < 5) {
				if (first_call_dbjum == false) {
					frame_ = 0;
					first_call_dbjum = true;
				}
					LoadImg("img//double_jum_left.png", des);
			}
			else {
				LoadImg("img//jum_left.png", des);
				first_call_dbjum = false;
			}
		}
		else
		{
			if (jum_times_ == 1 && y_val_ < 5) {
				if (first_call_dbjum == false) {
					frame_ = 0;
					first_call_dbjum = true;
				}
					LoadImg("img//double_jum_right.png", des);
			}
			else {
				LoadImg("img//jum_right.png", des);
				first_call_dbjum = false;
			}
		}
	}
}

bool MainObject::checkcollision(const SDL_Rect& rect,const Map& map_data)
{
	
	int x1 = (rect.x + map_x_)/64;
	int y1 = (rect.y)/64;
	int x2 = (rect.x + map_x_ + 30) / 64;
	int y2 = (rect.y + 30) / 64;

	if (map_data.tile[y2][x1] != BLANK_TILE && map_data.tile[y2][x1] < 21 || map_data.tile[y1][x1] != BLANK_TILE && map_data.tile[y1][x1] < 21) {
		return true;
	}
	return false;

}

void MainObject::Show_blood(SDL_Renderer* des)
{
	SDL_Rect r;
	r.x = rect_.x + 6;
	r.y = rect_.y - 5;
	r.w = blood * 5;
	r.h = 5;

	SDL_SetRenderDrawColor(des, 255, 0, 0, 255);
	SDL_RenderFillRect(des, &r);
	SDL_Rect rr;
	rr.x = rect_.x + 6 + r.w;
	rr.y = rect_.y - 5;
	rr.w = 50 - blood * 5;
	rr.h = 5;

	SDL_SetRenderDrawColor(des, 100, 100, 100, 255);
	SDL_RenderFillRect(des, &rr);
}