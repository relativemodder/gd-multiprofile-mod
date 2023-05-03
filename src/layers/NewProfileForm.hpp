#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../utils/Profiles.hpp"
#include "../utils/Game.hpp"

using namespace geode::prelude;

class NewProfileForm : public Popup<> {
protected:
    CCTextInputNode* input;

    bool setup() {
        m_noElasticity = true;
        this->m_mainLayer->setAnchorPoint({ 0, 0 });

        CCMenuItemSpriteExtra* okBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_longBtn01_001.png"),
            this,
            menu_selector(NewProfileForm::onOk)
        );

        this->input = CCTextInputNode::create(386, 47, "Enter new profile name", "bigFont.fnt");
        this->m_mainLayer->addChild(this->input);
        this->input->setPosition({ 285, 188 });
        this->input->setScale(0.5);

        CCMenu* btnMenu = CCMenu::create();
        btnMenu->addChild(okBtn);
        okBtn->setPosition({ 0, -50 });

        CCMenuItemLabel* okLabel = CCMenuItemLabel::create(CCLabelBMFont::create("Create", "bigFont.fnt"));
        okLabel->setScale(0.5);
        okLabel->setAnchorPoint({ 0.5, 0.5 });
        okLabel->setPosition({ 46, 16 });


        okBtn->addChild(okLabel);

        this->m_mainLayer->addChild(btnMenu);

        return true;
    }

    void onOk(CCObject*) {
        std::string new_profile_name = this->input->m_textField->getString();
        log::info(new_profile_name);


        profilesutils::loadIntoNew(new_profile_name, true);
    }

public:
    static NewProfileForm* create() {
        auto ret = new NewProfileForm();
        if (ret && ret->init(289, 161)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

};