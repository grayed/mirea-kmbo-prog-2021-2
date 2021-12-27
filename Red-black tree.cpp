#include <iostream>
#include <vector>

using namespace std;

// pizdec cho za derevo

enum NodeColor : int {
   BLACK = 0,
   RED,
};

// node struct
template<class Value>
struct BinaryTreeNode {
   Value Key;
   NodeColor Color;
   BinaryTreeNode<Value>* pLeft;
   BinaryTreeNode<Value>* pRight;
   BinaryTreeNode<Value>* pParent;

   BinaryTreeNode<Value>* SearchMin( ); // search min key node
   BinaryTreeNode<Value>* SearchMax( ); // search max key node

   // node constructor
   BinaryTreeNode( Value _Key, NodeColor _Color, BinaryTreeNode* _pParent, BinaryTreeNode* _pLeft, BinaryTreeNode* _pRight ) :
	  Key( _Key ), Color( _Color ), pParent( _pParent ), pLeft( _pLeft ), pRight( _pRight ) {
   };
};

template<class T>
BinaryTreeNode<T>* BinaryTreeNode<T>::SearchMin( ) {
   BinaryTreeNode<T>* pFinalNode = this;

   while ( pFinalNode->pLeft )
	  pFinalNode = pFinalNode->pLeft;

   return pFinalNode;
}

template<class T>
BinaryTreeNode<T>* BinaryTreeNode<T>::SearchMax( ) {
   BinaryTreeNode<T>* pFinalNode = this;
  
   while ( pFinalNode->pRight )
	  pFinalNode = pFinalNode->pRight;

   return pFinalNode;
}

// main tree class
template<class T>
class BinaryTree {
   // out-class functions
public:
   // class constructor
   BinaryTree( ) {
	  pRoot = nullptr;
   }

   // class destructor
   ~BinaryTree( ) {
	  Destroy( pRoot );
   }

   void Insert( T Key ); // insert node by key
   void Remove( T Key ); // delete node by key

   void Print( BinaryTreeNode< T >* pNode ) {
	  if ( pNode != NULL ) {
		 cout << pNode->Key;
		 Print( pNode->pLeft );
		 Print( pNode->pRight );
	  }
   }

   BinaryTreeNode<T>* Search( T Key ); // search node by key
   BinaryTreeNode<T>* SearchMin( ); // search min key node
   BinaryTreeNode<T>* SearchMax( ); // search max key node
public:
   class Iterator {
   public:
	  Iterator& Next( ) {
		 if ( !pCurrentNode->pParent &&
			  pCurrentNode->pLeft && NodeUsedInPost( pCurrentNode->pLeft ) &&
			  pCurrentNode->pRight && NodeUsedInPost( pCurrentNode->pRight ) ) {

			// the end...
			bEnd = true;
			return *this;
		 }

		 bool bLeftVerify = !pCurrentNode->pLeft || ( pCurrentNode->pLeft && NodeUsedInPost( pCurrentNode->pLeft ) );
		 bool bRightVerify = !pCurrentNode->pRight || ( pCurrentNode->pRight && NodeUsedInPost( pCurrentNode->pRight ) );

		 if ( bLeftVerify && bRightVerify ) {
			vecVerifyNodes.emplace_back( pCurrentNode );
			pCurrentNode = pCurrentNode->pParent;
			return *this;
		 }

		 if ( pCurrentNode->pLeft && !NodeUsedInPost( pCurrentNode->pLeft ) ) {
			pCurrentNode = pCurrentNode->pLeft;
			return *this;
		 }

		 if ( pCurrentNode->pRight && !NodeUsedInPost( pCurrentNode->pRight ) ) {
			pCurrentNode = pCurrentNode->pRight;
			return *this;
		 }
	  }

	  Iterator& Prev( ) {
		 if ( pCurrentNode && pCurrentNode->pParent ) {
			pCurrentNode = pCurrentNode->pParent;
			return *this;
		 } 

		 return *this;
	  }

	  void StepRight( ) {
		 if ( pCurrentNode->pRight )
			pCurrentNode = pCurrentNode->pRight->SearchMin( );
		 else if ( pCurrentNode->pParent ) {
			while ( pCurrentNode->pParent && pCurrentNode->pParent->pRight == pCurrentNode )
			   pCurrentNode = pCurrentNode->pParent;
			if ( pCurrentNode->pParent )
			   pCurrentNode = pCurrentNode->pParent;
			else
			   bEnd = true;
		 } else
			bEnd = true;
	  }

	  void StepLeft( ) {
		 if ( pCurrentNode->pLeft )
			pCurrentNode = pCurrentNode->pLeft->SearchMax( );
		 else if ( pCurrentNode->pParent ) {
			while ( pCurrentNode->pParent && pCurrentNode->pParent->pLeft == pCurrentNode )
			   pCurrentNode = pCurrentNode->pParent;
			if ( pCurrentNode->pParent )
			   pCurrentNode = pCurrentNode->pParent;
			else
			   bEnd = true;
		 } else
			bEnd = true;
	  }

	  Iterator& operator=( BinaryTreeNode< T >* pNode ) {
		 pCurrentNode = pNode;
		 return *this;
	  }

	  Iterator& operator=( const Iterator& It ) {
		 pCurrentNode = It.pCurrentNode;
		 vecVerifyNodes = It.vecVerifyNodes;
		 return *this;
	  }

	  Iterator& operator++( int ) {
		 StepRight( );

		 return *this;
	  }

	  Iterator& operator++( ) {
		 StepRight( );

		 return *this;
	  }

	  Iterator& operator--( int ) {
		 StepLeft( );

		 return *this;
	  }

	  Iterator& operator--( ) {
		 StepLeft( );

		 return *this;
	  }

	  bool NodeUsedInPost( BinaryTreeNode< T >* pNode ) {
		 for ( BinaryTreeNode< T >* Node : vecVerifyNodes ) {
			if ( Node->Key == pNode->Key )
			   return true;
		 }

		 return false;
	  }

	  bool End( ) {
		 return bEnd;
	  }

	  bool bEnd = false;

	  BinaryTreeNode< T >* pCurrentNode;

	  vector< BinaryTreeNode< T >* > vecVerifyNodes;
   };

   Iterator Begin( ) {
	  BinaryTree<T>::Iterator it;
	  it.pCurrentNode = pRoot;
	  it.vecVerifyNodes.emplace_back( pRoot );
	  return it;
   }

   // in-class functions
private:
   // Destroying red and black nodes
   void Destroy( BinaryTreeNode<T>*& Node );

   void LeftRotate( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* x ); // Left direction in node
   void RightRotate( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* y ); // Right direction in node

   void Remove( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode );// delete node by key
   void RemoveFixUp( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode, BinaryTreeNode<T>* pParent ); // Help function

   void Insert( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode ); // Insert node by internal
   void InsertFixUp( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode ); // Help function

   BinaryTreeNode<T>* Search( BinaryTreeNode<T>* Node, T Key ) const;  // search node by key by internal

// class members
private:
   BinaryTreeNode<T>* pRoot;
};

template<class T>
BinaryTreeNode<T>* BinaryTree<T>::Search( T Key ) {
   return Search( pRoot, Key );
}

template<class T>
BinaryTreeNode<T>* BinaryTree<T>::SearchMin( ) {
   BinaryTreeNode<T>* pFinalNode = pRoot;

   while ( pFinalNode->pLeft )
	  pFinalNode = pFinalNode->pLeft;

   return pFinalNode;
}

template<class T>
BinaryTreeNode<T>* BinaryTree<T>::SearchMax( ) {
   BinaryTreeNode<T>* pFinalNode = pRoot;

   while ( pFinalNode->pRight )
	  pFinalNode = pFinalNode->pRight;

   return pFinalNode;
}

template<class T>
BinaryTreeNode<T>* BinaryTree<T>::Search( BinaryTreeNode<T>* Node, T Key ) const {
   if ( Node == NULL || Node->Key == Key )
	  return Node;
   else
	  if ( Key > Node->Key )
		 return Search( Node->pRight, Key );
	  else
		 return Search( Node->pLeft, Key );
}

template<class T>
void BinaryTree<T>::Destroy( BinaryTreeNode<T>*& Node ) {
   // if node nullptr, we can't touch him
   if ( Node == NULL )
	  return;

   // also destroy left and right node's
   Destroy( Node->pLeft );
   Destroy( Node->pRight );

   // clear node
   delete Node;
   Node = nullptr;
}

template<class T>
void BinaryTree<T>::Insert( T Key ) {
   // create new node
   BinaryTreeNode<T>* pNewNode = new BinaryTreeNode<T>( Key, RED, NULL, NULL, NULL );

   // add new node
   Insert( pRoot, pNewNode );
}

template<class T>
void BinaryTree<T>::Remove( T Key ) {
   BinaryTreeNode<T>* pDeleteNode = Search( pRoot, Key );
   if ( pDeleteNode != NULL )
	  Remove( pRoot, pDeleteNode );
}

template<class T>
void BinaryTree<T>::Remove( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode ) {
   BinaryTreeNode<T>* pChild, * pParent;
   NodeColor Color;

   if ( pNode->pLeft != NULL && pNode->pRight != NULL ) {
	  BinaryTreeNode<T>* pReplace = pNode;

	  pReplace = pNode->pRight;
	  while ( pReplace->pLeft != NULL ) {
		 pReplace = pReplace->pLeft;
	  }

	  if ( pNode->pParent != NULL ) {
		 if ( pNode->pParent->pLeft == pNode )
			pNode->pParent->pLeft = pReplace;
		 else
			pNode->pParent->pLeft = pReplace;
	  } else
		 Root = pReplace;

	  pChild = pReplace->pRight;
	  pParent = pReplace->pParent;
	  Color = pReplace->Color;

	  if ( pParent == pNode )
		 pParent = pReplace;
	  else {
		 if ( pChild != NULL )
			pChild->pParent = pParent;
		 pParent->pLeft = pChild;

		 pReplace->pRight = pNode->pRight;
		 pNode->pRight->pParent = pReplace;
	  }

	  pReplace->pParent = pNode->pParent;
	  pReplace->Color = pNode->Color;
	  pReplace->pLeft = pNode->pLeft;
	  pNode->pLeft->pParent = pReplace;

	  if ( Color == BLACK )
		 RemoveFixUp( Root, pChild, pParent );

	  delete pNode;
	  return;
   }

   if ( pNode->pLeft != NULL )
	  pChild = pNode->pLeft;
   else
	  pChild = pNode->pRight;

   pParent = pNode->pParent;
   Color = pNode->Color;
   if ( pChild ) {
	  pChild->pParent = pParent;
   }

   if ( pParent ) {
	  if ( pNode == pParent->pLeft )
		 pParent->pLeft = pChild;
	  else
		 pParent->pRight = pChild;
   } else
	  pRoot = pChild;

   if ( Color == BLACK ) {
	  RemoveFixUp( Root, pChild, pParent );
   }

   delete pNode;
}

template<class T>
void BinaryTree<T>::RemoveFixUp( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode, BinaryTreeNode<T>* pParent ) {
   BinaryTreeNode<T>* pOtherNode;

   while ( ( !pNode ) || pNode->Color == BLACK && pNode != pRoot ) {
	  if ( pParent->pLeft == pNode ) {
		 pOtherNode = pParent->pRight;
		 if ( pOtherNode->Color == RED ) {
			pOtherNode->Color = BLACK;
			pParent->Color = RED;
			LeftRotate( Root, pParent );
			pOtherNode = pParent->pRight;
		 } else {
			if ( !( pOtherNode->pRight ) || pOtherNode->pRight->Color == BLACK ) {
			   pOtherNode->pLeft->Color = BLACK;
			   pOtherNode->Color = RED;
			   RightRotate( Root, pOtherNode );
			   pOtherNode = pParent->pRight;
			}
			pOtherNode->Color = pParent->Color;
			pParent->Color = BLACK;
			pOtherNode->pRight->Color = BLACK;
			LeftRotate( Root, pParent );
			pNode = Root;
			break;
		 }
	  } else {
		 pOtherNode = pParent->pLeft;
		 if ( pOtherNode->Color == RED ) {
			pOtherNode->Color = BLACK;
			pParent->Color = RED;
			RightRotate( Root, pParent );
			pOtherNode = pParent->pLeft;
		 }
		 if ( ( !pOtherNode->pLeft || pOtherNode->pLeft->Color == BLACK ) && ( !pOtherNode->pRight || pOtherNode->pRight->Color == BLACK ) ) {
			pOtherNode->Color = RED;
			pNode = pParent;
			pParent = pNode->pParent;
		 } else {
			if ( !( pOtherNode->pLeft ) || pOtherNode->pLeft->Color == BLACK ) {
			   pOtherNode->pRight->Color = BLACK;
			   pOtherNode->Color = RED;
			   LeftRotate( Root, pOtherNode );
			   pOtherNode = pParent->pLeft;
			}
			pOtherNode->Color = pParent->Color;
			pParent->Color = BLACK;
			pOtherNode->pLeft->Color = BLACK;
			RightRotate( Root, pParent );
			pNode = Root;
			break;
		 }
	  }
   }

   if ( pNode )
	  pNode->Color = BLACK;
}

template<class T>
void BinaryTree<T>::LeftRotate( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pX ) {
   BinaryTreeNode<T>* pY = pX->pRight;

   pX->pRight = pY->pLeft;

   if ( pY->pLeft != NULL )
	  pY->pLeft->pParent = pX;

   pY->pParent = pX->pParent;

   if ( pX->pParent == NULL )
	  Root = pY;
   else {
	  if ( pX == pX->pParent->pLeft )
		 pX->pParent->pLeft = pY;
	  else
		 pX->pParent->pRight = pY;
   }

   pY->pLeft = pX;
   pX->pParent = pY;
}

template<class T>
void BinaryTree<T>::RightRotate( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pY ) {
   BinaryTreeNode<T>* pX = pY->pLeft;

   pY->pLeft = pX->pRight;

   if ( pX->pRight != NULL )
	  pX->pRight->pParent = pY;

   pX->pParent = pY->pParent;

   if ( pY->pParent == NULL )
	  Root = pX;
   else {
	  if ( pY == pY->pParent->pRight )
		 pY->pParent->pRight = pX;
	  else
		 pY->pParent->pLeft = pX;
   }

   pX->pRight = pY;
   pY->pParent = pX;
}

template<class T>
void BinaryTree<T>::Insert( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode ) {
   BinaryTreeNode<T>* x = Root;
   BinaryTreeNode<T>* y = NULL;

   while ( x != NULL ) {
	  y = x;
	  if ( pNode->Key > x->Key )
		 x = x->pRight;
	  else
		 x = x->pLeft;
   }

   pNode->pParent = y;

   if ( y != NULL ) {
	  if ( pNode->Key > y->Key )
		 y->pRight = pNode;
	  else
		 y->pLeft = pNode;
   } else
	  Root = pNode;

   pNode->Color = RED;

   InsertFixUp( Root, pNode );
}

template<class T>
void BinaryTree<T>::InsertFixUp( BinaryTreeNode<T>*& Root, BinaryTreeNode<T>* pNode ) {
   BinaryTreeNode<T>* pParent;

   pParent = pNode->pParent;

   while ( pNode != pRoot && pParent->Color == RED ) {
	  BinaryTreeNode<T>* pSecondParent = pParent->pParent;

	  if ( pSecondParent->pLeft == pParent ) {
		 BinaryTreeNode<T>* pRight = pSecondParent->pRight;

		 if ( pRight != NULL && pRight->Color == RED ) {
			pParent->Color = BLACK;
			pRight->Color = BLACK;
			pRight->Color = RED;
			pNode = pRight;
			pParent = pNode->pParent;
		 } else {
			if ( pParent->pRight == pNode ) {
			   LeftRotate( Root, pParent );
			   swap( pNode, pParent );
			}

			RightRotate( Root, pRight );
			pSecondParent->Color = RED;
			pParent->Color = BLACK;
			break;
		 }
	  } else {
		 BinaryTreeNode<T>* pLeft = pSecondParent->pLeft;

		 if ( pLeft != NULL && pLeft->Color == RED ) {
			pSecondParent->Color = RED;
			pParent->Color = BLACK;
			pLeft->Color = BLACK;

			pNode = pSecondParent;
			pParent = pNode->pParent;
		 } else {
			if ( pParent->pLeft == pNode ) {
			   RightRotate( Root, pParent );
			   swap( pParent, pNode );
			}

			LeftRotate( Root, pSecondParent );
			pParent->Color = BLACK;
			pSecondParent->Color = RED;
			break;
		 }
	  }
   }

   Root->Color = BLACK;
}

void main( ) {
   BinaryTree< int > Tree;

   // add tree elements
   for ( size_t i = 0; i < 20; i++ ) {
	  Tree.Insert( i );
   }

   // useless functuion
   // Tree.Print( Tree.Begin( ).pCurrentNode );

 #if 1
   // iterate tree elements using iterator like std containers
   printf( "iterate right direction\n" );

   for ( BinaryTree<int>::Iterator i = Tree.Begin( ); !i.End( ); i++ ) {
	  printf( "%i\n", i.pCurrentNode->Key );
   }

   printf( "iterate left direction\n" );

   for ( BinaryTree<int>::Iterator i = Tree.Begin( ); !i.End( ); i-- ) {
	  printf( "%i\n", i.pCurrentNode->Key );
   }

   printf( "min key %i\n", Tree.SearchMin( )->Key );
   printf( "max key %i\n", Tree.SearchMax( )->Key );
#endif

#if 0
   // remove example
   Tree.Remove( 2 );

   printf( "----------------------------------------------------------------\n\n" );

   for ( BinaryTree<int>::Iterator i = Tree.Begin( ); !i.End( ); i++ ) {
	  printf( "%i\n", i.pCurrentNode->Key );
   }
#endif

   system( "pause" );
}