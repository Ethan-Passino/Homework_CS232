/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

//#include "crawler.h"
//#include "indexPage.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "crawler.h"
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

int main(int argc, char** argv){
  /* TODO: write the (simple) main function
  argv[1] will be the URL to index, if argc > 1 */
if(argc <= 2) {
  printf("You need to input a file and max number of hops. Execute like\n./webSearch <file> <max number of hops>\n");
  exit(-1);
}

//char startAddr[MAX_ADDR_LENGTH];

int MAX_N = atoi(argv[2]);
if(MAX_N <= 0) { //err handling
  printf("You need to input a file and max number of hops. Execute like\n./webSearch <file> <max number of hops>\n");
  printf("Make sure that maxHops is a integer and that it is greater than zero.\n");
  exit(-2);
}

FILE *fptr = fopen(argv[1], "r"); // read only
if(fptr == NULL) {
  printf("Unable to open the file specified.");
  printf("You need to input a file and max number of hops. Execute like\n./webSearch <file> <max number of hops>\n");
  exit(-3);
}
int n = 0;
int *hopsLimit = malloc(sizeof(int));
char url[MAX_ADDR_LENGTH];
printf("BEFORE LOOP1.\n");
while(fscanf(fptr, "%s %d", url, hopsLimit) && n < MAX_N) {
  printf("AFTER LOOP1.\n");
  int hopNum = 0;
  struct listNode *addrNode = NULL;
    printf("BEFORE LOOP3.\n");
  while (1) {
      printf("AFTER LOOP3.\n");
    if(!(contains(addrNode, url))) {
      // Start crawling/indexing the page
      printf("CRAWLING url: %s\n", url);
      addrNode = crawl(url, *hopsLimit);
      n++;
      printf("CRAWLED\n");
      printAddresses(addrNode);
      printf("Printed addreses.\n");
    }
    if(addrNode == NULL) {
      break;
    }
    if(addrNode->next == NULL) {
      break;
    }
    hopNum++;
    if(hopNum <= *hopsLimit && n < MAX_N) {
      // get the next url that was found and set it to be the next one to crawl (will be returned from crawl a pList of all the urls, do them in order) 
      if(addrNode->next == NULL) {
        printf("DEAD END\n");
        // dead end
        break;
      }
      printf("COPYING ADDR (%s) TO URL\n", addrNode->next->addr);
      strcpy(url, addrNode->next->addr);
      addrNode = addrNode->next;
      printf("COPIED\n");
    }
    else {
      break;
    }
  }
}

free(hopsLimit);

}