/* Closes an active HUD: finishes the message box when it is still open. */
typedef struct HudRecord {
    int field_00;
    char buffer[0x40];
    float time;
    float alpha;
    void *owner;
    unsigned char shown;
    unsigned char active;
} HudRecord;

extern int func_088ED2C0(void);
extern void func_088ED168(int arg);
extern void func_088EEB10(void);
extern void func_088ED0F4(void);
extern void func_088ECF10(void);

void func_088EA368(HudRecord *hud)
{
    if (hud->active != 0) {
        if (func_088ED2C0() == 0) {
            func_088ED168(0);
            func_088EEB10();
        }
        func_088ED0F4();
        func_088ECF10();
        hud->shown = 0;
        hud->active = 0;
        hud->field_00 = 0;
    }
}
