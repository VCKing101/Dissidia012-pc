#ifndef MAIN_PRESENTATION_CHANNEL_H
#define MAIN_PRESENTATION_CHANNEL_H
#ifdef __cplusplus
extern "C" {
#endif
/* Observed 16-byte vectors, 32-byte pending records and 0x240 channel stride.
 * Unknown words remain opaque; original class names and channel ownership
 * have not been established. Index wrappers preserve the native valid-index
 * precondition and add no bounds checks. */
typedef void (*MainPresentationCallback)(int);

typedef struct MainPresentationVector { float values[4]; } MainPresentationVector;
typedef struct MainPresentationEvent {
    MainPresentationVector value;
    float field_0010;
    MainPresentationCallback field_0014;
    int field_0018;
    unsigned char unknown_001C[4];
} MainPresentationEvent;
typedef struct MainPresentationChannel {
    MainPresentationVector value;
    MainPresentationVector interpolated_0010;
    MainPresentationEvent events_0020[16];
    int field_0220;
    float field_0224;
    float field_0228;
    float field_022C;
    int field_0230;
    int field_0234;
    unsigned char field_0238;
    unsigned char field_0239;
    unsigned char unknown_023A[6];
} MainPresentationChannel;
extern MainPresentationChannel D_09B47B50[];
extern const MainPresentationVector D_09B48210[];
extern MainPresentationVector *func_089A3868(MainPresentationVector *,const float *);
extern MainPresentationVector *func_08873B18(MainPresentationVector *,const float *);
extern int func_08873B64(MainPresentationChannel *,const float *,MainPresentationCallback,int,float);
extern int func_08873C20(MainPresentationChannel *,int,MainPresentationCallback,int,float);
extern MainPresentationChannel *func_08873CE0(int,unsigned char,int);
extern MainPresentationChannel *func_08873D34(unsigned int,MainPresentationCallback,int,unsigned char,int,float);
extern MainPresentationVector *func_08873B34(MainPresentationVector *,int);
extern void func_08873B5C(MainPresentationChannel *);
extern MainPresentationVector *func_08873DB0(const float *,int);
extern MainPresentationChannel *func_0887391C(MainPresentationChannel *,int,unsigned char);
extern void func_08874350(MainPresentationChannel *,unsigned int,MainPresentationCallback,int,float);
extern int func_088D4C5C(void);
#ifdef __cplusplus
}
#endif
#endif
