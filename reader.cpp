
#include <cstdint>
#include "dataflow_api.h"
#include "debug/dprint.h"

void kernel_main() {
    std::int32_t bank_id = get_arg_val<std::int32_t>(0);
    std::int32_t bank_id1 = get_arg_val<std::int32_t>(1);
    std::int32_t mem_addr = get_arg_val<std::int32_t>(2);
    std::int32_t mem_addr1 = get_arg_val<std::int32_t>(3);
    std::int32_t size_bytes = get_arg_val<std::int32_t>(4);
    std::int32_t size_bytes1 = get_arg_val<std::int32_t>(5);
    std::int32_t rank = get_arg_val<std::int32_t>(6);
    std::int64_t noc_addr = get_noc_addr_from_bank_id<true>(bank_id, mem_addr + size_bytes * 5 * rank);
    std::int64_t noc_addr1 = get_noc_addr_from_bank_id<true>(bank_id1, mem_addr1 + size_bytes1 * 5 * rank);
    for (int i = 0; i < 5; i += 1) {
        cb_reserve_back(0, 1);
        std::int32_t write_ptr = get_write_ptr(0);
        noc_async_read(noc_addr + static_cast<std::int64_t>(i * size_bytes), write_ptr, size_bytes);
        noc_async_read_barrier();
        cb_push_back(0, 1);
        cb_reserve_back(1, 1);
        std::int32_t write_ptr1 = get_write_ptr(1);
        noc_async_read(noc_addr1 + static_cast<std::int64_t>(i * size_bytes1), write_ptr1, size_bytes1);
        noc_async_read_barrier();
        cb_push_back(1, 1);
    }
}
