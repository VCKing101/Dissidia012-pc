/* Shared types for the Sony SDK library code at main 0x089AC568-0x089C26E4
 * (built with Sony psp-gcc). Field names are provisional: offsets come from
 * the retail instructions. */
#ifndef MAIN_SDK_GCC2_TYPES_H
#define MAIN_SDK_GCC2_TYPES_H

/* Library heap: func_089AB938 allocates, func_089ABF70 frees. */
extern void *func_089AB938(void *pool, int flags, int align, int size);
extern void func_089ABF70(int pool, void *p);
typedef void (*SdkElemFunc)(void *elem);
extern void *func_089ABAE8(void *pool, int flags, int align, int elem_size, int count, SdkElemFunc init);

/* Reference-counted resource header shared by the model's element arrays. */
typedef struct SdkRes16 {
    unsigned short ref;
    unsigned short pad2;
    void *data;
    void *f8;
    unsigned short fc;
    unsigned short pade;
} SdkRes16;

/* Model object destroyed by func_089ACD20. */
typedef struct SdkModel {
    unsigned short ref;
    unsigned short pad2;
    int f4;
    int f8;
    void *arr_c;
    void *arr_10;
    void *arr_14;
    void *arr_18;
    void *arr_1c;
    unsigned short n_20;
    unsigned short n_22;
    unsigned short n_24;
    unsigned short n_26;
    unsigned short n_28;
    unsigned short pad2a;
    int f2c[6];
    void *buf_44;
    void *buf_48;
    int f4c[2];
    void *buf_54;
} SdkModel;

typedef struct SdkVec2 { float x, y; } SdkVec2;
typedef struct SdkVec3 { float x, y, z; } SdkVec3;
typedef struct SdkVec4 { float x, y, z, w; } SdkVec4;

#endif
