/* Runs a callback over every effect in the list, stopping those it accepts. */
typedef struct FxList088AA130 FxList088AA130;

typedef struct FxVisitor088AA130 {
    int (*accept)(void *res, void *context);
    void *context;
} FxVisitor088AA130;

void func_088AAF8C(FxList088AA130 *list, FxVisitor088AA130 *visitor);

void func_088AA130(FxList088AA130 *list, int (*accept)(void *res, void *context), void *context)
{
    FxVisitor088AA130 visitor;
    visitor.accept = accept;
    visitor.context = context;
    func_088AAF8C(list, &visitor);
}
