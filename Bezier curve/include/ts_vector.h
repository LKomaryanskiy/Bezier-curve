#pragma once
#include <vector>
#include <mutex>

/*
	Implement the thread safe version template std::vector.
	I implemented methods which only i needed.
*/
template<class T>
class ts_vector
{
public:
	ts_vector(int size = 0)
	{
		std::lock_guard<std::mutex> lock(mutex);
		vector.resize(size);
	};

	T get_value(int i)
	{
		std::lock_guard<std::mutex> lock(mutex);
		return vector[i];
	};

	int size()
	{
		std::lock_guard<std::mutex> lock(mutex);
		return vector.size();
	};

	void resize(int size)
	{
		std::lock_guard<std::mutex> lock(mutex);
		vector.resize(size);
	};
	
	void set_value(int i, const T& value)
	{
		std::lock_guard<std::mutex> lock(mutex);
		vector[i] = value;
	};

	void clear()
	{
		std::lock_guard<std::mutex> lock(mutex);
		vector.clear();
	};

private:
	std::vector<T>			vector;
	mutable std::mutex		mutex;
};