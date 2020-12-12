// written by Jessalyn Wang
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <stack>
#include <stdexcept>
#include <fstream>
#include <array>
#include <sstream>
#include <iostream>
using namespace std;

// function for setting a queen and propagating all the surrounding squares so that they are no longer options
// inputs: an array representing the board, row and column placements of the queen, and the size of the board
// output: a boolean indicating success
bool set_and_propagate(int **x, int row, int col, int size) {
  int count = 0;
  if (x[row][col] != 0) { // if the spot is already taken by a queen, we return false to indicate this board has no solution
    return false; 
  }
  x[row][col] = 1; // set the queen to 1, indicating a queen
  int i, j;
  // iterate through the board and set any spaces the queen can attack to -1, indicating closed for queen placement
  for (i = 0; i < size; i++) {
    if (i != row)
      x[i][col] = -1;
    if (i != col)
      x[row][i] = -1;
    if (i != 0) {
      if (i <= row && i <= col)
        x[row-i][col-i] = -1;
      if ((i + row) < size && (i+col) < size)
        x[row+i][col+i] = -1;
      if (i <= row && (i+col) < size)
        x[row-i][col+i] = -1;
      if (i <= col && (i+row) < size)
        x[row+i][col-i] = -1;
    }
  }
  // iterates through the board again and counts the number of queens/spaces open for queens
  // this is so that if there are not enough remaining spaces for queens to exist, the stack will kill this branch early
  for(i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (x[row][col] == 0 or x[row][col] == 1) {
        count++;
      }
    }
  }
  // return whether or not there are enough spaces
  return count >= size;
}

// function to count the number of queens placed
// returns a pointer to an array of all the queens if there are enough, NULL otherwise
int** check_num_queens(int **x, int size) {
  // creates a 2d array to return
  int **y = new int*[size];
  int p, q, count = 0;
  for (p = 0; p < size; p++) {
    y[p] = new int[2];
    for (q = 0; q < 2; q++) {
      y[p][q] = 0;
    }
  }
  
  // counts the number of queens
  // also places the placements of any queens found into the array
  for (p = 0; p < size; p++) {
    for (q = 0; q < size; q++) {
      if (x[p][q] == 1) {
        y[q][0] = q;
        y[q][1] = p;
        count++;
      }
    }
  }
  
  // if the count is equal to the size
  // return the pointer
  if (count == size) {
    return y;
  } else { // otherwise, it frees the allocated memory and returns NULL;
    for (p = 0; p < size; p++) {
      delete[] y[p];
    }
    delete[] y;
    return NULL;
  }
}

int main(int argc, char** argv) {
  if (argc != 3) // must provide two arguments as input
  {
      throw std::invalid_argument("Usage: ./nqueens <INPUT FILE> <OUTPUT FILE>"); // throw error
  }
  
  // create some variables for later use and open the input and output files for reading and writing respectively
  string nextBoard;
  
  ifstream input;
  ofstream output;
  
  input.open(argv[1]);
  output.open(argv[2]);
  
  char *next_number, *char_input, *garbage;
  int column = -1, row = -1, size = -1, iter, j, h;
  long int current_number;
  bool flag;
  
  // iterate over each line of the file
  while (getline(input, nextBoard)) {
    // convert to a c string for strtok
    char_input = strdup(nextBoard.c_str());
    
    // gets the first number from the input, which is size
    next_number = strtok(char_input, " ");
    current_number = strtol(next_number, &garbage, 10);
    
    size = (int)current_number;
    
    // if the size is 0, write nothing and return
    // this is because technically there is a solution for a board of size 0: no queens are placed
    if (size == 0) {
      input.close();
      output.close();
      return 0;
    }
    
    // initializes a new board
    int **board = new int*[size];
    for (iter = 0; iter < size; iter++) {
      board[iter] = new int[size];
    }
    for (iter = 0; iter < size; iter++) {
      for (j = 0; j < size; j++) {
        board[iter][j] = 0;
      }
    }
    
    // runs over the rest of the input, and places all preliminary queens
    next_number = strtok(NULL, " ");
    while (next_number != NULL) {
      // uses a double flag system to tell what the next_number represents
      if (column < 0) {
        column = (int)strtol(next_number, &garbage, 10) - 1;
      } else if (row < 0) {
        row = (int)strtol(next_number, &garbage, 10) - 1;
      }
      
      if (column >= 0 && row >= 0) {
        flag = set_and_propagate(board, row, column, size);
        column = -1;
        row = -1;
        // if queens are stacked on top of each other, return no solution
        // alternatively, the board could just have not enough empty spaces to place any more queens
        if (!flag) {
          output << "No solution \n";
          break;
        }
      }
      next_number = strtok(NULL, " ");
    }
    
    // if the board is false, we delete the board and move on to the next line of the file
    if (!flag) {
      for (h = 0; h < size; h++) {
        delete[] board[h];
      }
      delete[] board;
      continue;
    }
    
    // if the code reaches this point, the initial board is probably okay
    // now, we try to solve it
    
    // first push the initial board
    stack<int **> states;
    states.push(board);
    int **popped;
    bool flag3 = false;
    // while the stack isnt empty, we pop a value
    while (!states.empty()) {
      popped = states.top();
      states.pop();
      flag3 = false;
      
      // checks to see if a board is solved. If it is, we write to the output and end the loop
      int **queen_pos = check_num_queens(popped, size);
      if (queen_pos) {
        int out;
        for (out = 0; out < size; out++) {
          output << to_string(queen_pos[out][0] + 1) + " " + to_string(queen_pos[out][1] + 1) + "\n";
        }
        for (h = 0; h < size; h++) {
          delete[] queen_pos[h];
        }
        delete[] queen_pos;
	flag3 = true;
        break;
      }
      
      // iteratively for this entire loop:
      // iterate through a popped board and find an empty spot for a queen_pos
      // if there is an empty spot set the spot to a queen an check to see if the board is completed
      // if the board has finished write the placements and move to the next line
      // otherwise, push the board as long as it can still potentially be completed and iterate again
      // it frees memory along the way
      int iter2, j2;
      for (iter = 0; iter < size; iter++) {
        for (j = 0; j < size; j++) {
          if (popped[iter][j] == 0) {
            int **board2 = new int*[size];
            for (iter2 = 0; iter2 < size; iter2++) {
              board2[iter2] = new int[size];
              for (j2 = 0; j2 < size; j2++) {
                board2[iter2][j2] = popped[iter2][j2];
              }
            }
            bool worked = set_and_propagate(board2, iter, j, size);
            queen_pos = check_num_queens(board2, size);
            if (!queen_pos && worked) {
              states.push(board2);
            } else if (queen_pos && worked) {
              int out2;
              for (out2 = 0; out2 < size; out2++) {
                output << to_string(queen_pos[out2][0] + 1) + " " + to_string(queen_pos[out2][1] + 1) + " ";
              }
              output << "\n";
              for (h = 0; h < size; h++) {
                delete[] queen_pos[h];
                delete[] board2[h];
              }
              delete[] queen_pos;
              delete[] board2;
              flag3 = true;
              break;
            } else {
              for (h = 0; h < size; h++) {
                delete[] board2[h];
              }
              delete[] board2;
            }
          }
        }
      }
      // free the memory of board2
      for (h = 0; h < size; h++) {
        delete[] popped[h];
      }
      delete[] popped;
      
      if (flag3) {
        break;
      }
      
    }
    
    // at this point, the stack could be empty, so we have a flag to indicate if it was
    if (!flag3) {
      output << "No solution\n";
    }
    
    // clear the stack and move on to the next lines
    while (!states.empty()) {
      int **trash;
      trash = states.top();
      states.pop();
      for (h = 0; h < size; h++) {
        delete[] trash[h];
      }
      delete[] trash;
    }
    
    // free the memory from strdup
    free(char_input);
    
  }
  
  
  input.close();
  output.close();
  return 0;
  
}
