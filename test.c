#include <stdio.h>
#include "fs.h"

int main() {
    create_fs();
    allocate_file("f1"); 
    set_filesize(0, 512);
    allocate_file("f2");
    set_filesize(1, 512);
    set_filesize(0, 1024);

    char *word = "hello";
    int i;
    for(i = 0; i < 5; i++) {
        write_byte(0, 510+i, &(word[i]));
    }
    word = "world";
    for(i = 0; i < 5; i++) {
        write_byte(1, i, &(word[i]));
    }
    
    print_fs();
    sync_fs();
    printf("Done\n");
    return 0;
}