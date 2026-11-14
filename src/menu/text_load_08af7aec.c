/* Loads the ability text file unless it is already the current one (mode 1). */
typedef struct TextLoadState08AF7AEC {
    char pad_000[0x8A0];
    void *field_8A0;
    int field_8A4;
} TextLoadState08AF7AEC;

typedef struct TextLoadOwner08AF7AEC {
    char pad_000[0x138];
    void *field_138;
    char pad_13C[0x4];
    TextLoadState08AF7AEC *field_140;
} TextLoadOwner08AF7AEC;

extern char D_08B0145C[];
extern void func_08AF7BA8(TextLoadOwner08AF7AEC *self);
extern void *func_0887B5A8(int size);
extern void func_0887B1E4(void *text);
extern void func_0887B260(void *text, const char *path, void *heap, int zero);

void func_08AF7AEC(TextLoadOwner08AF7AEC *self)
{
    void *heap;
    void *text;
    void *made;
    int one = 1;
    if (self->field_140->field_8A4 == one) {
        return;
    }
    func_08AF7BA8(self);
    heap = self->field_138;
    text = 0;
    made = func_0887B5A8(8);
    if (made != 0) {
        func_0887B1E4(made);
        text = made;
    }
    self->field_140->field_8A0 = text;
    func_0887B260(self->field_140->field_8A0, D_08B0145C, heap, 0);
    self->field_140->field_8A4 = one;
}
