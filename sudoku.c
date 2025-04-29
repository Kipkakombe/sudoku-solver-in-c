#include <stdio.h>
#include <math.h>

// Structure used to specify the sub-grid boundaries
struct Limits {
    int lower_limit;
    int upper_limit;
};

// Array for defining sub-grid limits for the rows and columns of the sudoku (0-2, 3-5, 6-8)
struct Limits grid[9] = {{0, 2}, {0, 2}, {0, 2},
                        {3, 5}, {3, 5}, {3, 5},
                        {6, 8}, {6, 8}, {6, 8}};

//A 9x9 Sudoku grid is filled with zeros to represent the empty cells
int sudoku[9][9] = {0};

/* Checks if a number is already in a given row
 * Returns 1 if the number is foun and 0 if not */
int check_row(int row, int target) {
    for (int i = 0; i < 9; i++) {
        if (sudoku[row][i] == target) {
            return 1;
        }
    }
    return 0;
}


/* Checks if a number already exists in a given column
 * Returns 1 if the number is found and  0 if not */
int check_col(int col, int target) {
    for (int i = 0; i < 9; i++) {
        if (sudoku[i][col] == target) {
            return 1;
        }
    }
    return 0;
}

/* Checks if a given number exists in a 3x3 sub-grid
 * Uses given boundaries to define the sub-grid
 * Returns 1 if the number is found and 0 if not */
int check_sub_grid(int target, int row_lower, int row_upper, int col_lower, int col_upper) {
    for (int row = row_lower; row <= row_upper; row++) {
        for (int col = col_lower; col <= col_upper; col++) {
            if (sudoku[row][col] == target) {
                return 1;
            }
        }
    }
    return 0;
}

/* Validates if a number exists in a given cell and if can be placed at a given position
 * Checks row, column, and sub-grid constraints
 * Returns 1 if an empty cell is valid and 0 if invalid */
int is_valid(int row, int col, int num) {
    return !check_row(row, num) && 
           !check_col(col, num) && 
           !check_sub_grid(num, grid[row / 3 * 3].lower_limit, grid[row / 3 * 3].upper_limit, 
                          grid[col / 3 * 3].lower_limit, grid[col / 3 * 3].upper_limit);
}


/* Find the next empty cell in the grid
 * Updates row and col pointers with coordinates
 * Returns 1 if found, 0 if no empty cells remain */
int find_empty_cell(int* row, int* col) {
    for (*row = 0; *row < 9; (*row)++) {
        for (*col = 0; *col < 9; (*col)++) {
            if (sudoku[*row][*col] == 0) {
                return 1;
            }
        }
    }
    return 0;
}


/* Solves the Sudoku puzzle using backtracking
 * Returns 1 if the sudoku is solved and 0 if no solution exists */
int solve_sudoku() {
    int row, col;
    if (!find_empty_cell(&row, &col)) {
        return 1;  // Puzzle solved
    }
    
    for (int num = 1; num <= 9; num++) {
        if (is_valid(row, col, num)) {
            sudoku[row][col] = num;
            
            if (solve_sudoku()) {
                return 1;
            }
            
            sudoku[row][col] = 0;  // Backtrack
        }
    }
    return 0;
}

/* Prints the Sudoku grid
 * Shows 3x3 sub-grid boundaries with separators */
void print_sudoku() {
    printf("\n");
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            printf("%d ", sudoku[row][col]);
            if (col == 2 || col == 5) {
                printf("| ");
            }
        }
        printf("\n");
        if (row == 2 || row == 5) {
            printf("------+-------+------\n");
        }
    }
}

/* Main function of the sudoku solver
 * Handles inputs and coordinates solving process */
int main() {
    printf("Enter each row as a 9-digit number (use 0 for empty cells):\n");
    for (int i = 0; i < 9; i++) {
        char* ordinal;
        if (i == 0) ordinal = "st";
        else if (i == 1) ordinal = "nd";
        else if (i == 2) ordinal = "rd";
        else ordinal = "th";
        
        printf("Enter %d%s row: ", i + 1, ordinal);
        long long k;  // Using long long to handle 9 digits
        
        if (scanf("%lld", &k) != 1 || k < 0 || k > 999999999) {
            printf("Invalid input! Use a 9-digit number.\n");
            return 1;
        }
        
        for (int j = 0; j < 9; j++) {
            sudoku[i][8-j] = (int)(k / (long long)pow(10, j)) % 10;
        }
    }
    
    printf("\nUNSOLVED SUDOKU");
    print_sudoku();
    
    if (solve_sudoku()) {
        printf("\nSOLVED SUDOKU");
        print_sudoku();
    } else {
        printf("\nNo solution exists.\n");
    }
    return 0;
}