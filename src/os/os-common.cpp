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
      os_ffmpegEmpty(context);
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

void os_ffmpegAttributes(FileContext& context)
{
   const std::string path = os_toUtf8(context.v_path->value);
   AVFormatContext* formatCtx = nullptr;

   if (avformat_open_input(&formatCtx, path.c_str(), nullptr, nullptr) != 0) {
      os_ffmpegEmpty(context);
      return;
   }

   if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
      avformat_close_input(&formatCtx);
      os_ffmpegEmpty(context);
      return;
   }

   AVCodecParameters* codecParams = nullptr;
   int videoStreamIndex = -1;
   bool hasAudio = false;
   bool hasVideo = false;
   bool hasAdditionalStream = false;

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

      if (codecType != AVMEDIA_TYPE_VIDEO) {
         hasAdditionalStream = true;
      }
   }

   if (! hasVideo) {
      avformat_close_input(&formatCtx);
      os_ffmpegEmpty(context);
      return;
   }

   // we know that this file has width and height
   // now check whether is a video

   context.v_width->value = static_cast<int64_t>(codecParams->width);
   context.v_height->value = static_cast<int64_t>(codecParams->height);
   
   if (hasAudio || hasAdditionalStream || os_isFfmpegVideoFormat(formatCtx->iformat->name)) {
      context.v_isvideo->value = true;
      context.v_isimage->value = false;
      context.v_duration->value = os_ffmpegPeriod(formatCtx->duration);

      avformat_close_input(&formatCtx);
      return;
   }

   if (! os_isAnyFfmpegFormat(formatCtx->iformat->name)) {
      avformat_close_input(&formatCtx);
      os_ffmpegEmpty(context);
      return;
   }

   context.v_isvideo->value = false;
   context.v_isimage->value = true;
   context.v_duration->value = p_per();

   avformat_close_input(&formatCtx);
   return;
}

void os_ffmpegEmpty(FileContext& context)
{
   context.v_isimage->value = false;
   context.v_isvideo->value = false;
   
   context.v_duration->value = p_per();
   context.v_height->value = P_NaN;
   context.v_width->value = P_NaN;
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
   // the ambiguity is still here
   // after all these checks, file can be an image or a video
   // this function is called in the last resort
   // formats below were taken from documentation of FFmpeg
   // they mean things that are video or audio/video

   static const std::unordered_set<std::string> FORMATS_OF_NOT_IMAGES = {
      "3g2", "3gp", "4xm", "a64", "alias_pix", "amv", "anm", "argo_asf", 
      "argo_brp", "argo_cvg", "asf", "asf_o", "asf_stream", "av1", "avi", 
      "avisynth", "avm2", "avs", "avs2", "avs3", "bethsoftvid", "bfi", 
      "bink", "binka", "bit", "bitpacked", "bmv", "brender_pix", "c93", 
      "cavsvideo", "cdxl", "cine", "dash", "derf", "dhav", "dirac", "dnxhd", 
      "dshow", "dv", "dvd", "dvdvideo", "dxa", "evc", "f4v", "film_cpk", "filmstrip", 
      "flic", "flv", "frm", "fwse", "gdigrab", "gdv", "gem_pipe", "h261", "h263", 
      "h264", "gxf", "hcom", "hds", "hevc", "hls", "idcin", "ifv", "ingenient", 
      "ipmovie", "ipod", "ipu", "ismv", "iv8", "ivf", "jv", "kux", "live_flv", "lvf", 
      "lxf", "matroska", "matroska,webm", "m4v", "mgsts", "mjpeg", "mjpeg_2000", "mlv", 
      "moflex", "mov", "mpeg1video", "mpeg2video", "mpegts", "mpegtsraw", "mpegvideo", 
      "mpjpeg", "mtv", "mv", "mvi", "mxg", "mov", "mp4", "mpeg", "nsv", "nuv", "pdv", 
      "nut", "ogg", "pva", "r3d", "rawvideo", "rl2", "roq", "rm", "sami", "sap", "sbc", 
      "sbg", "scc", "scd", "sdl,sdl2", "sdns", "sdp", "sdr2", "sds", "sdx", "segment", 
      "ser", "sga", "sgi_pipe", "shn", "siff", "simbiosis_imx", "sln", "smjpeg", "smk", 
      "smoothstreaming", "smush", "sol", "sox", "spdif", "spx", "srt", "stl", 
      "stream_segment,ssegment", "streamhash", "subviewer", "subviewer1", "sunrast_pipe", 
      "sup", "svag", "svcd", "svg_pipe", "svs", "swf", "sga", "smjpeg", "smk", "svcd", 
      "swf", "thp", "tiertexseq", "tmv", "usm", "v210", "v210x", "vc1", "vc1test", "vcd", 
      "vfwcap", "vidc", "vividas", "vivo", "vmd", "vob", "vplayer", "vvc", "wc3movie", 
      "webm", "webm_chunk", "webm_dash_manifest", "wsvqa", 
      "wtv", "wve", "xmv", "yop", "yuv4mpegpipe" 
   };

   return FORMATS_OF_NOT_IMAGES.find(value) != FORMATS_OF_NOT_IMAGES.end();
}

static bool os_isAnyFfmpegFormat(const std::string& value)
{
   static const std::unordered_set<std::string> ALL_FORMATS = {
      "3g2", "3gp", "4xm", "a64", "aa", "aac", "aax", "ac3", "ac4", "ace", "acm", "act", 
      "adf", "adp", "ads", "adts", "adx", "aea", "afc", "aiff", "aix", "alaw", "alias_pix", 
      "alp", "amr", "amrnb", "amrwb", "amv", "anm", "apac", "apc", "ape", "apm", "apng", 
      "aptx", "aptx_hd", "aqtitle", "argo_asf", "argo_brp", "argo_cvg", "asf", "asf_o", 
      "asf_stream", "ast", "au", "av1", "avi", "avif", "avisynth", "avm2", "avr", 
      "avs", "avs2", "avs3", "bethsoftvid", "bfi", "bfstm", "bink", "binka", "bit", 
      "bitpacked", "bmp_pipe", "bmv", "boa", "bonk", "brender_pix", "brstm", "c93", "caf", 
      "cavsvideo", "cdxl", "chromaprint", "cine", "codec2", "codec2raw",
      "cri_pipe", "dash", "daud", "dcstr", "dds_pipe", "derf", "dfa", "dfpwm", 
      "dhav", "dirac", "dnxhd", "dpx_pipe", "dsf", "dshow", "dsicin", "dss", "dts", "dtshd", 
      "dv", "dvd", "dvdvideo", "dxa", "ea", "ea_cdata", "eac3", "epaf", 
      "evc", "exr_pipe", "f32be", "f32le", "f4v", "f64be", "f64le",
      "film_cpk", "filmstrip", "fits", "flac", "flic", "flv",
      "frm", "fsb", "fwse", "g722", "g723_1", "g726", "g726le", "g729", 
      "gdigrab", "gdv", "gem_pipe", "genh", "gif", "gif_pipe", "gsm", "h261", 
      "h263", "h264", "hca", "hcom", "hdr_pipe", "hds", "hevc", "hls", 
      "iamf", "ico", "idcin", "idf", "ifv", "ilbc", "image2", "image2pipe", 
      "imf", "ingenient", "ipmovie", "ipod", "ipu", "ircam", "ismv", "iv8", 
      "ivf", "ivr", "j2k_pipe", "jpeg_pipe", "jpegls_pipe", "jpegxl_anim", 
      "jpegxl_pipe", "jv", "kux", "kvag", "laf", "latm", "lc3", "libgme", 
      "libopenmpt", "live_flv", "lmlm4", "loas", "lvf", "lxf", "m4v", 
      "matroska", "matroska,webm", "mca", "mgsts", "mjpeg", 
      "mjpeg_2000", "mkvtimestamp_v2", "mlp", "mlv", "mmf", "mods", "moflex", 
      "mp2", "mp3", "mpc", "mpc8", 
      "mpeg1video", "mpeg2video", "mpegts", "mpegtsraw", "mpegvideo", "mpjpeg",
      "msp", "mtaf", "mtv", "mulaw", "musx", "mv", "mvi", 
      "mxf", "mxf_d10", "mxf_opatom", "mxg", "nsv", "null", 
      "nuv", "oga", "ogv", "oma", "openal", "opus",  "paf", 
      "pam_pipe", "pbm_pipe", "pcx_pipe", "pdv", "pfm_pipe", "pgm_pipe", "pgmyuv_pipe", 
      "pgx_pipe", "phm_pipe", "photocd_pipe", "pictor_pipe", "png_pipe", 
      "pp_bnk", "ppm_pipe", "psd_pipe", "psp", "psxstr", "pva", "pvf", "qcp", "qdraw_pipe", 
      "qoa", "qoi_pipe", "r3d", "rawvideo", "rka", "rl2", 
      "roq", "rpl", "rtp_mpegts", "s16be", "s16le", 
      "s24be", "s24le", "s32be", "s32le", "s337m", "s8", "sap", "sbc",
      "sdns", "sdr2", "sds", "sdx",
      "sga", "sgi_pipe", "shn", "sln", "smjpeg", "smk", 
      "smoothstreaming", "sox", "spdif", "spx", 
      "sunrast_pipe", 
      "svag", "svcd", "svg_pipe", "svs", "swf", "tak", "thp", 
      "tiertexseq", "tiff_pipe", "tmv", "truehd", "tta", "ty", "u16be", 
      "u16le", "u24be", "u24le", "u32be", "u32le", "u8", "usm", "v210", 
      "v210x", "vag", "vc1", "vc1test", "vcd", "vfwcap", "vidc", "vividas", 
      "vivo", "vmd", "vob", "voc", "vplayer", "vqf", "vvc", "w64",
      "wav", "wc3movie", "webm", "webm_chunk", "webm_dash_manifest", "webp", 
      "webp_pipe",  "wsaud", "wsvqa", "wtv", "wv", "wve", "xa",
      "xbm_pipe", "xmv", "xpm_pipe", "xvag", "xwd_pipe", "xwma", "yop", "yuv4mpegpipe"
   };
   
   return ALL_FORMATS.find(value) != ALL_FORMATS.end();
}

}
