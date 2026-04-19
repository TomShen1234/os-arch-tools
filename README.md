# Container for Computer Architecture and Operating Systems class

This container contains all the required tools as well as the source code files for homeworks and hands-on activities for the 2 classes.

## Included Tools

See the section of `Dockerfile` that executes `apt-get`. Everything is installed during that time.

## Included Resources

Files under the `resources` directory inside this project will be copied to `/opt/resources` on the container.

- `vecadd.c`
- `vecadd-asm` (for ARM assembly hands-on)
- `openmp_matmul.cpp`
- `mpi` (includes `makeit-mpi` and `mpi_mm-rdv.c`)
- `ostep-code` (cloned directly from [this Github repository](https://github.com/remzi-arpacidusseau/ostep-code))
- `ostep-homework` (cloned directly from [this Github repository](https://github.com/remzi-arpacidusseau/ostep-homework))
- `va_hw.c`
- `t2-lock-multi.c`
