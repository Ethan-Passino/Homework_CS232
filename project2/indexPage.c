/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // ADDED FOR isalpha

#define ALPHA_SIZE (26)
#define MAX_BUFFER_SIZE 300000

/* TODO: structure definitions */
struct TrieNode
{
  int letter;
  struct TrieNode* nextNode[ALPHA_SIZE];
  int count;
};

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

/* TODO: change this return type */
struct TrieNode* indexPage(const char* url);

int addWordOccurrence(const char* word, struct TrieNode* root);

void printTrieContents(struct TrieNode* root, char *word, int *count);

int freeTrieMemory(struct TrieNode* root);

int getText(const char* srcAddr, char* buffer, const int bufSize);

void setupNode(struct TrieNode* node);

void addWordProcess(char *arr, int bytes, struct TrieNode *root);

void removeSpaces(char *str);


int main(int argc, char** argv){
  /* TODO: write the (simple) main function
  argv[1] will be the URL to index, if argc > 1 */
if(argc <= 1) {
  printf("You need to input a link. Execute like\n./indexPage <url>");
  exit(-1);
}
  
  char *wrd = malloc(sizeof(char) * 50);
  int *count = malloc(sizeof(int));
  *count = 0;

  struct TrieNode *root = indexPage(argv[1]);

  printTrieContents(root, wrd, count);

  freeTrieMemory(root);
  free(root);
  free(wrd);
  free(count);

  return 0;
}

/* TODO: define the functions corresponding to the above prototypes */

/* Creates a TrieNode with the given data*/
void setupNode(struct TrieNode* node) {
  node->count = 0;
  node->letter = 0;

  for(int i= 0; i < ALPHA_SIZE; i++) {
    node->nextNode[i] = NULL;
  }
  
}


/* TODO: change this return type */
struct TrieNode *indexPage(const char* url) 
{
  /* This is where we create the TrieNode
    Since we create it in the heap memory, 
    as long as we pass the memory address,
    we can access this node
  */

  struct TrieNode *root = malloc(sizeof(struct TrieNode));
  setupNode(root);
  root-> count = -100;
  printf("%s\n", url);
  // Data setup, get text
  char* buffer = malloc(sizeof(char) * MAX_BUFFER_SIZE);


  int bytes = getText(url, buffer, MAX_BUFFER_SIZE);
  if(!(bytes)) {
    // If it failed to read, we need to exit
    printf("Failed to connect to the website.");
    exit(-1);
  }


  if(buffer[0] == '\0') {
    printf("Had zero words fetched.");
    exit(-1);
  }

  setupNode(root); // Might need to remove this

  // Adding words and processing the buffer
  addWordProcess(buffer, bytes, root);

  free(buffer);

  return root;
}

int addWordOccurrence(const char* word, struct TrieNode *root)
{
  // Case when there are no characters.
  if(word[0] == '\0') {
      root->count += 1;
      return -1;
  }

  if(root->nextNode[word[0] - 'a'] == NULL) {
    struct TrieNode *node = malloc(sizeof(struct TrieNode));
    setupNode(node);
    root->letter = word[0];

    root->nextNode[word[0] - 'a'] = node;

    addWordOccurrence(&word[1], node);
    return 0;
  }

  // Node already exists case

  addWordOccurrence(&word[1], root->nextNode[word[0] - 'a']);
  return 0;
}

void printTrieContents(struct TrieNode *root, char *word, int *count) {

  // This is printing the number of the word occurences
  if(root->count > 0) {
    printf("%s: %d\n", word, root->count);
  }

  (*count)++;

  for(int i = 0; i < ALPHA_SIZE; i++) {
    // Checking if a node exists for this character
    if(root->nextNode[i] != NULL) {
      word[*count] = '\0';
      word[*count - 1] = i + 'a';

      printTrieContents(root->nextNode[i], word, count);
    }
  }
  (*count)--;
}

int freeTrieMemory(struct TrieNode *root)
{
  for(int i = 0; i < ALPHA_SIZE; i++) {
    if(root->nextNode[i] != NULL) {
      freeTrieMemory(root->nextNode[i]);
    }
  }

  for(int i = 0; i < ALPHA_SIZE; i++) {
    free(root->nextNode[i]);
  }

  return 0;
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);
  
  pipe = popen(buffer, "r");
  if(pipe == NULL) {
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}

void addWordProcess(char *array, int bytesRead, struct TrieNode *node) {
int counter = 0;
    char *postArray;

    // For manipulating the data
    postArray = malloc(sizeof(char) * bytesRead);

    for (int i = 0; i < bytesRead; i++)
    {
      // Making sure this is an alpha character
        if ((tolower(array[i]) <= 'z' && tolower(array[i]) >= 'a')) //if the character is a letter
        {
            postArray[counter] = tolower(array[i]);
            counter++;
        }
        else
        {
            postArray[counter] = ' ';
            counter++;
        }
    }
    postArray[bytesRead] = '\0';

    removeSpaces(postArray);

    // printf("%s", postArray);

    int spaceCount = 0;

    for (int i = 0; i < strlen(postArray); i++)
    {
        if (postArray[i] == ' ')
            spaceCount++;
    }

    int start = 0, end = 0;
    int stringLength;
    char *word = malloc(sizeof(char) * 50);


    // Executing addWordOccurence to add words to the node, after processing spaces.
    for (int i = 0; i < (spaceCount + 1); i++)
    {
        while (postArray[end] != ' ' )
        {
            end++;
        }
        stringLength = end - start;

        strncpy(word, &postArray[start], stringLength);

        word[stringLength] = '\0';

        if(word[0] >= 'a' && word[0] <= 'z')
        {
            printf("\t%s\n", word);

            addWordOccurrence(word, node);
        }
        end--;

        if (postArray[end] == '\0') break;

        end += 2;

        start = end;
    }

    free(word);
    free(postArray);

}

void removeSpaces(char *str) {
      int i = 0;
    int x = 0;
    for (; str[i]; ++i)
        if (!isspace(str[i]) || (i > 0 && !isspace(str[i - 1])))
        {
            str[x++] = str[i];
        }
    str[x] = '\0';
}
