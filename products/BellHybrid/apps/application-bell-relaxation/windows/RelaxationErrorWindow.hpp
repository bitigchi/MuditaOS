// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "presenter/RelaxationErrorPresenter.hpp"
#include <apps-common/popups/WindowWithTimer.hpp>
#include <memory>

namespace gui
{
    class BellBaseLayout;
    class RelaxationErrorWindow : public WindowWithTimer
    {
      public:
        explicit RelaxationErrorWindow(
            app::ApplicationCommon *app,
            std::unique_ptr<app::relaxation::RelaxationErrorContract::Presenter> &&presenter);

      private:
        std::unique_ptr<app::relaxation::RelaxationErrorContract::Presenter> presenter;

        void buildInterface() override;
        bool onInput(const gui::InputEvent &inputEvent) override;
        void registerCallbacks();
        void buildLayout();
    };
} // namespace gui