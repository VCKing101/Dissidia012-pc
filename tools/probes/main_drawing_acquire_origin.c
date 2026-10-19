typedef struct MainDrawingOriginPair {
    short first;
    short second;
} MainDrawingOriginPair;
extern void *func_088DA954(void *, int, MainDrawingOriginPair, unsigned char);

void *func_088DAA40(void *owner, int index, unsigned char flags) {
    MainDrawingOriginPair origin;
    origin.first = 0;
    origin.second = 0;
    return func_088DA954(owner,index,origin,flags);
}
