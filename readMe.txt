I didn't have to chance with weird inputs like 999999 inodes etc, so it can explode.

If you get fragmentation, you will get noticed. After you get fragmentation
don't try to run the program with the same file because file may be get corrupted.
You need to load fresh file from part2 then.


Small, medium and large files are added for read/write testing.

Program uses indirect pointers but not triple one.

You can test the program with the same command line rules, unfortunately
i don't have enough time for put them for you but it uses ext2 file system like inode
strategy so it's robust enough.

I will upload demo later.
