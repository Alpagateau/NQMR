#include "nqme_fs.hpp"

namespace NQME{

int initFS()
{    
  if(!nitroFSInit(NULL))
    printf("[ERROR] Couldnt load nitrofs \n");
    
  chdir(NQME_ROOT); 
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

int listDir(const char *path)
{
    DIR *pdir;
    struct dirent *pent;

    pdir = opendir(path);

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
