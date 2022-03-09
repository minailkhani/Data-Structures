#include "Maze.h"

MatrixElement*** get_matrix(int &n, int &m)
{
    int degree ;
    MatrixElement*** matrix = new MatrixElement** [n];
    for (int i = 0 ; i < n; i++)
        matrix[i] = new MatrixElement*[m];
    for(int j = 0 ; j<m ; j++)
        for (int i = 0 ; i < n; i++)
            matrix[i][j] = new MatrixElement ;
    

    for(int j = 0 ; j<m ; j++)
    {
        for (int i = 0 ; i < n; i++)
        {
            cin>>degree ;
            matrix[i][j]->col = j ;
            matrix[i][j]->row = i ;
            matrix[i][j]->difficulty = degree ;
        }
    }
    return matrix ;
}
int main()
{
    int n, m ;
    cin>>m>>n ;
    auto matrixMaze = get_matrix(n, m) ;
    Maze M(matrixMaze, n, m) ;
    M.dijkstra() ;
    M.display_solved_maze() ;
    return 1 ;
}