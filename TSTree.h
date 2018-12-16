#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

typedef struct TSTree {
	char data;
	unsigned int isEndOfString;
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

	else{ //equal
		if (*(word+1))
			insert(&( (*root)->eq), word+1);

		else
			(*root)->isEndOfString = 1;
	}
}

TSTree* traverseTSTree(TSTree* root, char *word){
	TSTree* target;
	if(root){
		if(*word < (root)->data){
			target = traverseTSTree(root->left, word);
		}

		else if(*word > (root)->data){
			target = traverseTSTree(root->right, word);
		}

		else{
			if(*(word+1) != '\0'){
				target = traverseTSTree(root->eq ,word+1);
			}
			else {
				return root->eq;
			}
		}
		return target;
	}
}

void freeTSTree(TSTree* root){
	if(root){
		freeTSTree(root->left);
		freeTSTree(root->right);
		freeTSTree(root->eq);
		free(root);
	}
}

void printTSTree(TSTree* root, char* buffer, int cursor){
	if(root){
		printTSTree(root->left, buffer, cursor);

		buffer[cursor] = root->data;
		if(root->isEndOfString){
			buffer[cursor+1] = '\0';
			printf("%s\n", buffer);
		}

		printTSTree(root->eq, buffer, cursor+1);
		printTSTree(root->right, buffer, cursor);
	}
}

int checkLastOneUtil(TSTree* root, int i){
	if(root){
		if((root->left == NULL) && (root->right == NULL)){
			if(root->isEndOfString){
				i++;
			}
			return i + checkLastOneUtil(root->eq, i);
		}
		return 0;
	}
	return 0;
}

int checkLastOne(TSTree* root){
	int i=0;
	i = checkLastOneUtil(root, 0);
	if(i == 1){
		return 1;
	}
	else{
		return 0;
	}
}
	
char* auto_complete(TSTree* root, char* buffer, int cursor){
	if(root){
		buffer[cursor] = root->data;
		if(root->isEndOfString){
			buffer[cursor+1]='\0';
		}
		auto_complete(root->eq, buffer, cursor+1);
	}
	return buffer;
}
		
/*
int main(){
	TSTree *root = NULL;
	TSTree *found = NULL;
	char test[MAX] = "catst";//received string
	char buffer[MAX];
	char* last;
	int abc;

	insert(&root, "cat");
	insert(&root, "cats");
	insert(&root, "caat");
	insert(&root, "catsstick");
	insert(&root, "catsjoy");
	insert(&root, "catstory");
	insert(&root, "catstire");
	insert(&root, "up");
	insert(&root, "bug");
//	insert(&root, "bb");
//	insert(&root, "but");

	printf("Following is traversal of ternary search tree\n");
	found = traverseTSTree(root, test);
	printTSTree(found, buffer, 0);
	if(checkLastOne(found)){
		last = auto_complete(found, buffer, 0);
		strcat(test, last);
		printf("%s\n",test);//sharedmemory asdfg = test
	}
	return 0;
}
*/	

