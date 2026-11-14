typedef struct SelectState {
    int value;
} SelectState;

typedef struct SelectOwner {
    char pad_00[0x74];
    SelectState selection;
} SelectOwner;

void func_08AF2D08(SelectOwner *self, int *out)
{
    SelectState *selection = &self->selection;
    *out = selection->value;
}
