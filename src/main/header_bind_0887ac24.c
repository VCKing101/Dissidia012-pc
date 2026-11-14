typedef struct Reader0887AC24 {
    int size;
    void *header;
} Reader0887AC24;

int func_0887A898(void *header);
void func_0887A6A0(void *header);

int func_0887AC24(Reader0887AC24 *self, void *header) {
    if (self->header != 0) {
        return 0;
    }
    self->header = header;
    self->size = func_0887A898(header);
    func_0887A6A0(self->header);
    return 1;
}
