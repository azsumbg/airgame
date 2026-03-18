#pragma once

#ifdef AIRGAME_EXPORTS
#define AIRGAME_API __declspec(dllexport)
#else 
#define AIRGAME_API __declspec(dllimport)
#endif

#include <cstdlib>
#include <random>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 750.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 700.0f };

constexpr int BAG_BAD_PTR{ 6001 };
constexpr int BAG_BAD_INDEX{ 6002 };
constexpr int BAG_BAD_PARAM{ 6003 };

enum class dirs { up = 0, down = 1, left = 2, right = 3, stop = 4 };
enum class move_dirs { straight = 0, left = 1, right = 2 };

enum class creatures {
	evil1 = 0, evil2 = 1, evil3 = 2, evil4 = 3, evil5 = 4, boss1 = 5, boss2 = 6, boss3 = 7
};
enum class clouds { cloud1 = 0, cloud2 = 1, cloud3 = 2, cloud4 = 3, cloud5 = 4 };
enum class tiles { sea = 0, tree1 = 1, tree2 = 2, tree3 = 3, field = 4 };
enum class powerups { big_gun = 0, rocket = 1, repair = 2, shield = 3 };
enum class shots { bullet = 0, blast = 1, rocket = 2 };
enum class actions { move = 0, shoot = 1, dir_changed = 2 };

struct FPOINT
{
	float x{ 0 };
	float y{ 0 };
};
struct FRECT
{
	float left{ 0 };
	float up{ 0 };
	float right{ 0 };
	float down{ 0 };
};

namespace dll
{
	class AIRGAME_API EXCEPTION
	{
	private:
		int what{ 0 };

	public:
		EXCEPTION(int what_happened) :what{ what_happened } {};

		const char* get()
		{
			switch (what)
			{
			case BAG_BAD_PTR:
				return "Bad pointer of BAG class !";

			case BAG_BAD_INDEX:
				return "Bad index passed to BAG class method !";

			case BAG_BAD_PARAM:
				return "Bad parameter passed to BAG class method !";
			}

			return "Unknown error in BAG class method !";
		}
	};

	template<typename T> class BAG
	{
	private: 
			size_t max_size{ 1 };	
			T* m_ptr{ nullptr };
			size_t next_pos{ 0 };

	public:
		BAG() :m_ptr{ reinterpret_cast<T*>(calloc(max_size,sizeof(T))) } {};
		BAG(size_t capacity) :max_size{ capacity }, m_ptr{ reinterpret_cast<T*>(calloc(capacity,sizeof(T))) } {};
		BAG(BAG& other)
		{
			if (m_ptr == other.m_ptr)throw EXCEPTION(BAG_BAD_PARAM);

			max_size = other.max_size;
			next_pos = other.next_pos;

			m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

			if (other.next_pos > 0)
				for (size_t count = 0; count < other.next_pos; ++count)m_ptr[count] = other.m_ptr[count];
		}
		BAG(BAG&& other)
		{
			if (other.m_ptr == nullptr || other.next_pos == 0)throw EXCEPTION(BAG_BAD_PARAM);

			m_ptr = other.m_ptr;
			max_size = other.max_size;
			next_pos = other.next_pos;

			other.m_ptr = nullptr;
			other.max_size = 0;
			other.next_pos = 0;
		}
		~BAG()
		{
			free(m_ptr);
		}

		BAG& operator=(BAG& other)
		{
			if (m_ptr == other.m_ptr || other.m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PARAM);

			free(m_ptr);
			m_ptr = nullptr;

			max_size = other.max_size;
			next_pos = other.next_pos;

			m_ptr = reinterpret_cast<T*>(calloc(max_size, sizeof(T)));

			if (other.next_pos > 0)
				for (size_t count = 0; count < other.next_pos; ++count)m_ptr[count] = other.m_ptr[count];

			return *this;
		}
		BAG& operator=(BAG&& other)
		{
			if (other.m_ptr == nullptr || other.next_pos == 0)throw EXCEPTION(BAG_BAD_PARAM);

			free(m_ptr);
			m_ptr = nullptr;

			m_ptr = other.m_ptr;
			max_size = other.max_size;
			next_pos = other.next_pos;

			other.m_ptr = nullptr;
			other.max_size = 0;
			other.next_pos = 0;

			return *this;
		}
		
		T& operator[](size_t index)
		{
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);
			if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);

			return m_ptr[index];
		}

		bool empty() const
		{
			return (next_pos == 0);
		}
		size_t size() const
		{
			return next_pos;
		}
		size_t capacity() const
		{
			return max_size();
		}
		void clear()
		{
			free(m_ptr);
			m_ptr = reinterpret_cast<T*>(calloc(1, sizeof(T)));
			max_size = 1;
			next_pos = 0;
		}

		void push_back(T element)
		{
			if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = element;
					++next_pos;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						m_ptr[next_pos] = element;
						++next_pos;
					}
				}
			}
		}
		void push_back(T* element)
		{
			if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos + 1 <= max_size)
				{
					m_ptr[next_pos] = *element;
					++next_pos;
				}
				else
				{
					++max_size;
					m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
					if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
					else
					{
						m_ptr[next_pos] = *element;
						++next_pos;
					}
				}
			}
		}

		void push_front(T element)
		{
			if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t index = next_pos; index > 0; --index)m_ptr[index] = m_ptr[index - 1];
						*m_ptr = element;
						++next_pos;
					}
					else
					{
						++max_size;
						m_ptr = reinterpret_cast(realloc(m_ptr, max_size * sizeof(T)));
						if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
						else
						{
							for (size_t index = next_pos; index > 0; --index)m_ptr[index] = m_ptr[index - 1];
							*m_ptr = element;
							++next_pos;
						}
					}
				}
			}
		}
		void push_front(T* element)
		{
			if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
			else
			{
				if (next_pos == 0)
				{
					*m_ptr = *element;
					++next_pos;
				}
				else
				{
					if (next_pos + 1 <= max_size)
					{
						for (size_t index = next_pos; index > 0; --index)m_ptr[index] = m_ptr[index - 1];
						*m_ptr = *element;
						++next_pos;
					}
					else
					{
						++max_size;
						m_ptr = reinterpret_cast(realloc(m_ptr, max_size * sizeof(T)));
						if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
						else
						{
							for (size_t index = next_pos; index > 0; --index)m_ptr[index] = m_ptr[index - 1];
							*m_ptr = *element;
							++next_pos;
						}
					}
				}
			}
		}

		void erase(size_t index)
		{
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);

			for (size_t count = index; count < next_pos - 1; ++count)m_ptr[count] = m_ptr[count + 1];
		}

		void insert(size_t index, T element)
		{
			if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);

			if (next_pos + 1 <= max_size)
			{
				for (size_t count = next_pos; count > index; --count)m_ptr[count] = m_ptr[count - 1];
				m_ptr[index] = element;
				++next_pos;
			}
			else
			{
				++max_size;
				m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
				if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
				else
				{
					for (size_t count = next_pos; count > index; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[index] = element;
					++next_pos;
				}
			}
		}
		void insert(size_t index, T* element)
		{
			if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
			if (index < 0 || index >= next_pos)throw EXCEPTION(BAG_BAD_INDEX);

			if (next_pos + 1 <= max_size)
			{
				for (size_t count = next_pos; count > index; --count)m_ptr[count] = m_ptr[count - 1];
				m_ptr[index] = *element;
				++next_pos;
			}
			else
			{
				++max_size;
				m_ptr = reinterpret_cast<T*>(realloc(m_ptr, max_size * sizeof(T)));
				if (m_ptr == nullptr)throw EXCEPTION(BAG_BAD_PTR);
				else
				{
					for (size_t count = next_pos; count > index; --count)m_ptr[count] = m_ptr[count - 1];
					m_ptr[index] = *element;
					++next_pos;
				}
			}
		}
	};

	////////////////////////////////////////////////

	class AIRGAME_API RANDIT
	{
	private:
		std::mt19937* twister{ nullptr };

	public:
		RANDIT();
		~RANDIT();

		int operator()(int min, int max);
		float operator()(float min, float max);
	};

	class AIRGAME_API PROTON
	{
	protected:
		float _width{ 0 };
		float _height{ 0 };

		float _speed{ 1.1f };

		float move_sx = 0;
		float move_ex = 0;
		float move_sy = 0;
		float move_ey = 0;

		bool ver_dir = 0;
		bool hor_dir = 0;

		float slope{ 0 };
		float intercept{ 0 };

		RANDIT _randit{};
		
	public:
		FPOINT start{};
		FPOINT end{};
		FPOINT center{};

		float x_rad{ 0 };
		float y_rad{ 0 };

		PROTON();
		PROTON(float _sx, float _sy);
		PROTON(float _sx, float _sy, float _s_width, float _s_height);

		virtual ~PROTON() {};

		float get_width() const;
		float get_height() const;

		void set_width(float new_width);
		void set_height(float new_height);
		void new_dims(float new_width, float new_height);

		void set_edges();
	
		void set_path(float to_where_x, float to_where_y);

		virtual bool move(float where_x, float where_y, float gear);
		virtual bool move(float gear) { return false; };
	};

	class AIRGAME_API GROUND :public PROTON
	{
	protected:
		float _speed{ 1.0f };

		GROUND(tiles _what_type, float _s_x, float _s_y);

	public:
		tiles type{ tiles::field };
		dirs dir{ dirs::down };

		bool move(float gear) override;

		void Release();

		static GROUND* create(tiles what_type, float s_x, float s_y);
	};

	class AIRGAME_API POWERUPS :public PROTON
	{
	private:
		
		POWERUPS(powerups _what, float _sx, float _sy);

	public:
		powerups type{ powerups::big_gun };
		
		void Release();

		static POWERUPS* create(powerups what, float sx, float sy);
	};

	class AIRGAME_API CLOUDS :public PROTON
	{
	private:

		CLOUDS(clouds _type, float _sx, float _sy);

	public:
		clouds type{ clouds::cloud1 };

		void Release();

		static CLOUDS* create(clouds type, float sx, float sy);
	};

	class AIRGAME_API SHOTS :public PROTON
	{
	private:
	
		SHOTS(shots _type, float _sx, float _sy, float _ex, float _ey);

	public:
		shots type{ shots::bullet };
		int damage = 0;
		dirs dir{ dirs::down };

		bool move(float gear)override;

		void Release();

		static SHOTS* create(shots type, float sx, float sy, float ex, float ey);
	};

	class AIRGAME_API HERO :public PROTON
	{
	private:
		int max_frames = 9;
		int frame_delay = 7;
		int max_frame_delay = 7;
		int current_frame = 0;
		
		move_dirs orientation{ move_dirs::straight };

		HERO(float _sx, float _sy);

	public:
		dirs dir{ dirs::stop };

		int lifes = 150;
		int damage = 20;
		int armour = 0;

		bool move(float gear) override;

		int get_frame();

		move_dirs get_move_dir()const;
		void set_move_dir(move_dirs new_move_dir);

		void Release();

		static HERO* create(float sx, float sy);
	};

	class AIRGAME_API EVILS :public PROTON
	{
	private:
		int max_frames = 0;
		int frame_delay = 0;
		int max_frame_delay = 0;
		int current_frame = 0;

		int attack_delay = 0;
		int max_attack_delay = 0;
		int damage = 20;

		move_dirs orientation{ move_dirs::straight };

		EVILS(creatures _what_type, float _sx, float _sy);

	public:
		creatures type{ creatures::evil1 };
 
		int lifes = 150;
		int armour = 0;

		FPOINT get_target_point()const;

		int get_frame();
		int attack();

		move_dirs get_move_dir()const;
		void set_move_dir(move_dirs new_move_dir);

		void Release();

		static EVILS* create(creatures what_type, float sx, float sy);
	};

	///////////////////////////////////////////////

	// FUNCTIONS **********************************

	float AIRGAME_API Distance(FPOINT first, FPOINT second);
	void AIRGAME_API Sort(BAG<FPOINT>& bag, FPOINT ref);

	bool AIRGAME_API Intersect(FRECT first, FRECT second);
	bool AIRGAME_API Intersect(FPOINT first, FPOINT second, float first_xrad, float second_xrad, float first_yrad, float second_yrad);

	actions AIRGAME_API AINextMove(EVILS& my_unit, FPOINT hero_center, BAG<FPOINT>& shot_bag, BAG<FPOINT>& other_creatures);
}