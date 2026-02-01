import os
from SCons.Script import *
import datetime
import shutil


def openOcdFlashImage(target, source, env):
    command = '"$FLASH_TOOL" '
    script_path = "scripts"
    if os.path.isdir(os.path.join(env["FLASH_TOOL_PATH"], script_path)):
        command += f'-s "%(FLASH_TOOL_PATH)s/{script_path}" '

    src_path = source[0].abspath.replace("\\", "/")

    command += "-f %(OCD_INTERFACE)s "
    # command += '-c "transport select jtag" '
    command += "-f %(OCD_TARGET)s "
    command += '-c "reset_config trst_only" '
    command += '-c "adapter speed 1500" '
    command += '-c "init" -c "reset init" '
    command += '-c "flash probe 0" '
    command += f'-c "program {src_path}" -c sw_reset_halt -c reset  -c exit'

    command = command % env
    res = env.Execute(command)
    logFile = open(target[0].abspath, "w")
    logFile.write(
        "Flash tool executed\r\n%s\r\n%s\r\nResult: %s\n"
        % (command, datetime.datetime.now(), res)
    )
    return res


def generate(env, **kw):
    if env.Detect("riscv-none-elf-gcc"):
        env["TOOL_PREFIX"] = "riscv-none-elf-"
    else:
        raise Exception('Unable to find build tools "riscv-none-elf-gcc"')

    env.Tool("gnu-tools")

    # use OpenOCD from path
    env["FLASH_TOOL_PATH"] = os.path.dirname(shutil.which("openocd"))
    env["FLASH_TOOL"] = f'{env["FLASH_TOOL_PATH"]}/openocd.exe'

    hexBuilder = Builder(
        action="$OBJCOPY -O ihex --only-section .isr_vectors --only-section .text --only-section .rodata --only-section .ctors --only-section .dtors --only-section .data --only-section .metadata_section $CUSTOM_HEX_PARAMS $SOURCE $TARGET",
        src_suffix=".elf",
        suffix=".hex",
    )

    flashBuilder = Builder(
        action=openOcdFlashImage, src_suffix=".elf", suffix="_flash.log"
    )

    env.Append(
        BUILDERS={
            "Hex": hexBuilder,
            "Flash": flashBuilder,
        }
    )

    env.Append(
        CCFLAGS=[
            "-finline-limit=60",
            "-fsingle-precision-constant",
            "-Wdouble-promotion",
            "--specs=nano.specs",
            "-mstrict-align",
            "-mno-save-restore",
            "-msmall-data-limit=8",
            "-fmessage-length=0",
            "-fno-common",
            "-fno-builtin",
        ]
    )

    env["CXXFLAGS"] = [
        "-fno-exceptions",
        "-fno-threadsafe-statics",
        "-fno-rtti",
        "-fuse-cxa-atexit",
        "-Woverloaded-virtual",
        "-std=c++20",
        "-Wno-volatile",
    ]

    if "DEVICE" in env:
        device = env["DEVICE"]

        target_arch = device["target_arch"]  # like "rv32imfc_zba_zbb_zbc_zbs_zicsr"
        target_abi = device["target_abi"]  # like "ilp32f"

        env.Append(
            CCFLAGS=[f"-march={target_arch}", f"-mabi={target_abi}", "-fno-exceptions"]
        )

        formattedLinkerPath = "%(MCUCPP_HOME)s/linker_scripts" % env
        formattedLinkerPath = env.Dir(formattedLinkerPath).srcnode().abspath
        formattedLinkerPath = f"-L{formattedLinkerPath}"

        env.Append(
            LINKFLAGS=[
                "-g3",
                "-ggdb",
                "-Xlinker",
                "-lgcc",
                "-lc",
                formattedLinkerPath,
                f"-march={target_arch}",
                f"-mabi={target_abi}",
                "-fno-use-cxa-atexit",
                "--specs=nano.specs",
            ]
        )

        env["ASFLAGS"] = [
            f"-march={target_arch}",
            f"-mabi={target_abi}",
            "-xassembler-with-cpp",
            f"-I{env.Dir('#/inc').srcnode().abspath}",
            "-fno-common",
            "-fno-builtin",
            "-c",
        ]


def exists(env):
    return env.Detect("arm-eabi-gcc")
