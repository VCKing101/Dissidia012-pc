#ifndef MAIN_AGENT2_STREAM_H
#define MAIN_AGENT2_STREAM_H

/* C++ object whose SNC vtable pointer sits at +0x154. Each vtable entry is
 * {short this-adjustment, short index, function}; virtual number N lives at
 * vtable offset 8 * (N + 1). Slots are named by that offset. */
typedef void (*Agent2StreamCallback)(void);

class Agent2Stream {
public:
    unsigned char unknown_0000[0x8];
    int field_0008;
    int field_000C;
    unsigned char unknown_0010[0x4];
    int field_0014;
    int field_0018;
    unsigned char unknown_001C[0x4];
    int field_0020;
    int field_0024;
    int field_0028;
    int field_002C;
    int field_0030;
    int field_0034;
    int field_0038;
    int field_003C;
    long long field_0040;
    unsigned char unknown_0048[0x100];
    unsigned char field_0148;
    unsigned char field_0149;
    unsigned char unknown_014A[0xA];

    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void slot_028();
    virtual void slot_030(char *name, int first, int second, int third);
    virtual void slot_038();
    virtual void slot_040();
    virtual void slot_048();
    virtual void slot_050();
    virtual void slot_058(char *name, int first, int second, int third,
                          int fourth, int fifth);
    virtual void slot_060();
    virtual void slot_068();
    virtual void slot_070();
    virtual void slot_078(char *name, int mode);
    virtual void slot_080();
    virtual void slot_088();
    virtual void slot_090();
    virtual void slot_098();
    virtual void slot_0A0(char *name, int first, int second, int third);
    virtual void slot_0A8();
    virtual void slot_0B0();
    virtual void slot_0B8();
    virtual void slot_0C0(char *name, int mode);
    virtual void slot_0C8();
    virtual void slot_0D0();
    virtual int slot_0D8(char *name, void *callback, void *argument,
                         int mode);
    virtual void slot_0E0();
    virtual void slot_0E8();
    virtual void slot_0F0();
    virtual void slot_0F8();
    virtual void slot_100();
    virtual void slot_108(char *name, int id);
};

#endif
