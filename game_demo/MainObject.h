
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <Vector>
#include "CommonFunction.h"
#include "BaseObject.h"
#include "BulletObject.h"


#define Gravity_speed 0.8
#define MAX_FALL_SPEED 10

#define PLAYER_JUMP 18


class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();
	
	enum Walktype
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,	
		WALK_LEFT = 2,
	};

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandelInputAction(SDL_Event events, SDL_Renderer* screen);
	void set_clips();
	void DoPlayer(Map& map_data, Map& map_data2);
	void CheckToMap(Map& map_data, Map& map_data2);
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; };
	void CenterEntityOnMap(Map& map_data);
	void UpdateImgPlayer(SDL_Renderer* des);
	void increase_coins() ;
	int return_coins() { return coins; };
	void exept_coins(int coins_) { coins = coins_; };
	bool return_collision() { return collision; };
	void HandleBullet(SDL_Renderer* des, const Map& map_data);
	int return_xval() { return x_val_; }
	virtual int get_xpos() { return x_pos_; }
	void set_xpos(const int& xp) { x_pos_ = xp; }
	void set_ypos(const int& yp) { y_pos_ = yp; }
	bool checkcollision(const SDL_Rect& rect, const Map& map_data);

	void set_status(const int& statuss) { status_ = statuss; }
	int get_status() { return status_; }

	void RemoveBullet(const int& idx);
	int return_actions() { return actions; };
	bool return_isdie(){ return is_die; }
	void set_yval(const float& y) { y_val_ = y; }

	//blood and lift

	void set_blood(const int& bl) { blood = bl; }
	int get_blood() { return blood; }

	void set_lift(const int& li) { lift = li; }
	int get_lift() { return lift; }

	SDL_Rect GetRectFrame();

	float get_ypos() { return y_pos_; }

	void set_bullet_list(std::vector<BulletObject*> bullet_list)
	{
		p_bullet_list = bullet_list;
	}
	std::vector<BulletObject*> get_bullet_list() const { return p_bullet_list; }
	bool get_isthrow() { return is_throw; }
	bool get_isnewlevel() { return is_newlevel; }
	bool get_jump() { return is_jump; }
	void Show_blood(SDL_Renderer* des);

private:

	std::vector<BulletObject*> p_bullet_list;

	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];
	
	Input input_type_;
	int frame_;
	int status_;
	int status;
	bool on_land;
	int map_x_;
	int map_y_;
	int jum_times;
	int jum_times_;
	int come_back_time_;
	int coins;
	int ld_jum;
	bool collision;
	bool first_call_dbjum;
	int actions;

	int start_; // map_data.start_x
	int PLAYER_SPEED = 8;
	bool is_die;
	bool is_throw;
	bool is_newlevel;

	int blood, lift;
	bool is_jump;
};

#endif