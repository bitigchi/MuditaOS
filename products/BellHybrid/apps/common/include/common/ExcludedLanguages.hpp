// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <vector>
#include <i18n/i18n.hpp>

namespace app
{
    const inline std::vector<Language> getExcludedLanguages()
    {
        return {{"Svenska"}};
    }
} // namespace app
