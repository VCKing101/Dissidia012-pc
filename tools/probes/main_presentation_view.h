#ifndef MAIN_PRESENTATION_PROBE_H
#define MAIN_PRESENTATION_PROBE_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct MainPresentationVector { float x,y,z,w; } MainPresentationVector;
typedef struct MainPresentationEvent {
    MainPresentationVector value;
    float field_0010;
    const void *field_0014;
    int field_0018;
    unsigned char unknown_001C[4];
} MainPresentationEvent;
typedef struct MainPresentationChannel {
    MainPresentationVector value;
    unsigned char unknown_0010[0x10];
    MainPresentationEvent events_0020[16];
    int field_0220;
    unsigned char unknown_0224[8];
    float field_022C;
    unsigned char unknown_0230[4];
    int field_0234;
    unsigned char unknown_0238[8];
} MainPresentationChannel;
extern MainPresentationChannel D_09B47B50[];
extern const MainPresentationVector D_09B48210[];
extern MainPresentationVector *func_089A3868(MainPresentationVector *,const MainPresentationVector *);
extern MainPresentationVector *func_08873B18(MainPresentationVector *,const MainPresentationVector *);
extern int func_08873B64(MainPresentationChannel *,const MainPresentationVector *,const void *,int,float);
extern int func_08873C20(MainPresentationChannel *,int,const void *,int,float);
extern void func_0887391C(MainPresentationChannel *,int,unsigned char);
extern void func_08874350(MainPresentationChannel *,unsigned int,const void *,int,float);
#ifdef __cplusplus
}
#endif
#endif
