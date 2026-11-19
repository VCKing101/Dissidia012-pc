// The system's libcurl (d012_curl.hpp). MIT licence; see port/README.md.

#include "d012_curl.hpp"

#if !defined(_WIN32) && !defined(__ANDROID__)

#include <dlfcn.h>

#include <iostream>
#include <type_traits>

namespace d012::curl {

const Api *api() {
    static const Api loaded = [] {
        Api c;
        void *lib = nullptr;
        for (const char *name : {"libcurl.so.4", "libcurl-gnutls.so.4", "libcurl.4.dylib"})
            if ((lib = dlopen(name, RTLD_NOW | RTLD_LOCAL)) != nullptr) break;
        if (lib == nullptr) {
            std::cout << "[updates] no libcurl on this system: " << dlerror() << std::endl;
            return c;
        }
        const auto get = [&](auto &slot, const char *name) {
            slot = reinterpret_cast<std::remove_reference_t<decltype(slot)>>(dlsym(lib, name));
        };
        get(c.easy_init, "curl_easy_init");
        get(c.easy_setopt, "curl_easy_setopt");
        get(c.easy_perform, "curl_easy_perform");
        get(c.easy_getinfo, "curl_easy_getinfo");
        get(c.easy_cleanup, "curl_easy_cleanup");
        get(c.easy_strerror, "curl_easy_strerror");
        get(c.slist_append, "curl_slist_append");
        get(c.slist_free_all, "curl_slist_free_all");
        return c;
    }();
    const Api &c = loaded;
    const bool complete = c.easy_init && c.easy_setopt && c.easy_perform && c.easy_getinfo && c.easy_cleanup &&
                          c.easy_strerror && c.slist_append && c.slist_free_all;
    return complete ? &c : nullptr;
}

} // namespace d012::curl

#endif
