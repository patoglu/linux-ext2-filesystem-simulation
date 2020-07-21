
#include "blocks.h"
long int current_fptr = 0 ;

int main(int argc, char const *argv[])
{
    int check_current =  0;
    FILE *to_disk;
    int i,j;
    int fd;
    char file_name[FILENAMESIZE];
    int i_node_count;
    int block_size;
    int total_block_count;
    int test[1024];
    char const static exit_message[] = "Exiting program...";
    char const static usage[] = "makeFileSystem <block_size> <free_inodes> <filename>";
    superblock gtu_superblock;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);


    if(argc != 4)
    {
        fprintf(stderr, "usage: %s\n", usage);
    }
    block_size = atoi(argv[1]) * 1024;
    i_node_count = atoi(argv[2]);


    memset(&file_name, 0, sizeof(file_name));
    strncpy(file_name, argv[3], strlen(argv[3]));
    //fd = creat(file_name, mode);
    create_disk(&to_disk, file_name, FILESIZE - 1);


    total_block_count = determine_total_block_count(block_size);
    set_i_node_count(&gtu_superblock, i_node_count);
    set_size_of_block(&gtu_superblock, log(block_size) / log(2));
    set_total_no_blocks(&gtu_superblock, total_block_count);
    set_inode_block_count(&gtu_superblock, get_i_node_count(&gtu_superblock), block_size);
    set_data_block_count(&gtu_superblock, total_block_count - get_inode_block_count(&gtu_superblock) - 3);


    bitvector *data_bit_map =  create_bitvec(get_data_block_count(&gtu_superblock));
    bitvector *i_node_bit_map =  create_bitvec(get_i_node_count(&gtu_superblock));
    //memset(i_node_bit_map->bits ,'\0', sizeof(i_node_bit_map->bits));
    for(i = 0 ; i <get_i_node_count(&gtu_superblock) ; ++i)
    {
        //printf("%d\n",i );
        set_bit(i_node_bit_map, i, FREE);
    }

    for(i = 0 ; i < (int)get_data_block_count(&gtu_superblock) ; ++i)
    {
        set_bit(data_bit_map, i, FREE);
    }
    set_bit(i_node_bit_map,2, NOTFREE ); // ROOT INODE.
    set_bit(data_bit_map, 0, NOTFREE); //root->data



    // START WRITING RESERVED AREA.


    //START WRITING SUPERBLOCK.
    int nth_block = 1;
    double current_block_count;
    fseek(to_disk, 0, SEEK_SET);
    //allocate inode table.
    gtu_superblock. i_node_starting_addr = block_size;
    //write_super_block(&to_disk, gtu_superblock);
    offset_inc(nth_block++, block_size);
    set_fp(&to_disk);

    //END WRITING SUPER BLOCK



    //START WRITING ALL INODES.
    i_node *whole_i_nodes = malloc(get_i_node_count(&gtu_superblock) * sizeof(i_node));
    for(i = 0 ; i < get_i_node_count(&gtu_superblock) ; ++i)
    {
        set_default_i_nodes(    &whole_i_nodes[i], DEFAULTINODEATTRIBUTE,
                            DEFAULTINODEATTRIBUTE, DEFAULTINODEATTRIBUTE,
                            DEFAULTINODEATTRIBUTE, DEFAULTINODEATTRIBUTE,
                            DEFAULTINODEATTRIBUTE, DEFAULTINODEATTRIBUTE,
                            _DIRECTORY);
    }





    //fill the inodes.

    int i_node_number_per_block = determine_i_node_number_per_block(block_size);
    gtu_superblock.i_node_block_count = (int) (get_i_node_count(&gtu_superblock) / i_node_number_per_block) + 1;
    for(i = 0 ; i < (int) (get_i_node_count(&gtu_superblock) / i_node_number_per_block) + 1; ++i)
    {
        for(j = 0 ; j < i_node_number_per_block ; ++j)
        {
            write_single_i_node(&to_disk, whole_i_nodes[j]);

        }

        offset_inc(nth_block++, block_size);
        set_fp(&to_disk);


    }
    gtu_superblock.i_node_bit_map_starting_addr = ftell(to_disk);

    //free the table after writing to file.

    // END WRITING INODES.


    //START WRITING INODE BITMAPS




    write_single_i_node_bit_map(&to_disk, i_node_bit_map);
    //printf("I am at.%ld\n",ftell(to_disk ));
    offset_inc(nth_block++, block_size);
    set_fp(&to_disk);
    //printf("Loop forward = %ld.\n", ftell(to_disk));
    gtu_superblock.data_bit_map_starting_addr = ftell(to_disk);
    // END WRITING INODE BITMAPS

    //printf("Writing data bitmap%d\n",ftell(to_disk) );
    //START WRITING DATA BIT MAP
    //printf("Writing data bitmap\n" );
    write_single_data_bit_map(&to_disk, data_bit_map);
    //printf("I am at.%ld\n",ftell(to_disk ));
    offset_inc(nth_block++, block_size);
    set_fp(&to_disk);
    //printf("Loop forward = %ld.\n", ftell(to_disk));

    gtu_superblock.data_blocks_starting_addr = ftell(to_disk);

    //END WRITING DATA BIT MAPS

    //goback to beginning.


    directory root;

    file file1,file2;

    file1.i_node = 2;
    file1.file_name=malloc(strlen(".") + 1);
    strcpy(file1.file_name,".");
    file1.type = _DIRECTORY;

    file2.i_node = 2;
    file2.file_name=malloc(strlen("..") + 1);
    strcpy(file2.file_name,"..");
    file2.type = _DIRECTORY;


    // Init array, don't forget
    init_directory(&root, 5);

    // Add elements
    add_file(&root, file1);
    add_file(&root, file2);





    /*directory *root = malloc(sizeof(directory));
    root -> size = 2;
    root->dir = malloc(root -> size * sizeof(dirent));
    root -> dir[0].i_node = 2;
    root -> dir[0].type = _DIRECTORY;
    memset(root->dir[0].file_name, '\0', sizeof(root->dir[0].file_name));
    strncpy(root->dir[0].file_name, ".", strlen("."));
    root -> dir[1].i_node = 2;
    root -> dir[1].type = _DIRECTORY;
    memset(root->dir[1].file_name, '\0', sizeof(root->dir[1].file_name));
    strncpy(root->dir[1].file_name, "..", strlen(".."));*/
    //show_dir(root);
    set_default_i_nodes(&whole_i_nodes[2],block_size, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,tm.tm_hour ,tm.tm_min,tm.tm_sec,_DIRECTORY);
    whole_i_nodes[2].direct_ptrs[0] = gtu_superblock.data_blocks_starting_addr;

    fseek(to_disk, 0, SEEK_SET);
    write_super_block(&to_disk, gtu_superblock);
    fseek(to_disk, gtu_superblock.i_node_starting_addr + 76 * 2, SEEK_SET);
    //printf("WRITING AT %d\n", ftell(to_disk));
    write_single_i_node(&to_disk, whole_i_nodes[2]);
    fseek(to_disk, gtu_superblock.data_blocks_starting_addr, SEEK_SET);

    fprintf(to_disk, "%d\n", root.used);
    for(i = 0 ; i < root.used ; ++i)
    {
        fprintf (to_disk, "%d`%d`%s\n",root.file[i].i_node, root.file[i].type, root.file[i].file_name);
    }

    long beg, end;
    beg = ftell(to_disk);
    fseek(to_disk, 0, SEEK_END);
    end = ftell(to_disk);
    //printf("beg and end %ld %ld\n",beg,end );
    //printf("Total data block count %ld\n",(end - beg) / block_size );

    free_dir(&root);

    free(file1.file_name);
    free(file2.file_name);

    fclose(to_disk);
    destroy_bitvec(i_node_bit_map);
    destroy_bitvec(data_bit_map);
    free(whole_i_nodes);


    return 0;
}
