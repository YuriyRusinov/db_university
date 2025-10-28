#include "university_singleton.hpp"

UniversitySingleton::UniversitySingleton() {
    m_dbCore = dbCore::create();
}
