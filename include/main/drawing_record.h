#ifndef MAIN_DRAWING_RECORD_H
#define MAIN_DRAWING_RECORD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Partial drawing-record layout recovered from shared menu service callees.
 * Field offsets are established; complete ownership and flag meanings are not. */
typedef struct MainDrawingRecordPrefix {
    float field_0000;
    float field_0004;
    float field_0008;
    signed short field_000C;
    signed short field_000E;
    float field_0010;
    float field_0014;
    float field_0018;
    float field_001C;
    unsigned short field_0020;
    signed short field_0022;
    void *field_0024;
    unsigned char field_0028[4];
    float *field_002C;
} MainDrawingRecordPrefix;

extern void *func_08935524(void *, const void *, unsigned int);
extern void func_088D9D24(MainDrawingRecordPrefix *);
extern void func_088D9DDC(MainDrawingRecordPrefix *);
extern void func_088D9B84(MainDrawingRecordPrefix *, float *);
extern void func_088D9C60(MainDrawingRecordPrefix *, int, int);
extern void func_088DAAA0(void *, void *);
extern void *func_088DAA40(void *, int, unsigned char);
extern void func_088DA0A0(void *);

typedef struct MainDrawingOwnerPrefix {
    unsigned char field_0000;
    unsigned char unknown_0001[3];
    void *field_0004;
    void *field_0008;
} MainDrawingOwnerPrefix;

/* Separate list prefix consumed by the drawing record allocator. The enabled
 * byte owner above belongs to a different observed interface. */
typedef struct MainDrawingListPrefix {
    MainDrawingRecordPrefix *head;
    MainDrawingRecordPrefix *tail;
    int count;
} MainDrawingListPrefix;
extern MainDrawingRecordPrefix *func_088DAA68(MainDrawingListPrefix *);

#ifdef __cplusplus
}
#endif

#endif
