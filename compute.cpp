
#include <cstdint>
#include "compute_kernel_api/common.h"
#include "compute_kernel_api/matmul.h"
#include "compute_kernel_api/tile_move_copy.h"
#include "compute_kernel_api/eltwise_unary/eltwise_unary.h"
#include "compute_kernel_api/eltwise_unary/fill.h"
#include "compute_kernel_api/eltwise_unary/exp.h"
#include "compute_kernel_api/eltwise_binary.h"
#include "compute_kernel_api/eltwise_binary_sfpu.h"
#include "compute_kernel_api/add_int_sfpu.h"
#include "debug/dprint.h"

namespace NAMESPACE {

void MAIN {
    binary_op_init_common(0, 1, 3);
    for (int i2 = 0; i2 < 5; i2 += 1) {
        cb_wait_front(0, 1);
        cb_wait_front(1, 1);
        tile_regs_acquire();
        fill_tile_init();
        fill_tile(0, 1.0);
        copy_tile_to_dst_init_short(1, 0);
        copy_tile(1, 0, 1);
        div_binary_tile_init();
        div_binary_tile(0, 1);
        tile_regs_commit();
        tile_regs_wait();
        cb_reserve_back(2, 1);
        pack_tile<false>(0, 2, 0);
        cb_push_back(2, 1);
        cb_wait_front(2, 1);
        tile_regs_release();
        tile_regs_acquire();
        mul_tiles_init(0, 2);
        mul_tiles(0, 2, 0, 0, 2);
        cb_reserve_back(3, 1);
        tile_regs_commit();
        tile_regs_wait();
        pack_tile<false>(2, 3, 0);
        cb_push_back(3, 1);
        cb_pop_front(0, 1);
        cb_pop_front(1, 1);
        cb_pop_front(2, 1);
        tile_regs_release();
    }
}

}
