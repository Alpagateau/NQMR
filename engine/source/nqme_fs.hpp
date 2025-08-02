#ifndef NQME_FS_H 
#define NQME_FS_H

#include <nds.h>
#include <stdio.h>
#include <fat.h>
#include <filesystem.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h> 
//#include "nf_fat.h"
#include "nqme_debug.hpp"

#define NQME_ROOT "nitro:/" //"/NQME/" //Root folder of the game files

namespace NQME{

/*
*  Initialise the filesystem. Automatically opens the folder at NQME_ROOT
*/
int initFS();

/*
* List files dans directories
*/
int listDir();

}
#endif
