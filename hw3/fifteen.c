/**
 * fifteen.c
 *
 * Refer to Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

#ifndef INT_MAX
    #define INT_MAX 12345678
#endif // INT_MAX

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void greet(void);
void init(void);
void draw(void);
short move(int tile);
short won(void);
int get_int();

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (1)
    {
        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        printf("\n");


        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Get an non-negative integer from user input
 * If the input is not non-negative integer, return INT_MAX
 */
 int get_int()
 {
    int input = 0;
    short invalid = 0;

    char c = getchar();
    if (c == '\n')
        return INT_MAX;

    while (c != '\n') 
    {
        if ((c >= '0') && ( c <= '9'))
        {
            input = input * 10 + (c - '0');
        } else {
            invalid = 1;
        }

        c = getchar();
    }

    if (invalid)
        return INT_MAX;
    else 
        return input;
 }

/**
 * Greets player.
 */
void greet(void)
{    
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int counter = d*d-1;
    for(int r = 0; r < d; r++) {
        for(int c = 0; c < d; c++) {
            if(counter == 0) {
                break;
            }
            board[r][c] = counter;
            counter--;
        }
    }

    if(((d*d)-1) % 2 != 0) {
        // this is the case there are an odd number of tiles (1 and 2 need to swap)
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }

    // -1 is an empty tile in our case.
    board[d-1][d-1] = -1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int r = 0; r < d; r++) {
        for(int c = 0; c < d; c++) {
            int tile = board[r][c];
            if(tile == -1) {
                // empty tile
                if(d > 3) {
                    // This is here to fix spacing issues when we have double digit numbers
                    // and single digit numbers on the same board.
                    printf("_  ");
                } else {
                    printf("_ ");
                }
            } else {
                if(tile <= 9 && d > 3) {
                    // This is here to fix spacing issues when we have double digit numbers
                    // and single digit numbers on the same board.
                    printf("%d  ", tile);
                }
                else { 
                    printf("%d ", tile);
                }
            }
        }
        printf("\n");
    }
  
}

/**
 * If tile borders empty space, moves tile and returns 1, else
 * returns 0.
 */
short move(int tile)
{
    // Checking to make sure the tile is in range in the board.
    if(tile > ((d*d) - 1) || tile < 1) {
        return 0;
    }

    // Finding the tiles row and column so we can check around it
    int row = 0;
    int col = 0;
    for  (int r = 0; r < d; r++) {
        for(int c = 0; c < d; c++) {
            if(board[r][c] == tile) {
                row = r;
                col = c;
                break;
            }
        }
    }

    // Checking the spaces around the tile
    // Keep in mind -1 is an empty space
    if(row+1 < d && board[row+1][col] == -1) {
        // above the tile is empty, so switch spaces
        board[row+1][col] = board[row][col];
        board[row][col] = -1; // Switching to an empty tile.
        return 1;
    }
    else if(row-1 >= 0 && board[row-1][col] == -1) {
        // below the tile is empty, so switch spaces
        board[row-1][col] = board[row][col];
        board[row][col] = -1;
        return 1;    
    }
    else if(col+1 < d && board[row][col+1] == -1) {
        // Right of tile is empty, so switch
        board[row][col+1] = board[row][col];
        board[row][col] = -1;
        return 1;
    }
    else if(col-1 >= 0 && board[row][col-1] == -1) {
        // left of tile is empty, so switch
        board[row][col-1] = board[row][col];
        board[row][col] = -1;
        return 1;
    }
    return 0;
}


/**
 * Returns 1 if game is won (i.e., board is in winning configuration),
 * else 0.
 */
short won(void)
{
    int counter = 1;
    for(int r = 0; r < d; r++) {
        for(int c = 0; c < d; c++) {
            if(counter > ((d*d) - 1)) {
                // This is here so it doesn't check for a tile that doesn't exist. 
                /*
                For example if this was a 4x4 board, if we didn't have this it would try to check tile 16
                which doesn't exist and therefore the result would return 0 (false) instead of 1 (true)
                and be incorrect.
                */
                break;
            }
            if(counter != board[r][c]) {
                return 0;
            }
            counter++;
        
        }
    }
    return 1;
}

/**
 * Question 1: The framework allows from 3x3 
 * all the way to 9x9 stated by the DIM_MIN and DIM_MAX variables.
 * Question 2: The game uses a 2d array to track where tiles are.
 * Question 3: The function "greet()" is called.
 * Question 4: I need to implement, init(), draw(), move(), and won().
*/
