#include "nqmt_fs.hpp"

namespace NQMT{

int initFS()
{
    if(fatInitDefault())
    {
        return chdir(NQMT_ROOT);
    }
    else 
    {
        return 1;
    }
}

int listDir()
{
    DIR *pdir;
    struct dirent *pent;

    pdir = opendir(".");

    if (pdir){

		while ((pent=readdir(pdir))!=NULL) {
	    	if(strcmp(".", pent->d_name) == 0 || strcmp("..", pent->d_name) == 0)
	        	continue;
	    	if(pent->d_type == DT_DIR)
	        	printf("[%s]\n", pent->d_name);
	    	else
	        	printf("%s\n", pent->d_name);
		}
		closedir(pdir);
	} else {
		printf ("opendir() failure; terminating\n");
        return 1;
	}
    return 0;
}

}