extern float D_089C9B38;
extern void func_089A296C(int mode);
extern void func_089A29C4(void);
extern void func_089A250C(void);
extern void func_089A2784(float angle, float x, float y, float z);
extern void func_089A2AF8(float *v);
extern void func_089A2A40(void);

void func_08873500(float *rot)
{
    func_089A296C(0);
    func_089A29C4();
    func_089A250C();
    func_089A2784(D_089C9B38 / 180.0f * rot[0], rot[3], rot[1], rot[2]);
    func_089A2AF8(rot + 4);
    func_089A2A40();
}
