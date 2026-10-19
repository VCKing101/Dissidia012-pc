#ifndef DISSIDIA_EXTRA_DATA_INSTALL_H
#define DISSIDIA_EXTRA_DATA_INSTALL_H

typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;
typedef unsigned long long u64;

enum {
    DATA_INSTALL_POWER_SUSPENDING = 0x00010000,
    DATA_INSTALL_POWER_RESUME_COMPLETE = 0x00040000
};

#define DATA_INSTALL_ERROR_NOT_FOUND 0x80010002U

/* Partial state layout inferred from the extra overlay. Unknown fields and
 * meanings remain unnamed; this declaration does not establish the full size. */
typedef struct DataInstallState {
    u8 unknown_0000[0x600];
    u32 buffer_address;        /* 0x600: PSP address */
    u32 buffer_capacity_bytes; /* 0x604 */
    u64 capacity_kib;  /* 0x608 */
    u64 available_kib; /* 0x610; capacity query reserves two clusters */
    u64 transfer_offset; /* 0x618: bytes */
    u32 fat_callback;   /* 0x620 */
    u32 power_callback; /* 0x624 */
    u32 entries[0xFD2]; /* 0x628: PSP addresses, capacity 4050 */
    u32 entry_count;    /* 0x4570 */
    u32 current_entry_index; /* 0x4574 */
    u32 field_4578;
    u32 field_457C;
    u32 field_4580[4];
    u32 field_4590;
    u32 field_4594;
    u32 field_4598;
    u8 flag_459C;
    u8 flag_459D;
    u8 flag_459E;
    u8 flag_459F;
    u8 unknown_45A0;
    u8 flag_45A1;
    u8 unknown_45A2[6]; /* Initialization clears 0x45A8 bytes. */
} DataInstallState;

typedef struct DataInstallOwner {
    DataInstallState *state;
} DataInstallOwner;

#endif
