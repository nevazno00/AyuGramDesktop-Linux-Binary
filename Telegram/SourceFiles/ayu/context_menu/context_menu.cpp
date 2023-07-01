// This is the source code of AyuGram for Desktop.
//
// We do not and cannot prevent the use of our code,
// but be respectful and credit the original author.
//
// Copyright @Radolyn, 2023

#include "context_menu.h"
#include "history/history_inner_widget.h"
#include "lang_auto.h"
#include "ui/widgets/popup_menu.h"
#include "base/unixtime.h"
#include "styles/style_chat.h"
#include "settings/settings_common.h"
#include "message_history_box.h"
#include "ayu/database/ayu_database.h"
#include "ayu/ayu_state.h"

namespace AyuUi {
    AyuPopupMenu::AyuPopupMenu(HistoryInner *parent) {
        _ayuSubMenu = std::make_unique<Ui::PopupMenu>(parent, st::popupMenuWithIcons);
    }

    void AyuPopupMenu::addHistoryAction(HistoryItem *item) {
        if (AyuDatabase::editedMessagesTableExists() && !((AyuDatabase::getEditedMessages(item)).empty())) {
            _ayuSubMenu->addAction(tr::ayu_EditsHistoryMenuText(tr::now), [=] {
                auto box = Box<MessageHistoryBox>(item);
                Ui::show(std::move(box));
            }, &st::menuIconInfo);
        }
    }

    void AyuPopupMenu::addHideMessageAction(HistoryItem *item) const {
        const auto settings = &AyuSettings::getInstance();
        const auto history = item->history();
        _ayuSubMenu->addAction(QString("Hide"), [=]() {
            const auto initKeepDeleted = settings->keepDeletedMessages;

            settings->set_keepDeletedMessages(false);
            history->destroyMessage(item);
            settings->set_keepDeletedMessages(initKeepDeleted);
        }, &st::menuIconClear);
    }

    void AyuPopupMenu::addReadUntilAction(HistoryItem *item) const {
        const auto history = item->history();
        _ayuSubMenu->addAction(tr::ayu_ReadUntilMenuText(tr::now), [=]() {
            AyuState::setAllowSendReadPacket(true);
            history->session().data().histories().readInboxOnNewMessage(item);
        }, &st::menuIconShowInChat);
    }
} // namespace AyuUi