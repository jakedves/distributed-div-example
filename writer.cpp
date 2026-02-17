
#include <cstdint>
#include "dataflow_api.h"
#include "debug/dprint.h"

void kernel_main() {
    std::int32_t bank_id2 = get_arg_val<std::int32_t>(0);
    std::int32_t mem_addr2 = get_arg_val<std::int32_t>(1);
    std::int32_t size_bytes2 = get_arg_val<std::int32_t>(2);
    std::int32_t rank1 = get_arg_val<std::int32_t>(3);
    std::int64_t noc_addr2 = get_noc_addr_from_bank_id<true>(bank_id2, mem_addr2 + size_bytes2 * 5 * rank1);
    for (int i1 = 0; i1 < 5; i1 += 1) {
        cb_wait_front(3, 1);
        std::int32_t read_ptr = get_read_ptr(3);
        noc_async_write(read_ptr, noc_addr2 + static_cast<std::int64_t>(i1 * size_bytes2), size_bytes2);
        noc_async_write_barrier();
        cb_pop_front(3, 1);
    }
}
