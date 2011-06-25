/*
 *
 * Diff Program using a LCS as a base.
 * Uses a defined line size to deal with very large files.
 *
 * Uses LCS algorithm as found in CLRS 3rd Edition.
 *
 *
 * Run as follows
 *
 * <program> File1 File2
 *
 * optional args = --show-all-lines allows the entire contents to be dumped to stdout
 *
 *
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  Structure used to keep track of our suffixes
 *  length is the length of the suffix
 *  pattern is the suffix itself
 *  start_index is the begining index of the suffix in our original text
 *  patern_length is the length of the substring pattern found in the suffx
 *  end_index is the end_index of the matching pattern relative our substring
 */
typedef struct suffix {
    int length;
    char* pattern;
    int start_index;
    int pattern_length;
    int end_index;
} Suffix;

/*  Simpled LinkedList Node
 *  After we locate patterns of a particular length, we need to remove duplicate matches.
 *  To do that, I store them in LinkedList form for easy traversal.
 */

typedef struct list_node {
   Suffix* val;
   struct list_node* next;
} LinkedNode;


void free_list(LinkedNode* head)
{
    LinkedNode* temp = head;
    while(head != NULL)
    {
        temp = head->next;
        free(head);
        head = temp;
    }
}

int min(int x, int y)
{ 
        if(x < y )
            return x;
        return y;
}

void search2(char* substr, char* original_text, int substr_len, int original_text_length)
{
   int i;
   for(i = 0; i < original_text_length - substr_len; i++)
   {
        if(strncmp(substr, &original_text[i], substr_len) == 0)
        {
            printf("Pattern found starting at position:\t%d \n", i);
        }
   }
}

char* search_prefix_for_match(Suffix suffix, Suffix txt)
{
       int i;
       char* substring;
       int slen = suffix.length;
       int tlen = txt.length;
       int min_length;
       min_length = min(slen, tlen);
       for(i = 0; i < min_length; i++)
       {
            if(suffix.pattern[i] != txt.pattern[i])
            {
                substring = (char*) malloc (sizeof(char)*(i+1));
                memset(substring, 0, sizeof(substring));
                strncpy(substring, &suffix.pattern[0], i);
                substring[i]='\0';
                return substring;
            }
       }
       return NULL;
}

int suffix_comparator(const void *a, const void *b) 
{ 
    Suffix *ia = (Suffix *)a;
    Suffix *ib = (Suffix *)b;
    int res = strcmp(ia->pattern, ib->pattern);
    return res;
}


int check_duplicates(char* x, LinkedNode* head, int lenx)
{
   LinkedNode* temp = head;
   while(temp != NULL)
    { 
        if(strncmp(x, temp->val->pattern, lenx) == 0)
        {
             return 1;
        }
        temp = temp->next;
    }
    return 0;
}



char* find_substring_in_text(char* suffix, int len, int plength)
{
    LinkedNode* head = NULL;
    LinkedNode* curr = NULL;
    //InstancesNode* curr_instance = NULL;
    char* needle;
    char* pattern;
    int j;
    char* matched_pattern;
    int match = 0;
    int p_length;

    Suffix* pArray = (Suffix*)malloc(sizeof(Suffix) * len);
    int i = 0;
    for ( i = 0; i < len; i++ ) {
        //Strlen on long patterns is kiling performance because it is O(n).
        //If we use a struct and then keep track of our suffix length this way, we can avoid huge costs all together.
        pArray[i].pattern = &suffix[i];
        pArray[i].length = (len-i);
        pArray[i].start_index = i;
    }
    qsort(pArray, len, sizeof(Suffix), suffix_comparator);  
    for(i = 0; i < len-1; i++)
    {
        matched_pattern = search_prefix_for_match(pArray[i], pArray[i+1]);
        if(matched_pattern != NULL)
        {
            p_length = strlen(matched_pattern);
            pArray[i].pattern_length = p_length;
            pArray[i].end_index = pArray[i].start_index + p_length;
            if(p_length >= 60 && plength == p_length && check_duplicates(matched_pattern, head, p_length) == 0)
            {
                if(head == NULL)
                {
                    head = (LinkedNode *)malloc(sizeof(LinkedNode));
                    head->val = &pArray[i];
                    head->next = NULL;
                    curr = head;
                }
                else
                {
                    curr->next = (LinkedNode *)malloc(sizeof(LinkedNode));
                    curr->next->next = NULL;
                    curr->next->val = &pArray[i];
                    curr = curr->next;
                }
                
            }
        }
        free(matched_pattern);
    } 

    curr = head;
    while(curr != NULL)
    {
            needle = (char*)malloc(sizeof(char) * ((curr->val->end_index - curr->val->start_index)+1));
            strncpy(needle, &(curr->val->pattern[0]), curr->val->end_index - curr->val->start_index);
            needle[curr->val->end_index - curr->val->start_index] = '\0';
            printf("Pattern Found:%s\r\nLength:\t%d\r\n", needle, strlen(needle));
            search2(needle, suffix, curr->val->end_index - curr->val->start_index, len);
            printf("\r\n");
            free(needle);
            curr = curr->next;
    }
    
    printf("\r\n");
    free_list(head);
    free(pArray);

}


char* get_file_contents(char* path, int* length)
{
	FILE * pFile;
	  long lSize;
	  char * buffer;
	  size_t result;

	  pFile = fopen ( path , "rb" );
	  if (pFile==NULL) {
		  exit (1);
	  }
	  fseek (pFile , 0 , SEEK_END);
	  lSize = ftell (pFile);
      lSize = lSize;
	  //To avoid having to call strlen on huge buffers, keep track of the length here.
	  *length = lSize;
	  rewind (pFile);
	  buffer = (char*) malloc (sizeof(char)*(lSize+1));
	  if (buffer == NULL) {
          puts("Error");
		  exit (2);
	  }
	  result = fread (buffer,1,lSize,pFile);
            buffer[lSize] = '\0';
	  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	  fclose (pFile);
	  return buffer;
}




int main(int argc, char* argv[])
{
    int length_of_pattern = 70;
	char* sequence_all;
	int sequence_all_length;
    int match_count;
	if(argc >= 2)
	{
            if(argc > 2)
            {  
                match_count = sscanf(argv[2], "%d", &length_of_pattern);
            }
			sequence_all = get_file_contents(argv[1],  &sequence_all_length);
            find_substring_in_text(sequence_all, sequence_all_length, length_of_pattern);
            free(sequence_all);
            return 0;
    }
	return 0;

}
