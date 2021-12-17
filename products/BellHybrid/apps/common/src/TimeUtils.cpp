// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <common/TimeUtils.hpp>

#include <time/time_conversion.hpp>
#include <time/dateCommon.hpp>

namespace utils::time
{
    std::time_t getCurrentTime()
    {
        return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    }

    std::time_t calculateTimeDifference(std::time_t alarmTime, std::time_t currentTime)
    {
        return Duration{alarmTime, currentTime}.get();
    }
    std::time_t calculateMinutesDifference(std::time_t alarmTime, std::time_t currentTime)
    {
        const auto floorMinutesAlarmTime   = alarmTime - alarmTime % 60;
        const auto floorMinutesCurrentTime = currentTime - currentTime % 60;
        return calculateTimeDifference(floorMinutesAlarmTime, floorMinutesCurrentTime);
    }

    UTF8 getBottomDescription(std::time_t timestamp)
    {
        const auto prefix   = translate("app_bellmain_home_screen_bottom_desc");
        const auto willRing = translate("app_bellmain_home_screen_bottom_desc_will_ring)";
        const auto in       = translate("app_bellmain_home_screen_bottom_desc_in");
        const auto duration = Duration{timestamp};
        const auto timeText = [](time_t hours, time_t minutes) -> std::string {
            if (hours == 0) {
                if (minutes == 0) {
                    return "24 " + translate("common_hours_short");
                }
                else if (minutes == 1) {
                    return translate("app_bellmain_home_screen_bottom_desc_less_than_one_minute");
                }
                else {
                    return std::to_string(minutes) + " " + translate("common_minute_short");
                }
            }
            else if (minutes == 0) {
                return std::to_string(hours) + " " + translate("common_hours_short");
            }
            // Get singular hour in case there is one hour plus minutes
            else if (hours == 1) && (minutes != 0) {
                return std::to_string(hours) + " " +
                    translate("common_hour_short") + " " +
                    translate("app_bellmain_home_screen_bottom_desc_and") +
                    std::to_string(minutes) + " " + translate("common_minute_short");
            }
            else {
                return std::to_string(hours) + " " +
                    translate("common_hours_short") + " " +
                    translate("app_bellmain_home_screen_bottom_desc_and") +
                    std::to_string(minutes) + " " + translate("common_minute_short");
            }
        }(duration.getHours(), duration.getMinutes());
        return UTF8("<text>" + prefix + willRing + in + "<br />" + timeText + "</text>");
    }
} // namespace utils::time
