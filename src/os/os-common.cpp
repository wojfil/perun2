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

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "os.h"
#include <time.h>
#include "../perun2.h"
#include "../datatype/parse/parse-asterisk.h"
#include <shlobj.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <algorithm>
#include <fstream>
#include <combaseapi.h>
#include <fcntl.h>



extern "C" {
   #include <libavformat/avformat.h>
   #include <libavcodec/avcodec.h>
   #include <libavutil/imgutils.h> 
}

namespace perun2
{

p_tim os_tomorrow()
{
   p_tim t = os_today();
   t.addDays(1);
   return t;
}

p_tim os_yesterday()
{
   p_tim t = os_today();
   t.addDays(-1);
   return t;
}

void os_sleepForMs(const p_nint ms, Perun2Process& p2)
{
   if (ms <= NINT_ZERO) {
      return;
   }

   p_nint loops = ms / OS_SLEEP_UNIT;
   const p_nint remainder = ms % OS_SLEEP_UNIT;

   os_rawSleepForMs(remainder);

   while (p2.state == State::s_Running && loops != NINT_ZERO) {
      os_rawSleepForMs(OS_SLEEP_UNIT);
      loops--;
   }
}

// attributes of something, that cannot exist (file name is empty string, contains not allowed chars, ...)
void os_loadEmptyAttributes(FileContext& context)
{
   const p_attrptr& attribute = context.attribute;

   if (attribute->has(ATTR_PATH)) {
      context.v_path->value.clear();
   }

   if (attribute->has(ATTR_FULLNAME)) {
      context.v_fullname->value.clear();
   }

   if (attribute->has(ATTR_DEPTH)) {
      context.v_depth->value = P_NaN;
   }

   if (attribute->has(ATTR_DRIVE)) {
      context.v_drive->value.clear();
   }

   if (!attribute->has(ATTR_EXISTS)) {
      return;
   }

   context.v_exists->value = false;
   context.v_isfile->value = false;
   context.v_isdirectory->value = false;

   if (attribute->has(ATTR_ACCESS)) {
      context.v_access->value = p_tim();
   }

   if (attribute->has(ATTR_ARCHIVE)) {
      context.v_archive->value = false;
   }

   if (attribute->has(ATTR_COMPRESSED)) {
      context.v_compressed->value = false;
   }

   if (attribute->has(ATTR_CHANGE)) {
      context.v_change->value = p_tim();
   }

   if (attribute->has(ATTR_CREATION)) {
      context.v_creation->value = p_tim();
   }

   if (attribute->has(ATTR_EMPTY)) {
      context.v_empty->value = false;
   }

   if (attribute->has(ATTR_ENCRYPTED)) {
      context.v_encrypted->value = false;
   }

   if (attribute->has(ATTR_EXTENSION)) {
      context.v_extension->value.clear();
   }

   if (attribute->has(ATTR_HIDDEN)) {
      context.v_hidden->value = false;
   }

   if (attribute->has(ATTR_LIFETIME)) {
      context.v_lifetime->value = p_per();
   }

   if (attribute->has(ATTR_MODIFICATION)) {
      context.v_modification->value = p_tim();
   }

   if (attribute->has(ATTR_NAME)) {
      context.v_name->value.clear();
   }

   if (attribute->has(ATTR_PARENT)) {
      context.v_parent->value.clear();
   }

   if (attribute->has(ATTR_READONLY)) {
      context.v_readonly->value = false;
   }

   if (attribute->has(ATTR_SIZE) || attribute->has(ATTR_SIZE_FILE_ONLY)) {
      context.v_size->value = P_NaN;
   }
   
   if (attribute->has(ATTR_IMAGE_OR_VIDEO)) {
      context.v_isimage->value = false;
      context.v_isvideo->value = false;
      context.v_width->value = P_NaN;
      context.v_height->value = P_NaN;
      context.v_duration->value = p_per();
   }
}

p_str os_extension(const p_str& value)
{
   for (p_int i = value.size() - 1; i >= 0; i--) {
      const p_char ch = value[i];

      if (ch == CHAR_DOT) {
         if (i == 0 || value[i - 1] == OS_SEPARATOR) {
            return p_str();
         }

         p_str result = value.substr(i + 1);
         str_toLower(result);
         return result;
      }
      else if (ch == OS_SEPARATOR) {
         return p_str();
      }
   }

   return p_str();
}

p_str os_fullname(const p_str& value)
{
   for (p_int i = value.size() - 1; i >= 0; i--) {
      if (value[i] == OS_SEPARATOR) {
         return value.substr(i + 1);
      }
   }

   return value;
}

p_str os_name(const p_str& value)
{
   p_int i = value.size() - 1;
   p_int dot = -1;
   p_bool sep = false;

   for (; i >= 0; i--) {
      const p_char ch = value[i];

      if (dot == -1 && ch == CHAR_DOT) {
         dot = i;
      }
      else if (ch == OS_SEPARATOR) {
         sep = true;
         break;
      }
   }

   if (sep) {
      return dot == -1
         ? value.substr(i + 1)
         : value.substr(i + 1, dot - i - 1);
   }

   return dot == -1
      ? value
      : value.substr(0, dot);
}

p_str os_parent(const p_str& path)
{
   for (p_int i = path.size() - 1; i >= 0; i--) {
      if (path[i] == OS_SEPARATOR) {
         return path.substr(0, i);
      }
   }

   return p_str();
}


p_bool os_attr_isImage(const p_str& path)
{
   const MediaAttributes media = os_ffmpegAttributes(path);
   return media.isImage;
}

p_bool os_attr_isVideo(const p_str& path)
{
   const MediaAttributes media = os_ffmpegAttributes(path);
   return media.isVideo;
}

p_num os_attr_width(const p_str& path)
{
   const MediaAttributes media = os_ffmpegAttributes(path);
   return media.width;
}

p_num os_attr_height(const p_str& path)
{
   const MediaAttributes media = os_ffmpegAttributes(path);
   return media.height;
}

p_per os_attr_duration(const p_str& path)
{
   const MediaAttributes media = os_ffmpegAttributes(path);
   return media.duration;
}

p_str os_attr_extension(const p_str& path)
{
   return os_extension(path);
}

p_str os_attr_fullname(const p_str& path)
{
   return os_fullname(path);
}

p_str os_attr_name(const p_str& path)
{
   return os_name(path);
}

p_str os_attr_parent(const p_str& path)
{
   return os_parent(path);
}

p_bool os_bothAreSeparators(const p_char left, const p_char right)
{
   return (left == right)
      && (left == OS_SEPARATOR);
}

p_str os_softTrim(const p_str& value)
{
   const p_int len = value.size();
   p_int start = 0;

   while (start < len && value[start] == CHAR_SPACE) {
      start++;
   }

   if (start == len) {
      return p_str();
   }

   p_int end = len - 1;

   while (end >= 0 && value[end] == CHAR_SPACE) {
      end--;
   }

   if (start == 0) {
      return end == len - 1
         ? value
         : value.substr(0, end + 1);
   }
   else {
      return end == len - 1
         ? value.substr(start)
         : value.substr(start, end - start + 1);
   }
}

void os_escapeQuote(p_str& path)
{
   const p_size length = path.size();
   if (path[0] == CHAR_QUOTATION_MARK && path[length - 1] == CHAR_QUOTATION_MARK) {
      path = path.substr(1, length - 2);
   }
}

p_str os_makeArg(const p_str& value)
{
   p_bool anySpace = false;
   p_size quotes = 0;

   for (const p_char ch : value) {
      switch (ch) {
         case CHAR_SPACE: {
            anySpace = true;
            break;
         }
         case CHAR_QUOTATION_MARK: {
            quotes++;
            break;
         }
      }
   }

   if (quotes == 0) {
      return anySpace
         ? str(CHAR_QUOTATION_MARK, value, CHAR_QUOTATION_MARK)
         : value;
   }
   else {
      p_str result;
      result.reserve(value.size() + quotes);

      for (const p_char ch : value) {
         if (ch == CHAR_QUOTATION_MARK) {
            result.push_back(CHAR_BACKSLASH);
         }
         result.push_back(ch);
      }

      return anySpace
         ? str(CHAR_QUOTATION_MARK, result, CHAR_QUOTATION_MARK)
         : result;
   }
}

p_str os_quoteEmbraced(const p_str& value)
{
   return value.find(CHAR_SPACE) == p_str::npos
      ? value
      : str(CHAR_QUOTATION_MARK, value, CHAR_QUOTATION_MARK);
}

static p_str os_toWideString(const std::string& str)
{
   std::locale loc("");
   std::vector<p_char> buffer(str.size());
   std::mbstowcs(&buffer[0], str.c_str(), str.size());
   return p_str(buffer.begin(), buffer.end());
}

MediaAttributes os_ffmpegAttributes(const p_str& filePath)
{
   const std::string path = os_toUtf8(filePath);
   AVFormatContext* formatCtx = nullptr;
   MediaAttributes result;

   if (avformat_open_input(&formatCtx, path.c_str(), nullptr, nullptr) != 0) {
      return result;
   }

   if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
      avformat_close_input(&formatCtx);
      return result;
   }

   AVCodecParameters* codecParams = nullptr;
   int videoStreamIndex = -1;
   bool hasAudio = false;
   bool hasVideo = false;

   for (p_size i = 0; i < formatCtx->nb_streams; i++) {
      const auto codecType = formatCtx->streams[i]->codecpar->codec_type;

      if (codecType == AVMEDIA_TYPE_AUDIO) {
         hasAudio = true;
      }
      else if (videoStreamIndex == -1 && codecType == AVMEDIA_TYPE_VIDEO) {
         hasVideo = true;
         videoStreamIndex = i;
         codecParams = formatCtx->streams[i]->codecpar;
      }
   }

   if (! hasVideo) {
      avformat_close_input(&formatCtx);
      return result;
   }

   // we know that this file has width and height
   // now check whether is a video or an image

   if (hasAudio) {
      if (os_isFfmpegVideoFormat(formatCtx->iformat->name)) {
         result.isVideo = true;
         result.width = static_cast<int64_t>(codecParams->width);
         result.height = static_cast<int64_t>(codecParams->height);
         result.duration = os_ffmpegPeriod(formatCtx->duration);
         avformat_close_input(&formatCtx);
         return result;
      }
   }
   else {
      if (os_isFfmpegImageFormat(formatCtx->iformat->name)) {
         result.isImage = true;
         result.width = static_cast<int64_t>(codecParams->width);
         result.height = static_cast<int64_t>(codecParams->height);
         avformat_close_input(&formatCtx);
         return result;
      }
   }
   avformat_close_input(&formatCtx);
   return result;
}


static p_per os_ffmpegPeriod(const int64_t units)
{
   if (units <= 0) {
      return p_per();
   }

   p_tnum seconds = (units + AV_TIME_BASE / 2) / AV_TIME_BASE;

   if (seconds < TNUM_SECONDS_IN_MINUTE) {
      return p_per(seconds, Period::u_Seconds);
   }

   p_per period;
   period.periodType = Period::pt_Difference;

   if (seconds >= TNUM_SECONDS_IN_DAY) {
      period.days = seconds / TNUM_SECONDS_IN_DAY;
      seconds %= TNUM_SECONDS_IN_DAY;

      if (period.days > TNUM_DAYS_IN_FEBRUARY) {
         period.periodType = Period::pt_Mingled;
      }
   }

   if (seconds >= TNUM_SECONDS_IN_HOUR) {
      period.hours = seconds / TNUM_SECONDS_IN_HOUR;
      seconds %= TNUM_SECONDS_IN_HOUR;
   }

   if (seconds >= TNUM_SECONDS_IN_MINUTE) {
      period.minutes = seconds / TNUM_SECONDS_IN_MINUTE;
      seconds %= TNUM_SECONDS_IN_MINUTE;
   }

   period.seconds = seconds;
   return period;
}

static bool os_isFfmpegVideoFormat(const std::string& value)
{
   static const std::unordered_set<std::string> FORMATS_OF_VIDEOS = {
      "vfwcap", "4xm", "3gp", "3g2", "a64", "matroska", "matroska,webm", "ivr",
      "amv", "argo_asf", "argo_brp", "argo_cvg", "asf", "asf_o", "asf_stream", "av1",
      "avi", "avr", "bethsoftvid", "bfi", "bink", "binka", "bmv", "cavsvideo", "cdxl", "m4v",
      "cdg", "cine", "dash", "dcstr", "derf", "dfa", "dhav", "dirac", "dnxhd",
      "dshow", "dsicin", "dv", "dvd", "dvdvideo", "dxa", "ea", "ea_cdata", "evc", "f4v",
      "film_cpk", "flic", "flv", "frm", "gdigrab", "gdv", "gem_pipe", "gxf", "h261", "h263",
      "h264", "hds", "hevc", "hls", "hnm", "idcin", "ifv", "ogv", "mjpeg", "mjpeg_2000", "mp4",
      "mov,mp4,m4a,3gp,3g2,mj2", "nuv", "imf", "ingenient", "ipmovie",
      "ipod", "ipu", "ismv", "iv8", "ivf",  "kux", "live_flv", "luodat", "lvf", "lxf", "mgsts", "mlv", "mm", "mods",
      "moflex", "mov", "mpeg", "mpeg1video", "mpeg2video", "mpegts", "mpegtsraw", "mpegvideo",
      "msnwctcp", "mtaf", "mtv", "mv", "mvi", "mxf", "mxf_d10", "mxf_opatom", "mxg", "nc",
      "nsv", "null", "nut", "obu", "rawvideo", "pdv", "paf", "pmp", "psp", "pva", "r3d", "redspark",
      "rm", "roq", "rpl", "rsd", "rtp", "rtp_mpegts", "rtsp", "sdr2", "ser", "smjpeg", "smk", "smoothstreaming",
      "smush", "sol", "svcd", "svs", "tee", "thp", "tiertexseq", "tmv", "ty", "uncodedframecrc", "v210",
      "v210x", "vc1", "vc1test", "vcd", "vidc", "vividas", "vivo", "vmd", "vob", "vvc", "wady", "wc3movie",
      "webm", "webm_chunk", "webm_dash_manifest", "wsvqa", "wtv", "xmv", "yuv4mpegpipe", "ogg", "psxstr"
   };

   return FORMATS_OF_VIDEOS.find(value) != FORMATS_OF_VIDEOS.end();
}


static bool os_isFfmpegImageFormat(const std::string& value)
{
   static const std::unordered_set<std::string> FORMATS_OF_IMAGES = {
      "adf", "alias_pix", "anm", "apng", "avif", "bmp_pipe", "brender_pix", "dpx_pipe", "exr_pipe",
      "filmstrip", "fits", "gif", "gif_pipe", "hdr_pipe", "ico", "idf", "iff", "jv", "mpjpeg", "msp",
      "pam_pipe", "pbm_pipe", "pcx_pipe", "pfm_pipe", "pgm_pipe", "pgmyuv_pipe", "pgx_pipe", "phm_pipe",
      "photocd_pipe", "pictor_pipe", "png_pipe", "ppm_pipe", "psd_pipe", "qdraw_pipe", "qoi_pipe",
      "sgi_pipe", "svg_pipe", "xbm_pipe", "sga", "sunrast_pipe", "tiff_pipe", "vbn_pipe", "webp_pipe",
      "xpm_pipe", "xwd_pipe", "txd", "webp", "image2", "image2pipe",
      "j2k_pipe", "jpeg_pipe", "jpegls_pipe", "jpegxl_anim",
      "jpegxl_pipe",
   };

   return FORMATS_OF_IMAGES.find(value) != FORMATS_OF_IMAGES.end();
}


}
