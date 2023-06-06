#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "memory/memory.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "isr80h/isr80h.h"
#include "task/task.h"
#include "task/process.h"
#include "fs/file.h"
#include "disk/streamer.h"
#include "gdt/gdt.h"
#include "config.h"
#include "task/tss.h"
#include "status.h"
#include "keyboard/keyboard.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
}
void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y*VGA_WIDTH) + x] = terminal_make_char(c, colour);

}

void terminal_backspace()
{
    if((terminal_row == 0 && terminal_col == 0))
        return;

    if(terminal_col == 0)
    {
        terminal_col = VGA_WIDTH;
        terminal_row -= 1;
    }

    terminal_col--;
    terminal_writechar(' ', 10);

    if(!terminal_col)
    {
        terminal_row -= 1;
        terminal_col = VGA_WIDTH;
    }

    terminal_col--;

}

void terminal_writechar(char c, char colour)
{
    if(c == '\n')
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    if(c == 0x08)
    {
        terminal_backspace();
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;
    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_initialize()
{
    video_mem = (uint16_t*)(0xB8000);
    for(int y = 0; y<VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }

}

void boot_message(int fd)
{
    if(fd)
    {
        //print("\nhello.txt opened successfully\n");

        char buf[500];
        //fseek(fd, 2, SEEK_SET);
        fread(buf,499,1,fd);
        buf[499] = 0x00;
        print(buf);
    }
}

void print(const char* str)
{
    size_t len = strlen(str);
    for(int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 10); // 10 is light green
    }
}
static struct paging_4gb_chunk* kernel_chunk = 0;

void panic(const char* msg)
{
    print (msg);
    while(1){}
}

void kernel_page()
{
    kernel_registers();
    paging_switch(kernel_chunk);
}

struct tss tss;
struct gdt gdt_real[KYOKOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[KYOKOS_TOTAL_GDT_SEGMENTS] = 
{
    {.base = 0x00, .limit = 0x00, .type = 0x00}, //null segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x9a}, //kernel code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x92}, //kernel data segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf8}, //user code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf2}, //user data segment
    {.base = (uint32_t)&tss, .limit = sizeof(tss), .type = 0xE9}  //tss segment
};



void kernel_main()
{
    terminal_initialize();

    //panic("Kernel Panic");
    
    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, KYOKOS_TOTAL_GDT_SEGMENTS);

    //load the gdt
    gdt_load(gdt_real, sizeof(gdt_real));

    //initialize heap
    kheap_init();

    //initialize filesystems
    fs_init();

    //search and initialize the disk
    disk_search_and_init();

    //initialize idt
    idt_init();

    //setup tss
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;

    //load the tss
    tss_load(0x28);

    //setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    //switch to kernel paging chunk
    paging_switch(kernel_chunk);

/* paging test
    char* ptr = kzalloc(4096);
    paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITABLE | PAGING_IS_PRESENT);
 */

    //enable paging
    enable_paging();

/*paging test
    char* ptr2 = (char*)0x1000;
    ptr2[0] = 'A';
    ptr2[1] = 'B';
    print(ptr2);
    print(ptr);
    */


    //enable system interrupts
    //enable_interrupt();
/*heap test
    void* ptr = kmalloc(50);
    void* ptr2 = kmalloc(5000);

    void* ptr3 = kmalloc(5600);

    kfree(ptr);
    void* ptr4 = kmalloc(50);

    if(ptr || ptr2 || ptr3 || ptr4)
    {

    }
*/
/*parser test
    struct path_root* root_path = pathparser_parse("0:/bin/shell.exe", NULL);
    if(root_path)
    {

    }
*/

/*disk stream test

    struct disk_stream* stream = diskstreamer_new(0);
    diskstreamer_seek(stream, 0x201);
    unsigned char c = 0;
    diskstreamer_read(stream, &c, 1);
    while(1) {}
*/

/*file test
    


    int fd = fopen("0:/hello.txt", "r");
    if(fd)
    {
        struct file_stat s;
        fstat(fd, &s);
        boot_message(fd);

        if(fclose(fd)==0)
            print("\nfile closed\n");
    }
*/
    isr80h_register_commands();

    keyboard_init();

    struct process* process = 0;
    int res = process_load_switch("0:/blank.bin", &process);
    if(res != kYOKOS_ALL_OK)
    {
        panic("failed to load blank.bin\n");
    }

    int fd = fopen("0:/hello.txt", "r");
    if(fd)
    {
        boot_message(fd);
        fclose(fd);
    }
    task_run_first_ever_task();
    while(1) {}

    
}