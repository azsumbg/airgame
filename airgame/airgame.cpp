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