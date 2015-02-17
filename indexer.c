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
int buildIndex(char *docname);
WordNode* makeWordNode(char *wp);
int isInHash(HashTable *table, char *wp);
int addToHashTable(char *wp, HashTable *hash, WordNode *wnp);
int hasCurrentDoc(WordNode *wp, char *docn);
DocumentNode* makeDocumentNode(char *docn);
DocumentNode* getDocumentNode(WordNode *wnode,char *docn);
int main(int argc, char* argv[])
{	char c;
	char *directory;
	char *resultsfile;

    /*Program parameter processing===============================================================================================*/
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

		//store rewriting filename
    	char *rewritingfile;
    	rewritingfile=calloc(1,sizeof(char)*strlen(argv[3])+1);
	}

	/*=======================================================================================================================*/
	
	/*Initialize data structures, allocate Inverted_index, zero it, and set links to NULL.===================================*/

	//store target directory name
	directory=calloc(1,sizeof(char)*strlen(argv[0])+1);
    strcpy(directory,argv[0]);

    //store resulting filename
    resultsfile=calloc(1,sizeof(char)*strlen(argv[1])+1);

    //HASHTABLE
    HashTable *hash = calloc(1,sizeof(HashTable));

    char **files=NULL;
    int numOfFiles = GetFilenamesInDir(directory,&files);

    for(int i=0;i<numOfFiles;i++){
    	buildIndex(files[i]);
    }






    /*======================================================================================================================*/


	return 0;
}

/**
* buildIndex- Extract words from doc, store them into appropriate nodes and HashTable
* @docname- name of document to extract
* return: 0 on completion
**/
int buildIndex(char *docname){
	int pos = 0;
	char *word;
 	char *doc = docname;
 	WordNode *wNode;
 	DocumentNode *docnode;
  	while((pos = GetNextWord(doc, pos, &word)) > 0) {
    	// create new WordNode if word is not in HashTable and add to HashTable
    	NormalizeWord(word);
  		if(isInHash(hash,word)>MAX_HASH_SLOT){
  			wNode=makeWordNode(word);
  			addToHashTable(word,hash,wNode);
  		}
  		//else, if word is in HashTable, set to correct pointer
  		else{
  			wNode=getWordNode(word);
  		}

  		//check if current doc node exists
  		if(hasCurrentDoc(wNode,doc)){
  			//does exist, increment frequency
  			docnode=getDocumentNode(wNode,doc);
  			docnode->freq++;

  		}
  		else{
  			//does not exist, create new Document node
  			docnode=makeDocumentNode(doc);
  			wNode->page=docnode;
  		}
      	free(word);
 	 }
	return 0;
}

/**makeWordNode- create new WordNode, return pointer to new WordNode
*	@wp-word to put inside new WordNode
*	return: pointer to new WordNode
**/
WordNode* makeWordNode(char *wp){
    WordNode *node = calloc(1,sizeof(WordNode));
  	node->word = calloc(1,sizeof(char)*strlen(wp));
	strcpy(node->word,wp);
	node->next=NULL;
    node->page=NULL;
    return node;
}

/**getWordNode- retrieve pointer from HashTable
*	@wp-Word of WordNode pointer to retrieve from HashTable
*	return: pointer to WordNode
**/
WordNode* getWordNode(char *wp){
	WordNode *getNode;
	unsigned long hashIndex = JenkinsHash(wp,MAX_HASH_SLOT);
	getNode=hash->table[hashIndex];
	return getNode;
}

/**isInHash- check to see if word is in HashTable
*	@table-HashTable to check
*	@wp-word to check if in HashTable
*	return: MAX_HASH_SLOT*2 if not found, hashIndex otherwise
**/
int isInHash(HashTable *table, char *wp){
	WordNode *node;
	unsigned long hashIndex = JenkinsHash(wp,MAX_HASH_SLOT);

	if(!wp || !hash){
        return MAX_HASH_SLOT*2;
    }

    node = hash->table[hashIndex];

    if (!node){
        return MAX_HASH_SLOT*2;
    }

    while (node){
        if (!strcmp(node->word,wp)){
            return hashIndex;
        }
        node = node->next;
    }
    return MAX_HASH_SLOT*2;
}

/** addToHashTable- add node to HashTable
*	@wp-word to check if HashTable already contains
*	@hash-HashTable to add to
*	@wnp-WordNode to add to HashTable
*	return: 0 on success, 1 otherwise
**/
int addToHashTable(char *wp, HashTable *hash, WordNode *wnp){
    WordNode *node;
    WordNode *newNode;
    unsigned long hashIndex = JenkinsHash(wp,MAX_HASH_SLOT);
    if(!wp || !hash){
        return 1;
    }

    node = hash->table[hashIndex];

    //node does not already exist in HashTable at current hashindex
    if (!node){
        newNode=wnp;
        hash->table[hashIndex] = newNode;
        return 0;
    }

    //node currently exists at current index; traverse till empty spot
    while(node){
        if(!node->next){
            newNode = wnp;
            node->next = newNode;
            return 0;
        }
        node = node->next;
    }
    return 1;
}


/**
* hasCurrentDoc- check if WordNode has current DocumentNode
* @wp- WordNode to search
* @docn- Document number
* return- 1 if DocumentNode is within WordNode, 0 if not
**/
int hasCurrentDoc(WordNode *wp, char *docn){
	WordNode *node = wp;
	DocumentNode *doc;

	doc = node->page;

	//if doc is null
	if(!doc){
		return 0;
	}

	while(doc){
		//doc is same number as current docnode
		if(doc->doc_id==atoi(docn)){
			return 1;
		}
			
		doc=doc->next;
	}

	return 0;
}

/**
* makeDocumentNode- create new DocumentNode
* @docn: number of document
* return: pointer to new document
**/
DocumentNode* makeDocumentNode(char *docn){
	DocumentNode *node = calloc(1,sizeof(DocumentNode));
  	node->doc_id = atoi(docn);
  	node->freq=1;
	node->next=NULL;
    return node;
}

/**
* getDocumentNode- retrieve document within WordNode based on number
* @wnode: WordNode to search
* @docn: Document with number to find
* return: Pointer to document with matching number
**/
DocumentNode* getDocumentNode(WordNode *wnode,char *docn){
	DocumentNode *getNode;
	getNode=wnode->page;
	while(getNode){
		if(getNode->doc_id==atoi(docn)){
			return getNode;
		}
		getNode=getNode->next;
	}
	return getNode;
}


