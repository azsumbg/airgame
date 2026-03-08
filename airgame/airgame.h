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
	evil1 = 0, evil2 = 1, evil3 = 2, evil4 = 3, evil5 = 4, hero = 5,
	boss1 = 6, boss2 = 7, boss3 = 8
};
enum class clouds { cloud1 = 0, cloud2 = 1, cloud3 = 2, cloud4 = 3, cloud5 = 4 };
enum class tiles { sea = 0, tree1 = 1, tree2 = 2, tree3 = 3 };

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












}
