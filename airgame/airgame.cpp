#include "pch.h"
#include "airgame.h"

// RANDIT CLASS *************************

dll::RANDIT::RANDIT()
{
	std::random_device rd{};
	std::seed_seq sq{ rd(),rd(), rd(), rd(), rd(), rd(), rd(), rd() };

	twister = new std::mt19937(sq);
}
dll::RANDIT::~RANDIT()
{
	if (twister)delete twister;
}

int dll::RANDIT::operator()(int min, int max)
{
	if (max <= min)return -1;

	std::uniform_int_distribution distrib(min, max);

	return distrib(*twister);
}
float dll::RANDIT::operator()(float min, float max)
{
	if (max <= min)return -1;

	std::uniform_real_distribution<float> distrib(min, max);

	return distrib(*twister);
}

/////////////////////////////////////////

// PROTON CLASS *************************

dll::PROTON::PROTON()
{
	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}
dll::PROTON::PROTON(float _sx, float _sy)
{
	start.x = _sx;
	start.y = _sy;

	_width = 1.0f;
	_height = 1.0f;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}
dll::PROTON::PROTON(float _sx, float _sy, float _s_width, float _s_height)
{
	start.x = _sx;
	start.y = _sy;

	_width = _s_width;
	_height = _s_height;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}

float dll::PROTON::get_width() const
{
	return _width;
}
float dll::PROTON::get_height() const
{
	return _height;
}

void dll::PROTON::set_width(float new_width)
{
	_width = new_width;
	
	end.x = start.x + _width;
	
	x_rad = _width / 2.0f;
	
	center.x = start.x + x_rad;
}
void dll::PROTON::set_height(float new_height)
{
	_height = new_height;

	end.y = start.y + _height;

	y_rad = _height / 2.0f;

	center.y = start.y + y_rad;
}
void dll::PROTON::new_dims(float new_width, float new_height)
{
	_width = new_width;
	_height = new_height;

	end.x = start.x + _width;
	end.y = start.y + _height;

	x_rad = _width / 2.0f;
	y_rad = _height / 2.0f;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}

void dll::PROTON::set_edges()
{
	end.x = start.x + _width;
	end.y = start.y + _height;

	center.x = start.x + x_rad;
	center.y = start.y + y_rad;
}

void dll::PROTON::set_path(float to_where_x, float to_where_y)
{
	move_sx = start.x;
	move_ex = to_where_x;

	move_sy = start.y;
	move_ey = to_where_y;

	ver_dir = false;
	hor_dir = false;

	if (move_sx == move_ex || (move_ex > start.x && move_ex <= end.x))
	{
		ver_dir = true;
		return;
	}
	if (move_sy == move_ey || (move_ey > start.y && move_ey <= end.y))
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = start.y - start.x * slope;
}

bool dll::PROTON::move(float where_x, float where_y, float gear)
{
	float my_speed = _speed + gear / 10.0f;

	set_path(where_x, where_y);

	if (ver_dir)
	{
		if (move_ey < move_sy)
		{
			start.y -= my_speed;
			set_edges();
			if (end.y <= sky - scr_height)return false;
		}
		else
		{

			start.y += my_speed;
			set_edges();
			if (start.y >= ground + scr_height)return false;
		}
	}
	else if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			start.x -= my_speed;
			set_edges();
			if (end.x <= -scr_width)return false;
		}
		else
		{

			start.x += my_speed;
			set_edges();
			if (start.x >= scr_width + scr_width)return false;
		}
	}
	else if (move_sx > move_ex)
	{
		float target_y = 0;

		if (center.x <= scr_width / 2.0f)target_y = center.x * tanf((30.0f + _randit(5.0f, 15.0f)) * (3.14159f / 180.0f));
		else target_y = (scr_width - center.x) * tanf((30.0f + _randit(5.0f, 15.0f)) * (3.14159f / 180.0f));

		if (move_ey > move_sy)set_path(where_x, center.y + target_y);
		else set_path(where_x, center.y - target_y);

		if (ver_dir)
		{
			if (move_ey < move_sy)
			{
				start.y -= my_speed;
				set_edges();
				if (end.y <= sky - scr_height)return false;
			}
			else
			{

				start.y += my_speed;
				set_edges();
				if (start.y >= ground + scr_height)return false;
			}
		}
		else if (hor_dir)
		{
			if (move_ex < move_sx)
			{
				start.x -= my_speed;
				set_edges();
				if (end.x <= -scr_width)return false;
			}
			else
			{

				start.x += my_speed;
				set_edges();
				if (start.x >= scr_width + scr_width)return false;
			}
		}
		else
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.y <= sky - scr_height || start.y >= ground + scr_height
				|| (end.x <= -scr_width || start.x >= scr_width + scr_width))return false; 
		}
	}
	else
	{
		float target_y = 0;

		if (center.x <= scr_width / 2.0f)target_y = center.x * tanf((30.0f + _randit(5.0f, 15.0f)) * (3.14159f / 180.0f));
		else target_y = (scr_width - center.x) * tanf((30.0f + _randit(5.0f, 15.0f)) * (3.14159f / 180.0f));

		if (move_ey > move_sy)set_path(where_x, center.y + target_y);
		else set_path(where_x, center.y - target_y);

		if (ver_dir)
		{
			if (move_ey < move_sy)
			{
				start.y -= my_speed;
				set_edges();
				if (end.y <= sky - scr_height)return false;
			}
			else
			{
				start.y += my_speed;
				set_edges();
				if (start.y >= ground + scr_height)return false;
			}
		}
		else if (hor_dir)
		{
			if (move_ex < move_sx)
			{
				start.x -= my_speed;
				set_edges();
				if (end.x <= -scr_width)return false;
			}
			else
			{
				start.x += my_speed;
				set_edges();
				if (start.x >= scr_width + scr_width)return false;
			}
		}
		else
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.y <= sky - scr_height || start.y >= ground + scr_height
				|| (end.x <= -scr_width || start.x >= scr_width + scr_width))return false;
		}
	}

	return true;
}

////////////////////////////////////////

// GROUND CLASS ************************

dll::GROUND::GROUND(tiles _what_type, float _s_x, float _s_y) :PROTON(_s_x, _s_y)
{
	type = _what_type;

	if (type == tiles::field)new_dims(1000.0f, 750.0f);
	else new_dims(32.0f, 32.0f);
}

bool dll::GROUND::move(float gear)
{
	float my_speed = 1.0f + gear / 5.0f;

	switch (dir)
	{
	case dirs::up:
		start.y -= my_speed;
		set_edges();
		if (end.y <= -scr_height)return false;
		break;

	case dirs::down:
		start.y += my_speed;
		set_edges();
		if (start.y >= 2.0f * scr_height)return false;
		break;
	}
	
	return true;
}

void dll::GROUND::Release()
{
	delete this;
}

dll::GROUND* dll::GROUND::create(tiles what_type, float s_x, float s_y)
{
	GROUND* ret{ nullptr };
	
	ret = new GROUND(what_type, s_x, s_y);

	return ret;
}

////////////////////////////////////////

// POWERUPS CLASS **********************

dll::POWERUPS::POWERUPS(powerups _what, float _sx, float _sy) :PROTON(_sx, _sy, 32.0f, 32.0f)
{
	type = _what;
}

void dll::POWERUPS::Release()
{
	delete this;
}

dll::POWERUPS* dll::POWERUPS::create(powerups what, float sx, float sy)
{
	dll::POWERUPS* ret{ nullptr };

	ret = new POWERUPS(what, sx, sy);

	return ret;
}

////////////////////////////////////////

// CLOUDS CLASS ***********************

dll::CLOUDS::CLOUDS(clouds _type, float _sx, float _sy) :PROTON(_sx, _sy)
{
	type = _type;
	
	switch (type)
	{
	case clouds::cloud1:
		_speed = 1.5f;
		new_dims(150.0f, 77.0f);
		break;

	case clouds::cloud2:
		_speed = 1.4f;
		new_dims(180.0f, 81.0f);
		break;

	case clouds::cloud3:
		_speed = 1.6f;
		new_dims(120.0f, 63.0f);
		break;

	case clouds::cloud4:
		_speed = 1.4f;
		new_dims(200.0f, 79.0f);
		break;

	case clouds::cloud5:
		_speed = 1.3f;
		new_dims(220.0f, 70.0f);
		break;
	}
}

void dll::CLOUDS::Release()
{
	delete this;
}

dll::CLOUDS* dll::CLOUDS::create(clouds type, float sx, float sy)
{
	CLOUDS* ret{ nullptr };

	ret = new CLOUDS(type, sx, sy);

	return ret;
}

///////////////////////////////////////

// SHOTS CLASS ************************

dll::SHOTS::SHOTS(shots _type, float _sx, float _sy, float _ex, float _ey) :PROTON(_sx, _sy)
{
	type = _type;

	switch (type)
	{
	case shots::bullet:
		new_dims(11.0f, 11.0f);
		_speed = 5.0f;
		damage = 10;
		break;

	case shots::blast:
		new_dims(25.0f, 22.0f);
		_speed = 4.0f;
		damage = 20;
		break;

	case shots::rocket:
		new_dims(10.0f, 37.0f);
		_speed = 4.0f;
		damage = 50;
		break;
	}

	set_path(_ex, _ey);

	if (move_ey < move_sy)dir = dirs::up;
}

bool dll::SHOTS::move(float gear)
{
	float my_speed = _speed + gear / 10.0f;

	if (ver_dir)
	{
		if (move_sy > move_ey)
		{
			start.y -= my_speed;
			set_edges();
			if (start.y <= sky)return false;
		}
		else
		{
			start.y += my_speed;
			set_edges();
			if (end.y >= ground)return false;
		}
	}
	else if (hor_dir)
	{
		if (move_sx > move_ex)
		{
			start.x -= my_speed;
			set_edges();
			if (start.y <= 0)return false;
		}
		else
		{
			start.x += my_speed;
			set_edges();
			if (end.x >= scr_width)return false;
		}
	}
	else
	{
		if (move_sx > move_ex)
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x <= 0 || start.y <= sky || end.y >= ground)return false;
		}
		else
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (start.x >= scr_width || end.y <= sky || start.y >= ground)return false;
		}
	}

	return true;
}

void dll::SHOTS::Release()
{
	delete this;
}

dll::SHOTS* dll::SHOTS::create(shots type, float sx, float sy, float ex, float ey)
{
	SHOTS* ret{ nullptr };

	ret = new SHOTS(type, sx, sy, ex, ey);

	return ret;
}

///////////////////////////////////////

// HERO CLASS *************************

dll::HERO::HERO(float _sx, float _sy) :PROTON(_sx, _sy, 80.0f, 80.0f) 
{
	_speed = 10.0f;
};

bool dll::HERO::move(float gear)
{
	float my_speed = _speed + gear;

	if((orientation==move_dirs::left || orientation == move_dirs::right)
		&& (dir == dirs::up || dir == dirs::down))
	{
		orientation = move_dirs::straight;
		return true;
	}

	switch (dir)
	{
	case dirs::left:
		if (start.x - my_speed / 10.0f >= 0)
		{
			orientation = move_dirs::left;
			start.x -= my_speed / 10.0f;
			set_edges();
		}
		else
		{
			orientation = move_dirs::straight;
			return false;
		}
		break;

	case dirs::right:
		if (end.x + my_speed / 10.0f <= scr_width)
		{
			orientation = move_dirs::right;
			start.x += my_speed / 10.0f;
			set_edges();
		}
		else
		{
			orientation = move_dirs::straight;
			return false;
		}
		break;

	case dirs::up:
		orientation = move_dirs::straight;
		if (start.y - my_speed >= sky)
		{
			start.y -= my_speed;
			set_edges();
		}
		else return false;
		break;

	case dirs::down:
		orientation = move_dirs::straight;
		if (end.y + my_speed <= ground)
		{
			start.y += my_speed;
			set_edges();
		}
		else return false;
		break;
	}

	return true;
}

int dll::HERO::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++current_frame;
		if (current_frame > max_frames)current_frame = 0;
	}
	
	return current_frame;
}

move_dirs dll::HERO::get_move_dir()const
{
	return orientation;
}
void dll::HERO::set_move_dir(move_dirs new_move_dir)
{
	orientation = new_move_dir;
	current_frame = 0;

	switch (orientation)
	{
	case move_dirs::straight:
		max_frames = 9;
		frame_delay = 7;
		max_frame_delay = 7;
		break;

	case move_dirs::left:
		max_frames = 6;
		frame_delay = 10;
		max_frame_delay = 10;
		break;

	case move_dirs::right:
		max_frames = 6;
		frame_delay = 10;
		max_frame_delay = 10;
		break;
	}
}

char dll::HERO::get_current_ammo() const
{
	return current_ammo;
}
bool dll::HERO::set_current_ammo(char to_what)
{
	switch (to_what)
	{
	case BULLET:
		current_ammo = to_what;
		return true;

	case BIG_GUN:
		if (big_gun_found)
		{
			current_ammo = to_what;
			return true;
		}
		break;
		
	case ROCKET:
		if (rockets_available > 0)
		{
			current_ammo = to_what;
			return true;
		}
		break;
	}

	return false;
}

void dll::HERO::Release()
{
	delete this;
}

dll::HERO* dll::HERO::create(float sx, float sy)
{
	HERO* ret{ nullptr };
	
	ret = new HERO(sx, sy);

	return ret;
}

///////////////////////////////////////

dll::EVILS::EVILS(creatures _what_type, float _sx, float _sy) :PROTON(_sx, _sy)
{
	type = _what_type;

	switch (type)
	{
	case creatures::evil1:
		new_dims(90.0f, 66.0f);
		_speed = 0.5f;
		max_frames = 11;
		frame_delay = 6;	
		attack_delay = 100;
		lifes = 100;
		damage = 8;
		armour = 0;
		break;

	case creatures::evil2:
		new_dims(80.0f, 53.0f);
		_speed = 0.6f;
		max_frames = 11;
		frame_delay = 6;
		attack_delay = 90;
		lifes = 80;
		damage = 6;
		armour = 0;
		break;

	case creatures::evil3:
		new_dims(100.0f, 93.0f);
		_speed = 0.5f;
		max_frames = 12;
		frame_delay = 6;
		attack_delay = 140;
		lifes = 150;
		damage = 12;
		armour = 0;
		break;

	case creatures::evil4:
		new_dims(85.0f, 80.0f);
		_speed = 0.7f;
		max_frames = 11;
		frame_delay = 6;
		attack_delay = 120;
		lifes = 110;
		damage = 9;
		armour = 0;
		break;

	case creatures::evil5:
		new_dims(70.0f, 57.0f);
		_speed = 0.8f;
		max_frames = 13;
		frame_delay = 5;
		attack_delay = 80;
		lifes = 80;
		damage = 5;
		armour = 0;
		break;

	case creatures::boss1:
		new_dims(200.0f, 67.0f);
		_speed = 0.3f;
		max_frames = 3;
		frame_delay = 20;
		attack_delay = 100;
		lifes = 350;
		damage = 40;
		armour = 5;
		break;

	case creatures::boss2:
		new_dims(200.0f, 143.0f);
		_speed = 0.3f;
		max_frames = 1;
		frame_delay = 30;
		attack_delay = 100;
		lifes = 330;
		damage = 35;
		armour = 3;
		break;

	case creatures::boss3:
		new_dims(250.0f, 118.0f);
		_speed = 0.2f;
		max_frames = 9;
		frame_delay = 7;
		attack_delay = 120;
		lifes = 400;
		damage = 50;
		armour = 8;
		break;
	}

	max_frame_delay = frame_delay;
	max_attack_delay = attack_delay;

	float tx = _randit(0.0f, scr_width);

	if (tx <= scr_width / 2.0f)set_path(scr_width, ground);
	else set_path(0, ground);
}

FPOINT dll::EVILS::get_target_point()const
{
	return FPOINT{ move_ex, move_ey };
}

int dll::EVILS::get_frame()
{
	--frame_delay;
	if (frame_delay <= 0)
	{
		frame_delay = max_frame_delay;
		++current_frame;
		if (current_frame > max_frames)current_frame = 0;
	}

	return current_frame;
}
int dll::EVILS::attack()
{
	--attack_delay;
	if (attack_delay <= 0)
	{
		attack_delay = max_attack_delay;
		return damage;
	}
	
	return 0;
}

move_dirs dll::EVILS::get_move_dir()const
{
	return orientation;
}
void dll::EVILS::set_move_dir(move_dirs new_move_dir)
{
	orientation = new_move_dir;
	current_frame = 0;

	switch (type)
	{
	case creatures::evil1:
		switch (orientation)
		{
		case move_dirs::straight:
			max_frames = 11;
			frame_delay = 6;
			break;

		case move_dirs::left:
			max_frames = 5;
			frame_delay = 12;
			break;

		case move_dirs::right:
			max_frames = 5;
			frame_delay = 12;
			break;
		}
		break;

	case creatures::evil2:
		switch (orientation)
		{
		case move_dirs::straight:
			max_frames = 11;
			frame_delay = 6;
			break;

		case move_dirs::left:
			max_frames = 4;
			frame_delay = 13;
			break;

		case move_dirs::right:
			max_frames = 6;
			frame_delay = 11;
			break;
		}
		break;

	case creatures::evil3:
		switch (orientation)
		{
		case move_dirs::straight:
			max_frames = 12;
			frame_delay = 6;
			break;

		case move_dirs::left:
			max_frames = 5;
			frame_delay = 12;
			break;

		case move_dirs::right:
			max_frames = 5;
			frame_delay = 12;
			break;
		}
		break;

	case creatures::evil4:
		switch (orientation)
		{
		case move_dirs::straight:
			max_frames = 11;
			frame_delay = 6;
			break;

		case move_dirs::left:
			max_frames = 4;
			frame_delay = 13;
			break;

		case move_dirs::right:
			max_frames = 5;
			frame_delay = 12;
			break;
		}
		break;

	case creatures::evil5:
		switch (orientation)
		{
		case move_dirs::straight:
			max_frames = 13;
			frame_delay = 5;
			break;

		case move_dirs::left:
			max_frames = 4;
			frame_delay = 13;
			break;

		case move_dirs::right:
			max_frames = 4;
			frame_delay = 13;
			break;
		}
		break;
	}

	max_frame_delay = frame_delay;
}

bool dll::EVILS::move(float where_x, float where_y, float gear)
{
	float my_speed = _speed + gear / 10.0f;

	if (ver_dir)
	{
		orientation = move_dirs::straight;
		if (move_sy > move_ey)
		{
			start.y -= my_speed;
			set_edges();
			if (start.y <= sky)return false;
		}
		else
		{
			start.y += my_speed;
			set_edges();
			if (end.y >= ground)return false;
		}
	}
	else if (hor_dir)
	{
		orientation = move_dirs::straight;
		if (move_sx > move_ex)
		{
			start.x -= my_speed;
			set_edges();
			if (start.y <= 0)return false;
		}
		else
		{
			start.x += my_speed;
			set_edges();
			if (end.x >= scr_width)return false;
		}
	}
	else
	{
		if (move_sx > move_ex)
		{
			orientation = move_dirs::left;
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x <= 0 || start.y <= sky || end.y >= ground)return false;
		}
		else
		{
			orientation = move_dirs::right;
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (start.x >= scr_width || end.y <= sky || start.y >= ground)return false;
		}
	}

	return true;
}

void dll::EVILS::Release()
{
	delete this;
}

dll::EVILS* dll::EVILS::create(creatures what_type, float sx, float sy)
{
	EVILS* ret{ nullptr };
	
	ret = new EVILS(what_type, sx, sy);

	return ret;
}

// FUNCTIONS *******************************************************************

float dll::Distance(FPOINT first, FPOINT second)
{
	float a = static_cast<float>(pow(abs(second.x - first.x), 2));
	float b = static_cast<float>(pow(abs(second.y - first.y), 2));

	return static_cast<float>(sqrt(a + b));
}
void dll::Sort(BAG<FPOINT>& bag, FPOINT ref)
{
	bool is_ok = false;

	if (bag.size() < 2)return;

	while (!is_ok)
	{
		is_ok = true;
		for (size_t i = 0; i < bag.size() - 1; ++i)
		{
			if (Distance(bag[i], ref) > Distance(bag[i + 1], ref))
			{
				FPOINT temp{ bag[i] };
				bag[i] = bag[i + 1];
				bag[i + 1] = temp;
				is_ok = false;
			}
		}
	}
}

bool dll::Intersect(FRECT first, FRECT second)
{
	if (!(first.left > second.right || first.right < second.left
		|| first.up > second.down || first.down < second.up))return true;
	
	return false;
}
bool dll::Intersect(FPOINT first, FPOINT second, float first_xrad, float second_xrad, float first_yrad, float second_yrad)
{
	if (abs(second.x - first.x) <= first_xrad + second_xrad &&
		abs(second.y - first.y) <= first_yrad + second_yrad)return true;
	
	return false;
}

actions dll::AINextMove(EVILS& my_unit, FPOINT hero_center, BAG<FPOINT>& shot_bag, BAG<FPOINT>& other_creatures)
{
	FPOINT temp_move{ my_unit.get_target_point() };
	
	float temp_movex{ temp_move.x };
	float temp_movey{ temp_move.y };

	actions ret{ actions::move };

	if (!shot_bag.empty())Sort(shot_bag, my_unit.center);
	if (!other_creatures.empty())Sort(other_creatures, my_unit.center);

	if (!other_creatures.empty())
	{
		if (Distance(other_creatures[0], my_unit.center) <= 100.0f)
		{
			if (other_creatures[0].x > my_unit.center.x)
			{
				my_unit.set_path(0, my_unit.start.y);
				ret = actions::dir_changed;
			}
			else if (other_creatures[0].x < my_unit.center.x)
			{
				my_unit.set_path(scr_width, my_unit.start.y);
				ret = actions::dir_changed;
			}
			else
			{
				if (other_creatures[0].y > my_unit.center.y)
				{
					my_unit.set_path(my_unit.start.x, sky);
					ret = actions::dir_changed;
				}
				else
				{
					my_unit.set_path(my_unit.start.x, ground);
					ret = actions::dir_changed;
				}
			}
		}
	}
	else if (!shot_bag.empty())
	{
		if (Distance(shot_bag[0], my_unit.center) <= 100.0f)
	{
		if (shot_bag[0].x > my_unit.center.x)
		{
			my_unit.set_path(0, my_unit.start.y);
			ret = actions::dir_changed;
		}
		else if (shot_bag[0].x < my_unit.center.x)
		{
			my_unit.set_path(scr_width, my_unit.start.y);
			ret = actions::dir_changed;
		}
		else
		{
			if (shot_bag[0].y > my_unit.center.y)
			{
				my_unit.set_path(my_unit.start.x, sky);
				ret = actions::dir_changed;
			}
			else
			{
				my_unit.set_path(my_unit.start.x, ground);
				ret = actions::dir_changed;
			}
		}
	}
	}
	else if (Distance(hero_center, my_unit.center) <= 250.0f) ret = actions::shoot;
	else
	{
		if (my_unit.start.x <= 0)
		{
			my_unit.set_path(scr_width, 2.5f * scr_height);
			my_unit.set_move_dir(move_dirs::right);
		}
		else
		{
			if (my_unit.end.x >= scr_width)
			{
				my_unit.set_path(0, 2.5f * scr_height);
				my_unit.set_move_dir(move_dirs::left);
			}
		}
	}


	return ret;
}