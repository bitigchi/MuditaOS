// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "LockBox.hpp"

#include <locks/windows/LockInputWindow.hpp>
#include <Image.hpp>

namespace gui
{
    class LockBoxInconstantSize : public LockBox
    {
      public:
        void buildLockBox(unsigned int pinSize) override;
        void clear() final;
        void popChar(unsigned int charNum) final;
        void putChar(unsigned int charNum) final;

      protected:
        explicit LockBoxInconstantSize(LockInputWindow *lockBaseWindow) : lockWindow(lockBaseWindow)
        {}

        void buildPinLabels(unsigned int pinSize);

      private:
        LockInputWindow *lockWindow;
    };
} // namespace gui