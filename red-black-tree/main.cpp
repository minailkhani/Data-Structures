#include "RBT.h"

int main()
{
     RedBlackTree RBT ;
     char reqType ;
     int reqVal ;
     while (true)
     {
          cin>>reqType ;
          if(reqType == 'q')
               return 0 ;
          cin>>reqVal ;
          if(reqType == 'i')
          {
               RBT.insert(reqVal) ;
               RBT.BST() ;  
          }
          else if(reqType == 'd')
          {
               if(RBT.deletion(reqVal))
                    RBT.BST() ;
          }
          else if(reqType == 's')
          {
               RBT.search(reqVal) ;
          }     
     }
     return 0 ;
};