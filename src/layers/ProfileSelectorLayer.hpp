#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <vector>

#include "ProfileList.hpp"
#include "../objects/ProfileObject.hpp"
#include "../utils/Profiles.hpp"
#include "NewProfileForm.hpp"

using namespace geode::prelude;

class ProfileSelectorLayer : public Popup<> {
protected:
    bool setup() {
        m_noElasticity = true;

        this->m_mainLayer->setAnchorPoint({ 0, 0 });
        this->setupTitleLabel();
        this->setupProfileList();
        this->setupCreateProfileBtn();

        return true;
    }

    void onNewProfile(CCObject*) {
        auto popup = NewProfileForm::create();
        popup->show();
    }

    void setupCreateProfileBtn() {
        CCMenu* menu = CCMenu::create();
        menu->setID("create-profile-button-menu");

        this->m_mainLayer->addChild(menu);

        menu->setAnchorPoint({ 0, 0 });
        menu->setPosition({ 429,21 });
        menu->setContentSize({ 110, 110 });

        CCMenuItemSpriteExtra* newBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_newBtn_001.png"),
            this,
            menu_selector(ProfileSelectorLayer::onNewProfile)
        );

        menu->addChild(newBtn);
        newBtn->setAnchorPoint({ 0.5, 0.5 });
        newBtn->setPosition({ 26, 26 });
    }

    void setupProfileList() {
        CCArray* profile_array = CCArray::create();

        std::string profiles_path = profilesutils::checkDirExistsOrCreate();
        std::vector<std::string> profile_list = profilesutils::getProfileList(profiles_path);

        profilesutils::createTemplates();

        for (auto profile_name : profile_list) {
            ProfileObject* profile = ProfileObject::create();
            profile->name = profile_name;

            profile_array->addObject(profile);
        }
        

        ProfileList* profileList = ProfileList::create(profile_array, { 300, 158 });
        this->m_mainLayer->addChild(profileList);
        profileList->setPosition({ 137, 71 });

        profileList->setAnchorPoint({ 0.5, 1 });
    }

    void setupTitleLabel() {
        CCLabelBMFont* title_label = CCLabelBMFont::create("Choose a profile", "bigFont.fnt");
        this->m_mainLayer->addChild(title_label);

        title_label->setPosition({ 285, 254 });
        title_label->setScale(0.65);
    }

public:
    static ProfileSelectorLayer* create() {
        auto ret = new ProfileSelectorLayer();
        if (ret && ret->init(348.f, 230.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

};