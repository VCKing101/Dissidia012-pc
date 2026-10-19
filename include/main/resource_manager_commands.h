#ifndef MAIN_RESOURCE_MANAGER_COMMANDS_H
#define MAIN_RESOURCE_MANAGER_COMMANDS_H

/* External command tokens are identified by address; their semantic labels
 * and the complete manager/object types are not yet recovered. */
extern unsigned int D_089D08E0;
extern unsigned int D_089D08E4;
extern unsigned int D_089D08E8;
extern unsigned int D_089D08EC;
extern unsigned int D_089D08F8;
extern unsigned int D_089D08FC;

extern int func_0899016C(unsigned int, unsigned int, int, unsigned char);
extern int func_0899029C(int);
extern int func_0899036C(int);
extern int func_089904B0(int);
extern int func_08991660(int, unsigned int, int, unsigned int);
extern float func_088A1C38(void *);

typedef struct MainManagerSlotPrefix {
    unsigned char unknown_0000[0x68];
    unsigned char field_0068[20];
} MainManagerSlotPrefix;

extern int func_0899075C(unsigned int, unsigned int, int);
extern int func_089907F8(int);
extern int func_08990CA0(int);
extern int func_08990DC8(int);

#endif
