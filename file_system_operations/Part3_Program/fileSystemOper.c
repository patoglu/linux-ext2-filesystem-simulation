#include "blocks.h"

long int current_fptr;
superblock gtu_superblock;
i_node *single_i_node;
directory current_dir;
bitvector *data_bit_map;
bitvector *i_node_bit_map;
FILE *from_disk;
FILE *to_disk;
char disk[255];
char operation[20];
char _path[255];
int total_dir_count = 0;
int total_file_count = 0;
indirect indirect_block;

//
int main(int argc, char const *argv[]) {
    /* code */
    int i;
    int path_count = 0;
    int selection = -1;
    char *base;
    char** tokens;
    char stripped[255];
    char** linux_file_write = NULL;
    char linux_file_name[255];
    //directory current_dir;
    //single_i_node = malloc(sizeof(i_node));

    if(argc < 3)
    {
        fprintf(stderr, "%s\n", "Whoops, bad parameters. Please don't try to explode my program :>");
        exit(0);
    }
    else
    {
        if(argc == 3)
        {
            strncpy(disk, argv[1], strlen(argv[1]));

            strncpy(operation, argv[2], strlen(argv[2]));
        }
        else if(argc == 4)
        {
            strncpy(disk, argv[1], strlen(argv[1]));

            strncpy(operation, argv[2], strlen(argv[2]));
            strncpy(_path, argv[3], strlen(argv[3]));


        }
        else if(argc == 5)
        {
            strncpy(disk, argv[1], strlen(argv[1]));

            strncpy(operation, argv[2], strlen(argv[2]));
            strncpy(_path, argv[3], strlen(argv[3]));
            strncpy(linux_file_name, argv[4], strlen(argv[4]));

            //linux_file_name[strlen(argv[4])] = '\0';

        }


    }


    if(argc == 3)
    {
        if(strcmp(operation, "dumpe2fs") == 0)
        {


            _dumpe2fs();
        }
        else if(strcmp(operation, "fsck") == 0)
        {
            _fsck();
        }


    }


    else if(argc == 4)
    {
        if(strcmp(operation, "list") == 0)
        {

            tokens = tokenize_dir(argv[3], &path_count);

            //printf("aa%s\n",_path );
            traverse_path(tokens, path_count);

            //printf("Seg\n" );
            list();


        }
        else if(strcmp(operation, "mkdir") == 0)
        {


            int path_len;
            base = (char*)basename(argv[3]);
            if(strlen(base) == 0)
            {
                fprintf(stderr, "%s\n","can't add / dir to /. Exiting prog." );
                exit(0);
            }

            path_len = strlen(_path);
            _path[path_len - strlen(base) - 1] = '\0';
            tokens = tokenize_dir(_path, &path_count);
            traverse_path(tokens, path_count);
            _mkdir(base);
        }

        else if(strcmp(operation, "rmdir") == 0)
        {
            int path_len;
            base = (char*)basename(argv[3]);
            if(strlen(base) == 0)
            {
                fprintf(stderr, "%s\n","can't remove / dir to /. Exiting prog." );
                exit(0);
            }

            path_len = strlen(_path);
            _path[path_len - strlen(base) - 1] = '\0';
            tokens = tokenize_dir(_path, &path_count);
            traverse_path(tokens, path_count);
            _rmdir(base);

        }


        else if(strcmp(operation, "del") == 0)
        {
            /*int path_len;
            base = (char*)basename(argv[3]);
            if(strlen(base) == 0)
            {
                fprintf(stderr, "%s\n","can't del / Exiting prog." );
                _exit(0);
            }
            path_len = strlen(_path);
            _path[path_len - strlen(base) - 1] = '\0';
            tokens = tokenize_dir(_path, &path_count);
            traverse_path(tokens, path_count);
            _delet(base);*/
            printf("Del is not implemented\n");
            _exit(0);
        }
    }
    else if(argc == 5)
    {



        if(strcmp(operation, "write") == 0)
        {
            open_for_reading(&from_disk);
            read_super_block(&from_disk);
            fclose(from_disk);



            int path_len;
            base = (char*)basename(argv[3]);
            if(strlen(base) == 0)
            {
                fprintf(stderr, "%s\n","can't add / file to /. Exiting prog." );
                exit(0);
            }

            path_len = strlen(_path);
            _path[path_len - strlen(base) - 1] = '\0';
            tokens = tokenize_dir(_path, &path_count);
            traverse_path(tokens, path_count);
            _write(argv[4], base);

            //read_linux_file_into_2d(linux_file_name, linux_file_write, (int)pow(2,gtu_superblock.log_size_of_block));

        }


        else if(strcmp(operation, "read") == 0)
        {
            open_for_reading(&from_disk);
            read_super_block(&from_disk);
            fclose(from_disk);



            int path_len;
            base = (char*)basename(argv[3]);
            if(strlen(base) == 0)
            {
                fprintf(stderr, "%s\n","can't add / file to /. Exiting prog." );
                exit(0);
            }
            path_len = strlen(_path);
            _path[path_len - strlen(base) - 1] = '\0';
            tokens = tokenize_dir(_path, &path_count);
            traverse_path(tokens, path_count);
            _read(argv[4], base);

            //read_linux_file_into_2d(linux_file_name, linux_file_write, (int)pow(2,gtu_superblock.log_size_of_block));

        }

    }









    //ADD DIR LIKE THIS.
    //traverse_path(tokens, path_count);
    //_mkdir(add_dir);





    return 0;
}
