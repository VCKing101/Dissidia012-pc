/* Shows icon `index` with the frame for `kind` (on == 1) or hides it. */
typedef struct KindIcons {
    char pad_000[0x438];
    void *field_438[8];
} KindIcons;

typedef struct KindIconPanel {
    char pad_000[0x850];
    KindIcons *field_850;
} KindIconPanel;

extern short D_08AE4576[];
extern void func_088D9CD8(void *icon);
extern void func_088D9CFC(void *icon);
extern void func_088D9F5C(void *icon, int frame, int one);

void func_08AE12AC(KindIconPanel *self, unsigned char on, int index, int kind)
{
    if (on == 1) {
        func_088D9CD8(self->field_850->field_438[index]);
        func_088D9F5C(self->field_850->field_438[index], D_08AE4576[kind], 1);
    } else {
        func_088D9CFC(self->field_850->field_438[index]);
    }
}
