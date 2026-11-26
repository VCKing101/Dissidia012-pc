/* Camera Edit: delete the camera data entry in force at the current play frame, moving later entries down. */
#include "main/replay_types.h"

extern "C" int func_088C9FE8(D012Replay *replay)
{
    D012ReplayCameraEntry *cam = (D012ReplayCameraEntry *)replay->camera;
    int n = replay->camera_data_count;
    if (n <= 0) {
        return 0;
    }
    for (int j = 0; j < n; j++) {
        if (j == 0 || replay->play_frame >= cam[j].frame) {
            if (j == n - 1 || replay->play_frame < cam[j + 1].frame) {
                for (int i = j; i < n; i++) {
                    cam[i] = cam[i + 1];
                }
                n = replay->camera_data_count;
                replay->camera_data_count = n - 1;
                if (j == 0 && replay->camera_data_count > 0) {
                    replay->camera[0].frame = 0;
                }
                return 1;
            }
        }
    }
    return 0;
}
