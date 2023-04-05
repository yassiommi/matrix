# Matrix Operations

The program is designed to do various operations on matrices. It takes matrices as *.mat files, and will write the result of the operation in another file. 

## How to use 

A command line is designed for an easier use of the program. The commands works as follows:
[FUNC] [FIRST MATRIX FILE] [SECOND MATRIX FILE (IF NEEDED)] [OPTIONS] [OUTPUT FILE]
| FUNC     | OPTION | MEANING                                                                                 |
|----------|--------|-----------------------------------------------------------------------------------------|
| add      | -      | adds the the first matrix and the second matrix                                         |
| subtract | -      | subtract the second matrix from the first matrix                                        |
| multiply | scalar | multiplies the first and second matrix, or the scalar and the matrix if scalar provided |
| equal    | -      | determines if the first matrix is equal to the second matrix                            |
| trace    | -      | calculates the trace of the input matrix                                                |
| power    | power  | calculates the power of the input matrix using the multiply function                    |
| det      | -      | calculates the determinant of the input matrix                                          |
