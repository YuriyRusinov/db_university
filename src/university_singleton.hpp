#pragma once

#include <memory>
#include <iostream>

class UniversitySingleton {
public:
    static UniversitySingleton& getUnivStudentsS() { static UniversitySingleton instance; return instance; }

private:
    UniversitySingleton() = default;
    UniversitySingleton( const UniversitySingleton& U ) = delete;
    UniversitySingleton& operator= ( const UniversitySingleton& U ) = delete;
    UniversitySingleton( UniversitySingleton&& ) = delete;
    UniversitySingleton& operator=( UniversitySingleton&& ) = delete;
    virtual ~UniversitySingleton() { std::cout << __FUNCTION__ << std::endl; }
};
