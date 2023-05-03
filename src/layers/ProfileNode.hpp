#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../objects/ProfileObject.hpp"
#include "../utils/Game.hpp"
#include "../utils/Profiles.hpp"
#include "EditProfileForm.hpp"

#include <string>
#include <chrono>
#include <iostream>
#include <thread>
#include <utility>


using namespace geode::prelude;

struct ProfileNode : public TableViewCell {

    ProfileObject* myCustomData;

    static ProfileNode* create(const char* name, CCSize size) {
        ProfileNode* cell = new ProfileNode(name, size);

        if (cell) {

            cell->setAnchorPoint({ 0.5, 1 });
            return cell;
        } else {
            CC_SAFE_DELETE(cell);

            return nullptr;
        }
    }


    void onUse(CCObject*) {
        log::info("onUse");
        LoadingCircle* loadingCircle = LoadingCircle::create();
        loadingCircle->setFade(false);
        loadingCircle->show();

        std::string name(this->myCustomData->name);
        profilesutils::loadIntoNew(name);
    }

    void setIndex(unsigned int index) {
        this->index = index;
    }


    void onRename(CCObject*) {
        EditProfileForm::create(this->myCustomData->name)->show();
    }

    void loadFromMyData(ProfileObject* object) {
        this->myCustomData = object;
        this->m_backgroundLayer->setOpacity(0x00);
        this->setAnchorPoint({ 0.5, 0.5 });

        this->m_mainLayer->setContentSize({ 300, 50 });

        bool isCurrent = profilesutils::readCurrentProfile() == object->name;

        extension::CCScale9Sprite* underlayer = extension::CCScale9Sprite::create("square02b_small.png");
        underlayer->setContentSize({ 300, 50.0f });
        underlayer->setColor({ 0, 0, 0 });
        underlayer->setOpacity(isCurrent ? 130 : 100);
        underlayer->setZOrder(-1);
        
        this->m_mainLayer->addChild(underlayer);
        
        CCLabelBMFont* profileName = CCLabelBMFont::create(object->name.c_str(), "bigFont.fnt");
        underlayer->addChild(profileName);



        underlayer->setPosition({ 150, 25 });

        profileName->setAnchorPoint({ 0, 0.5 });
        profileName->setPosition({ 19, 25 });
        profileName->setScale(0.65f);

        CCMenu* menu = CCMenu::create();
        
        CCMenuItemSpriteExtra* useButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(isCurrent ? "GJ_selectSongOnBtn_001.png" : "GJ_selectSongBtn_001.png"),
            isCurrent ? nullptr : this,
            !isCurrent ? menu_selector(ProfileNode::onUse) : nullptr
        );
        CCMenuItemSpriteExtra* renameButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_editObjBtn3_001.png"),
            this,
            menu_selector(ProfileNode::onRename)
        );

        underlayer->addChild(menu);
        menu->addChild(useButton);
        menu->addChild(renameButton);

        menu->setAnchorPoint({ 0.5, 0.5 });
        menu->setContentSize({ 75, 75 });
        menu->setPosition({ 226, 75 });

        useButton->setPosition({ 44, -51 });
        renameButton->setPosition({ 4, -51 });

    }
protected:
    unsigned int index;

    ProfileNode(const char* name, CCSize size) : TableViewCell(name, size.width, size.height) { }
};