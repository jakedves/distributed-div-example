# Tenstorrent Example

Current working example of taking a Fortran + OpenMP program, and compiling it to Metalium.

It is the output of:
```
tt-xftn div.F90
```

What it performs:
- Splits the computation between n == 2 cores
- Each core computes it's bank address to read/write from via knowing it's rank (currently done on the baby RISC-V itself)
- Each core handles `data_size / num_cores (10000 / 2 == 5000 or 5 x 32*32 tiles)` tiles each, handling updating local addresses
- Compute core uses both matrix and vector unit
- Compute core uses and declares its own intermediate circular buffer

Known issues:
- Seems to override the first 1/4 of an input tile...?

