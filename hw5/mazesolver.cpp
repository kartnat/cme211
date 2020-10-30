
#include <iostream>
#include <string>

#define ni 205
#define nj 205
int main(int argc, char *argv[]) {

    // Print usage information for insufficient number of inputs
    if (argc < 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << " " << argv[0] << " <maze file> <solution file>" << std::endl; 
        return 0;
    }

    std::string mazefile = argv[1];
    std::string solnfile = argv[2];

    // Check if the static array is large enough for the maze
    int nrows, ncols;
    std::ifstream f;
    f.open(mazefile.c_str());
    if (f.is_open()) {
        f >> nrows >> ncols;
        if (nrows > ni or ncols > nj) {
            std::cout << "Not enough storage available" << std::endl;
            return 0;
        }
    }
        
    // Initializing all array elements to 1's
    int a[ni][nj];
    for (int i=0; i<ni; i++) {
        for (int j=0; j<nj; j++) {
            a[i][j] = 1;
        }
    }
    
    // Changing wall locations to 0's
    int i;
    int j;
    while (f >> i >> j) {
        a[i][j] = 0;
    }
    f.close();    

    // Finding the starting position on the top row
    std::ofstream g;
    g.open(solnfile.c_str());
    int row=0;
    int col=0;
    while (a[row][col] != 1) {
        col++;
    }
    
    //Writing row and column index of staring position to the solution file, g
    if (g.is_open()) {
        g << row << " " << col <<std::endl; 
    }
    
    enum direction
    {
        left,
        right,
        up,
        down
    };
    
    // Main loop which goes through the maze following the right-hand rule
    // Attempt to move right, straight, or left, in that order.  This is 
    // re-interpreted for each direction user is currently facing. (Hence the
    // four cases)
    direction d = down;
    // Keep going until last row is reached
    while (row != nrows-1) {
        switch(d)
        {
            case down:
                // Move location but also update direction
                if (col != 0 && a[row][col-1] != 0) {
                    col = col-1;
                    d = left; 
                }
                else if (row != nrows-1 && a[row+1][col]!= 0) {
                    row = row+1;
                    d = down;
                }
                else if (col != ncols-1 && a[row][col+1] != 0) {
                    col = col+1;
                    d = right; 
               }
                else {
                    row=row-1;
                    d = up; 
                }
                // Writing updated location to solution file
                g << row << " " << col << std::endl;
                break;
            // Repeating for ther three directions: up, left, and right
            case up:
                if (col != ncols-1 && a[row][col+1] != 0) {
                    col = col+1;
                    d = right; 
                }
                else if (row != 0 && a[row-1][col] != 0) {
                    row = row-1;
                    d = up; 
                }
                else if (col != 0 && a[row][col-1] != 0) {
                    col = col-1;
                    d = left; 
                }
                else {
                    row=row+1;
                    d = down; 
                }
                g << row << " " << col << std::endl;
                break;
            case left:
                if (row != 0 && a[row-1][col] != 0) {
                    row = row-1;
                    d = up; 
                }
                else if (col != 0 && a[row][col-1] != 0) {
                    col=col-1;
                    d = left;            }
                else if (row != nrows-1 && a[row+1][col] != 0) {
                    row=row+1;
                    d = down; 
                }
                else {
                    col=col+1;
                    d = right; 
                }      
                g << row << " " << col << std::endl;
                break;
            case right:
                if (row != nrows-1 && a[row+1][col] != 0) {
                    row=row+1;
                    d = down;
                }
                else if (col != ncols-1 && a[row][col+1] != 0) {
                    col=col+1;
                    d = right; 
                }
                else if (row != 0 && a[row-1][col] != 0) {
                    row=row-1;
                    d = up; 
                }
                else {
                    col=col-1;
                    d = left;
                }
                g << row << " " << col << std::endl;
                break;
            }
        }
        g.close();
    return 0;
}
