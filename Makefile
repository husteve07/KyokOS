FILES = ./Build/kernel.asm.o ./Build/kernel.o ./Build/disk/disk.o ./Build/keyboard/classic.o ./Build/keyboard/keyboard.o ./Build/disk/streamer.o ./Build/isr80h/io.o ./Build/task/process.o ./Build/isr80h/isr80h.o ./Build/isr80h/misc.o ./Build/task/task.o ./Build/task/task.asm.o ./Build/task/tss.asm.o ./Build/fs/pparser.o ./Build/fs/file.o ./Build/fs/fat/fat16.o ./Build/string/string.o ./Build/idt/idt.asm.o ./Build/idt/idt.o ./Build/memory/memory.o ./Build/io/io.asm.o ./Build/gdt/gdt.o ./Build/gdt/gdt.asm.o ./Build/memory/heap/heap.o ./Build/memory/heap/kheap.o ./Build/memory/paging/paging.o ./Build/memory/paging/paging.asm.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/Boot.bin ./bin/kernel.bin user_programs
	rm -rf ./bin/os.bin
	dd if=./bin/Boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin
	sudo mount -t vfat ./bin/os.bin /mnt/d
	# Copy a file over
	sudo cp ./hello.txt /mnt/d
	sudo cp ./programs/blank/blank.bin /mnt/d
	sudo umount /mnt/d
	
./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./Build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./Build/kernelfull.o
./bin/Boot.bin: ./src/Boot/Boot.asm
	nasm -f bin ./src/Boot/Boot.asm -o ./bin/Boot.bin
./Build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./Build/kernel.asm.o
./Build/kernel.o: ./src/kernel.c
	i686-elf-gcc $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./Build/kernel.o
./Build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./Build/idt/idt.asm.o
./Build/gdt/gdt.o: ./src/gdt/gdt.c
	i686-elf-gcc $(INCLUDES) -I./src/gdt $(FLAGS) -std=gnu99 -c ./src/gdt/gdt.c -o ./Build/gdt/gdt.o
./Build/gdt/gdt.asm.o: ./src/gdt/gdt.asm
	nasm -f elf -g ./src/gdt/gdt.asm -o ./Build/gdt/gdt.asm.o
./Build/keyboard/classic.o: ./src/keyboard/classic.c
	i686-elf-gcc $(INCLUDES) -I./src/keyboard $(FLAGS) -std=gnu99 -c ./src/keyboard/classic.c -o ./Build/keyboard/classic.o
./Build/keyboard/keyboard.o: ./src/keyboard/keyboard.c
	i686-elf-gcc $(INCLUDES) -I./src/keyboard $(FLAGS) -std=gnu99 -c ./src/keyboard/keyboard.c -o ./Build/keyboard/keyboard.o
./Build/isr80h/isr80h.o: ./src/isr80h/isr80h.c
	i686-elf-gcc $(INCLUDES) -I./src/isr80h $(FLAGS) -std=gnu99 -c ./src/isr80h/isr80h.c -o ./Build/isr80h/isr80h.o
./Build/isr80h/io.o: ./src/isr80h/io.c
	i686-elf-gcc $(INCLUDES) -I./src/isr80h $(FLAGS) -std=gnu99 -c ./src/isr80h/io.c -o ./Build/isr80h/io.o
./Build/isr80h/misc.o: ./src/isr80h/misc.c
	i686-elf-gcc $(INCLUDES) -I./src/misc $(FLAGS) -std=gnu99 -c ./src/isr80h/misc.c -o ./Build/isr80h/misc.o
./Build/idt/idt.o: ./src/idt/idt.c
	i686-elf-gcc $(INCLUDES) -I./src/idt $(FLAGS) -std=gnu99 -c ./src/idt/idt.c -o ./Build/idt/idt.o
./Build/memory/memory.o: ./src/memory/memory.c
	i686-elf-gcc $(INCLUDES) -I./src/memory $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./Build/memory/memory.o
./Build/task/process.o: ./src/task/process.c
	i686-elf-gcc $(INCLUDES) -I./src/task $(FLAGS) -std=gnu99 -c ./src/task/process.c -o ./Build/task/process.o
./Build/task/task.o: ./src/task/task.c
	i686-elf-gcc $(INCLUDES) -I./src/task $(FLAGS) -std=gnu99 -c ./src/task/task.c -o ./Build/task/task.o

./Build/task/task.asm.o: ./src/task/task.asm
	nasm -f elf -g ./src/task/task.asm -o ./Build/task/task.asm.o

./Build/task/tss.asm.o: ./src/task/tss.asm
	nasm -f elf -g ./src/task/tss.asm -o ./Build/task/tss.asm.o

./Build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./Build/io/io.asm.o
./Build/memory/heap/heap.o: ./src/memory/heap/heap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/heap.c -o ./Build/memory/heap/heap.o
./Build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/heap $(FLAGS) -std=gnu99 -c ./src/memory/heap/kheap.c -o ./Build/memory/heap/kheap.o
./Build/memory/paging/paging.o: ./src/memory/paging/paging.c
	i686-elf-gcc $(INCLUDES) -I./src/memory/paging $(FLAGS) -std=gnu99 -c ./src/memory/paging/paging.c -o ./Build/memory/paging/paging.o
./Build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./Build/memory/paging/paging.asm.o
./Build/disk/disk.o: ./src/disk/disk.c
	i686-elf-gcc $(INCLUDES) -I./src/disk $(FLAGS) -std=gnu99 -c ./src/disk/disk.c -o ./Build/disk/disk.o
./Build/disk/streamer.o: ./src/disk/streamer.c
	i686-elf-gcc $(INCLUDES) -I./src/disk $(FLAGS) -std=gnu99 -c ./src/disk/streamer.c -o ./Build/disk/streamer.o
./Build/fs/fat/fat16.o: ./src/fs/fat/fat16.c
	i686-elf-gcc $(INCLUDES) -I./src/fs -I./src/fat $(FLAGS) -std=gnu99 -c ./src/fs/fat/fat16.c -o ./Build/fs/fat/fat16.o
./Build/fs/file.o: ./src/fs/file.c
	i686-elf-gcc $(INCLUDES) -I./src/fs $(FLAGS) -std=gnu99 -c ./src/fs/file.c -o ./Build/fs/file.o
./Build/fs/pparser.o: ./src/fs/pparser.c
	i686-elf-gcc $(INCLUDES) -I./src/fs $(FLAGS) -std=gnu99 -c ./src/fs/pparser.c -o ./Build/fs/pparser.o
./Build/string/string.o: ./src/string/string.c
	i686-elf-gcc $(INCLUDES) -I./src/string $(FLAGS) -std=gnu99 -c ./src/string/string.c -o ./Build/string/string.o

user_programs:
	cd ./programs/blank && $(MAKE) all

user_programs_clean:
	cd ./programs/blank && $(MAKE) clean

clean: user_programs_clean
	rm -rf ./bin/Boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf ${FILES}
	rm -rf ./Build/kernelfull.o