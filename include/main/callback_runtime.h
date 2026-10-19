#ifndef MAIN_CALLBACK_RUNTIME_H
#define MAIN_CALLBACK_RUNTIME_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MainTask MainTask;
typedef void (*MainTaskCallback)(MainTask *);
#define MAIN_TASK_CALLBACK_DEFINED 1

/* Layout recovered from the callback-block constructor and runners. Names
 * describe byte offsets; full ownership and callback signatures are unresolved. */
typedef void (*MainRegisteredCallback)(void);
typedef struct MainCallbackBlock {
    unsigned int field_0000;
    unsigned int field_0004;
    unsigned int field_0008;
    unsigned int field_000C;
    unsigned int field_0010;
    unsigned int field_0014;
    MainRegisteredCallback field_0018;
    MainRegisteredCallback field_001C;
    MainRegisteredCallback field_0020;
    signed char field_0024;
    unsigned char field_0025;
    unsigned char field_0026;
    unsigned char padding_0027;
    unsigned int field_0028;
    unsigned int field_002C;
    const void *field_0030;
} MainCallbackBlock;

extern void func_0886503C(MainCallbackBlock *, MainRegisteredCallback,
                         MainRegisteredCallback, MainRegisteredCallback);
extern void func_0886504C(MainCallbackBlock *, unsigned int, unsigned int);
extern MainTask *func_08865088(MainCallbackBlock *, unsigned int, int,
                              MainTaskCallback, const void *);
extern int func_08865158(MainCallbackBlock *);

extern const unsigned char D_089C9348[];
typedef struct MainOverlayModePrefix { signed char mode; } MainOverlayModePrefix;
extern void func_08933ACC(MainOverlayModePrefix *, int);

#ifdef __cplusplus
}
#endif

#endif
