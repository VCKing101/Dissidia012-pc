typedef struct Kind088860C8 {
    short field_00;
    unsigned short id;
    short field_04;
} Kind088860C8;

typedef struct Info088860C8 {
    unsigned char unknown_00[0xC];
    short id;
} Info088860C8;

typedef struct Link088860C8 {
    int handle;
    Info088860C8 *info;
    int kind;
} Link088860C8;

extern Kind088860C8 D_089C9FD8[];

int func_088860C8(Link088860C8 *self) {
    int result = 0;
    if (self->handle == 0) {
        result = 1;
    } else if (self->info != 0) {
        result = self->info->id == D_089C9FD8[self->kind].id;
    }
    return result;
}
