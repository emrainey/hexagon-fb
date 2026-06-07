
#include "hexagon/port/posix/PosixRegistry.h"

PosixRegistry::PosixRegistry() {
    // Set a default IconDirectory key so it is not nullptr
    setKey(Key("IconDirectory", "."));
}

void PosixRegistry::setKey(Key k) {
    m_keys.insert_or_assign(k.getName(), k);
}

Key *PosixRegistry::getKey(std::string name) {
    auto it = m_keys.find(name);
    if (it != m_keys.end()) {
        return &(it->second);
    }
    return nullptr;
}