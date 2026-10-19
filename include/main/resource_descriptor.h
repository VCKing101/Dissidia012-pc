#ifndef MAIN_RESOURCE_DESCRIPTOR_H
#define MAIN_RESOURCE_DESCRIPTOR_H

/* Observed resource-handle fields only; these are partial object layouts. */
typedef struct MainResourceHandlePrefix {
    int field_0000;
    unsigned int field_0004;
    float field_0008;
    float field_000C;
    float field_0010;
    unsigned int field_0014;
    unsigned int field_0018;
    unsigned int field_001C;
} MainResourceHandlePrefix;

typedef struct MainDescriptorPrefix {
    signed char field_0000;
    unsigned char field_0001;
    unsigned char field_0002;
    unsigned char field_0003;
    unsigned short field_0004;
    unsigned short field_0006;
    unsigned short field_0008;
    unsigned char unknown_000A[2];
    unsigned int field_000C;
    unsigned char subobject_0010[0x766];
} MainDescriptorPrefix;

extern void *D_089D6C88;
extern void func_08992F04(void *);
extern int func_088A1380(void *, int, unsigned int, unsigned int, int);
extern int func_088A13E4(void *, int);
extern int func_088A1470(void *, int, unsigned char);
extern int func_088A1540(void *, int);
extern int func_088A1578(void *, int);
extern int func_088A159C(void *, int);
extern int func_0889F980(MainResourceHandlePrefix *);
extern void func_088B7E30(void *);
extern void func_088B83F0(void *);
extern const signed short D_089CBCC8[];

#endif
