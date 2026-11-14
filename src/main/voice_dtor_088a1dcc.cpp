/* Deleting destructor of a voice: deletes its stream and returns it to the pool. */
struct Stream088A1DCC {
    virtual ~Stream088A1DCC();
};

extern "C" void func_088A2104(void *p);

struct Voice088A1DCC {
    char pad_00[0x50];
    Stream088A1DCC *stream;
    int field_54;

};

extern "C" {

void func_088A1DCC(Voice088A1DCC *v, unsigned int flags)
{
    if (v != 0) {
        if (v->stream != 0) {
            delete v->stream;
            v->stream = 0;
        }
        v->field_54 = 0;
        if (flags & 1) {
            func_088A2104(v);
        }
    }
}

}
