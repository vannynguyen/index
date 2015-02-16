/*	indexer.c	Tiny Search Engine indexer

	Project name: Tiny Search Engine
	Component name: indexer
	
	Primary Author:	Vanny Nguyen

	Special considerations:  
		(e.g., special compilation options, platform limitations, etc.) 
	
======================================================================*/
// do not remove any of these sections, even if they are empty
//
// ---------------- Open Issues 

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf
#include <math.h>
#include <unistd.h>                          //access
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>                        //stats
#include <errno.h>                           //errors
#include <getopt.h>

// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "web.h"                             // curl and html functionality
#include "list.h"                            // webpage list functionality
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"                           // utility stuffs

// ---------------- Constant definitions 

// ---------------- Macro definitions

// ---------------- Structures/Types 

// ---------------- Private variables 

// ---------------- Private prototypes 

/*====================================================================*/
int main(int argc, char* argv[])
{	char c;
	char *directory;
	char *resultsfile;

    /*=======================================================================================================================*/
    struct stat s;
    int err;
	if (argc == 1 || strstr(argv[1],"--help")){
		printf ("Usage: %s [TARGET_DIRECTORY] [RESULTS FILENAME] [RESULTS FILENAME] [REWRITTEN FILENAME]\n", argv[0]); //help command
		printf("\n\n");
		printf("[TARGET_DIRECTORY]: MUST BE A VALID, CREATED DIRECTORY OF CURLED WEBPAGES TO INDEX\n\n\n");
		printf("[RESULTS FILENAME]: MUST BE A VALID, CREATED FILE FOR RECORDING INDICES\n\n\n");\
		printf("[REWRITTEN FILENAME]: MUST BE A VALID, CREATED FILE FOR RE-RECORDING INDICES\n\n\n");
		return 0;
	}

    while ((c = getopt (argc, argv, "")) > 0) {

        if (c == -1){ //No options left
            break;
        }

        switch (c) { //Switch the options
         default:
            fprintf (stderr,"Invalid argument for %s\n", argv[0]); //Bad argument default

        }
    }

    argc -= optind; //Decrement argument number
    argv += optind; //Increment argument pointer
    //check for sufficient arguments
    if(argc!=2 && argc!=4){
        fprintf(stderr, "Insufficient arguments\n");
        return 1;
    }

    //check if directory exists
    err = stat(argv[0],&s);
    if(-1 == err){
        if(ENOENT == errno) {
            //does not exist
            fprintf(stderr,"Directory does not exist\n");
            return 1;
        }

    }
    else {
        if(!S_ISDIR(s.st_mode)){
            //exists but not a directory
            fprintf(stderr,"File is not a directory\n");
            return 1;
        }
    }

    if(argc==4){
	    //check if result names are the same
	    if(strcmp(argv[1],argv[2])!=0){
	    	fprintf (stderr,"Result filenames (%s and %s) do not match\n", argv[1],argv[2]);
	    	return 1;
	    }
	    //check if result file exists
	    if( access( argv[1], F_OK ) == -1 ) {
	    // file doesn't exists
	    	fprintf (stderr,"Result file (%s) does not exist", argv[1]);
	    	return 1;
		}
		//check if rewritten file exists
	    if( access( argv[3], F_OK ) == -1 ) {
	    // file doesn't exists
	    	fprintf (stderr,"Rewrite file (%s) does not exist", argv[3]);
	    	return 1;
		}
	}
	
	/*=======================================================================================================================*/

	
	directory=calloc(1,sizeof(char)*strlen(argv[0])+1);
    strcpy(directory,argv[0]);
	return 0;
}
