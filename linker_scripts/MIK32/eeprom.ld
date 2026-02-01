
OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)

ENTRY(_start)


MEMORY {
  eeprom (RX): ORIGIN = 0x01000000, LENGTH = 8K
  ram (RWX): ORIGIN = 0x02000000, LENGTH = 16K
}

__stack_size = DEFINED(__stack_size) ? __stack_size : 1024;

SECTION_ALIGNMENT = 4;

REGION_ALIAS("REGION_TEXT", eeprom);
REGION_ALIAS("REGION_RAM", ram);

INCLUDE sections.lds
