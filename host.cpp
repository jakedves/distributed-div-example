
#include <tt-metalium/host_api.hpp>
#include <tt-metalium/device.hpp>

#include "../utils-tt/tt-utils.h"

using namespace tt;
using namespace tt::tt_metal;


std::int32_t main() {
    Program program = CreateProgram();
    std::variant<CoreCoord, CoreRange, CoreRangeSet> core_range = CoreRange({0, 0}, {0, 1});
    IDevice* device = CreateDevice(0);
    std::vector<float> v(10000, 5.5f);
    std::vector<float> v1(10000, 11.0f);
    std::vector<float> v2(10000, 90.0f);
    std::shared_ptr<Buffer> buffer = CreateBuffer({.device=device, .size=40000, .page_size=40000, .buffer_type = BufferType::DRAM});
    CircularBufferConfig cb_config = CircularBufferConfig(10 * 4096, {{3, tt::DataFormat::Float32}}).set_page_size(3, 4096);
    CBHandle cb = CreateCircularBuffer(program, core_range, cb_config);
    std::shared_ptr<Buffer> buffer1 = CreateBuffer({.device=device, .size=40000, .page_size=40000, .buffer_type = BufferType::DRAM});
    CircularBufferConfig cb_config1 = CircularBufferConfig(10 * 4096, {{0, tt::DataFormat::Float32}}).set_page_size(0, 4096);
    CBHandle cb1 = CreateCircularBuffer(program, core_range, cb_config1);
    std::shared_ptr<Buffer> buffer2 = CreateBuffer({.device=device, .size=40000, .page_size=40000, .buffer_type = BufferType::DRAM});
    CircularBufferConfig cb_config2 = CircularBufferConfig(10 * 4096, {{1, tt::DataFormat::Float32}}).set_page_size(1, 4096);
    CBHandle cb2 = CreateCircularBuffer(program, core_range, cb_config2);
    EnqueueWriteBuffer(device->command_queue(), buffer, v2, false);
    EnqueueWriteBuffer(device->command_queue(), buffer1, v, false);
    EnqueueWriteBuffer(device->command_queue(), buffer2, v1, false);
    CircularBufferConfig cb_config3 = CircularBufferConfig(10 * 4096, {{2, tt::DataFormat::Float32}}).set_page_size(2, 4096);
    CBHandle cb3 = CreateCircularBuffer(program, core_range, cb_config3);
    KernelHandle kernel = CreateKernel(program, "reader.cpp", core_range, DataMovementConfig{.processor = DataMovementProcessor::RISCV_1, .noc=NOC::RISCV_1_default});
    KernelHandle kernel1 = CreateKernel(program, "writer.cpp", core_range, DataMovementConfig{.processor = DataMovementProcessor::RISCV_0, .noc=NOC::RISCV_0_default});
    KernelHandle kernel2 = CreateKernel(program, "compute.cpp", core_range, ComputeConfig {.math_fidelity = MathFidelity::LoFi, .fp32_dest_acc_en = false, .math_approx_mode = false, .compile_args = {}});
    for (int rank = 0; rank < 2; rank += 1) {
        std::variant<CoreCoord, CoreRange, CoreRangeSet> core = CoreCoord{rank % 1, rank};
        SetRuntimeArgs(program, kernel, core, {0, 0, buffer1->address(), buffer2->address(), 4096, 4096, rank});
        SetRuntimeArgs(program, kernel1, core, {0, buffer->address(), 4096, rank});
    }
    SetRuntimeArgs(program, kernel2, core_range, {});
    EnqueueProgram(device->command_queue(), program, false);
    Finish(device->command_queue());
    EnqueueReadBuffer(device->command_queue(), buffer, v2, true);
    EnqueueReadBuffer(device->command_queue(), buffer1, v, true);
    EnqueueReadBuffer(device->command_queue(), buffer2, v1, true);

    print_n_tiles(v, 1);
    print_n_tiles(v1, 1);
    print_n_tiles(v2, 9);
}
