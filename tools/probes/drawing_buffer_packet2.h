#ifndef DRAWING_BUFFER_PACKET2_H
#define DRAWING_BUFFER_PACKET2_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct DrawingPacket2Info {
    short key_0000;
    unsigned char unknown_0002[0x0B];
    signed char field_000D;
    unsigned char unknown_000E[0x0E];
    char path_001C[0x10];
} DrawingPacket2Info;
typedef struct DrawingPacket2Buffer {
    int field_0000;
    DrawingPacket2Info *field_0004;
    void *field_0008;
    int field_000C;
} DrawingPacket2Buffer;
typedef struct DrawingPacket2Count {
    unsigned char unknown_0000[8];
    int count_0008;
} DrawingPacket2Count;
typedef struct DrawingPacket2Context {
    unsigned char unknown_0000[4];
    DrawingPacket2Count *field_0004;
    DrawingPacket2Info *field_0008;
} DrawingPacket2Context;

extern void *D_089D6C30;
extern DrawingPacket2Info *func_0889F3E4(void *, int);
extern int func_0889F5E8(DrawingPacket2Context *, int);
extern int func_0893476C(char *, const char *, ...);
extern const char D_089CC0A4[];
extern int func_08862C40(const char *, int);
extern int func_08862854(const char *, void *, int, int);
extern int func_0889F290(void *, void *, int);
extern int func_0889F2D4(void *, int);
extern void func_0889F340(void *, int);
extern int func_0889F35C(void *);
extern int func_0889F2B8(void *);
extern void func_0885C538(void *, void *, int);
extern int func_0889F6A0(void *, int, unsigned int, unsigned int);
extern int func_0889F704(void *);
extern void func_0889F750(void *, int, int, int, float);
extern void func_0889F830(void *, int);
extern int func_0889F980(void *);

#ifdef __cplusplus
}
#endif
#endif
