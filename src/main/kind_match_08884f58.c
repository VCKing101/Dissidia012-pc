typedef struct Kind08884F58 {
    short field_00;
    unsigned short id;
    unsigned char unknown_04[6];
} Kind08884F58;

typedef struct Info08884F58 {
    unsigned char unknown_00[0xC];
    short id;
} Info08884F58;

typedef struct Owner08884F58 {
    int field_00;
    Info08884F58 *info;
    unsigned char unknown_08[0xC];
    int kind;
} Owner08884F58;

extern Kind08884F58 D_089C9F88[];

int func_08884F58(Owner08884F58 *self) {
    int result = 0;
    if (self->info != 0) {
        result = self->info->id == D_089C9F88[self->kind].id;
    }
    return result;
}
