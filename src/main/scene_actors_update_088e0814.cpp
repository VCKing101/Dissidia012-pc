/* Updates every actor of the scene through its virtual sceneUpdate(). */
#include "main/agent19_types.h"

struct ActorScene {
    char pad_0000[0x1220];
    char actors[0x17B1 - 0x1220];
    unsigned char active;
};

extern "C" {
extern void func_088E8268(void *actors);
extern Agent19ActorBase *func_0881CBB8(Agent19ActorBase *after, int team);
extern void func_0881CB5C(void);

void func_088E0814(ActorScene *scene)
{
    Agent19ActorBase *actor;

    if (scene->active != 0) {
        func_088E8268(scene->actors);
        actor = 0;
        while ((actor = func_0881CBB8(actor, -1)) != 0) {
            actor->sceneUpdate();
        }
        func_0881CB5C();
    }
}
}
