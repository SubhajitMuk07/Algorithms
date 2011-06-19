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


//Process 100 characters at a time for memory
#define LINE_SIZE 100


inline int max(int a, int b)
{
  return a > b ? a : b;
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
	  //To avoid having to call strlen on huge buffers, keep track of the length here.
	  *length = lSize;
	  rewind (pFile);

	  buffer = (char*) malloc (sizeof(char)*lSize);
	  if (buffer == NULL) {
		  exit (2);
	  }

	  result = fread (buffer,1,lSize,pFile);
	  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	  fclose (pFile);
	  return buffer;
}

int main(int argc, char* argv[])
{
	int show_all_lines = 0;
	char* sequence_x = NULL;
	char* sequence_y = NULL;
	int sequence_x_length = 0;
	int sequence_y_length = 0;
	char x_line[LINE_SIZE + 1];
	char y_line[LINE_SIZE + 1];
	int x_idx = 0;
	int y_idx = 0;
	int x_done = 0;
	int y_done = 0;
	int i = 0;
	int j = 0;
	int diff_count = 0;
	int M = 0;
	int N = 0;
	int** C = NULL;

	if(argc >= 3)
	{
			sequence_x = get_file_contents(argv[1],  &sequence_x_length);
			sequence_y = get_file_contents(argv[2], &sequence_y_length);
			if(sequence_x == NULL || sequence_y == NULL)
			{
				if(sequence_x != NULL)
				{
					free(sequence_x);
				}
				else if(sequence_y != NULL)
				{
					free(sequence_y);
				}
				puts("Error opening file.  Please try again.");
				exit(0);
			}
	}
	else
	{
			puts("Invalid arguments.  Missing FileA and/or File B.  Please try again.");
			exit(0);
	}

	if(argc > 3)
	{
		if(strcmp(argv[3], "--show-all-lines") == 0)
		{
			show_all_lines = 1;
		}
	}



	while( x_done < 1 && y_done < 1)
	{

			memset(x_line, 0, sizeof(x_line));

			if(x_idx + LINE_SIZE < sequence_x_length)
			{
				strncpy(x_line, &sequence_x[x_idx], LINE_SIZE);
				x_line[LINE_SIZE] = '\0';
				x_idx = x_idx + LINE_SIZE;
			}
			else
			{
				strncpy(x_line, &sequence_x[x_idx], sequence_x_length - x_idx );
				x_line[LINE_SIZE] = '\0';
				x_idx = x_idx + (sequence_x_length - x_idx);
				x_done = 1;
			}

			memset(y_line, 0, sizeof(y_line));
			if(y_idx + LINE_SIZE < sequence_y_length)
			{
				strncpy(y_line, &sequence_y[y_idx], LINE_SIZE);
				y_line[LINE_SIZE] = '\0';
				y_idx = y_idx + LINE_SIZE;
			}
			else
			{
				strncpy(y_line, &sequence_y[y_idx], sequence_y_length - y_idx );
				y_line[LINE_SIZE] = '\0';
				y_idx = y_idx + (sequence_y_length - y_idx);
				y_done = 1;
			}

			if(show_all_lines < 1)
			{
				if(strncmp(x_line, y_line, LINE_SIZE+1) == 0)
				{
					continue;
				}
			}


			M = strlen(x_line);
			N = strlen(y_line);
			C = malloc((M+1) * sizeof(int *));

			for(i = 0; i <= M; i++)
			{
				C[i] = malloc((N+1)* sizeof(int));
			}


			for(i = M-1; i >= 0; i--)
			{
				for(j = N-1; j>=0; j--)
				{
					if(x_line[i] == y_line[j])
					{
						C[i][j] = C[i+1][j+1] + 1;

					}
					else
					{
						C[i][j] = max(C[i+1][j], C[i][j+1]);
					}
				}
			}

			i = 0, j = 0;
			while(i < M && j < N)
			{
				if(x_line[i] == y_line[j])
				{
					if(show_all_lines > 0)
					{
						printf("%c", x_line[i]);
					}
					i++;
					j++;
				}
				else if(C[i+1][j] >= C[i][j+1])
				{
					if(show_all_lines > 0)
						printf("(>%c)", x_line[i++]);
					else
					{
						++diff_count;
						break;
					}
				}
				else
				{
					if(show_all_lines > 0)
							printf("(<%c)", y_line[j++]);
					else
					{
						++diff_count;
						break;
					}
				}
			}

			if(show_all_lines > 0)
			{
			while(i < M || j < N)
			{
				if(i == M)
				{
					printf("(<%c)", y_line[j++]);
				}
				else if(j == N)
				{
					printf("(>%c)", x_line[i++]);
				}
			}
			}

			if(show_all_lines == 0)
			{
				if(diff_count > 0)
				{
					printf("%s | \r\n%s", x_line, y_line);
					diff_count = 0;
				}

				printf("\r\n\r\n");
			}


			for (i = 0; i <= M; i++) {
				    free(C[i]);
			}
			free(C);

	}

	free(sequence_x);
	free(sequence_y);
	return 0;

}
