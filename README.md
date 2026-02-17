# Tenstorrent Example

Current working example of taking a Fortran + OpenMP program, and compiling it to Metalium.

What it performs:
- Splits the computation between n cores
- Each core computes it's bank address to read/write from via knowing it's rank (currently done on the baby RISC-V itself)
- Each core handles `data_size / num_cores` tiles itself, handling updating local addresses
- Compute core uses both matrix and vector unit
- Compute core uses and declares its own intermediate circular buffer
