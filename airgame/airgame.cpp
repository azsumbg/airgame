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
			if (end.y <= sky - scr_width / 2.0f)return false;
		}
		else
		{

			start.y += my_speed;
			set_edges();
			if (start.y >= ground + scr_width / 2.0f)return false;
		}
	}
	else if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			start.x -= my_speed;
			set_edges();
			if (end.x <= -scr_width / 2.0f)return false;
		}
		else
		{

			start.x += my_speed;
			set_edges();
			if (start.x >= scr_width + scr_width / 2.0f)return false;
		}
	}
	else if (move_sx > move_ex)
	{
		float target_y = 0;

		if (center.x <= scr_width / 2.0f)target_y = center.x * tanf((30.0f + _randit(5.0f, 15.0f)) * (3.14159f / 180.0f));
		else target_y = (scr_width - center.x) * tanf((30.0f + _randit(5.0f, 15.0f)) * (3.14159f / 180.0f));

		if (move_ey > move_sy)set_path(center.x, center.y + target_y);
		else set_path(center.x, center.y - target_y);

		if (ver_dir)
		{
			if (move_ey < move_sy)
			{
				start.y -= my_speed;
				set_edges();
				if (end.y <= sky - scr_width / 2.0f)return false;
			}
			else
			{

				start.y += my_speed;
				set_edges();
				if (start.y >= ground + scr_width / 2.0f)return false;
			}
		}
		else if (hor_dir)
		{
			if (move_ex < move_sx)
			{
				start.x -= my_speed;
				set_edges();
				if (end.x <= -scr_width / 2.0f)return false;
			}
			else
			{

				start.x += my_speed;
				set_edges();
				if (start.x >= scr_width + scr_width / 2.0f)return false;
			}
		}
		else
		{
			start.x -= my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.y <= sky - scr_width / 2.0f || start.y >= ground + scr_width / 2.0f
				|| (end.x <= -scr_width / 2.0f || start.x >= scr_width + scr_width / 2.0f))return false;
		}
	}
	else
	{
		float target_y = 0;

		target_y = center.x * tanf((45.0f + _randit(5.0f, 15.0f)) * 3.14159f / 180.0f);

		if (move_ey > move_sy)set_path(center.x, center.y + target_y);
		else set_path(center.x, center.y - target_y);

		if (ver_dir)
		{
			if (move_ey < move_sy)
			{
				start.y -= my_speed;
				set_edges();
				if (end.y <= sky - scr_width / 2.0f)return false;
			}
			else
			{

				start.y += my_speed;
				set_edges();
				if (start.y >= ground + scr_width / 2.0f)return false;
			}
		}
		else if (hor_dir)
		{
			if (move_ex < move_sx)
			{
				start.x -= my_speed;
				set_edges();
				if (end.x <= -scr_width / 2.0f)return false;
			}
			else
			{

				start.x += my_speed;
				set_edges();
				if (start.x >= scr_width + scr_width / 2.0f)return false;
			}
		}
		else
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.y <= sky - scr_width / 2.0f || start.y >= ground + scr_width / 2.0f
				|| (end.x <= -scr_width / 2.0f || start.x >= scr_width + scr_width / 2.0f))return false;
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
		new_dims(150.0f, 77.0f);
		break;

	case clouds::cloud2:
		new_dims(180.0f, 81.0f);
		break;

	case clouds::cloud3:
		new_dims(120.0f, 63.0f);
		break;

	case clouds::cloud4:
		new_dims(200.0f, 79.0f);
		break;

	case clouds::cloud5:
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
			if (start.x <= 0 || start.y <= sky || end.x >= ground)return false;
		}
		else
		{
			start.x += my_speed;
			start.y = start.x * slope + intercept;
			set_edges();
			if (end.x >= scr_width || start.y <= sky || end.x >= ground)return false;
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







// FUNCTIONS *******************************************************************

float dll::Distance(FPOINT first, FPOINT second)
{
	float a{ pow(abs(second.x - first.x), 2) };
	float b{ pow(abs(second.y - first.y), 2) };

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
	if (!(first.left >= second.right || first.right <= second.left
		|| first.up >= second.down || first.down <= second.up))return true;
	
	return false;
}
bool dll::Intersect(FPOINT first, FPOINT second, float first_xrad, float second_xrad, float first_yrad, float second_yrad)
{
	if (abs(second.x - first.x) <= first_xrad + second_xrad &&
		abs(second.y - first.y) <= first_yrad + second_yrad)return true;
	
	return false;
}

