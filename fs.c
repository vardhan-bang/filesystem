#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fs.h"

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

int find_empty_inode() {
    int i;
    for(i = 0; i < sb.num_inodes; i++) {
        if(inodes[i].first_block == -1) {
            return i;
        }
    }
    return -1;
}

int find_empty_block() {
    int i;
    for(i = 0; i < sb.num_blocks; i++) {
        if(dbs[i].next_block_num == -1) {
            return i;
        }
    }
    return -1;
}

void shorten_file(int bn) {
    printf("%d\n", bn);
    int nn = dbs[bn].next_block_num;
    if(nn >= 0) {
        shorten_file(nn);
    }
    
    dbs[bn].next_block_num = -1;
}

int get_block_num(int file, int offset) {
    int togo = offset;
    int bn = inodes[file].first_block;

    while(togo > 0) {
        bn = dbs[bn].next_block_num;
        togo--;
    }

    return bn;
}


// initialize new fs
void create_fs() {
    sb.num_inodes = 10;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof(struct disk_block);

    int i;
    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for(i = 0; i <  sb.num_inodes; i++) {
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        strcpy(inodes[i].name, "emptyfi");
    }

    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);
    for(i = 0; i <  sb.num_blocks; i++) {
        dbs[i].next_block_num = -1;
        //dbs[i].data = "";
    }

}

// load a fs
void mount_fs() {
    FILE *file;
    file = fopen("fs_data", "r");

    fread(&sb, sizeof(sb), 1, file);

    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    dbs = malloc(sizeof(struct disk_block) * sb.num_blocks);

    fread(inodes, sizeof(struct inode), sb.num_inodes, file);
    fread(dbs, sizeof(struct disk_block), sb.num_blocks, file);

    fclose(file);
};

// write the fs
void sync_fs() {
    FILE *file;
    file = fopen("fs_data", "w+");

    //write superblock
    fwrite(&sb, sizeof(sb), 1, file);
    fwrite(inodes, sizeof(struct inode), sb.num_inodes, file);
    fwrite(dbs, sizeof(struct disk_block), sb.num_blocks, file);

    fclose(file);
}   

void print_fs() {
    printf("Superblock info\n");
    printf("\tNumber of inodes: %d\n", sb.num_inodes);
    printf("\tNumber of disk blocks: %d\n", sb.num_blocks);
    printf("\tSize of disk blocks: %d\n", sb.size_blocks);

    printf("Inodes info\n");
    printf("%12s %15s %12s\n", "Size", "First Block", "Name");
    int i;
    for(i = 0; i <  sb.num_inodes; i++) {
        printf("%12d %15d %12s\n", inodes[i].size, inodes[i].first_block, inodes[i].name);
    }

    printf("Disk Blocks info\n");
    printf("%15s%20s\n", "Block Number", "Next Block Number");
    for(i = 0; i <  sb.num_blocks; i++) {
        printf("%15d%20d\n", i, dbs[i].next_block_num);
    }
}

int allocate_file(char name[8]) {
    //find an empty inode
    int in = find_empty_inode();
    
    //find and claim disk block
    int block = find_empty_block();
    
    //claim them
    inodes[in].first_block = block;
    dbs[block].next_block_num = -2;
    
    //fix name
    strcpy(inodes[in].name, name);

    //return file descriptor
    return in;
}

void set_filesize(int filenum, int size) {
    //how many blocks for size?
    int temp = size + BLOCKSIZE - 1;
    int num = temp/BLOCKSIZE;

    int bn = inodes[filenum].first_block;
    
    num--;

    //grow file if needed
    while(num > 0) {
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2) {
            int empty = find_empty_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[bn].next_block_num;
        num--;
    }

    //shrink file if needed
    shorten_file(bn);
    dbs[bn].next_block_num = -2;
    
}
void write_byte(int filenum, int pos, char *data) {
    int relative_block = pos/BLOCKSIZE;
    
    int bn = get_block_num(filenum, relative_block);

    int offset = pos%BLOCKSIZE;

    dbs[bn].data[pos] = *data;
}

 
