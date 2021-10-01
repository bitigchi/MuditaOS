// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "popups/BellTurnOffOptionWindow.hpp"

#include <apps-common/widgets/BellBaseLayout.hpp>
#include <apps-common/ApplicationCommon.hpp>
#include <common/data/StyleCommon.hpp>
#include <common/windows/BellTurnOffWindow.hpp>
#include <i18n/i18n.hpp>
#include <options/OptionBellMenu.hpp>

namespace gui
{
    BellTurnOffOptionWindow::BellTurnOffOptionWindow(app::ApplicationCommon *app, const char *name)
        : BellShortOptionWindow(app, name), yesStr{utils::translate("common_yes")}, noStr{utils::translate("common_no")}
    {
        addOptions(settingsOptionsList());
        setListTitle(utils::translate("app_bell_turn_off_question"));
        optionsList->setBoundaries(Boundaries::Continuous);
        optionsList->setAlignment(Alignment(Alignment::Vertical::Center));
    }

    std::list<Option> BellTurnOffOptionWindow::settingsOptionsList()
    {
        std::list<gui::Option> settingsOptionList;
        auto addWinSettings = [&](const UTF8 &name, std::function<bool(Item &)> activatedCallback) {
            settingsOptionList.emplace_back(std::make_unique<gui::option::OptionBellMenu>(
                name, activatedCallback, [=](gui::Item &) { return true; }, this));
        };

        addWinSettings(noStr, [this](auto &) {
            application->returnToPreviousWindow();
            return true;
        });
        addWinSettings(yesStr, [this](auto &) {
            application->switchWindow(BellTurnOffWindow::name);
            return true;
        });

        return settingsOptionList;
    }
} /* namespace gui */