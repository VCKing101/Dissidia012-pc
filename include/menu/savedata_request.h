#ifndef MENU_SAVEDATA_REQUEST_H
#define MENU_SAVEDATA_REQUEST_H

/* Observed 32-bit PSP savedata request fields. Offsets agree with PSPSDK's
 * firmware >= 2.00 API layout; opaque regions remain unmodeled.
 * https://github.com/pspdev/pspsdk/blob/master/src/utility/psputility_savedata.h */
/* Native ScePspDateTime layout used by the SDK slot list and RTC conversion.
 * https://github.com/pspdev/pspsdk/blob/master/src/base/psptypes.h */
typedef struct MenuSaveTimestamp {
    unsigned short year;
    unsigned short month;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned int microsecond;
} MenuSaveTimestamp;

typedef struct MenuSaveSlotEntry {
    int mode;
    MenuSaveTimestamp created;
    MenuSaveTimestamp accessed;
    MenuSaveTimestamp modified;
    char name[20];
} MenuSaveSlotEntry;

typedef struct MenuSaveSlotList {
    int capacity;
    int count;
    MenuSaveSlotEntry *entries;
} MenuSaveSlotList;

typedef struct MenuSavedataRequest {
    unsigned char common[0x30];
    int mode;
    int bind;
    int overwrite;
    char game_name[13];
    unsigned char reserved_0049[3];
    char save_name[20];
    char (*name_list)[20];
    char file_name[13];
    unsigned char reserved_0071[3];
    void *data;
    unsigned int data_capacity;
    unsigned int data_size;
    unsigned char unknown_0080[0x548];
    int focus;
    unsigned char unknown_05CC[0x10];
    unsigned char key[16];
    unsigned int secure_version;
    int multi_status;
    MenuSaveSlotList *id_list;
    unsigned char unknown_05F8[8];
} MenuSavedataRequest;

#endif
