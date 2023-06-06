#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR                                        0x08
#define KERNEL_DATA_SELECTOR                                        0x10


#define KYOKOS_TOTAL_INTERRUPTS                                     512

#define KYOKOS_HEAP_SIZE_BYTES                                      104857600        //100 mb of heap
#define KYOKOS_HEAP_BLOCK_SIZE                                      4096             //size per block
#define KYOKOS_HEAP_ADDRESS                                         0X01000000          //starting address of heap (osdev)
#define KYOKOS_HEAP_TABLE_ADDRESS                                   0X00007E00    //conventional mem address (osdev)

#define KYOKOS_SECTOR_SIZE                                          512           //disk sector size

#define KYOKOS_MAX_FILESYSTEMS                                      12
#define KYOKOS_MAX_FILE_DESCRIPTORS                                 512

#define KYOKOS_MAX_PATH                                             108

#define KYOKOS_TOTAL_GDT_SEGMENTS                                   6

#define KYOKOS_PROGRAM_VIRTUAL_ADDRESS                              0X400000
#define KYOKOS_USER_PROGRAM_STACK_SIZE                              1024*16
#define KYOKOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START                  0X3FF000
#define KYOKOS_PROGRAM_VIRTUAL_STACK_ADDRESS_END                    KYOKOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START - KYOKOS_USER_PROGRAM_STACK_SIZE

#define KYOKOS_MAX_PROGRAM_ALLOCATIONS                              1024
#define KYOKOS_MAX_PROCESSES                                        12

#define USER_DATA_SEGMENT                                           0X23
#define USER_CODE_SEGMENT                                           0X1b

#define KYOKOS_MAX_ISR80H_COMMANDS                                  1024

#define KYOKOS_KEYBOARD_BUFFER_SIZE                                 1024

#define CLASSIC_KEYBOARD_KEY_RELEASED                               0x80
#define ISR_KEYBOARD_INTERRUPT                                      0x21
#define KEYBOARD_INPUT_PORT                                         0x60
#endif