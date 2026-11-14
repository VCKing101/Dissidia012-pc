/* Draws every line of the text panel and its icon. */
typedef struct LinePanel {
    char pad_00[0x4];
    void *field_04;
    void **field_08;
    char pad_0C[0x8];
    int field_14;
    int field_18;
    int field_1C[(0xA8 - 0x1C) / 4];
    unsigned short field_A8[(0xF0 - 0xA8) / 2];
    int field_F0;
} LinePanel;

extern void func_088D3AE8(void *layout, int line, int text, int y);
extern void func_088D4584(void *layout, int line, int kind, void *icon);

void func_08AF9880(void *self, LinePanel *panel)
{
    int i;
    void *icon;
    for (i = 0; i < panel->field_F0; i++) {
        func_088D3AE8(panel->field_04, panel->field_14, panel->field_A8[i], panel->field_14 + panel->field_1C[i] + 1);
    }
    icon = panel->field_08[panel->field_18];
    if (icon != 0) {
        func_088D4584(panel->field_04, panel->field_14, 0x1F, icon);
    }
}
