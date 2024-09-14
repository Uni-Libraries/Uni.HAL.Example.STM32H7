# Uni.HAL STM32H7 example

## Compilation

* for NUCLEO board
```shell

mkdir build
cmake .. -DOX_TARGET=NUCLEO
cmake --build . --parallel
```

## Developer rules

* Do not use `printf()`/`vprintf()`/`snprintf()`. Use `uni_hal_io_stdio_printf()`/`uni_hal_io_stdio_vprintf()`/`uni_hal_io_stdio_snprintf()` instead:
```c
#include <uni_hal.h>
uni_hal_io_stdio_printf("%s - %s \r\n", "hello", "world");
```
