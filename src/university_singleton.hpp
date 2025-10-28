#pragma once

#include <memory>
#include <iostream>

#include "dbCore.h"

class UniversitySingleton {
public:
    static UniversitySingleton& getUnivStudentsS() { static UniversitySingleton instance; return instance; }
    dbCore* getCore() const { return m_dbCore.get(); }
private:
    UniversitySingleton();
    UniversitySingleton( const UniversitySingleton& U ) = delete;
    UniversitySingleton& operator= ( const UniversitySingleton& U ) = delete;
    UniversitySingleton( UniversitySingleton&& ) = delete;
    UniversitySingleton& operator=( UniversitySingleton&& ) = delete;
    virtual ~UniversitySingleton() { std::cout << __FUNCTION__ << std::endl; }

    std::unique_ptr<dbCore> m_dbCore;
};
