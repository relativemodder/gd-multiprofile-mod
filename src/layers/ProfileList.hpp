#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "ProfileNode.hpp"

using namespace geode::prelude;

struct ProfileList : public CustomListView {
public:
    static ProfileList* create(CCArray* data, CCSize size) {
        ProfileList* view = new ProfileList();

        if (view && view->init(data, BoomListType::Default, size.width, size.height)) {
            view->autorelease();
            
            return view;
        } else {
            CC_SAFE_DELETE(view);

            return nullptr;
        }
    }

protected:
    void setupList() override {
        unsigned int count = this->m_entries->count();

        this->m_itemSeparation = 60.0f;
        this->setAnchorPoint({ 0.5, 1 });
        this->m_tableView->setAnchorPoint({ 0.5, 1 });

        if (count) {
            this->m_tableView->reloadData();

            if (count == 1) {
                this->m_tableView->moveToTopWithOffset(this->m_itemSeparation);
            }
            else {
                this->m_tableView->moveToTop();
            }
        }
    }

    TableViewCell* getListCell(const char* name) override {
        return ProfileNode::create(name, { this->m_width , this->m_itemSeparation });
    }
    void loadCell(TableViewCell* cell, unsigned int index) override {
        ProfileObject* profile = reinterpret_cast<ProfileObject*>(this->m_entries->objectAtIndex(index));


        reinterpret_cast<ProfileNode*>(cell)->setIndex(index);
        reinterpret_cast<ProfileNode*>(cell)->loadFromMyData(profile);
    }
};