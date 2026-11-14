#ifndef AGENT6_TYPES_H
#define AGENT6_TYPES_H
/* Placeholder polymorphic menu part: virtual N sits at vtable offset 8 * (N + 1). */
class A6Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
};

/* Placeholder object whose own vtable pointer is at offset 0 and has many virtuals. */
class A6Big {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
};

/* Placeholder polymorphic part whose virtuals take one argument. */
class A6PartArg {
public:
    virtual void w0(int);
    virtual void w1(int);
    virtual void w2(int);
    virtual void w3(int);
    virtual void w4(int);
    virtual void w5(int);
    virtual void w6(int);
    virtual void w7(int);
    virtual void w8(int);
    virtual void w9(int);
    virtual void w10(int);
    virtual void w11(int);
    virtual void w12(int);
    virtual void w13(int);
    virtual void w14(int);
    virtual void w15(int);
};
#endif
