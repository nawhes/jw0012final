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

void traverseTSTreeUtil(TSTree* root, char* buffer, int cursor){
	if(root){
		traverseTSTreeUtil(root->left, buffer, cursor);

		buffer[cursor] = root->data;
		if(root->isEndOfString){
			buffer[cursor+1] = '\0';
			printf("%s\n", buffer);
		}

		traverseTSTreeUtil(root->eq, buffer, cursor+1);

		traverseTSTreeUtil(root->right, buffer, cursor);
	}
}

void traverseTSTree(TSTree* root){
	char buffer[MAX];
	traverseTSTreeUtil(root, buffer, 0);
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

	insert(&root, "cat");
	insert(&root, "cats");
	insert(&root, "up");
	insert(&root, "bug");

	printf("Following is traversal of ternary search tree\n");
	traverseTSTree(root);

	printf("\nFollowing are search results for cats, bug and cat respectively\n");
	searchTSTree(root, "cats")? printf("Found\n"): printf("Not Found\n");
	searchTSTree(root, "bu")? printf("Found\n"): printf("Not Found\n");
	searchTSTree(root, "cat")? printf("Found\n"): printf("Not Found\n");

	return 0;
}
	

