#ifndef NQMT_FS_H 
#define NQMT_FS_H

#include <nds.h>
#include <stdio.h>
#include <fat.h>
//#include <filesystem.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h> 
//#include "nf_fat.h"

#define NQMT_ROOT "/NQMT/" //Root folder of the game files

/*
*  Initialise the filesystem. Automatically opens the folder at NQMT_ROOT
*/
int initFS();

/*
* List files dans directories
*/
int listDir();

#endif