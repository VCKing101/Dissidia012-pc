#ifndef AGENT9_TYPES_H
#define AGENT9_TYPES_H
/* Placeholder polymorphic object whose virtuals take three int arguments;
   virtual N sits at vtable offset 8 * (N + 1). */
class A9Virt3 {
public:
    virtual void c0(int, int, int);
    virtual void c1(int, int, int);
    virtual void c2(int, int, int);
    virtual void c3(int, int, int);
    virtual void c4(int, int, int);
    virtual void c5(int, int, int);
    virtual void c6(int, int, int);
    virtual void c7(int, int, int);
    virtual void c8(int, int, int);
    virtual void c9(int, int, int);
};

/* Placeholder object whose virtuals take one int argument and return an int. */
class A9Ret1 {
public:
    virtual int r0(int);
    virtual int r1(int);
    virtual int r2(int);
    virtual int r3(int);
    virtual int r4(int);
    virtual int r5(int);
    virtual int r6(int);
    virtual int r7(int);
    virtual int r8(int);
    virtual int r9(int);
    virtual int r10(int);
    virtual int r11(int);
    virtual int r12(int);
    virtual int r13(int);
    virtual int r14(int);
    virtual int r15(int);
};

/* Placeholder object whose virtuals take no argument and return an int. */
class A9Ret0 {
public:
    virtual int q0();
    virtual int q1();
    virtual int q2();
    virtual int q3();
    virtual int q4();
    virtual int q5();
    virtual int q6();
    virtual int q7();
    virtual int q8();
    virtual int q9();
    virtual int q10();
    virtual int q11();
    virtual int q12();
    virtual int q13();
    virtual int q14();
    virtual int q15();
};

/* Placeholder object whose virtuals take four int arguments. */
class A9Virt4 {
public:
    virtual void d0(int, int, int, int);
    virtual void d1(int, int, int, int);
    virtual void d2(int, int, int, int);
    virtual void d3(int, int, int, int);
    virtual void d4(int, int, int, int);
    virtual void d5(int, int, int, int);
    virtual void d6(int, int, int, int);
    virtual void d7(int, int, int, int);
    virtual void d8(int, int, int, int);
    virtual void d9(int, int, int, int);
    virtual void d10(int, int, int, int);
    virtual void d11(int, int, int, int);
    virtual void d12(int, int, int, int);
    virtual void d13(int, int, int, int);
    virtual void d14(int, int, int, int);
    virtual void d15(int, int, int, int);
};

/* Placeholder object whose virtuals take one int argument. */
class A9Virt1 {
public:
    virtual void e0(int);
    virtual void e1(int);
    virtual void e2(int);
    virtual void e3(int);
    virtual void e4(int);
    virtual void e5(int);
    virtual void e6(int);
    virtual void e7(int);
    virtual void e8(int);
    virtual void e9(int);
    virtual void e10(int);
    virtual void e11(int);
    virtual void e12(int);
    virtual void e13(int);
    virtual void e14(int);
    virtual void e15(int);
};
#endif


