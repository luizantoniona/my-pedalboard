#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <mutex>

template <typename T> class Singleton {
public:
    static T& instance() {
        std::lock_guard<std::mutex> lock( _mutex );
        if ( !_instance ) {
            _instance = std::make_unique<T>();
        }
        return *_instance;
    }

    static bool initialized() {
        return _instance != nullptr;
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

    Singleton( const Singleton& ) = delete;
    Singleton& operator=( const Singleton& ) = delete;

private:
    static std::unique_ptr<T> _instance;
    static std::mutex _mutex;
};

template <typename T> std::unique_ptr<T> Singleton<T>::_instance = nullptr;

template <typename T> std::mutex Singleton<T>::_mutex;

#endif // SINGLETON_H
