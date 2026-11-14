typedef struct SwitchState {
    char pad_000[0x400];
    char field_400[0x480];
    char field_880[4];
} SwitchState;

typedef struct SwitchOwner {
    char pad_0000[0xDB30];
    SwitchState *field_DB30;
} SwitchOwner;

typedef struct SwitchLayout {
    char pad_000[0x89C];
    int field_89C;
} SwitchLayout;

typedef struct SwitchGlobal {
    char pad_000[0x140];
    SwitchLayout *field_140;
} SwitchGlobal;

extern SwitchGlobal *D_08B01CF8;
extern char D_08AD94AC[];
extern char D_08AD94B4[];
extern char D_08AD946C[];
extern int func_08AF96B0(SwitchOwner *self);
extern void func_088D3CF0(SwitchState *state, char *anim, char *name);
extern void func_088D3F7C(int layout, char *anim, char *name, int flags);
extern void func_08AD1514(SwitchOwner *self, int value);

void func_08ACAE20(SwitchOwner *self)
{
    if (func_08AF96B0(self) != 0) {
        func_088D3CF0(self->field_DB30, self->field_DB30->field_400, D_08AD94AC);
    } else {
        func_088D3F7C(D_08B01CF8->field_140->field_89C, self->field_DB30->field_880, D_08AD94B4, 0);
        func_088D3CF0(self->field_DB30, self->field_DB30->field_400, D_08AD946C);
        func_08AD1514(self, 0);
    }
}
