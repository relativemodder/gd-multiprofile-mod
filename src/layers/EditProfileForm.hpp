#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "../utils/Profiles.hpp"
#include "../utils/Game.hpp"
#include <string>

using namespace geode::prelude;

class EditProfileForm : public Popup<std::string> {
protected:
    CCTextInputNode* input;
    std::string old_name_string;

    bool setup(std::string oldName) {
        this->old_name_string = oldName;

        m_noElasticity = true;
        this->m_mainLayer->setAnchorPoint({ 0, 0 });

        CCMenuItemSpriteExtra* okBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_longBtn01_001.png"),
            this,
            menu_selector(EditProfileForm::onOk)
        );

        this->input = CCTextInputNode::create(386, 47, "Enter new profile name", "bigFont.fnt");
        this->input->m_textField->setString(oldName.c_str());
        this->m_mainLayer->addChild(this->input);
        this->input->setPosition({ 285, 188 });
        this->input->setScale(0.5);

        CCMenu* btnMenu = CCMenu::create();
        btnMenu->addChild(okBtn);
        okBtn->setPosition({ 0, -50 });

        CCMenuItemLabel* okLabel = CCMenuItemLabel::create(CCLabelBMFont::create("Rename", "bigFont.fnt"));
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

        profilesutils::renameProfile(this->old_name_string, new_profile_name);

        AppDelegate::sharedApplication()->trySaveGame();
        profilesutils::restartGame();
        this->onClose(m_closeBtn);
    }

public:
    static EditProfileForm* create(std::string oldName) {
        auto ret = new EditProfileForm();
        if (ret && ret->init(289, 161, oldName)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

};