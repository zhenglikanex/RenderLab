#pragma once

#include <memory>

namespace Aurora
{

	template<class T>
	class Singleton
	{
	public:
		Singleton() = default;
		~Singleton() = default;

		static const std::unique_ptr<T>& GetInstance()
		{
			if (instance_) {
				return instance_;
			}

			instance_ = std::make_unique<T>();

			return instance_;
		}
	protected:
		static std::unique_ptr<T> instance_;
	};

	template<class T> std::unique_ptr<T> Singleton<T>::instance_ = std::unique_ptr<T>();
}
