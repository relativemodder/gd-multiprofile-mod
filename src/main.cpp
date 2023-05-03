#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "layers/ProfileSelectorLayer.hpp"

using namespace geode::prelude;

class $modify(MainMenuLayer, MenuLayer) {
	bool init() {
		auto ret = MenuLayer::init();

		CCMenu* profile_menu = dynamic_cast<CCMenu*>(this->getChildByID("profile-menu"));
		CCMenuItemSpriteExtra* multiprofile_layer_button = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("accountBtn_friends_001.png"),
			nullptr,
			this,
			menu_selector(MainMenuLayer::onMultiprofile)
		);

		profile_menu->addChild(multiprofile_layer_button);
		multiprofile_layer_button->setPosition({ 78, 26 });

		CCSprite* internal_sprite = dynamic_cast<CCSprite*>(multiprofile_layer_button->getChildren()->objectAtIndex(0));
		internal_sprite->setScale(0.9f);

		return ret;
	}

	void onMultiprofile(CCObject* sender) {
		geode::log::info("{}", "Clicked Multi-profile button!");

		ProfileSelectorLayer::create()->show();
	}
};
