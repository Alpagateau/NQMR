#include "nqmt_fs.hpp"

namespace NQMT{

int initFS()
{
    if(!fatInitDefault())
      return 1;
    //chdir(NQMT_ROOT);
    
    if(!nitroFSInit(NULL))
      printf("Couldnt load nitrofs");
    chdir(NQMT_ROOT); 
    return 0;
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
    //NE_ModelRotate(model, 0, 1, 0);
    // Render
    //NE_ProcessInput();  // Updates camera, etc.
		

}
