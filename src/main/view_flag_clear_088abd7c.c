/* Clears bit 0 of the view's flag word when the global view has it set. */
typedef struct View088ABD7C {
    char pad_00[0x3F5FC];
    unsigned int flags;
} View088ABD7C;

extern View088ABD7C *D_089D712C;

int func_088ABD7C(View088ABD7C *view)
{
    if (D_089D712C->flags & 1) {
        view->flags &= ~1;
        return 1;
    }
    return 0;
}
