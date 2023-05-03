#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ProfileObject : public CCObject {
public:
    static ProfileObject* create() {
        ProfileObject* profile = new ProfileObject();

        if (profile) {
            return profile;
        }
        else {
            CC_SAFE_DELETE(profile);
            return nullptr;
        }
    }
public:
    std::string name;
};