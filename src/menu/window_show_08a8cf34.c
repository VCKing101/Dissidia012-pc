/* Shows (on == 1) or hides the second window and its three effects. */
typedef struct Window2Parts {
    char pad_000[0x38];
    void *field_38;
    void *field_3C;
    char pad_040[0x260 - 0x40];
    void *field_260;
    void *field_264;
    void *field_268;
} Window2Parts;

typedef struct Window2Owner {
    char pad_00[0x8];
    Window2Parts *field_08;
} Window2Owner;

extern void func_088D9CD8(void *icon);
extern void func_088D9CFC(void *icon);
extern void func_0887B924(void *effect);
extern void func_0887B944(void *effect);

void func_08A8CF34(Window2Owner *self, unsigned char on)
{
    if (on == 1) {
        func_088D9CD8(self->field_08->field_38);
        func_088D9CD8(self->field_08->field_3C);
        func_0887B924(self->field_08->field_260);
        func_0887B924(self->field_08->field_264);
        func_0887B924(self->field_08->field_268);
    } else {
        func_088D9CFC(self->field_08->field_38);
        func_088D9CFC(self->field_08->field_3C);
        func_0887B944(self->field_08->field_260);
        func_0887B944(self->field_08->field_264);
        func_0887B944(self->field_08->field_268);
    }
}
