/* Shared structures for agent21's matches in main. */
#ifndef AGENT21_TYPES_H
#define AGENT21_TYPES_H

#ifdef __cplusplus
/* Observer told about the save-data flow's progress (vtable entries are
   {short delta, fn} pairs; virtual N sits at offset 8 * (N + 1)). */
struct FlowListener {
    virtual void onBegin();      /* 0x08 */
    virtual void onReset();      /* 0x10 */
    virtual void v2();           /* 0x18 */
    virtual void onUpdate();     /* 0x20 */
    virtual void onFailed();     /* 0x28 */
    virtual void onFinished();   /* 0x30 */
    virtual void v6();           /* 0x38 */
    virtual void v7();           /* 0x40 */
    virtual void onClosed();     /* 0x48 */
};

/* Client object the flow reports to (field_1BC); its vptr follows one word. */
struct FlowClient {
    int field_00;
    virtual int v0();
    virtual int onProgress(void *request, int arg, int a, int b);   /* 0x10 */
    virtual int onIdle(void *request);                              /* 0x18 */
    virtual int v3();
    virtual int onComplete(void *request, int a, int b);            /* 0x28 */
    virtual int onOpen(void *request);                              /* 0x30 */
    virtual int onClose(void *request);                             /* 0x38 */
    virtual int isBusy(void *request);                              /* 0x40 */
    virtual int v8();
    virtual int onSelect(int arg);                                  /* 0x50 */
    virtual int v10();
    virtual int onConfirm(int arg);                                 /* 0x60 */
    virtual int v12(); virtual int v13();
    virtual int onCancel(int arg);                                  /* 0x78 */
    virtual int onRetry(int arg);                                   /* 0x80 */
    virtual int v16(); virtual int v17();
    virtual int onError(int arg);                                   /* 0x98 */
    virtual int onWarning(int arg);                                 /* 0xA0 */
    virtual int query20(int arg);                                   /* 0xA8 */
    virtual int query21();                                          /* 0xB0 */
    virtual int query22(int arg);                                   /* 0xB8 */
    virtual int query23();                                          /* 0xC0 */
};

/* Second observer (field_1B4); vptr at 0. */
struct FlowSink {
    virtual void onShow();                                          /* 0x08 */
    virtual void onHide();                                          /* 0x10 */
    virtual void v2();
    virtual void onSelect(int arg);                                 /* 0x20 */
    virtual void onNotify(int arg);                                 /* 0x28 */
    virtual void onConfirm(int arg);                                /* 0x30 */
    virtual void v6(); virtual void v7();
    virtual void onCancel(int arg);                                 /* 0x48 */
    virtual void onRetry(int arg);                                  /* 0x50 */
    virtual void v10(); virtual void v11();
    virtual void onError(int arg);                                  /* 0x68 */
    virtual void onWarning(int arg);                                /* 0x70 */
    virtual void setActive(int on);                                 /* 0x78 */
    virtual void onMessage(int arg);                                /* 0x80 */
};
#else
struct FlowListener;
struct FlowClient;
struct FlowSink;
#endif

/* Bound callback: a handler table and the object it acts on. */
typedef struct FlowCallback {
    void *handlers;
    void *owner;
} FlowCallback;

/* Save-data flow object (sub-request at +4, state at 0x190). */
typedef struct DataFlow {
    int field_000;
    char request[0x18C];            /* 0x004, driven by func_08903xxx */
    int state;                      /* 0x190 */
    int field_194;
    int field_198;
    int field_19C;
    FlowCallback onDone;            /* 0x1A0 */
    int field_1A8;
    FlowCallback onError;           /* 0x1AC */
    struct FlowSink *sink;          /* 0x1B4 */
    int field_1B8;
    struct FlowClient *client;      /* 0x1BC */
    char pad_1C0[0x139F8 - 0x1C0];
    char files[0x14C58 - 0x139F8];  /* 0x139F8 */
    unsigned char field_14C58;
    char pad_14C59[0x14C64 - 0x14C59];
    struct FlowListener *listener;  /* 0x14C64 */
} DataFlow;

/* Queued save request (0x14 bytes). */
typedef struct SaveRequest {
    int index;
    int field_04;
    int arg0;
    int arg1;
    unsigned char active;
} SaveRequest;

typedef struct SaveEntry {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
} SaveEntry;

/* Save queue: 16 entries, the current one, two pending requests. */
typedef struct SaveQueue {
    char pad_000[8];
    SaveEntry entries[16];          /* 0x008 */
    int field_108;
    int current;                    /* 0x10C */
    SaveRequest pending;            /* 0x110 */
    SaveRequest opening;            /* 0x124 */
} SaveQueue;

typedef struct BoardSlot { char data[0x50]; int value; } BoardSlot;       /* 0x54 */
typedef struct BoardItem { char data[0x3C]; int time; char pad[0xC]; } BoardItem; /* 0x4C */

/* Board with two slots and eight items (constructor func_0890C384). */
typedef struct Board {
    char base[0x10];
    union {
        unsigned int word;
        struct {
            unsigned int visible : 1;
            unsigned int active : 1;
        } bits;
    } flags;                        /* 0x10 */
    int field_14;
    int field_18;
    char pad_1C[0x40 - 0x1C];
    int field_40;
    int selected;                   /* 0x44 */
    int time;                       /* 0x48 */
    BoardSlot slots[2];             /* 0x04C */
    BoardItem items[8];             /* 0x0F4 */
    int slotCount;                  /* 0x354 */
    int itemCount;                  /* 0x358 */
} Board;

typedef struct SceneActor {
    char pad_000[0x126];
    unsigned char hidden;           /* 0x126 */
} SceneActor;

#ifdef __cplusplus
/* Fighter object: data first, vptr at 0x544. */
struct ScenePlayerBase {
    char pad_000[0x80];
    float pos[3];                   /* 0x080 */
    char pad_08C[0x544 - 0x8C];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void release();         /* 0xF8 */
};
struct ScenePlayer : ScenePlayerBase {
    char pad_548[0x5AC - 0x548];
    SceneActor *actor;              /* 0x5AC */
    char pad_5B0[0xBA0 - 0x5B0];
    char timer[0xC];                /* 0xBA0 */
    unsigned char timerDone;        /* 0xBAC */
};
#else
typedef struct ScenePlayer {
    char pad_000[0x80];
    float pos[3];                   /* 0x080 */
    char pad_08C[0x5AC - 0x8C];
    SceneActor *actor;              /* 0x5AC */
    char pad_5B0[0xBA0 - 0x5B0];
    char timer[0xC];                /* 0xBA0 */
    unsigned char timerDone;        /* 0xBAC */
} ScenePlayer;
#endif

typedef struct SceneInfo {
    char pad_00[6];
    unsigned char bgm;              /* 0x06 */
} SceneInfo;

/* Scene driver: state machine at 0x68 with a timer at 0x6C. */
typedef struct Scene {
    char pad_000[0x48];
    int field_048;
    char pad_04C[0x54 - 0x4C];
    struct SceneInfo *info;         /* 0x054 */
    char pad_058[0x60 - 0x58];
    int field_060;
    int field_064;
    int state;                      /* 0x068 */
    float timer;                    /* 0x06C */
    char pad_070[0x88 - 0x70];
    int field_088;
    int field_08C;
    char pad_090[0x570 - 0x90];
    int field_570;
    char pad_574[0x6C8 - 0x574];
    void *heap;                     /* 0x6C8 */
    int field_6CC;
    struct ScenePlayer *player;     /* 0x6D0 */
    char pad_6D4[0x6F4 - 0x6D4];
    int handles[4];                 /* 0x6F4 */
    struct ScenePlayer *units[4];   /* 0x704 */
    char pad_714[0x720 - 0x714];
    char lock[0x744 - 0x720];       /* 0x720 */
    int sounds;                     /* 0x744 */
    char pad_748[0x758 - 0x748];
    int bgmId;                      /* 0x758 */
    int bgmHandle;                  /* 0x75C */
    char pad_760[0x7CC - 0x760];
    int field_7CC;
    float field_7D0;
    char pad_7D4[0x7E3 - 0x7D4];
    unsigned char started;          /* 0x7E3 */
    unsigned char field_7E4;
    unsigned char waiting;          /* 0x7E5 */
    unsigned char field_7E6;
    unsigned char field_7E7;
    unsigned char paused;           /* 0x7E8 */
    unsigned char field_7E9;
} Scene;

#endif
