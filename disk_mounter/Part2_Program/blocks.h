#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define RESERVELIMIT 0.12
#define FILENAMESIZE 255
#define FAIL -3
#define SUCCESS 3
#define FILESIZE 1048576
#define INODESIZE 80
#define FREE 1
#define NOTFREE 0
#define DEFAULTINODEATTRIBUTE -99
#define _DIRECTORY 313
#define _FILE 314
#define _SYMBOLICLINK 315
#define _HARDLINK 316
#define MAXFILELENGTH 255




typedef struct superblock_t
{
    unsigned int i_node_count;
    unsigned int data_block_count;
    unsigned int i_node_starting_addr;
    unsigned int log_size_of_block;
    unsigned int total_no_blocks;
    unsigned int i_node_block_count;
    unsigned int i_node_bit_map_starting_addr;
    unsigned int data_bit_map_starting_addr;
    unsigned int data_blocks_starting_addr;
}superblock;


typedef struct {
    char* bits;
    unsigned int size;
}bitvector;

#pragma pack(push,1)
typedef struct inode_t
{
    unsigned int type;
    unsigned int file_size;
    unsigned int day;
    unsigned int month;
    unsigned int year;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int direct_ptrs[8];
    unsigned int indirect_ptr;
    unsigned int double_indirect_ptr;
    unsigned int triple_indirect_ptr;

}i_node;

typedef struct file_t
{
    unsigned int i_node;
    unsigned int type;
    char *file_name;
} file;

// array of structs
typedef struct directory_t
{
    file *file;
    unsigned int used;
    unsigned int size;
} directory;


void show_dir(directory *dir);
void create_disk(FILE **to_disk ,const char* path, int size);

void write_super_block(FILE **to_disk, superblock gtu_superblock);
void write_single_i_node(FILE **to_disk, i_node single_i_node);
void write_single_i_node_bit_map(FILE **to_disk, bitvector *single_bit);
void write_single_data_bit_map(FILE **to_disk, bitvector *single_bit);
void set_fp(FILE **to_disk);

void offset_inc(int nth_block, int block_size);

int determine_total_block_count(int block_length);
int determine_reserved_block_count(int total_block_count);
int determine_i_node_number_per_block(int block_length);

bitvector* create_bitvec(unsigned int n_bits);
void destroy_bitvec(bitvector *vec);
void set_bit(bitvector* vec, unsigned int index, unsigned int bit);
int get_bit(bitvector* vec, unsigned int index);




void set_i_node_starting_addr(superblock* gtu_superblock, unsigned int block_size);
void set_i_node_count(superblock* gtu_superblock, unsigned int i_node_count);
void set_data_block_count(superblock* gtu_superblock, unsigned int data_block_count);
void set_size_of_block(superblock* gtu_superblock, unsigned int log_size_of_block);
void set_total_no_blocks(superblock* gtu_superblock, unsigned int total_no_blocks);
void set_inode_block_count(superblock* gtu_superblock, unsigned int i_node_count, int block_size);
unsigned int get_inode_block_count(superblock* gtu_superblock);
unsigned int get_i_node_count(superblock* gtu_superblock);
unsigned int get_data_block_count(superblock* gtu_superblock);
unsigned int get_i_node_starting_addr(superblock* gtu_superblock);
unsigned int get_log_size_of_block(superblock* gtu_superblock);
unsigned int get_total_no_blocks(superblock* gtu_superblock);
void set_default_i_nodes(
i_node *i_node_par,
unsigned int log_file_size,
unsigned int day,
unsigned int month,
unsigned int year,
unsigned int hour,
unsigned int minute,
unsigned int second,
unsigned int type
);




void init_directory(directory *a, unsigned int initial_size);
void add_file(directory *a, file _file);
void free_dir(directory *a);





extern long int current_fptr;
