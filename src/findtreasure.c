/********************************************
 * @author : Gökhan ÖZELOĞLU                *
 *                                          *
 * @date : 21.10.2018 - 31.10.2018          *
 *                                          *
 * @email : b21627557@cs.hacettepe.edu.tr   *
 ********************************************
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**@details: This struct stores the input informations taking from command line as an argument.
 * @purpose: Number of function parameter is being less and it looks better and readable.
 */
struct Inputs {
    int row_size;
    int column_size;
    int key_matrix_size;
};

/** @param: file is a file pointer
 *  @param: map_matrix is a double-pointer array which in integer type to store the map matrix.
 *  @param: input is a struct object that holds command line arguments informations.
 *  @details: Firstly, the function allocates the memory for each columns dynamically.
 *  Reads the map matrix row-by-row and each rows appends into pointer-to-pointer(2D) array.
 *  Memory allocation was done dynamically.
 */
void read_map_matrix(FILE *file, int **map_matrix, struct Inputs input) {
    int i, j;

    for (i = 0; i < input.row_size; i++) {
        *(map_matrix + i) = (int *) malloc(sizeof(int) * input.column_size);   /* Dynamic memory allocation for each rows' columns.*/
    }

    for (i = 0; i < input.row_size; i++) {     /* Reading map matrix's rows and columns with for loops. */
        for (j = 0; j < input.column_size; j++) {
            fscanf(file, "%d", *(map_matrix + i) + j);
        }
    }
}

/** @param: File pointer
 *  @param: key_matrix is a double pointer integer array that stores the key matrix array.
 *  @param: input is a struct object that holds input informations.
 *  @details: Firstly, the function allocates the memory for each columns dynamically.
 *  Reads the key matrix row-by-row and each rows appends into pointer-to-pointer(2D) array
 *  Memory allocation was done dynamically.
 */
void read_key_matrix(FILE *file, int **key_matrix, struct Inputs input) {
    int i, j;

    for (i = 0; i < input.key_matrix_size; i++) {
        *(key_matrix + i) = (int *) malloc(sizeof(int) * input.key_matrix_size);  /* Allocation of each row's columns*/
    }

    for (i = 0; i < input.key_matrix_size; i++) {     /* Reading the file's row and column. */
        for (j = 0; j < input.key_matrix_size; j++) {
            fscanf(file, "%d", *(key_matrix + i) + j);
        }
    }
}

/** @param: Takes only one parameter from command line in cartesian  notation.
 * @details: Parse the {row} x {column} notation into separated form, like [row, column].
 * @return: The function returns pointer array which size is 2.
 * @purpose: That is an "helper" function.*/
int* parse_row_column(char cartesian_notation[]) {
    int *row_column_array = (int *) malloc(sizeof(int) * 2);  /* Array is initialized for storing row-column sizes. */

    char *token;    /* char variable for tokenizer operation */
    token = strtok(cartesian_notation, "xX");   /* Upper-case or lower-case possibility is handled. */
    row_column_array[0] = atoi(token);    /* row size is assigned to array as a first element after converted with atoi() */

    token = strtok(NULL, "xX");
    row_column_array[1] = atoi(token);    /* column size is assigned to array as a second element after converted with atoi() */

    return row_column_array;
}

/** @param: map_matrix is our main matrix array.
 *  @param: key_matrix is other matrix for multiplication
 *  @param: mid_point_row represents middle point of sub-matrix to find exact point.
 *  @param: mid_point_column represents middle point of sub-matrix to find exact point.
 *  @param: size_key_matrix represents size of key matrix for multiplication.
 *  @details: This function multiples the key and map matrices row-by-row. Adds the result variable which is defined in function scope.
 *  start variable is used for get the start and finish indexes in map matrix.
 * @returns: result -> Just returns the result of matrix multiplication.*/
int multiple_matrix(int **map_matrix, int **key_matrix, int mid_point_row, int mid_point_column, int size_key_matrix) {
    int i, j, x, y;
    int result = 0;
    int start = size_key_matrix / 2;

    /*toDo: Try to use pointer notation instead of square brackets notation.*/
    for (i = mid_point_row - start, y = 0; i <= mid_point_row + start ; i++, y++) {
        for (j = mid_point_column - start, x = 0 ; j <= mid_point_column + start ; j++, x++) {
            result += map_matrix[i][j] * key_matrix[y][x];
        }
    }
    return result;
}


/**@param: direction gives the information about search direction
 * @param: mid_row represents middle row point of searching sub-matrix
 * @param: mid_column represents middle row point of searching sub-matrix
 * @param: input is a struct object that holds the input information.
 * @param: bound_num represents key matrix's mid point to calculate boundaries.
 * @returns: direction
 * @details: Function controls the boundaries. If direction goes to outside of the boundaries, changes the opposite
 * side. If direction goes to valid side, function does not change it.*/
int control_direction(int direction, int mid_row, int mid_column, struct Inputs input, int bound_num) {

    if (direction == 1 && mid_row == bound_num) {   /* Controls up-side in map matrix */
        return 2;
    }
    else if (direction == 2 && mid_row == input.row_size - bound_num - 1) {   /* Controls down-side in map matrix */
        return 1;
    }
    else if (direction == 4 && mid_column == bound_num) {   /* Controls left-side in map matrix */
        return  3;
    }
    else if (direction == 3 && mid_column == input.column_size - bound_num - 1) {     /* Controls right-side in map matrix */
        return 4;
    } else {    /* Direction is valid. */
        return direction;
    }
}

/**@param: file_output is a FILE pointer to write ouput.
 * @param: map_matrix represents the map matrix array and it is used for searching.
 * @param: key_matrix represents a square-matrix and multiply with sub-matrix of map_matrix
 * @param: mid_row represents the middle row point in searching sub-matrix
 * @param: mid_column represents the middle column point in searching sub-matrix
 * @param: input is a struct object which holds inputs giving in command line.
 * @return: The function's type is void. So, there is not type. Nevertheless, it is a recursive function that calls itself.
 * Also, program exists from the function in base case and it returns nothing.
 * @details:  If-else statements controls the directions. According to direction which finds with modulus operation, function calls
 * itself recursively or terminates the function when treasure is found.
 */
void search_treasure(FILE *file_output, int **map_matrix, int **key_matrix, int mid_row, int mid_column, struct Inputs input)  {
    int multiple_result = multiple_matrix(map_matrix, key_matrix, mid_row, mid_column, input.key_matrix_size);    /* Matrix multiplication */
    int direction = multiple_result  % 5;   /* Modulus operation */

    direction = control_direction(direction, mid_row, mid_column, input, input.key_matrix_size/2);    /* Controls the boundaries */

    fprintf(file_output ,"%d,%d:%d\n", mid_row, mid_column, multiple_result);   /* Writes ont the output file */

    if (direction == 0) {   /* Treasure is found */
        return; /* Finishes the function. */
    }
    else if (direction == 1) {  /* Searching operation goes to up direction */
        mid_row -= input.key_matrix_size;
    }
    else if  (direction == 2) { /* Searching operation goes to down direction */
        mid_row += input.key_matrix_size;
    }
    else if (direction == 3) {  /* Searching operation goes to right direction*/
        mid_column += input.key_matrix_size;
    }
    else if (direction == 4) {  /* Searching operation goes to left direction */
        mid_column -= input.key_matrix_size;
    }
    return search_treasure(file_output, map_matrix, key_matrix, mid_row, mid_column, input);
}

int main(int argc, char const *argv[]) {

    /*  File pointers are created here. */
    FILE *file_key_matrix;
    FILE *file_map_matrix;
    FILE *file_output;

    int **key_matrix;
    int **map_matrix;

    struct Inputs input;    /* Struct object is created */

    if (argc != 6) {    /* This if statement is handle the argument numbers. Gives an error message if there is not 6 argument. */
        printf("Number of argument is less or more than 6!\n");
        exit(EXIT_FAILURE);
    }

    file_map_matrix = fopen(argv[3], "r");  /* Map matrix is opened. */

    if (file_map_matrix == NULL) {  /* Control statement to make sure that map matrix is opened successfully. */
        printf("Map matrix file could not open. There is some problemç\n");
        exit(EXIT_FAILURE);   /* Terminates the program uin unexpected situation. */
    }

    file_key_matrix = fopen(argv[4], "r"); /* Key matrix is opened. */

    if (file_key_matrix == NULL) {  /* Control statement to make sure key matrix file is opened successfully. */
        printf("Key matrix could not open! There is some problem.\n");
        exit(EXIT_FAILURE);    /* The program terminates if key matrix could not open. */
    }

    file_output = fopen(argv[5], "w");  /* Output file is opened. */

    if (file_output == NULL) {  /* Control statement to make sure that output file is opened successfully. */
        printf("Output file could not open! There is some problem.\n");
        exit(EXIT_FAILURE);    /* The program terminates if output file could not open. */
    }

    input.key_matrix_size = atoi(argv[2]);

    if (input.key_matrix_size <= 0 || input.key_matrix_size % 2 != 1) { /* Make control the situation that key matrix size is entered negative or even number. */
        printf("Invalid key matrix size is entered!\n");
        exit(EXIT_FAILURE);
    }

    key_matrix = (int **) malloc(input.key_matrix_size * sizeof(int *));    /* Allocation of rows */
    read_key_matrix(file_key_matrix, key_matrix, input); /* Calls the function to read key matrix. */

    int *row_column_arr = parse_row_column(argv[1]);    /* Function parse the row-column info. */
    input.row_size = *(row_column_arr + 0);
    input.column_size = *(row_column_arr + 1);

    if (input.row_size <= 0 || input.column_size <= 0) {    /* The situation that row or column sizes cannot be negative is being controlled here. */
        printf("Map matrix size cannot be negative!\n");
        exit(EXIT_FAILURE);
    }

    map_matrix = (int **) malloc(input.row_size * sizeof(int *));   /* Allocation of map_matrix (Rows) */
    read_map_matrix(file_map_matrix, map_matrix, input);   /* Calls the function to read map matrix. */

    int first_mid_point = input.key_matrix_size / 2;    /* For the first searching operation finds the middle points. */
    search_treasure(file_output, map_matrix, key_matrix, first_mid_point, first_mid_point, input);   /* Searching function */

    /* Files are closed */
    fclose(file_map_matrix);
    fclose(file_key_matrix);
    fclose(file_output);

    /* Free all pointers */
    int i;
    for (i = 0 ; i < input.row_size ; i++) {
        free(map_matrix[i]);
    }
    free(map_matrix);

    for (i = 0 ; i < input.key_matrix_size ; i++) {
        free(key_matrix[i]);
    }
    free(key_matrix);

    free(row_column_arr);

    return 0;
}
