#include "blocks.h"


void create_disk(FILE **to_disk, const char* path, int size)
{
    *to_disk = fopen (path, "w");
    if(*to_disk == NULL)
    {
        fprintf(stderr, "%s\n", "Error creating disk. Exiting program." );
        exit(EXIT_FAILURE);
    }
    fseek(*to_disk, size , SEEK_SET); //go to 1 MB forward.
    fputc('\0', *to_disk); //place the EOF
    rewind(*to_disk);   //go back to beginning of the file.
}

bitvector* create_bitvec(unsigned int n_bits) {
    bitvector* vec = (bitvector*)malloc(sizeof(bitvector));
    int size = sizeof(bitvector) * ceil(n_bits/8.0);
    vec->size = size;
    vec->bits = (char*)calloc(size, 1);
    return vec;
}

void destroy_bitvec(bitvector *vec) {
    free(vec->bits);
    free(vec);
}

void set_bit(bitvector* vec, unsigned int index, unsigned int bit) {
    int byte = index >> 3;
    int n = sizeof(index)*8-3;
    int offset = ((unsigned) index << n) >> n;
    if (bit) {
        vec->bits[byte] |= 1 << (7-offset);
    } else {
        vec->bits[byte] &= ~(1 << (7-offset));
    }
}

int get_bit(bitvector* vec, unsigned int index) {
    unsigned int byte = index >> 3;
    unsigned int n = sizeof(index)*8-3;
    unsigned int offset = ((unsigned) index << n) >> n;
    if (vec->bits[byte] & (1 << (7- offset))) {
        return 1;
    } else {
        return 0;
    }
}




int determine_total_block_count(int block_length)
{
    return FILESIZE / block_length;
}
/*int determine_reserved_block_count(int total_block_count)
{
    return (int) total_block_count * RESERVELIMIT;
}*/


int determine_i_node_number_per_block(int block_length)
{
    return (int) block_length / INODESIZE;
}



void set_i_node_count(superblock* gtu_superblock, unsigned int i_node_count)
{
    gtu_superblock -> i_node_count = i_node_count;
}



void set_data_block_count(superblock* gtu_superblock, unsigned int data_block_count)
{
    gtu_superblock -> data_block_count = data_block_count;
}



void set_i_node_starting_addr(superblock* gtu_superblock, unsigned int i_node_starting_addr)
{
    gtu_superblock -> i_node_starting_addr = i_node_starting_addr;
}



void set_size_of_block(superblock* gtu_superblock, unsigned int log_size_of_block)
{
    gtu_superblock -> log_size_of_block = log_size_of_block;
}



void set_total_no_blocks(superblock* gtu_superblock, unsigned int total_no_blocks)
{
    gtu_superblock -> total_no_blocks = total_no_blocks;
}

void set_inode_block_count(superblock* gtu_superblock, unsigned int i_node_count, int block_size)
{
    int block_count = (block_size / i_node_count);
    gtu_superblock -> i_node_block_count = block_count;
}
unsigned int get_inode_block_count(superblock* gtu_superblock)
{
    return gtu_superblock -> i_node_block_count;
}




unsigned int get_i_node_count(superblock* gtu_superblock)
{
    return gtu_superblock -> i_node_count;
}

unsigned int get_data_block_count(superblock* gtu_superblock)
{
    return gtu_superblock -> data_block_count;
}

unsigned int get_i_node_starting_addr(superblock* gtu_superblock)
{
    return gtu_superblock -> i_node_starting_addr;
}

unsigned int get_log_size_of_block(superblock* gtu_superblock)
{
    return gtu_superblock -> log_size_of_block;
}

unsigned int get_total_no_blocks(superblock* gtu_superblock)
{
    return gtu_superblock -> total_no_blocks;
}


void write_super_block(FILE **to_disk, superblock gtu_superblock)
{
    if(fwrite (&gtu_superblock, sizeof(superblock), 1, *to_disk) != 0)
    {
        //fprintf(stderr, "%s\n","Wrote superblock to file correctly." );
    }
    else
    {
        fprintf(stderr, "%s\n","Error writing super block to file. Exiting program.");
    }
}

void write_single_i_node(FILE **to_disk, i_node single_i_node)
{
    if(fwrite (&single_i_node, sizeof(i_node), 1, *to_disk) != 0)
    {
        //fprintf(stderr, "%s\n","Wrote inode to file correctly." );
    }
    else
    {
        fprintf(stderr, "%s\n","Error writing inode block to file. Exiting program.");
    }
}

void write_single_i_node_bit_map(FILE **to_disk, bitvector *single_bit)
{


        fwrite(single_bit->bits,single_bit->size,1,*to_disk);
        //printf("%s\n",single_bit->bits );

}
void write_single_data_bit_map(FILE **to_disk, bitvector *single_bit)
{

    fwrite(single_bit->bits,single_bit->size,1,*to_disk);
}
void set_fp(FILE **to_disk)
{
    if(fseek(*to_disk, current_fptr, SEEK_SET) == -1)
    {
        perror("fseek");
    }
}

void set_default_i_nodes(
i_node *i_node_par,
unsigned int file_size,
unsigned int day,
unsigned int month,
unsigned int year,
unsigned int hour,
unsigned int minute,
unsigned int second,
unsigned int type
)
{
    int i;
    i_node_par -> file_size = file_size;
    i_node_par -> day =  day;
    i_node_par -> month =  month;
    i_node_par -> year = year;
    i_node_par -> hour = hour;
    i_node_par -> minute = minute;
    i_node_par -> second = second;
    for(i = 0; i < 8 ; ++i)
    {
        i_node_par -> direct_ptrs[i] = DEFAULTINODEATTRIBUTE;
    }
    i_node_par -> indirect_ptr = DEFAULTINODEATTRIBUTE;
    i_node_par -> double_indirect_ptr = DEFAULTINODEATTRIBUTE;
    i_node_par -> triple_indirect_ptr = DEFAULTINODEATTRIBUTE;
    i_node_par -> type = type;
}

void offset_inc(int nth_block, int block_size)
{
    current_fptr = nth_block * block_size;
}

/*void show_dir(directory *direct)
{
    int i;
    for(i = 0 ; i < direct -> size ; ++i)
    {
        if(direct -> dir[i].type == _DIRECTORY)
        {
            printf("DIR --inode: %d\t\t%s\n",direct-> dir[i].i_node,direct-> dir[i].file_name );
        }
        else if(direct -> dir[i].type == _FILE)
        {
            printf("REGFILE --inode: %d\t\t%s\n",direct-> dir[i].i_node, direct->dir[i].file_name );
        }

    }
}*/


void init_directory(directory *a, unsigned int initial_size)
{
    int i = 0;
    // Allocate initial space
    a->file = malloc(initial_size * sizeof(file));

    a->used = 0;           // no elements used
    a->size = initial_size; // available nr of elements

    // Initialize all values of the array to 0
    for(i = 0; i<initial_size; i++)
    {
        memset(&a->file[i],0,sizeof(file));
    }
}

// Add element to array
void add_file(directory *a, file _file)
{
    int i = 0;
    if (a->used == a->size)
    {
        a->size *= 2;
        a->file = realloc(a->file, a->size * sizeof(file));

        // Initialize the last/new elements of the reallocated array
        for(i = a->used; i<a->size; i++)
        {
            memset(&a->file[i],0,sizeof(file));
        }
    }

    // Copy name
    a->file[a->used].file_name = (char*)malloc(strlen(_file.file_name) + 1);
    strcpy(a->file[a->used].file_name, _file.file_name);

    // Copy ID
    a->file[a->used].i_node=_file.i_node;

    a->file[a->used].type=_file.type;

    a->used++;
}

void free_dir(directory *a)
{
    int i = 0;
    // Free all name variables of each array element first
    for(i=0; i<a->used; i++)
    {
        free(a->file[i].file_name);
        a->file[i].file_name=NULL;
    }

    // Now free the array
    free(a->file);
    a->file = NULL;

    a->used = 0;
    a->size = 0;
}
