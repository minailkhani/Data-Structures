#include "BTree.h"

class MatrixElement
{
public:
    int distance = INT_MAX, difficulty, row, col ;
    bool isVisited = false ;
    MatrixElement *prev = NULL ;
};
class MazeNode
{
public:
    int row, column, distance = INT_MAX ;
    MazeNode *prevEdge = NULL ;
    bool operator< (const MazeNode other) const
    {
        if(this->distance == other.distance)
        {
            if(this->row == other.row)
                return this->column < other.column ;
            return this->row < other.row ;
        }
        return this->distance < other.distance ;
    }
    bool operator<= (const MazeNode other) const
    {
        return (this->distance < other.distance) || 
            (this->distance == other.distance && this->row < other.row ) ||
            (this->distance == other.distance && this->row == other.row && this->column <= other.column) ;
    }
    bool operator== (const MazeNode other) const
    {
        return this->distance == other.distance && 
               this->column == other.column && this->row == other.row ;
    }
    bool operator>= (const MazeNode other) const
    {
        return (this->distance > other.distance) || 
            (this->distance == other.distance && this->row > other.row ) ||
            (this->distance == other.distance && this->row == other.row && this->column >= other.column) ;           
    }
    bool operator> (const MazeNode other) const
    {
        if(this->distance == other.distance)
        {
            if(this->row == other.row)
                return this->column > other.column ;
            return this->row > other.row ;
        }
        return this->distance > other.distance ;        
    }
    friend ostream& operator <<(ostream&os, const MazeNode& obj)
    {
        if(obj.distance == INT_MAX)
            return os<<"(("<<obj.row<<" , "<<obj.column<<")"<<" INT_MAX)\t" ;
        else
            return os<<"(("<<obj.row<<" , "<<obj.column<<") "<<obj.distance<<"\t" ;
    }
};
class Maze
{
public:
    Maze(MatrixElement*** m,int i, int j)
        :matrix(m), numOfRow(i), numOfCol(j)
    {
        insert_matrix_into_bTree() ;
        i = 0 ;
        while (matrix[i++][0]->difficulty == 0 )
        {
            if(i >= numOfRow)
            {
                cerr<<"Unsolvable1!\n" ;
                exit(0) ;
            }
        }
        firstEdge = matrix[i-1][0] ;

        i = 0 ;
        while (matrix[i++][numOfCol-1]->difficulty == 0)
            if(i >= numOfRow)
            {
                cerr<<"Unsolvable2!\n" ;
                exit(0) ;
            }
        lastEdge = matrix[i-1][numOfCol-1] ;
    }
    void dijkstra(MatrixElement* startEdge = NULL)
    {
        int a ;
        if(startEdge == NULL)
        {
            startEdge = firstEdge ;
            visit_startEdge() ;
        }
        if(bTree.is_empty() ) 
            return ;
        update_neighbors(startEdge) ;
        auto newStartEdge = find_Edge_to_visit() ;
        visit(newStartEdge, startEdge) ;
        dijkstra(matrix[newStartEdge.row][newStartEdge.column]) ;
    }
    void display_solved_maze()
    {
        update_matrix_of_solved_maze() ;
        display_matrix() ;
    }
    void display_matrix()
    {
        cout<<endl ;
        cout<<setw(4) ;
        for(int j = 0 ; j<numOfCol ; j++)
        {
            for (int i = 0 ; i < numOfRow; i++)
                cout<<matrix[i][j]->difficulty<<setw(4) ;   
            cout<<endl ;
        }
    }
private:
    BTree<MazeNode> bTree ;
    const int numOfRow, numOfCol ;
    MatrixElement *lastEdge ,* firstEdge ;
    MatrixElement ***matrix ;
    LinkedList<MatrixElement*> visitedEdge ;
    void update_matrix_of_solved_maze()
    {
        auto last = visitedEdge.findd(lastEdge)->data ;
        while (last->prev)
        {
            last->difficulty = -1 ;
            last = last->prev ;
        }
        last->difficulty = -1 ;
    }
    void visit_startEdge()
    {
        matrix[firstEdge->row][firstEdge->col]->isVisited = true ;
        delete_from_bTree(firstEdge) ;
        matrix[firstEdge->row][firstEdge->col]->distance = matrix[firstEdge->row][firstEdge->col]->difficulty ;
        visitedEdge.push_back(matrix[firstEdge->row][firstEdge->col]) ;
    }
    void update_neighbors(MatrixElement* justVisited)
    {
        auto i = justVisited->row , j = justVisited->col ;
        if( i-1>=0 )
            if  ( matrix[i-1][j]->difficulty != 0 && !matrix[i-1][j]->isVisited )
                update_node(matrix[i-1][j], matrix[i-1][j]->difficulty+justVisited->distance, justVisited) ;
        if( j-1>=0 )
            if  ( matrix[i][j-1]->difficulty != 0 && !matrix[i][j-1]->isVisited )
                update_node(matrix[i][j-1],matrix[i][j-1]->difficulty+justVisited->distance, justVisited) ;
        if( i+1<numOfRow )
            if  ( matrix[i+1][j]->difficulty != 0 && !matrix[i+1][j]->isVisited )
                update_node(matrix[i+1][j], matrix[i+1][j]->difficulty+justVisited->distance, justVisited) ;
        if( j+1<numOfCol )
             if  ( matrix[i][j+1]->difficulty != 0 && !matrix[i][j+1]->isVisited )
                update_node(matrix[i][j+1], matrix[i][j+1]->difficulty+justVisited->distance, justVisited) ;
    }
    void update_node(MatrixElement* neighbor, int newdistance, MatrixElement* prev)
    {
        if(neighbor->distance > newdistance )
        {
            delete_from_bTree(neighbor) ;
            neighbor->distance = newdistance ;
            neighbor->prev = prev ;

            insert_key_into_bTree(neighbor->row, neighbor->col, newdistance) ;    
        }
    }
    void delete_from_bTree(MatrixElement* element)
    {    
        MazeNode node ;
        node.row = element->row ;
        node.column = element->col ;
        node.distance = element->distance ;
        bTree.deletion(node) ;
    }
    void visit(MazeNode nodeToVisit, MatrixElement* prev)
    {
        matrix[nodeToVisit.row][nodeToVisit.column]->isVisited = true ;
        bTree.deletion(nodeToVisit) ;
        visitedEdge.push_back(matrix[nodeToVisit.row][nodeToVisit.column]) ;
    }
    MazeNode find_Edge_to_visit()
    {
        return bTree.get_min(bTree.get_root())->keys[0]->data ;
    }
    void insert_key_into_bTree(int i, int j,int distance = INT_MAX)
    {
        MazeNode NewKey ;
        NewKey.row = i ;
        NewKey.column = j ;
        NewKey.distance = distance ;
        bTree.insert(NewKey) ;
    }
    void insert_matrix_into_bTree()
    {
        for (int i = 0 ; i < numOfRow; i++)
            for(int j = 0 ; j<numOfCol ; j++)
                if(matrix[i][j]->difficulty != 0)
                    insert_key_into_bTree(i , j) ;
    }
} ;
