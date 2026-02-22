/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "../datatype/incr-constr.hpp"
#include "../attribute.hpp"


namespace perun2
{

// while sleeping
// check every 300 ms if the program received an interruption signal
p_constexpr p_nint OS_SLEEP_UNIT = NINT_300;


p_tim os_yesterday();
p_tim os_tomorrow();
void os_sleepForMs(const p_nint ms, Perun2Process& p2);

void os_loadEmptyAttributes(FileContext& context);

// get values of filesystem variables:
p_str os_extension(const p_str& value);
p_str os_fullname(const p_str& value);
p_str os_name(const p_str& value);
p_str os_parent(const p_str& path);

p_str os_attr_extension(const p_str& path);
p_str os_attr_fullname(const p_str& path);
p_str os_attr_name(const p_str& path);
p_str os_attr_parent(const p_str& path);

struct MediaAttributes
{
    p_bool isImage = false;
    p_bool isVideo = false;
    p_num width = P_NaN;
    p_num height = P_NaN;
    p_per duration;
};

p_bool os_attr_isImage(const p_str& path);
p_bool os_attr_isVideo(const p_str& path);
p_num os_attr_width(const p_str& path);
p_num os_attr_height(const p_str& path);
p_per os_attr_duration(const p_str& path);

p_bool os_bothAreSeparators(const p_char left, const p_char right);
p_str os_softTrim(const p_str& value);

void os_escapeQuote(p_str& path);
p_str os_quoteEmbraced(const p_str& value);

static p_str os_toWideString(const std::string& str);
MediaAttributes os_ffmpegAttributes(const p_str& filePath);
static p_per os_ffmpegPeriod(const int64_t units);
static bool os_isFfmpegVideoFormat(const std::string& value);
static bool os_isFfmpegImageFormat(const std::string& value);

}
