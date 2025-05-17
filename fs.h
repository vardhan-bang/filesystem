#define BLOCKSIZE 512

struct superblock {
    int num_inodes;
    int num_blocks;
    int size_blocks;
};

struct inode {
    int size;
    int first_block;
    char name[8];
};

struct disk_block {
    int next_block_num;
    char data[BLOCKSIZE];
};

void create_fs();  // initialize new fs
void mount_fs();   // load a fs
void sync_fs();    // write the fs

int allocate_file(char name[8]); //return file number
void set_filesize(int filenum, int size);
void write_byte(int filenum, int pos, char *data);

void print_fs(); // print info about fs