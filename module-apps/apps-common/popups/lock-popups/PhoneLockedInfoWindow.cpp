// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "PhoneLockedInfoWindow.hpp"

#include <service-appmgr/Controller.hpp>
#include <application-desktop/data/DesktopStyle.hpp>

#include <i18n/i18n.hpp>

using namespace gui;

PhoneLockedInfoWindow::PhoneLockedInfoWindow(app::Application *app, const std::string &name)
    : WindowWithTimer(app, name, std::chrono::seconds(2))
{
    buildInterface();
}

void PhoneLockedInfoWindow::onBeforeShow([[maybe_unused]] ShowMode mode, SwitchData *data)
{
    auto info = dynamic_cast<PhoneLockedInfoData *>(data);

    mStage = (info && info->getStage() == PhoneLockedInfoData::Stage::Waiting) ? PhoneLockedInfoData::Stage::Waiting
                                                                               : PhoneLockedInfoData::Stage::Idle;

    if (mStage == PhoneLockedInfoData::Stage::Waiting) {
        bottomBar->setActive(BottomBar::Side::LEFT, true);
        bottomBar->setActive(BottomBar::Side::CENTER, false);
        bottomBar->setActive(BottomBar::Side::RIGHT, true);
        infoIcon->text->setRichText(utils::translate("app_desktop_press_to_complete_unlock"));
    }
    else {
        bottomBar->setActive(BottomBar::Side::LEFT, true);
        bottomBar->setActive(BottomBar::Side::CENTER, true);
        bottomBar->setActive(BottomBar::Side::RIGHT, true);
        infoIcon->text->setRichText(utils::translate("app_desktop_press_to_unlock"));
    }

    WindowWithTimer::onBeforeShow(mode, data);
}

bool PhoneLockedInfoWindow::onInput(const InputEvent &inputEvent)
{
    // Right key = go back
    if (inputEvent.isShortRelease(KeyCode::KEY_RF)) {
        detachTimerIfExists();
        application->returnToPreviousWindow();
        return true;
    }

    // Left key = SOS call, it should work all the time
    else if (inputEvent.isShortRelease(KeyCode::KEY_LF) && bottomBar->isActive(BottomBar::Side::LEFT)) {
        app::manager::Controller::sendAction(application,
                                             app::manager::actions::EmergencyDial,
                                             std::make_unique<SwitchData>(),
                                             app::manager::OnSwitchBehaviour::RunInBackground);
        return true;
    }

    // Pnd key = go to PIN code screen
    else if (inputEvent.isShortRelease(KeyCode::KEY_PND) && mStage == PhoneLockedInfoData::Stage::Waiting) {
        detachTimerIfExists();
        application->returnToPreviousWindow();
        application->getPhoneLockSubject().unlock();
        return true;
    }

    // Enter key = start unlocking if we are in STARTING stage
    else if (inputEvent.isShortRelease(KeyCode::KEY_ENTER) && mStage == PhoneLockedInfoData::Stage::Idle) {
        mStage = PhoneLockedInfoData::Stage::Waiting;
        bottomBar->setActive(BottomBar::Side::CENTER, false);
        infoIcon->text->setRichText(utils::translate("app_desktop_press_to_complete_unlock"));
        resetTimer();
        return true;
    }

    // Any other key - reset timers and go to STARTING stage
    else if (inputEvent.isShortRelease() && mStage == PhoneLockedInfoData::Stage::Waiting) {
        mStage = PhoneLockedInfoData::Stage::Idle;
        bottomBar->setActive(BottomBar::Side::CENTER, true);
        infoIcon->text->setRichText(utils::translate("app_desktop_press_to_unlock"));
        resetTimer();
        return true;
    }

    return WindowWithTimer::onInput(inputEvent);
}

status_bar::Configuration PhoneLockedInfoWindow::configureStatusBar(status_bar::Configuration appConfiguration)
{
    appConfiguration.disable(status_bar::Indicator::NetworkAccessTechnology);
    appConfiguration.disable(status_bar::Indicator::Time);
    appConfiguration.enable(status_bar::Indicator::PhoneMode);
    appConfiguration.enable(status_bar::Indicator::Lock);
    appConfiguration.enable(status_bar::Indicator::Battery);
    appConfiguration.enable(status_bar::Indicator::Signal);
    appConfiguration.enable(status_bar::Indicator::SimCard);
    return appConfiguration;
}

void PhoneLockedInfoWindow::buildInterface()
{
    WindowWithTimer::buildInterface();

    bottomBar->setText(BottomBar::Side::LEFT, utils::translate("app_desktop_emergency"));
    bottomBar->setText(BottomBar::Side::CENTER, utils::translate("app_desktop_unlock"));
    bottomBar->setText(BottomBar::Side::RIGHT, utils::translate("common_back"));

    infoIcon = new gui::Icon(this,
                             style::window::default_left_margin,
                             style::window::default_vertical_pos,
                             style::window::default_body_width,
                             style::window::default_body_height,
                             "unlock_icon_W_G",
                             utils::translate("app_desktop_press_to_unlock"));

    infoIcon->setAlignment(Alignment::Horizontal::Center);
}