#include <HAL/7_Seg/seg_cfg.h>

const seg_cfg_t SEG_cfg[SEGMENT_MAX_NUM] = {
    [SEGMENT_1] = {
        .inital_val = 0,
        .port = GPIOA,
        .type = SEGMENT_COMMON_ANODE
    },
};

