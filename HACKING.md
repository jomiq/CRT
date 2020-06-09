# Hacking

This template was derived from the ChibiOS/RT demo project for **ARM-Cortex-M7 NUCLEO STM32F746** to facilitate development on Arch linux. As a minimum an ARM compiler is needed. Suggest `arm-none-eabi-gcc` and `arm-none-eabi-newlib`. 

Base project can be found at `ChibiOS/demos/STM32/RT-STM32F746ZG-NUCLEO144`

For reference:
```
# System
Linux ARCH 5.6.15-arch1-1

# Compiler 
arm-none-eabi-gcc 10.1.0-1
arm-none-eabi-newlib 3.3.0-1

# Tools
stlink 1.6.0-1
arm-none-eabi-gdb 9.2-1
openocd 1:0.10.0-3
```

A sensible set of configurations are provided for vscode. To use them install the `c/c++` and `cortex-debug` extensions. 

## Included tools
 

* **tools/c_cpp_properties.py** <br> 
Produces IntelliSense .json configuration from `make` output. **PYTHON2** script. Adding a new module under `source` will generally require the paths to be reconfigured:
    ```
    $ make clean
    $ make | python2 tools/c_cpp_properties.py -C \
      arm-none-eabi-gcc > .vscode/c_cpp_properties.json
    ```

* **tools/flashme.sh**<br>
Flashes mcu using st-flash. 

* **tools/ttyme.sh**<br>
Launch minicom serial terminal. 
Edit to reflect your setup. 

## Directories

```
.
├── build
│   └── ...           [Output]
├── cfg
│   ├── chconf.h      [RTOS config]
│   ├── halconf.h     [HAL setup]
│   └── mcuconf.h     [Device config]
├── ChibiOS
│   └── ...           [ChibiOS submodule]
├── include
│   └── ...           [Project level headers]
├── libs
│   └── ...           [Libraries]
├── source
│   ├── <module>
|   |   ├── <module>.c
│   |   └── <module>.h
│   └── ...           [Everything in the source 
|                      directory is compiled 
|                      using autobuild.mk]
├── tools
│   └── ...           [Utilities]
├── HACKING.md
├── license.txt
├── main.c            [Program entry point]
├── Makefile          [Magic]
└── README.md

```

###