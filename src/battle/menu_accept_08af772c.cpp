typedef struct MenuAccept {
    char pad_00[0x3C];
    int pending;
    int accepted;
} MenuAccept;

extern "C" {

bool func_08AF772C(MenuAccept *self)
{
    if (self->pending != 0) {
        self->accepted = 1;
        return true;
    }
    return false;
}

}
