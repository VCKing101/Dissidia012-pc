#pragma once

// The system's libcurl, opened the first time it is asked for, on Linux (not
// Windows, which has WinHTTP, nor Android, where no app can load it): SteamOS
// and nearly every desktop Linux have it, and a program linked to it would not
// start where it is missing. The update check (d012_updates.cpp) and the
// update's download (d012_update_install.cpp) use it. Only the calls and the
// numbers they need, from libcurl's stable ABI (curl.h).
// MIT licence; see port/README.md.

#if !defined(_WIN32) && !defined(__ANDROID__)

namespace d012::curl {

struct Api {
    void *(*easy_init)() = nullptr;
    int (*easy_setopt)(void *, int, ...) = nullptr;
    int (*easy_perform)(void *) = nullptr;
    int (*easy_getinfo)(void *, int, ...) = nullptr;
    void (*easy_cleanup)(void *) = nullptr;
    const char *(*easy_strerror)(int) = nullptr;
    void *(*slist_append)(void *, const char *) = nullptr;
    void (*slist_free_all)(void *) = nullptr;
};

// Options (CURLOPT_*).
inline constexpr int kOptWriteData = 10001;
inline constexpr int kOptUrl = 10002;
inline constexpr int kOptUserAgent = 10018;
inline constexpr int kOptHttpHeader = 10023;
inline constexpr int kOptWriteFunction = 20011;
inline constexpr int kOptTimeout = 13;
inline constexpr int kOptLowSpeedLimit = 19;
inline constexpr int kOptLowSpeedTime = 20;
inline constexpr int kOptFailOnError = 45;
inline constexpr int kOptFollowLocation = 52;
inline constexpr int kOptConnectTimeout = 78;
inline constexpr int kOptNoSignal = 99;
inline constexpr int kOptProtocols = 181;
inline constexpr int kOptRedirectProtocols = 182;
inline constexpr int kOptResumeFromLarge = 30116;  // a curl_off_t
// CURLPROTO_HTTPS.
inline constexpr long kProtocolHttps = 1L << 1;
// Information (CURLINFO_*).
inline constexpr int kInfoResponseCode = 0x200002;
// Results (CURLcode).
inline constexpr int kWriteError = 23;

// Null when this system has no libcurl.
[[nodiscard]] const Api *api();

} // namespace d012::curl

#endif
