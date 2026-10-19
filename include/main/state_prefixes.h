#ifndef MAIN_STATE_PREFIXES_H
#define MAIN_STATE_PREFIXES_H

#include "main/callback_runtime.h"

/* These state helpers operate on the same observed callback-block fields.
 * Share the type so callback addresses retain their function-pointer types. */
typedef MainCallbackBlock MainStatePrefix;

typedef struct MainFloatRecordPrefix {
    int field_0000;
    float field_0004;
    float field_0008;
    float field_000C;
} MainFloatRecordPrefix;

#endif
