#pragma once

#ifndef  THREATS_OBJECT_H_
#define	 THREATS_OBJECT_H_

#include "CommonFunction.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define THREATS_GRAVITY 0.8
#define THREATS_SPEED 3;
#define THREATS_MAX_FALLSPEED 10

class ThreatsObject : public BaseObject
{
public: 
	ThreatsObject();
	~ThreatsObject();


	enum TYPEMOVE
	{
		STATIC_THREAT = 0,
		MOVE_IN_SPACE_THREAT = 1,
	};

	void set_type_move(const int& x) { type_move_ = x;}
	int get_type_move() { return type_move_; }


	void set_x_val(const float& xval) { x_val_ = xval; }
	void set_y_val(const float& yval) { y_val_ = yval; }

	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float yp) { y_pos_ = yp; }

	float get_x_pos()const { return x_pos_; }
	float get_y_pos()const { return y_pos_; }

	void SetMapxy(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }

	void set_clips();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }
	void DoPlayer(Map& gmap);
	void ChecktoMap(Map& gmap);
	int get_status() { return status; }
	void set_status(const int& sta) {
		if (sta == 1) {
			input_type_.left_ = 1;
		}
		else if (sta == 2)
		{
			input_type_.right_ = 1;
		}
	}

	void GetPlayerRect(const SDL_Rect& player) { player_Rect = player; }


	void InitThreats();
	SDL_Rect GetRectFrame();

	//void set_type_move(const int& typeM) { type_move_ = typeM; }
	void set_animationpos(const int& pos_a, const int& pos_b) { animation_a_ = pos_a, animation_b_ = pos_b; }
	void set_input_left(const int& ipleft) { input_type_.left_ = ipleft; }
	void ImpMoveType(SDL_Renderer* screen);

	std::vector<BulletObject*> get_bullet_list() const { return bullet_list; }

	void RemoveBullet(const int& idx);

	void set_bullet_list(const std::vector<BulletObject*>& b1_list) { bullet_list = b1_list; }
	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
	void makeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
	void return_player_val(const int& xval) { player_x_val = xval; }
	void tracking_player(const Map& map_data, const float& xp, const float& yp);

	int get_blood() { return blood; }
	void set_blood(const int& bl) { blood = bl; }
	void set_maxblood(const int& bl) { max_blood = bl; }
	void get_val(const int& val_) { val = val_; }
	void Show_blood(SDL_Renderer* des);
	void set_threatval(const int& tval) { threat_val = tval; }
	void set_level(const int& le) { level_ = le; }

private:
	int map_x_;
	int map_y_;
	float x_val_;
	float y_val_;
	float x_pos_;
	float y_pos_;
	bool on_land;
	int come_back_time;
	SDL_Rect frame_clip_[8];
	int width_frame_;
	int height_frame_;
	int frame_;
	int status;

	int type_move_;
	int animation_a_;
	int animation_b_;
	Input input_type_;
	int check_collision;
	int player_x_val;
	SDL_Rect player_Rect;

	std::vector<BulletObject*> bullet_list;

	int blood;
	int val;
	int max_blood;
	int threat_val;
	int level_;
};



#endif // ! THREATS_OBJECT_H

