#pragom once
#include <memory>

namespace aurora
{

	template<class T>
	class Singleton
	{
	public:
		Singleton(){}

		~Singleton(){}

		static const std::unique_ptr<T>& GetInstance()
		{
			if (instance_) {
				return instance_;
			}

			instance_ = std::make_unique<T>();

			return instance_;
		}

		virtual bool Initialized()
		{
			return true;
		}

	protected:
		static std::unique_ptr<T> instance_;
	};

	template<class T> std::unique_ptr<T> Singleton<T>::instance_ = std::unique_ptr<T>();
}

#endif