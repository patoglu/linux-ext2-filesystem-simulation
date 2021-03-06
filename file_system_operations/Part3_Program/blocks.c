#include "blocks.h"




bitvector* create_bitvec(unsigned int n_bits) {
    bitvector* vec = (bitvector*)malloc(sizeof(bitvector) + 1);
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




void write_single_i_node(FILE **to_disk, int nth_i_node)
{
    fseek(*to_disk, gtu_superblock.i_node_starting_addr +
    nth_i_node * PADDED, SEEK_SET);
    fwrite(single_i_node, sizeof(i_node), 1, *to_disk);

}

/*void write_single_i_node(FILE **to_disk, i_node single_i_node)
{
    if(fwrite (&single_i_node, sizeof(single_i_node), 1, *to_disk) != 0)
    {
        //fprintf(stderr, "%s\n","Wrote inode to file correctly." );
    }
    else
    {
        fprintf(stderr, "%s\n","Error writing inode block to file. Exiting program.");
    }
}*/

void write_single_i_node_bit_map(FILE **fp, bitvector *single_bit)
{

        fseek(*fp, gtu_superblock.i_node_bit_map_starting_addr, SEEK_SET);

        fwrite(single_bit->bits,single_bit->size,1,*fp);
}
void write_single_data_bit_map(FILE **fp, bitvector *single_bit)
{
    fseek(*fp, gtu_superblock.data_bit_map_starting_addr, SEEK_SET);

    fwrite(single_bit->bits,single_bit->size,1,*fp);
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
}
*/

void read_super_block(FILE **from_disk)
{
    fread(&gtu_superblock, sizeof(superblock), 1, *from_disk);
}




void open_for_reading(FILE  **from_disk)
{
    *from_disk = fopen (disk, "r");
    if (*from_disk == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
}
void open_for_writing(FILE  **to_disk)
{
    *to_disk = fopen (disk, "r+");
    if (*to_disk == NULL)
    {
        fprintf(stderr, "\nError opening file\n");
        exit (1);
    }
}
void read_single_i_node(FILE **from_disk, int nth_i_node)
{

    fseek(*from_disk, gtu_superblock.i_node_starting_addr +
    nth_i_node * PADDED, SEEK_SET);
    //printf("Reading at %d\n", ftell(*from_disk));
    fread(single_i_node, sizeof(i_node), 1, *from_disk);
}
/*void read_single_directory(FILE ** from_disk)
{
    int store_size;
    fseek(*from_disk, single_i_node -> direct_ptrs[0], SEEK_SET);
    fread(&current_dir, sizeof(directory), 1, *from_disk);
    store_size = current_dir . size;
    printf("size %d\n",store_size );
    //fseek(*from_disk, single_i_node -> direct_ptrs[0], SEEK_SET);

    //current_dir->dir = malloc(store_size * sizeof(dirent));

    //fread(current_dir, sizeof(directory), 1, *from_disk);


}*/
void print_meta_data()
{
    printf("type %d\n", single_i_node -> type);
    printf("file_size %d\n", single_i_node -> file_size);
    printf("day %d\n", single_i_node -> day);
    printf("month %d\n", single_i_node -> month);
    printf("year %d\n", single_i_node -> year);
    printf("hour %d\n", single_i_node -> hour);
    printf("minute %d\n", single_i_node -> minute);
    printf("second %d\n", single_i_node -> second);
    printf("direct_ptrs[0] %d\n", single_i_node -> direct_ptrs[0]);
    printf("direct_ptrs[1] %d\n", single_i_node -> direct_ptrs[1]);
    printf("direct_ptrs[2] %d\n", single_i_node -> direct_ptrs[2]);
    printf("indirect_ptr %d\n", single_i_node -> indirect_ptr);
    printf("double_indirect_ptr %d\n", single_i_node -> double_indirect_ptr);
    printf("triple_indirect_ptr %d\n", single_i_node -> triple_indirect_ptr);
}

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

void load_current_dir(FILE **from_disk)
{
    int i;
    file temp;
    int used, i_node, type;
    char file_name[255];
    fseek(*from_disk, single_i_node -> direct_ptrs[0] , SEEK_SET);
    fscanf(*from_disk, "%d\n", &used);
    init_directory(&current_dir, 100);
    for(i = 0 ; i < used ; ++i)
    {

        fscanf(*from_disk, "%d`%d`%s\n",&i_node, &type, file_name);
        temp.file_name = malloc(strlen(file_name) + 1);
        strcpy(temp.file_name, file_name);
        temp.i_node = i_node;
        temp.type = type;
        add_file(&current_dir, temp);
        free(temp.file_name);
    }
}




void load_data_bit_map(FILE **fp)
{
    data_bit_map = create_bitvec(gtu_superblock.data_block_count);

    fseek(*fp,gtu_superblock.data_bit_map_starting_addr , SEEK_SET);

    fread(data_bit_map -> bits, data_bit_map -> size ,1, *fp);

}
void load_i_node_bit_map(FILE **fp)
{
    i_node_bit_map = create_bitvec(gtu_superblock.i_node_count);
    fseek(*fp,gtu_superblock.i_node_bit_map_starting_addr , SEEK_SET);
    fread(i_node_bit_map -> bits, i_node_bit_map -> size ,1, *fp);

}


void print_i_node_bit_map()
{
    int free_i_nodes = 0;
    int i;
    int a;
    int status = -1;
    for(i = 0 ; i < gtu_superblock.i_node_count ; ++i)
    {
        status = get_bit(i_node_bit_map, i);
        if(status == FREE)
        {
            free_i_nodes++;
            printf("%d. inode -> FREE.\n", i );
        }
        else if(status == NOTFREE)
        {
            printf("%d. inode -> NOTFREE.\n", i );
            single_i_node = malloc(sizeof(i_node));
            read_single_i_node(&from_disk, i);
            if(single_i_node->type == _FILE)
            {
                //just skip it gods sake
                free(single_i_node);
            }
            else
            {
                load_current_dir(&from_disk);
                for(a = 0 ; a < current_dir.used ; ++a)
                {
                    printf("[ I node no: %d ] ||| [Filename: %s]\n",current_dir.file[a].i_node,current_dir.file[a].file_name );
                    if(strcmp(current_dir.file[a].file_name, ".") && strcmp(current_dir.file[a].file_name, "..") && current_dir.file[a].type == _DIRECTORY)
                    {
                        total_dir_count++;
                    }
                    else if(strcmp(current_dir.file[a].file_name, ".") &&strcmp(current_dir.file[a].file_name, "..") && current_dir.file[a].type == _FILE)
                    {
                        total_file_count++;
                    }
                }
                free_dir(&current_dir);
                free(single_i_node);
            }


        }

    }
    printf("Total %d i_nodes are free.\n\nTotal %d inodes are not free\n",free_i_nodes, gtu_superblock.i_node_count - free_i_nodes );
    printf("Total directory count %d\n\nTotal file count %d\n", total_dir_count, total_file_count);
}
void print_data_bit_map()
{
    int i;
    int status = -1;
    int free_data_block_count = 0;
    for(i = 0 ; i < gtu_superblock.data_block_count ; ++i)
    {
        status = get_bit(data_bit_map, i);
        if(status == FREE)
        {
            free_data_block_count++;
            //printf("%d. data -> FREE.free\n", i );
        }
        else if(status == NOTFREE)
        {
            //printf("%d. data -> NOTFREE.\n", i );
        }
    }
    printf("Total %d data blocks are free.\n\nTotal %d data blocks are not free.\n", free_data_block_count, gtu_superblock.data_block_count - free_data_block_count);
}





/*https://www.it-swarm.dev/tr/c/c-de-strtok-kullanimi/941300129/*/
char** tokenize_dir(const char* input, int *string_count)
{
    char* str = strdup(input);
    int count = 0;
    int capacity = 10;
    char** result = malloc(capacity*sizeof(*result));

    char* tok=strtok(str,"/");

    while(1)
    {
        if (count >= capacity)
            result = realloc(result, (capacity*=2)*sizeof(*result));

        result[count++] = tok? strdup(tok) : tok;

        if (!tok) break;

        tok=strtok(NULL,"/");
    }
    *string_count = count - 1;

    free(str);
    return result;
}
//return target inode number. if not found return error.
int traverse_path(char **path, int path_count)
{
    single_i_node = malloc(sizeof(i_node));
    //open dir.
    open_for_reading(&from_disk);
    read_super_block(&from_disk);
    int i,j,k;
    char **it;
    it = path;
    int current_i_node = -1;
    int found = -1;

    int path_counter = 0;

    //read root dir
    read_single_i_node(&from_disk, 2);

    load_current_dir(&from_disk);
    free(single_i_node);
    //read root dir.
    for(i = 0 ; i < path_count ; ++i)
    {
        //printf("P count %d it is %s\n",path_count, it[i] );
        for(j = 0; j < current_dir.used ; ++j)
        {
            //printf("Current dir size is %d\n", current_dir.used);
            //printf("Current dir name is %d\n", current_dir.used);
            //printf("Comparing%s %s \n",current_dir.file[j].file_name, it[i]);
            //printf("%d \t\t %s\n", current_dir.file[j].i_node, current_dir.file[j].file_name);
            if(strcmp(current_dir.file[j].file_name, it[i]) == 0)
            {
                //printf("Comparing%s %s FOUND\n",current_dir.file[j].file_name, it[i]);

                //we got the target directory, let's fetch it's inode no.
                current_i_node = current_dir.file[j].i_node;
                //if(get_bit(i_node_bit_map, j) == FREE)
                //{
                //    fprintf(stderr, "%s\n", "Path not found(bit).");
                //    exit(1);
                //}
                if(current_dir.file[j].type == _FILE)
                {
                    //fprintf(stderr, "%s\n", "Can't traverse into file.Exiting program");

                }
                else
                {
                    single_i_node = malloc(sizeof(i_node));
                    read_single_i_node(&from_disk, current_i_node);
                    free_dir(&current_dir);
                    load_current_dir(&from_disk);
                    for(k = 0 ; k < current_dir.used ; ++k)
                    {
                        //printf("Name %s\t", current_dir.file[k].file_name);
                        //printf("Inode no->%d", current_dir.file[k].i_node);
                        //print_single_file_meta_data();
                    }
                    //printf("PRINTING CURRENT DIRECTORRY\n");
                    free(single_i_node);
                    found = 1;
                    break;
                }
            //free(single_i_node);

            }
            //printf("j and current dir %d %d\n", j, current_dir.used );

        }
        if((j == current_dir.used && found != 1) )
        {
            //FREE RESOURCES AND EXIT.
            fclose(from_disk);
            free_dir(&current_dir);
            for(it=path; it && *it; ++it)
            {

                free(*it);
            }
            //free_dir(&current_dir);
            free(path);

            fprintf(stderr, "%s\n", "Path not found.");
            _exit(1);
        }
        found = -1;


    }



    fclose(from_disk);
    for(it=path; it && *it; ++it)
    {

        free(*it);
    }
    //free_dir(&current_dir);
    //print_i_node_bit_map();
    //destroy_bitvec(i_node_bit_map);
    free(path);
    return found;
}

/* file size */ /*file type*/ /*Last Mod*/ /*file_name*/
//assume that you have the desired directory here.
void list()
{

    open_for_reading(&from_disk);
    read_super_block(&from_disk);
    int i;

    for(i = 0; i < current_dir.used ; ++i)
    {
        single_i_node = malloc(sizeof(i_node));
        printf("%s\t", current_dir.file[i].file_name);
        printf("%d\t", current_dir.file[i].i_node);
        read_single_i_node(&from_disk, current_dir.file[i].i_node);
        print_single_file_meta_data();
        free(single_i_node);
    }
    fclose(from_disk);
    free_dir(&current_dir);

}

void print_single_file_meta_data()
{
    if(single_i_node -> type == _DIRECTORY)
    {
        printf("%s\t", "DIRECTORY" );
    }
    else if(single_i_node -> type == _FILE)
    {
        printf("%s\t", "REGULAR FILE" );
    }

    printf("%d\t", single_i_node -> file_size );
    month_print(single_i_node -> month);
    printf("%02d\t", single_i_node -> day);
    printf("%02d:%02d:%02d\n", single_i_node->hour, single_i_node->minute, single_i_node->second);



}



void month_print(int month_no)
{
    switch(month_no)
    {
        case 1:
            printf("Jan ");
            break;

        case 2:
            printf("Feb ");
            break;
        case 3:
            printf("Mar ");
            break;
        case 4:
            printf("Apr ");
            break;
        case 5:
            printf("May ");
            break;
        case 6:
            printf("Jun ");
            break;
        case 7:
            printf("Jul ");
            break;
        case 8:
            printf("Aug ");
            break;
        case 9:
            printf("Sept ");
            break;
        case 10:
            printf("Oct ");
            break;
        case 11:
            printf("Nov ");
            break;
        case 12:
            printf("Dec ");
            break;
        default:
            printf("%s\n", "Wrong selection" );
            break;
    }
}


void _mkdir(const char dir_name[255])
{

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int free_i_node_bit_map_index;
    int free_data_bit_map_index;
    int i;
    int parent_dir;
    long int parent_data_loc;
    int parent_i_node_no;
    open_for_reading(&from_disk);
    read_super_block(&from_disk);

    //get the parent dir inode no.
    for(i = 0 ; i < current_dir.used ; ++i)
    {
        //printf("%s\n",current_dir.file[i].file_name);
        if(strcmp(current_dir.file[i].file_name, ".") == 0)
        {
            parent_dir = current_dir.file[i].i_node;
        }
    }
    load_i_node_bit_map(&from_disk); // load the inodebitmap


    free_i_node_bit_map_index = find_free_i_node_bit_map();

    if(free_i_node_bit_map_index == -1)
    {
        fprintf(stderr, "%s\n","i nodes are full. Exiting the program.");
        _exit(0);
    }
    set_bit(i_node_bit_map,free_i_node_bit_map_index, NOTFREE ); //modify it.

    //print_i_node_bit_map();
    fclose(from_disk); //close reading fp
    open_for_writing(&to_disk); //open writing fp
    write_single_i_node_bit_map(&to_disk, i_node_bit_map); //write to specified location.
    //print_i_node_bit_map();
    fclose(to_disk); //close
    destroy_bitvec(i_node_bit_map); //free



    //read data bit map
    open_for_reading(&from_disk);
    load_data_bit_map(&from_disk);

    free_data_bit_map_index = find_free_data_bit_map();
    //print_data_bit_map();


    if(free_data_bit_map_index == -1)
    {
        fprintf(stderr, "%s\n","i nodes are full." );
        exit(1);
    }
    set_bit(data_bit_map,free_data_bit_map_index, NOTFREE ); //modify it.

    fclose(from_disk);

    open_for_writing(&to_disk); //open writing fp
    write_single_data_bit_map(&to_disk, data_bit_map); //write to specified location.
    fclose(to_disk); //close
    destroy_bitvec(data_bit_map); //free


    open_for_reading(&from_disk);
    //get current directory i node.
    single_i_node = malloc(sizeof(i_node));
    read_single_i_node(&from_disk, parent_dir);
    parent_data_loc = single_i_node -> direct_ptrs[0];

    free(single_i_node);


    fclose(from_disk);





    /*single_i_node = malloc(sizeof(i_node));
    set_default_i_nodes(single_i_node,pow(2,gtu_superblock.log_size_of_block), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,tm.tm_hour ,tm.tm_min,tm.tm_sec,_DIRECTORY);
    single_i_node.direct_ptrs[0] = gtu_superblock.data_blocks_starting_addr + pow(2,gtu_superblock.log_size_of_block) *free_data_bit_map_index;*/

    open_for_writing(&to_disk);
    file new_file;
    new_file.i_node = free_i_node_bit_map_index;
    new_file.file_name = malloc(strlen(dir_name) +1);
    strcpy(new_file.file_name, dir_name);
    new_file.type = _DIRECTORY;

    add_file(&current_dir, new_file);
    free(new_file.file_name);

    fseek(to_disk, parent_data_loc, SEEK_SET);
    fprintf(to_disk, "%d\n", current_dir.used);
    for(i = 0 ; i < current_dir.used ; ++i)
    {
        fprintf (to_disk, "%d`%d`%s\n",current_dir.file[i].i_node, current_dir.file[i].type, current_dir.file[i].file_name);
    }

    fclose(to_disk);
    free_dir(&current_dir);

    open_for_writing(&to_disk);
    single_i_node = malloc(sizeof(i_node));
    set_default_i_nodes(single_i_node,pow(2,gtu_superblock.log_size_of_block), tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,tm.tm_hour ,tm.tm_min,tm.tm_sec,_DIRECTORY);
    single_i_node->direct_ptrs[0] = gtu_superblock.data_blocks_starting_addr + pow(2,gtu_superblock.log_size_of_block) *free_data_bit_map_index;
    write_single_i_node(&to_disk, free_i_node_bit_map_index);
    free(single_i_node);
    fclose(to_disk);




    directory new_dir;

    file dot,dot_dot;

    dot.i_node = free_i_node_bit_map_index;
    dot.file_name=malloc(strlen(".") + 1);
    strcpy(dot.file_name,".");
    dot.type = _DIRECTORY;

    dot_dot.i_node = parent_dir;
    dot_dot.file_name=malloc(strlen("..") + 1);
    strcpy(dot_dot.file_name,"..");
    dot_dot.type = _DIRECTORY;


    // Init array, don't forget
    init_directory(&new_dir, 100);

    // Add elements
    add_file(&new_dir, dot);
    add_file(&new_dir, dot_dot);

    open_for_writing(&to_disk);
    fseek(to_disk, gtu_superblock.data_blocks_starting_addr + pow(2,gtu_superblock.log_size_of_block) *free_data_bit_map_index, SEEK_SET);
    fprintf(to_disk, "%d\n", new_dir.used);
    for(i = 0 ; i < new_dir.used ; ++i)
    {
        fprintf (to_disk, "%d`%d`%s\n",new_dir.file[i].i_node, new_dir.file[i].type, new_dir.file[i].file_name);
    }
    //print_all_i_node_detailed();


    free_dir(&new_dir);
    free(dot.file_name);
    free(dot_dot.file_name);
    fclose(to_disk);


}


void _rmdir(const char dir_name[255])
{
    open_for_reading(&from_disk);
    read_super_block(&from_disk);
    int exist = -1;
    int i;
    int dir_i_node;
    int parent_i_node_no;
    int data_bit_no;
    long int dir_data_loc;
    long int parent_data_loc;


    //GET THE PARENT DIRECTORY INODE NO.

    for(i = 0 ; i < current_dir.used ; ++i)
    {

        if(strcmp(current_dir.file[i].file_name, ".") == 0)
        {
            parent_i_node_no = current_dir.file[i].i_node;
            break;
        }
    }

    //GET THE CANDIDATE DIRECTORY INODE NO.
    for(i = 0 ; i < current_dir.used ; ++i)
    {

        if(strcmp(dir_name, current_dir.file[i].file_name) == 0)
        {
            exist = 1;
            dir_i_node = current_dir.file[i].i_node;

            break;
        }
    }
    //IF THAT DIRECTORY DOESNT EXIST PRINT ERROR MESSAGE AND EXIT
    if(exist != 1)
    {
        free_dir(&current_dir);
        fclose(from_disk);
        fprintf(stderr, "%s\n","directory doesn't exist.");
        exit(1);
    }
    free_dir(&current_dir);
    //WE DONE WITH PARENT DIRECTORY SO FREE IT.
    fclose(from_disk);
    //GET THE CANDIDATE INODE AND LOAD IT'S DIRECTORY TO SEE INSIDE.
    //ALSO SAVE THE STARTING ADDRESS OF THAT CANDIDATE INODE.
    single_i_node = malloc(sizeof(i_node));
    open_for_reading(&from_disk);

    read_single_i_node(&from_disk, dir_i_node);
    load_current_dir(&from_disk);
    for(i = 0 ; i < current_dir.used ; ++i)
    {

        //fprintf (stderr, "%d`%d`%s\n",current_dir.file[i].i_node, current_dir.file[i].type, current_dir.file[i].file_name);


    }

    dir_data_loc = single_i_node -> direct_ptrs[0];
    free(single_i_node);
    fclose(from_disk);

    if(current_dir.used != 2)
    {

        fprintf(stderr, "%s\n","You can't remove a directory with files/directories." );

        exit(1);
    }
    free_dir(&current_dir);

    //LOAD BOTH BITMAPS. SET INODE BITMAP (EASY ONE)
    //SET DATA BIT MAP-> MAKE OFFSET CALCULATION
    open_for_reading(&from_disk);
    load_i_node_bit_map(&from_disk);
    load_data_bit_map(&from_disk);
    set_bit(i_node_bit_map,dir_i_node, FREE);



    data_bit_no = (dir_data_loc - gtu_superblock.data_bit_map_starting_addr) / pow(2,gtu_superblock.log_size_of_block);
    set_bit(data_bit_map, data_bit_no, FREE);


    fclose(from_disk);
    free_dir(&current_dir);
    //WRITE BACK BITMAPS
    open_for_writing(&to_disk);

    write_single_data_bit_map(&to_disk, data_bit_map);
    write_single_i_node_bit_map(&to_disk, i_node_bit_map);

    destroy_bitvec(data_bit_map);
    destroy_bitvec(i_node_bit_map);

    fclose(to_disk);
    //NOW WE CAN REMOVE THE FILE
    file removed_file;
    removed_file.i_node = dir_i_node;
    removed_file.file_name = malloc(strlen(dir_name) + 1);
    strcpy(removed_file.file_name, dir_name);

    //GET THE PARENT INODE AGAIN
    single_i_node = malloc(sizeof(i_node));
    open_for_reading(&from_disk);
    read_single_i_node(&from_disk, parent_i_node_no);
    parent_data_loc = single_i_node -> direct_ptrs[0];
    load_current_dir(&from_disk);
    free(single_i_node);
    fclose(from_disk);
    open_for_writing(&to_disk);
    remove_file(&current_dir, removed_file);

    fseek(to_disk, parent_data_loc, SEEK_SET);
    fprintf(to_disk, "%d\n", current_dir.used);
    for(i = 0 ; i < current_dir.used ; ++i)
    {
        fprintf (to_disk, "%d`%d`%s\n",current_dir.file[i].i_node, current_dir.file[i].type, current_dir.file[i].file_name);
    }
    free_dir(&current_dir);
    free(removed_file.file_name);
    fclose(to_disk);

}

int find_free_i_node_bit_map()
{
    int i;
    int ret = -1;
    for(i = 3 ; i < gtu_superblock.i_node_count ; ++i)
    {
        if(get_bit(i_node_bit_map, i) == FREE)
        {
            ret = i;

        }
    }
    if(ret == -1)
    {
        fprintf(stderr, "%s\n","Run out of inodes. Exiting the prog.");
        exit(0);
    }
    return ret;
}

int find_free_data_bit_map()
{
    int i;
    int ret = -1;
    for(i = 1 ; i < gtu_superblock.data_block_count ; ++i)
    {
        if(get_bit(data_bit_map, i) == FREE)
        {
            ret = i;

        }
    }
    if(ret == -1)
    {
        fprintf(stderr, "%s\n","Run out of data entries.. Exiting program.");
        fprintf(stderr, "%s\n", "Disk is fragmented. If you want to continue testing, you need to load new disk from part2.If you run this program with old disk file, weird things will occur. Please don't do that.\n" );

         printf("Terminating in 3 seconds...\n" );
         sleep(3);
        _exit(0);
    }
    return ret;
}


void print_all_i_node_detailed()
{

    int i;
    open_for_reading(&from_disk);

    for(i = 0 ; i < gtu_superblock.i_node_count ; ++i)
    {
        printf("\n\n\n");
        single_i_node = malloc(sizeof(i_node));
        read_single_i_node(&from_disk, i);
        print_meta_data();

        free(single_i_node);
        printf("\n\n\n");
    }

    fclose(from_disk);

}


void remove_file(directory *a, file _file)
{

    int i;
    int x;
    int delete_pos = -1;

    for(i = 0 ; i < a -> used ; ++i)
    {
        if((strcmp(a -> file[i].file_name, _file.file_name) == 0) &&
           (a -> file[i].i_node == _file.i_node))
        {
               delete_pos = i;
               break;
        }


    }
    if(delete_pos >= a -> used )
    {
    }

  /*for(x = delete_pos + 1 ; x < a -> used ; ++x)
    {
        printf("?\n" );
        //free(a -> file[x - 1].file_name);
        //a -> file[x - 1].file_name = (char*)malloc(strlen( a -> file[x].file_name) + 1);
        strncpy(a -> file[x - 1].file_name, a -> file[x].file_name, strlen(a -> file[x].file_name));
    }*/

    free(a->file[delete_pos].file_name);
    a->file[delete_pos].file_name=NULL;
    memmove(a -> file + delete_pos, a -> file + delete_pos + 1, ((a -> used) - delete_pos - 1 ) * sizeof(file));
    a->used--;

    a->file = realloc(a->file, (a -> used) * sizeof(file));

}


/*void read_linux_file_into_2d(char linux_file_name[255], char **linux_file_write, int block_size)
{
    size_t buf_size;
    int i;
    float block_count;
    FILE *from_linux_file;
    long file_size;
    from_linux_file = fopen(linux_file_name, "r");
    if(from_linux_file == NULL)
    {
        fprintf(stderr, "%s\n", "Couldn't open linux file.");
        exit(EXIT_FAILURE);
    }

    fseek(from_linux_file, 0, SEEK_END);
    file_size = ftell(from_linux_file);
    fseek(from_linux_file, 0, SEEK_SET);

    block_count = (int)ceil(file_size / block_size);
    printf("block count is %d\n",block_count );


    linux_file_write = (char **) calloc(block_count, sizeof(char *));

    for(i = 0 ; i < block_count ; ++i)
    {
        linux_file_write[i] = (char *) calloc(block_size, sizeof(char));
    }

    while((buf_size = fread(linux_file_write[i], sizeof(char), block_size - 1, from_linux_file)))
    {
        printf("kopko\n" );
        //linux_file_write[block_size] = '\0';
    }
    fclose(from_linux_file);
}

*/
void free_linux_file_into_2d(char **linux_file_write, int block_count)
{
    int i;
    for(i = 0 ; i < block_count ; ++i)
    {
        free(linux_file_write[i]);
    }
    free(linux_file_write);
}


void _write(const char linux_file_name[255], char base[])
{
    int reserved_blocks;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    FILE* linux_file;
    int block_size;
    int i;
    int current_dir_i_node_no;
    int current_data_location;
    int new_file_i_node_no;
    open_for_reading(&from_disk);
    read_super_block(&from_disk);
    load_i_node_bit_map(&from_disk);
    fclose(from_disk);

    block_size = pow(2, gtu_superblock.log_size_of_block);


    for(i = 0 ; i < current_dir.used ; ++i)
    {
        if(strcmp(current_dir.file[i].file_name, ".") == 0)
        {
            current_dir_i_node_no = current_dir.file[i].i_node;
            break;
        }
    }


    open_for_reading(&from_disk);
    single_i_node = malloc(sizeof(i_node));
    read_single_i_node(&from_disk, current_dir_i_node_no);
    current_data_location = single_i_node -> direct_ptrs[0];
    free(single_i_node);
    fclose(from_disk);



    open_for_writing(&to_disk);
    new_file_i_node_no = find_free_i_node_bit_map();
    set_bit(i_node_bit_map,new_file_i_node_no, NOTFREE );
    write_single_i_node_bit_map(&to_disk, i_node_bit_map);
    destroy_bitvec(i_node_bit_map);
    fclose(to_disk);


    file new_file;
    new_file.i_node = new_file_i_node_no;
    new_file.file_name = malloc(strlen(base) +1);
    strcpy(new_file.file_name, base);
    new_file.type = _FILE;
    add_file(&current_dir, new_file);


    open_for_writing(&to_disk);
    fseek(to_disk, current_data_location, SEEK_SET);
    fprintf(to_disk, "%d\n", current_dir.used);
    for(i = 0 ; i < current_dir.used ; ++i)
    {
        fprintf (to_disk, "%d`%d`%s\n",current_dir.file[i].i_node, current_dir.file[i].type, current_dir.file[i].file_name);
    }
    free(new_file.file_name);
    free_dir(&current_dir);
    fclose(to_disk);


    open_for_reading(&from_disk);
    load_data_bit_map(&from_disk);
    fclose(from_disk);


    linux_file = fopen(linux_file_name, "r");
    if(linux_file == NULL)
    {
        fprintf(stderr, "%s\n", "Error opening linux file.");
        exit(EXIT_FAILURE);
    }
    int block_count;
    fseek(linux_file,0,SEEK_END);
    int file_size = ftell(linux_file);

    fseek(linux_file,0,SEEK_SET);
    block_count =  (file_size / block_size) + 1;


    int curr_data_index = 0;
    int raw_addr;
    int count = 0;
    int current_block = 0;
    char ch;
    int bytes;
    int data_index = 0;
    int data_offset = 0;
    open_for_writing(&to_disk);
    single_i_node = malloc(sizeof(i_node));
    set_default_i_nodes(single_i_node,file_size, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,tm.tm_hour ,tm.tm_min,tm.tm_sec,_FILE);

    char *buffer = calloc(block_size, sizeof(char));
    if(buffer == NULL)
    {
        fprintf(stderr, "%s\n", "Failed to allocate mem.");
    }
    memset(buffer,0,strlen(buffer));
    int indirect_counter = 0;
    int indirect_data_index;

    for(i = 0 ; i < 200 ; ++i)
    {
        indirect_block.indirectptrs[i] = -99;
    }
    for(i = 0 ; i < block_count ; ++i)
    {

        bytes = fread(buffer, sizeof(char), block_size, linux_file);
        if(i <= 8)
        {
            data_index = find_free_data_bit_map();
            if(data_index == -1)
            {
                fprintf(stderr, "%s\n","No more data entries,exiting" );
                exit(0);
            }
            fseek(to_disk, gtu_superblock.data_bit_map_starting_addr, SEEK_SET);
            set_bit(data_bit_map, data_index, NOTFREE);
            write_single_data_bit_map(&to_disk, data_bit_map);
            data_offset = gtu_superblock.data_blocks_starting_addr + data_index * block_size;
            fseek(to_disk, data_offset, SEEK_SET);
            fwrite(buffer, sizeof(char), block_size, to_disk);
            single_i_node->direct_ptrs[i] = data_offset;
        }
        else if(i > 8 && i <= 208) //one level of indirection
        {
            if(i == 9)
            {
                //create indirect data block. write bit map
                indirect_data_index = find_free_data_bit_map();
                set_bit(data_bit_map, indirect_data_index, NOTFREE);
                fseek(to_disk, gtu_superblock.data_bit_map_starting_addr, SEEK_SET);
                write_single_data_bit_map(&to_disk, data_bit_map);
                single_i_node ->indirect_ptr = gtu_superblock.data_blocks_starting_addr + block_size * indirect_data_index;
                //find index for data.
                data_index = find_free_data_bit_map();
                set_bit(data_bit_map, data_index, NOTFREE);
                fseek(to_disk, gtu_superblock.data_bit_map_starting_addr, SEEK_SET);
                write_single_data_bit_map(&to_disk, data_bit_map);


                data_offset = gtu_superblock.data_blocks_starting_addr + data_index * block_size;
                fseek(to_disk, data_offset, SEEK_SET);
                fwrite(buffer, sizeof(char), block_size, to_disk);
                indirect_block.indirectptrs[indirect_counter++] = data_offset;
                fseek(to_disk, gtu_superblock.data_blocks_starting_addr + indirect_data_index * block_size, SEEK_SET);
                write_indirect_block(&to_disk);
             }
             else
             {

                 data_index = find_free_data_bit_map();
                 set_bit(data_bit_map, data_index, NOTFREE);
                 fseek(to_disk, gtu_superblock.data_bit_map_starting_addr, SEEK_SET);
                 write_single_data_bit_map(&to_disk, data_bit_map);


                 data_offset = gtu_superblock.data_blocks_starting_addr + data_index * block_size;
                 fseek(to_disk, data_offset, SEEK_SET);
                 fwrite(buffer, sizeof(char), block_size, to_disk);
                 indirect_block.indirectptrs[indirect_counter++] = data_offset;
                 fseek(to_disk, gtu_superblock.data_blocks_starting_addr + indirect_data_index * block_size, SEEK_SET);
                 write_indirect_block(&to_disk);
             }

            //write indirect ptr.
        }


        memset(buffer,0,strlen(buffer));

    }









    free(buffer);




    fclose(linux_file);
    write_single_i_node(&to_disk, new_file_i_node_no);
    free(single_i_node);
    fclose(to_disk);
    destroy_bitvec(data_bit_map);

}

void _dumpe2fs()
{
    printf("======================DUMPE2FS======================\n");


    open_for_reading(&from_disk);

    read_super_block(&from_disk);

    load_i_node_bit_map(&from_disk);
    load_data_bit_map(&from_disk);
    print_i_node_bit_map();
    printf("=====================================================\n");
    destroy_bitvec(i_node_bit_map);

    print_data_bit_map();
    printf("=====================================================\n");
    printf("Total block count: %d\n\n",gtu_superblock.total_no_blocks);
    printf("I node count: %d\n\n", gtu_superblock.i_node_count );
    printf("Block size %d\n", (int)pow(2, gtu_superblock.log_size_of_block));
    printf("=====================================================\n");
    destroy_bitvec(data_bit_map);
    fclose(from_disk);
    printf("======================DUMPE2FS======================\n");
    printf("<Scroll up to see more info, or redirect the output to file.>\n" );
}

void _read(const char linux_file_name[255], char base[])
{

    int found_file = -1;
    int i;
    int file_i_node_no;


    for(i = 0 ; i < current_dir.used ; ++i)
    {
        if(strcmp(current_dir.file[i].file_name, base) == 0)
        {
            file_i_node_no = current_dir.file[i].i_node;
            found_file = 1;
        }
    }

    if(found_file == -1)
    {
        fprintf(stderr, "%s\n","No such file, exiting the program" );
        exit(EXIT_FAILURE);
    }

    open_for_reading(&from_disk);
    read_super_block(&from_disk);
    int block_size = pow(2,gtu_superblock.log_size_of_block);
    single_i_node = malloc(sizeof(i_node));
    read_single_i_node(&from_disk, file_i_node_no);
    int a,b;
    char *buffer = calloc(block_size, sizeof(char));

    FILE *linux_file;
    linux_file = fopen(linux_file_name, "w+");
    if(linux_file == NULL)
    {
        fprintf(stderr, "%s\n", "Error opening linux file.");
        exit(EXIT_FAILURE);
    }
    for(i = 0 ; i <= 8 ;++i)
    {
        if(single_i_node->direct_ptrs[i] == -99)
        {
            break;
        }
        else
        {
            fseek(from_disk, single_i_node->direct_ptrs[i], SEEK_SET);
            fread(buffer, sizeof(char), block_size, from_disk);
            buffer[block_size - 1] = '\0';
            printf("%s\n",buffer );
            fwrite(buffer, sizeof(char), block_size, linux_file);
            memset(buffer,0,strlen(buffer));
        }

    }
    int indirect_addr;
    indirect_addr = single_i_node -> indirect_ptr;

    fseek(from_disk, indirect_addr, SEEK_SET);
    read_indirect_block(&from_disk);
    if(single_i_node -> indirect_ptr != -99)
    {
        for(i = 0 ; i < 200 ; ++i)
        {
            if(indirect_block.indirectptrs[i] != -99)
            {
                fseek(from_disk,indirect_block.indirectptrs[i], SEEK_SET );
                fread(buffer, sizeof(char), block_size, from_disk);
                buffer[block_size - 1] = '\0';
                fwrite(buffer, sizeof(char), block_size, linux_file);
                memset(buffer,0,strlen(buffer));
            }

        }
    }




    free(buffer);
    fclose(from_disk);
    free(single_i_node);
    fclose(linux_file);


}


void write_indirect_block(FILE **to_disk)
{
    if(fwrite (&indirect_block, sizeof(indirect), 1, *to_disk) != 0)
    {
        //fprintf(stderr, "%s\n","Wrote superblock to file correctly." );
    }
    else
    {
        fprintf(stderr, "%s\n","Error writing indirect block to file. Exiting program.");
    }
}

void read_indirect_block(FILE **from_disk)
{
    fread(&indirect_block, sizeof(indirect), 1, *from_disk);
}




void _delet(const char base_name[255])
{

    int i;
    int found = -1;
    open_for_reading(&from_disk);
    read_super_block(&from_disk);
    fclose(from_disk);
    int deleted_file_i_node_no;
    int dir_i_node_no;
    int dir_data_loc;

    for(i = 0 ; i < current_dir.used ; ++i)
    {fprintf (stderr, "number six%s\n", current_dir.file[i].file_name);
        if((strcmp(current_dir.file[i].file_name, base_name) == 0) && current_dir.file[i].type == _FILE)
        {
            fprintf (stderr, "number seven%s\n", current_dir.file[i].file_name);

            deleted_file_i_node_no = current_dir.file[i].i_node;
            found = 1;
            break;
        }
    }

    for(i = 0 ; i < current_dir.used ; ++i)
    {
        if(strcmp(current_dir.file[i].file_name, ".") == 0)
        {
            fprintf (stderr, "%d`%d`%s\n",current_dir.file[i].i_node, current_dir.file[i].type, current_dir.file[i].file_name);

            deleted_file_i_node_no = current_dir.file[i].i_node;
        }
    }
    printf("There ?\n" );
    if(found == -1)
    {
        fprintf(stderr, "%s\n", "No such file, terminating program.");
        _exit(EXIT_FAILURE);
    }

    file removed_file;
    printf("DELETED FILE I NODE NO IS %d\n",deleted_file_i_node_no );
    removed_file. i_node = deleted_file_i_node_no;
    printf("How about that\n");
    removed_file.file_name = malloc(strlen(base_name) + 1);
    strcpy(removed_file.file_name, base_name);
    printf("How about that\n");
    remove_file(&current_dir, removed_file);
    printf("\n");
    open_for_reading(&from_disk);
    single_i_node = malloc(sizeof(i_node));
    read_single_i_node(&from_disk, dir_i_node_no);
    dir_data_loc = single_i_node->direct_ptrs[0];
    free(single_i_node);
    fclose(from_disk);


    open_for_writing(&to_disk);
    fseek(to_disk, dir_data_loc, SEEK_SET);
    fprintf(to_disk, "%d\n", current_dir.used);
    for(i = 0 ; i < current_dir.used ; ++i)
    {
        fprintf (to_disk, "%d`%d`%s\n",current_dir.file[i].i_node, current_dir.file[i].type, current_dir.file[i].file_name);
    }
    fclose(to_disk);


}

void _fsck()
{
    int i = 0;
    open_for_reading(&from_disk);
    read_super_block(&from_disk);
    load_i_node_bit_map(&from_disk);
    load_data_bit_map(&from_disk);
    for(i = 0 ; i < gtu_superblock. i_node_count ; ++i)
    {
        printf("%d \t",get_bit(i_node_bit_map,i));
        if(i % 10 == 0)
        {
            printf("\n");
        }
    }
    printf("\n\n" );
    printf("=================================================================\n" );
    printf("\n\n" );
    for(i = 0 ; i < gtu_superblock. data_block_count ; ++i)
    {
        printf("%d \t",get_bit(data_bit_map,i));
        if(i % 10 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");


    destroy_bitvec(i_node_bit_map);
    destroy_bitvec(data_bit_map);

    fclose(from_disk);


}
