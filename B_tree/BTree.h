#ifndef BTREE_H
#define BTREE_H

#include <cassert>
#include <iostream>

using namespace std;

template<class T>
class Btree
{
private:
	static const int M = 3;
	static const int KEY_MAX = 2 * M -1;	// 5
	static const int KEY_MIN = M - 1;		// 2
	static const int CHILD_MAX = KEY_MAX + 1;	// 6
	static const int CHILD_MIN = KEY_MIN + 1;	// 3

	struct Node
	{
		bool isLeaf;
		int keyNum;
		T keyValue[KEY_MAX];	// 5
		Node* pChild[CHILD_MAX];// 6

		Node(bool b = true, int n = 0):isLeaf(b), keyNum(n){}
	};

public:
	Btree();
	~Btree();
	bool insert(const T& key);
	bool remove(const T& key);
	void display() const;
	bool contain(const T& key) const;
	void clear();
private:
	void recursive_clear(Node* pNode);
	void deleteNode(Node* &pNode);
	bool search(Node* pNode, const T& key) const;
	void splitChild(Node* pParent, int nChildIndex, Node* pChild);
	void insertNonFull(Node* pNode, const T& key);
	void displayInConcavo(Node* pNode, int count) const;
	void mergeChild(Node* pParent, int index);
	void recursive_remove(Node* pNode, const T& key);
	T getPredecessor(Node *pNode);
	T getSuccessor(Node *pNode);
private:
	Node* m_pRoot;

};

template<class T> Btree<T>::Btree()
{
	m_pRoot = NULL; // create a void B tree
}

template<class T>
Btree<T>::~Btree()
{
	clear();
}

template<class T>
bool Btree<T>::insert(const T& key)
{
	if(contain(key))
	{
		return false;
	}
	else
	{
		// if a NULL tree
		if(m_pRoot == NULL)	
		{
			// create node without key
			m_pRoot = new Node();	
			// add first key
			m_pRoot->keyValue[m_pRoot->keyNum] = key;
			m_pRoot->keyNum++;
		}
		else 
		{
			if(m_pRoot->keyNum == KEY_MAX)
			{
				// create new node for saving split tree 2-1-2
				Node* pNode = new Node();
				pNode->isLeaf = false;
				pNode->pChild[0] = m_pRoot;
				splitChild(pNode, 0, m_pRoot);
				m_pRoot = pNode;
			}
			insertNonFull(m_pRoot, key);
		}
		return true;
	}
}

template<class T>
bool Btree<T>::remove(const T& key)
{
	if(!search(m_pRoot, key))
	{
		return false;
	}
	if(m_pRobot->keyNum == 1)
	{
		if(m_pRoot->isLeaf)
		{
			clear();
			return true;
		}
		else
		{
			Node* pChild1 = m_pRoot->pChild[0];
			Node* pChild2 = m_pRoot->pChild[1];
			if(pChild->keyNum == KEY_MIN && pChild->keyNum == KEY_MIN)
			{
				mergeChild(m_pRoot, 0);
				deleteNode(m_pRoot);
				m_pRoot = pChild1;
			}
		}
	}
	recursive_remove(m_pRoot, key);
	return true;
}
template<class T>
void Btree<T>::display() const
{
	displayInConcavo(m_pRoot, KEY_MAX * 10);
}

template<class T>
bool Btree<T>::contain(const T& key) const
{
	return search(m_pRoot, key);
}

template<class T>
void Btree<T>::clear()
{
	recursive_clear(m_pRoot);
	m_pRoot = NULL;
}

template<class T>
void Btree<T>::recursive_clear(Node* pNode)
{
	if(pNode != NULL)
	{
		if(!pNode->isLeaf)
		{
			for(int i = 0; i <= pNode->keyNum; ++i)
			{
				recursive_clear(pNode->pChild[i]);
			}
		}
		deleteNode(pNode);
	}
}

template<class T> void Btree<T>::deleteNode(Node* &pNode)
{
	if(pNode != NULL)
	{
		delete pNode;
		pNode = NULL;
	}
}

template<class T>
bool Btree<T>::search(Node* pNode, const T& key) const
{
	if(pNode == NULL)
	{
		return false;
	}
	else
	{
		int i;
		for(i = 0; i < pNode->keyNum && key > *(pNode->keyValue + i); ++i)
		{
		}
		if(i < pNode->keyNum && key == pNode->keyValue[i])
		{
			return true;
		}
		else
		{
			if(pNode->isLeaf)
			{
				return false;
			}
			else
			{
				return search(pNode->pChild[i], key);
			}
		}
	}
}

// splite 5 childs into 2-1-2 tree
template<class T> 
void Btree<T>::splitChild(Node* pParent, int nChildIndex, Node* pChild)
{
	Node* pRightNode = new Node();
	pRightNode->isLeaf = pChild->isLeaf;
	pRightNode->keyNum = KEY_MIN;
	int i;
	for(i = 0; i < KEY_MIN; ++i)
	{
		// put child node except first two keys into new right
		pRightNode->keyValue[i] = pChild->keyValue[i + CHILD_MIN]; 
	}
	if(!pChild->isLeaf)
	{
		for(i = 0; i < CHILD_MIN; ++i)
		{
			pRightNode->pChild[i] = pChild->pChild[i + CHILD_MIN];
		}
	}
	// reduce left child key number to 2
	pChild->keyNum = KEY_MIN;
	for(i = pParent->keyNum; i > nChildIndex; --i)
	{
		pParent->pChild[i + 1] = pParent->pChild[i];
		pParent->keyValue[i] = pParent->keyValue[i - 1];
	}
	// parent add 1 key
	++pParent->keyNum;
	// right node add
	pParent->pChild[nChildIndex + 1] = pRightNode;
	// parent add the mid key of previous
	pParent->keyValue[nChildIndex] = pChild->keyValue[KEY_MIN];
}

template<class T>
void Btree<T>::insertNonFull(Node* pNode, const T& key)	// before fill a node < MAX
{
	// parent keys
	int i = pNode->keyNum;
	// if this node is a leaf
	if(pNode->isLeaf)	
	{
		// compare key and find place for insert
		while(i > 0 && key < pNode->keyValue[i - 1])
		{
			pNode->keyValue[i] = pNode->keyValue[i - 1];
			-- i;
		}
		pNode->keyValue[i] = key;
		pNode->keyNum++;
	}
	else
	{
		// compare with parent key and find place
		while(i > 0 && key < pNode->keyValue[i - 1])
			--i;
		// find child tree to insert
		Node* pChild = pNode->pChild[i];
		if(pChild->keyNum == KEY_MAX)
		{
			splitChild(pNode, i, pChild);
			if(key > pNode->keyValue[i])
				pChild = pNode->pChild[i + 1];
		}
		insertNonFull(pChild, key);
	}
}

template<class T>
void Btree<T>::displayInConcavo(Node* pNode, int count) const
{
	if(pNode != NULL)
	{
		int i, j;
		for(i = 0; i < pNode->keyNum; ++i)
		{
			if(!pNode->isLeaf)
			{
				displayInConcavo(pNode->pChild[i], count - 8);
			}
			for(j = count; j >= 0; --j)
			{
				cout << "-";
			}
			cout << pNode->keyValue[i] << endl;
		}
		if(!pNode->isLeaf)
		{
			displayInConcavo(pNode->pChild[i], count - 8);
		}
	}
}

template<class T>
void Btree<T>::mergeChild(Node* pParent, int index)
{
	Node* pChild1 = pParent->pChild[index];
	Node* pChild2 = pParent->pChild[index + 1];

	pChild1->keyNum = KEY_MAX;
	pChild1->keyValue[KEY_MIN] = pParent->keyValue[index];
	int i;
	for(i = 0; i < KEY_MIN; ++i)
	{
		pChild1->keyValue[i + KEY_MIN + 1] = pChild2->keyValue[i];
	}
	if(!pChild1->isLeaf)
	{
		for(i = 0; i < CHILD_MIN; ++i)
		{
			pChild1->pChild[i + CHILD_MIN] = pChild2->pChild[i];
		}
	}
	--pParent->keyNum;
	for(i = index; i < pParent->keyNum; ++i)
	{
		pParent->keyValue[i] = pParent->keyValue[i + 1];
		pParent->pChild[i + 1] = pParent->pChild[i + 2];
	}
	deleteNode(pChild2);
}

template<class T>
void Btree<T>::recursive_remove(Node* pNode, const T& key)
{
	int i = 0;
	while(i < pNode->keyNum && key > pNode->keyValue[i])
		++i;
	if(i < pNode->keyNum && key == pNode->keyValue[i])
	{
		if(pNode->isLeaf)
		{
			--pNode->keyNum;
			for(; i < pNode->keyNum; ++i)
			{
				pNode->keyValue[i] = pNode->keyValue[i + 1];
			}
			return;
		}
		else
		{
			Node* pChildNode = pNode->pChild[i];
			if(pChildNode->keyNum == KEY_MIN)
			{
				Node* pLeft = i > 0 ? pNode->pChild[i - 1] : NULL;
				Node* pRight = i < pNode->keyNum ? pNode->pChild[i + 1] : NULL;
				int j;
				if(pLeft && pLeft->keyNum >= CHILD_MIN)
				{
					for(j = pChildNode->keyNum; j > 0; --j)
					{
						pChildNode->keyValue[j] = pChildNode->keyValue[j - 1];
					}
					pChildNode->keyValue[0] = pNode->keyValue[i - 1];
					if(!pLeft->isLeaf)
					{
						for(j = pChildNode->keyNum + 1; j > 0; --j)
						{
							pChildNode->pChild[j] = pChildNode->pChild[j - 1];
						}
						pChildNode->pChild[0] = pLeft->pChild[pLeft->keyNum];
					}
					++pChildNode->keyNum;
					pNode->keyValue[i] = pLeft->keyValue[pLeft->keyNum - 1];
					--pLeft->keyNum;
				}
				else if(pRight && pRight->keyNum >= CHILD_MIN)
				{
					pChildNode->keyValue[pChildNode->keyNum] = pNode->keyValue[i];
					++pChildNode->keyNum;
					pNode->keyValue[i] = pRight->keyvalue[0];

					--pRight->keyNum;
					for(j = 0; j < pRight->keyNum; ++j)
					{
						pRight->keyValue[j] = pRight->keyValue[j + 1];
					}
					if(!pRight->isLeaf)
					{
						pChildNode->pChild[pChildNode->keyNum] = pRight->pChild[0];
						for(j = 0; i <= pRight->keyNum; ++j)
						{
							pRight->pChild[j] = pRight->pChild[j + 1];
						}
					}
				}
				else if(pLeft)
				{
					mergeChild(pNode, i - 1);
					pChildNode = pLeft;
				}
				else if(pRight)
				{
					mergeChild(pNode, i);
				}
			}
			recursive_remove(pChildNode, key);
		}
	}
}

template<class T>
T Btree<T>::getPredecessor(Node *pNode)
{
	while(!pNode->isLeaf)
	{
		pNode = pNode->pChild[pNode->keyNum];
	}
	retrun pNode->keyValue[pNode->keyNum - 1];
}

template<class T>
T Btree<T>::getSuccessor(Node *pNode)
{
	while(!pNode->isLeaf)
	{
		pNode = pNode->pChild[0];
	}
	return pNode->keyvalue[0];
}

#endif