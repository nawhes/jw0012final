#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define MAX 50

typedef struct TSTree {
	char data;
	unsigned isEndOfString: 1;
	struct TSTree *left, *eq, *right;
}TSTree;

TSTree* newTSTree(char data){
	TSTree* temp = (TSTree*) malloc(sizeof(TSTree));
	temp->data = data;
	temp->isEndOfString = 0;
	temp->left = NULL;
	temp->eq = NULL;
	temp->right = NULL;
	return temp;
}

void insert(TSTree** root, char *word){
	if(!(*root))
		*root = newTSTree(*word);

	if((*word) < (*root)->data) //less
		insert(&( (*root)->left), word);

	else if((*word) > (*root)->data) //more
		insert(&( (*root)->right ), word);

	else{
		if (*(word+1)) //equal
			insert(&( (*root)->eq), word+1);

		else
			(*root)->isEndOfString = 1;
	}
}

void traverseTSTreeUtil2(TSTree* root, char* buffer, int cursor){
	if(root){
		traverseTSTreeUtil2(root->left, buffer, cursor);

		buffer[cursor] = root->data;
		if(root->isEndOfString){
			buffer[cursor+1] = '\0';
			printf("%s\n",buffer);
		}
		
		traverseTSTreeUtil2(root->eq, buffer, cursor+1);

		traverseTSTreeUtil2(root->right, buffer, cursor);
	}
}


void traverseTSTreeUtil(TSTree* root, char* buffer, int cursor, char *word){
	if(root){
		if(*word < (root)->data){
			traverseTSTreeUtil(root->left, buffer, cursor, word);
		}

		else if(*word > (root)->data){
			traverseTSTreeUtil(root->right, buffer, cursor, word);
		}

		else{
			buffer[cursor] = *word;
			if(*(word+1) == '\0'){
				
				traverseTSTreeUtil2(root->eq, buffer, cursor+1);
			}
			traverseTSTreeUtil(root->eq, buffer, cursor+1, word+1);
		}
	}
}

void traverseTSTree(TSTree* root, char* query){
	char buffer[MAX];
	traverseTSTreeUtil(root, buffer, 0, query);
}

int searchTSTree(TSTree* root, char *word){
	if(!root)
		return 0;
	
	if(*word < (root)->data)
		return searchTSTree(root->left, word);

	else if(*word > (root)->data)
		return searchTSTree(root->right, word);

	else{
		if(*(word+1) == '\0'){
			return root->isEndOfString;
		}

		return searchTSTree(root->eq, word+1);
	}
}
/*
void queryTSTree(TSTree* root){
	char buffer[MAX];
	queryTSTreeUtil(root,buffer,0);
}
*/

int main(){
	TSTree *root = NULL;
	char test[MAX];

	test[0] = 'c';

	insert(&root, "cat");
	insert(&root, "cats");
	insert(&root, "caat");
	insert(&root, "catsstick");
	insert(&root, "up");
	insert(&root, "bug");

	printf("Following is traversal of ternary search tree\n");
	traverseTSTree(root, test);

	printf("\nFollowing are search results for cats, bu and cat respectively\n");
	searchTSTree(root, "cats")? printf("Found\n"): printf("Not Found\n");
	searchTSTree(root, "bu")? printf("Found\n"): printf("Not Found\n");
	searchTSTree(root, "cat")? printf("Found\n"): printf("Not Found\n");

	return 0;
}
	

