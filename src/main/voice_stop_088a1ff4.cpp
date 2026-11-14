/* Stops the voice's stream (virtual 6). */
struct Stream088A1FF4 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void stop();
};

struct Voice088A1FF4 {
    char pad_00[0x50];
    Stream088A1FF4 *stream;
};

extern "C" {

void func_088A1FF4(Voice088A1FF4 *v)
{
    v->stream->stop();
}

}
