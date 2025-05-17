# filesystem
A simple FAT (File Allocation Table) based File System implementation in C.

#### Usage
1. Edit test.c, following functions are available
    - create_fs() - Create a blank new file system
    - mount_fs() - Load files to memory
    - sync_fs() - Save files to drive
    - allocate_file(char *name[8]) - Create new file (Max name size: 8 characters)
    - set_filesize(int filenum, int size) - Allocate blocks to a file
    - write_bytes(int filenum, int pos, char *data) - Write a byte to the file at specified position
    - print_fs() - Display info about the file system

2. Build with `make`
3. Run with `./test` or `make run`
4. To view saved files (fs_data), use a hex viewer like `xxd` or `hexdump`
