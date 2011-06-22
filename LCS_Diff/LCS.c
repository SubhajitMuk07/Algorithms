/*
 *
 * Inputs:
 *	Two file paths which we should diff.  Options arguments include --show-all-lines as explained below.
 *
 * Outputs:
 *	By default the input files are broken into 70 character 'lines'.  When two of these lines are determined to be different, they are displayed to the screen.
 *	Lines prefixed with > are from the first file.
 *	Lines prefixed with < are from the second file.
 *	The output will also display the following message:  Difference located at <File1>:[start-index]-[end-index] and <file2>:[start-index]-end[index]
 *	This is the same output from diff as seen on linux.
 *	If the --show-all-lines option is included the output will be the merged version of two files with > indicating changes from the first file and < being differences from the second.
 *
 * Summary:
 * 	Diff Program using a LCS algorithm as a base.
 * 	Uses a defined line size to deal with very large files.
 *
 * 	Uses LCS algorithm as described in CLRS "Introduction to Algorithms"  3rd Edition.
 *	Basically, we build our LCS sub-problem solutions table and we can then use those values to determine when our files differed.
 *
 * 	This is going to do the same thing as the following shell commands:
 *
 * 	$ fold ACGT_x -w 70 > ACGT_x_lines
 * 	$ fold ACGT_y -w 70 > ACGT_y_lines
 * 	$ diff ACGT_x_lines ACGT_y_lines
 * 
 * 	I had always wondered how diff would work so I decided to program it instead of just using a script.
 *		
 *
 *	Output is fairly simple, it dumps out the 70 character 'lines' as diff does when run as outlined above.
 *	See below for optional argument outputs
 *
 * 	The program should be run as follows
 *
 * 	find_diff File1 File2 [optional-args]
 *
 * 	optional-args = --show-all-lines which allows the entire contents to be dumped to stdout
 * 
 * Notes: 
 *	 I highly recommend redirecting stdout to a file if you run the above command.  Even with | more, it is hard to read.
 *	 I am respecting whitespace (\r\n) so they will count as differences if present.
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//Process 70 characters at a time for memory
#define LINE_SIZE 70

/*
 * Name:
 *	int max(int a, int b)
 * 
 * Input:
 *	Expects two integers to compare
 * 
 * Output:
 * 	Returns the larger of the two arguments passed to the function.
 *
 * Side Effects:
 *	N/A
 *
 */
int max(int a, int b)
{
  return a > b ? a : b;
}

/*
 * Name:
 *	char* get_file_contents(char* path, int* length)
 *
 * Input:
 * 	Expects two parameters:
 *		1.  The file path to read either relative or fully pathed
 *		2.  A pointer to an integer.  This integer is going to be used to track file length to avoid repeated O(n) calls to strlen
 *
 * Output:
 * 	Returns a pointer to a character array containing the entire file contents.
 * 
 * Side Effects:
 * 	The caller is responsible for freeing the buffer.
 *
 */
char* get_file_contents(char* path, int* length)
{
	  FILE * inputFile;
	  long lSize;
	  char * buffer;
	  size_t result;
	
	  //Open our file for reading.
	  inputFile = fopen ( path , "rb" );
	
 	  //If our file pointer is null, we can't proceed.
	  if (inputFile==NULL) {
		  puts("Error reading file.  Please check the file path.");
		  exit (1);
	  }

	  //We need to know the size of the entire file for both reading and for matrix size
	  fseek (inputFile , 0 , SEEK_END);
	  lSize = ftell (inputFile);
	  //To avoid having to call strlen on huge buffers, keep track of the length here.
	  *length = lSize;

	  //Back up to the start of the file
	  rewind (inputFile);

	  //Allocate a buffer to hold the file contents	
	  buffer = (char*) malloc (sizeof(char)*lSize);

          //If malloc fails, we can't proceed.	
	  if (buffer == NULL) 
	  {
		  puts("Memory allocation error.  Program will stop.");
		  exit (1);
	  }

	  //Read our contet into the buffer and return the buffer.
	  result = fread (buffer,1,lSize,inputFile);
	  if (result != lSize) {
		puts ("File I/O Error"); 
		exit (1);
 	  }
	  //Close our file and return our buffer
	  //Caller is responsible for deallocation.
	  fclose (inputFile);
	  return buffer;
}

int main(int argc, char* argv[])
{
	//Optional argument switch
	int show_all_lines = 0;

	//File contents pointers
	char* sequence_x = NULL;
	char* sequence_y = NULL;

	//Total file lengths
	int sequence_x_length = 0;
	int sequence_y_length = 0;

	//Line buffers
	char x_line[LINE_SIZE + 1];
	char y_line[LINE_SIZE + 1];

	//Buffer counters, should be length 70 or less
	int x_line_length = 0;
	int y_line_length = 0;
	
	
	//Index counters, used to keep track of how much we have read relative to the entire file size
	int x_idx = 0;
	int y_idx = 0;

	//Boolean switches to indicate when we are done processing each of our files.
	int x_done = 0;
	int y_done = 0;
	
	//Loop counters
	int i = 0;
	int j = 0;
	int diff_count = 0;
	
	//Length counters.  M is the length of our buffer for file 1, N is the length of our buffer for file 2
	int M = 0;
	int N = 0;

	//This is the pointer to our dynamic programming result table
	int** C = NULL;

	if(argc >= 3)
	{
			//Read our two files to get their contents
			sequence_x = get_file_contents(argv[1],  &sequence_x_length);
			sequence_y = get_file_contents(argv[2], &sequence_y_length);
	}
	else
	{
			//We need a minimum of two command line arguments
			puts("Invalid arguments. Arguments at minimum must include two files.  Please try again.");
			exit(0);
	}

	//Check for our optional flag, if present, set our switch.
	if(argc > 3)
	{
		if(strcmp(argv[3], "--show-all-lines") == 0)
		{
			show_all_lines = 1;
		}
	}


	//This is the heart of the algorithm
	//What it does is to create a dynamic programming solution table to the LCS problem.
	//Once that is finished, we simply read the table back out to find the differences
	while( x_done < 1 && y_done < 1)
	{

			//Clear our line buffer each iteration
			memset(x_line, 0, sizeof(x_line));

			//If we can read a full line
			if(x_idx + LINE_SIZE < sequence_x_length)
			{
				//Copy the line and update our index variables
				strncpy(x_line, &sequence_x[x_idx], LINE_SIZE);
				x_idx = x_idx + LINE_SIZE;
				x_line_length = LINE_SIZE;
			}
			else
			{
				//Since we cant read a full line, read as much as is left
				strncpy(x_line, &sequence_x[x_idx], sequence_x_length - x_idx );
				x_line_length = sequence_x_length - x_idx;
				x_idx = x_idx + (sequence_x_length - x_idx);
				x_done = 1;
			}

			//This code is the same as the above code but for our second input file
			memset(y_line, 0, sizeof(y_line));
			if(y_idx + LINE_SIZE < sequence_y_length)
			{
				strncpy(y_line, &sequence_y[y_idx], LINE_SIZE);
				y_line_length = LINE_SIZE;
				y_idx = y_idx + LINE_SIZE;
			}
			else
			{
				strncpy(y_line, &sequence_y[y_idx], sequence_y_length - y_idx );
				y_line_length = sequence_y_length - y_idx;
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


			//M and N are sizes used to create our MxN solutions table
			M = x_line_length;
			N = y_line_length;

		
			//Allocate memory for our sub-problem solutions table
			C = malloc((M+1) * sizeof(int *));
			for(i = 0; i <= M; i++)
			{
				C[i] = malloc((N+1)* sizeof(int));
			}

			for(i = 1; i <= M; i++)
				C[i][0]=0;
			for(j=0; j <=N; j++)
				C[0][j]=0;

			/*
			 * This is the LCS algorithm.
			 * The goal of the algorithm is to find the longest common sequence of our two sequences
			 * If the characters at the indices are equal, we should add one to the length of our longest sequence found so far
			 * Otherwise we should use the largest  value from our previous sub-problem.
 			 * 
			 * The length of the LCS is at C[M][N].
			 *
			 * 
			 */
			for(i = 1; i <= M;  i++)
			{
				for(j = 1; j<= N; j++)
				{
					if(x_line[i] == y_line[j])
					{
						C[i][j] = C[i-1][j-1] + 1;
					}
					else
					{
						C[i][j] = max(C[i-1][j], C[i][j-1]);
					}
				}
			}

			/*
			 *
			 * This is interesting.  In most cases we don't care about seeing the merged contents.
			 * So as soon as we find a difference, break out of this and just display the lines for comparison
			 *
			 * Now if the user has included the --show-all-lines option, then we should show the merged result and not break out early. 
			 * We can use the LCS lengths we chose to then determine how and which characters differed from our texts.
			 *
			 */
			i = 0, j = 0;
			while(i < M && j < N)
			{
				//Our characters are equal, don't print them unless we are displaying the merged version
				if(x_line[i] == y_line[j])
				{
					if(show_all_lines > 0)
					{
						printf("%c", x_line[i]);
					}
					i++;
					j++;
				}
				else{
				//Our first file contains a difference from our second, break now that we know the lines have a diff
				//If we are showing the merged contents, display and continue in our loop
				if(C[i-1][j] >= C[i][j-1])
				{
					if(show_all_lines > 0)
						printf("(>%c)", x_line[i++]);
					else
					{
						++diff_count;
						break;
					}
				}
				//File2 is different as well.
				//Break if not showing all lines, otherwise display the character and continue on in this loop.
				if(C[i-1][j] <=C[i][j-1])
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
			}

			//We may need to take care of leftover differences if one was shorter than the other, do that here.
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

			//If we are not showing the merged files, just display the lines aligned as diff does
			//This will allow the user to easiily see how the files differed.
			if(show_all_lines == 0)
			{
				if(diff_count > 0)
				{
					printf("Difference located at starting at %s:%d-%d and %s:%d-%d:\r\n", argv[1],  x_idx-x_line_length, x_idx, argv[2], y_idx-y_line_length, y_idx);
					printf("< %s | \r\n> %s", x_line, y_line);
					diff_count = 0;
				}

				printf("\r\n\r\n");
			}

			
			//Free our matrix items
			for (i = 0; i <= M; i++) {
				    free(C[i]);
			}
			free(C);

	}
	//Clean up memory and end
	free(sequence_x);
	free(sequence_y);
	printf("\r\n");
	return 0;

}
