#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>
#include <mutex>

template <typename T> class Singleton {
public:
    template <typename... Args> static T& instance( Args&&... args ) {
        std::call_once( initFlag, [ & ]() {
            instancePtr.reset( new T( std::forward<Args>( args )... ) );
        } );
        return *instancePtr;
    }

    Singleton( const Singleton& ) = delete;
    Singleton& operator=( const Singleton& ) = delete;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

private:
    static std::unique_ptr<T> instancePtr;
    static std::once_flag initFlag;
};

template <typename T> std::unique_ptr<T> Singleton<T>::instancePtr = nullptr;

template <typename T> std::once_flag Singleton<T>::initFlag;

#endif // SINGLETON_H
