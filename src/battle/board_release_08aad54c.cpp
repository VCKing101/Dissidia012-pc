struct BoardPiece {
    int id;

    static void release(BoardPiece *piece) {}
};

struct BoardRelease {
    char pad_00[0x4];
    char stream[0x4];
    BoardPiece pieces[3];
    BoardPiece marks[2];
};

extern "C" {

extern void func_08A81EDC(void *stream);
extern void func_08AABD2C(BoardRelease *self);

void func_08AAD54C(BoardRelease *self)
{
    for (int i = 0; i < 3; i++) {
        BoardPiece::release(&self->pieces[i]);
    }
    for (int i = 0; i < 2; i++) {
        BoardPiece::release(&self->marks[i]);
    }
    func_08A81EDC(self->stream);
    func_08AABD2C(self);
}

}
