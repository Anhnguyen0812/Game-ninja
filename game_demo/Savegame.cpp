#include "Savegame.h"


Savegame::Savegame()
{

}
Savegame::~Savegame()
{

}

bool Savegame::open(const char* name) {
	fo.open(name);
	if (fo)
	{
		return true;
	}
	return false;
}
bool Savegame::read(const std::string& name) {
	fi.open(name);
	if (fi)
	{
		return true;
	}
	return false;
}

void Savegame::savemap(const Map& map_data, const Map& map_data2)
{
	fo << map_data.start_x_ << " " << map_data.start_y_ << std::endl;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 200; j++) {
			fo << map_data.tile[i][j] << " ";
		}
		fo << std::endl;
	}
	fo << map_data2.start_x_ << " " << map_data2.start_y_ << std::endl;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 200; j++) {
			fo << map_data2.tile[i][j] << " ";
		}
		fo << std::endl;
	}

}

void Savegame::saveplayer(MainObject player)
{
	int xpos = player.get_xpos();
	int ypos = player.get_ypos();

	int status = player.get_status();

	fo << xpos << " " << ypos << " " << status << " " << player.get_blood() << " " << player.get_lift() << std::endl;

}

void Savegame::savethreat(std::vector<ThreatsObject*> list_threats)
{
	fo << list_threats.size() << std::endl;
	for (int i = 0; i < list_threats.size(); i++)
	{
		fo <<list_threats.at(i)->get_type_move() << " " << (int)list_threats.at(i)->get_x_pos() << " " << (int)list_threats.at(i)->get_y_pos() << " " << list_threats.at(i)->get_status() << " " << list_threats.at(i)->get_blood() << std::endl;
	}
}

void Savegame::save_time(const Uint32& val_time, const Uint32& last_time)
{
	fo << val_time << " " << last_time;
}

void Savegame::get(Map& map_data, Map& map_data2, MainObject& p_player)
{
	fi >> map_data.start_x_ >> map_data.start_y_;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 200; j++) {
			fi >> map_data.tile[i][j];
		}

	}

	fi >> map_data.start_x_ >> map_data.start_y_;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 200; j++) {
			fi >> map_data2.tile[i][j];
		}

	}

	int xpos, ypos, status_, blood, lift;
	fi >> xpos >> ypos >> status_ >> blood >> lift;

	p_player.set_xpos(xpos);
	p_player.set_ypos(ypos);
	p_player.set_status(status_);
	p_player.set_blood(blood);
	p_player.set_lift(lift);
	
	
}
void Savegame::get_threats(std::vector<ThreatsObject*>& list_threats, SDL_Renderer* g_screen)
{
	int n;
	fi >> n;
	ThreatsObject* threats = new ThreatsObject[n];
	for (int i = 0; i < n; i++)
	{
		int type, xpos, ypos, status, blood;
		fi >> type >> xpos >> ypos >> status >> blood;

		if (type == ThreatsObject::MOVE_IN_SPACE_THREAT)
		{
			ThreatsObject* p_threat = threats + i;
			p_threat->set_input_left(1);
			p_threat->LoadImg("img//threat//threat_left.png", g_screen);

			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);

			p_threat->set_x_pos(xpos);
			p_threat->set_y_pos(ypos);
			p_threat->set_blood(blood);
			p_threat->set_maxblood(1);
			p_threat->get_val(10 + level_ * 2);
			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			p_threat->set_animationpos(pos1, pos2);
			p_threat->set_status(status);
			p_threat->set_clips();
			p_threat->set_threatval(level_ * 2 + 1);
			p_threat->set_level(level_);
			list_threats.push_back(p_threat);
		}
		else
		{
			ThreatsObject* p_threat = threats + i;

				p_threat->LoadImg("img//threat_level1.png", g_screen);
				p_threat->set_input_left(0);
				p_threat->set_type_move(ThreatsObject::STATIC_THREAT);

				p_threat->set_x_pos(xpos);
				p_threat->set_y_pos(ypos);

				p_threat->get_val(10 + level_ * 2);
				p_threat->set_maxblood(level_ * 2);
				p_threat->set_blood(blood);
				p_threat->set_status(status);
				p_threat->set_threatval(level_ * 2 + 1);

				BulletObject* p_bullet = new BulletObject;
				p_threat->InitBullet(p_bullet, g_screen);
				p_threat->set_clips();
				list_threats.push_back(p_threat);
				p_threat->set_level(level_);
		}
	}
}

void Savegame::save_level(const int& level) {
	fo << level << std::endl;
}

void Savegame::get_level(int& level)
{
	fi >> level;
	level_ = level;
}

void Savegame::get_time(Uint32& val_time, Uint32& last_time)
{
	fi >> val_time >> last_time;
}