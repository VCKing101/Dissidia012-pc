/* Deletes an effect object of the effect list. */
class DisplayObject {
public:
    virtual ~DisplayObject();
};

extern "C" int func_08A04624(void *display, DisplayObject *child)
{
    if (child == 0) {
        return -1;
    }
    delete child;
    return 0;
}
