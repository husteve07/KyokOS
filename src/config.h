#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR                                        0x08
#define KERNEL_DATA_SELECTOR                                        0x10


#define KYOKOS_TOTAL_INTERRUPTS                                     512

#define KYOKOS_HEAP_SIZE_BYTES                                      104857600        //100 mb of heap
#define KYOKOS_HEAP_BLOCK_SIZE                                      4096             //size per block
#define KYOKOS_HEAP_ADDRESS                                         0X01000000          //starting address of heap (osdev)
#define KYOKOS_HEAP_TABLE_ADDRESS                                   0X00007E00    //conventional mem address (osdev)


#endif