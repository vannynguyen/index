/* ========================================================================== */
/* File: common.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the common defines and data structures.
 *
 */
/* ========================================================================== */
#ifndef COMMON_H
#define COMMON_H

// ---------------- Prerequisites e.g., Requires "math.h"
#include <stddef.h>                          // size_t

// ---------------- Constants
#define INTERVAL_PER_FETCH 1                 // seconds between fetches

#define MAX_DEPTH 4                          // don't want to go too far do

// limit crawling to only this domain
#define URL_PREFIX "http://old-www.cs.dartmouth.edu/~cs50/tse"

// ---------------- Structures/Types

typedef struct WebPage {
    char *url;                               // url of the page
    char *html;                              // html code of the page
    size_t html_len;                         // length of html code
    int depth;                               // depth of crawl
} WebPage;

typedef struct DocumentNode {
  struct DocumentNode *next;         // pointer to the next member of the list.
  int doc_id;                        // document identifier
  int freq;                          // number of occurrences of the word
} DocumentNode;

typedef struct WordNode {
  struct WordNode *next;            // pointer to the next word (for collisions)
  char *word;                       // the word
  DocumentNode *page;               // pointer to the first element of the page list.
} WordNode;



// ---------------- Public Variables

// ---------------- Prototypes/Macros

#endif // COMMON_H
