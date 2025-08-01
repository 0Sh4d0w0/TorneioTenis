#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_NAME 20
#define NODES 15

typedef struct _PLAYER
{
	char name[MAX_NAME];
	int sets;
}PLAYER;

typedef struct _BTREE_NODE
{
	PLAYER* data;
	struct _BTREE_NODE* left;
	struct _BTREE_NODE* right;
} BTREE_NODE;

typedef BTREE_NODE* BTREE;
typedef enum _BTREE_LOCATION { BTREE_LEFT, BTREE_RIGHT } BTREE_LOCATION;
typedef enum _BOOLEAN { FALSE = 0, TRUE = 1 } BOOLEAN;
typedef enum _STATUS { ERROR = 0, OK = 1 } STATUS;

#define DATA(node) ((node)->data)
#define LEFT(node) ((node)->left)
#define RIGHT(node) ((node)->right)
#define EMPTY NULL
#define NO_LINK NULL

STATUS readPlayersFromFile(PLAYER**, char*);

BTREE_NODE* newBtreeNode(PLAYER* data);

BTREE_NODE* initNode(PLAYER*, BTREE_NODE*, BTREE_NODE*);

BTREE_NODE* createBtree(PLAYER**, int, int);

void freeBtree(BTREE);

void showPlayers(BTREE_NODE*);

BOOLEAN isLeaf(BTREE);

void showLeafs(BTREE);

int getDepth(BTREE);

void showGames(BTREE);

void showWinner(BTREE);

int menu(void);

int main(void)
{
	BTREE btree;
	PLAYER* players[NODES];
	char file_name[MAX_NAME] = "dados.txt";

	if (readPlayersFromFile(players, file_name))
	{
		btree = createBtree(players, 0, NODES);

		int k = 0;

		do{

		system("cls");
		k = menu();

		switch (k)
		{
			case 1:
				// a) Mostrar jogadores
				system("cls");
				showPlayers(btree);
				break;
				
			case 2:
				// c) Mostrar numero de eliminatorias
				system("cls");
				printf("Numero de Eliminatorias: %d\n", getDepth(btree));
				break;
		
			case 3:
				// b) Mostrar jogos
				system("cls");
				showGames(btree);
				break;
		
			case 4:
				// d) Mostrar vencedor
				system("cls");
				printf("\nVencedor: %s\n", DATA(btree)->name);
				showWinner(btree);
				break;
			
			case 0:
				break;
			
			default:
				system("cls");
				printf("Opcao invalida!\n");
				break;
		}

		if (k != 0)
		{
			printf("\nPressione ENTER para voltar ao menu...\n");
			getchar();
			getchar();
		}

		}while(k != 0);
		

		//showLeafs(btree);
		freeBtree(btree);
	}
	else
	{
		printf("ERRO na leitura do ficheiro\n");
		system("pause");
		return 0;
	
	}
}

int menu()
{
	int i = 0;
	
	//Menu
	printf("----------Menu----------\n");
	printf("1. Mostrar jogadores\n");
	printf("2. Mostrar numero de eliminatorias\n");
	printf("3. Mostrar jogos\n");
	printf("4. Mostrar vencedor\n");
	printf("0. Sair\n");
	printf("Escolha uma opcao: ");
	scanf("%d", &i);
	
	return i;
}

STATUS readPlayersFromFile(PLAYER** players, char* file_name)
{
	FILE* fp;
	int j, i = 0;
	PLAYER* ptr_data;
	if ((fp = fopen(file_name, "r")) != NULL)
	{
		while (!feof(fp))
		{
			if ((ptr_data = (PLAYER*)malloc(sizeof(PLAYER))) != NULL)
			{
				fscanf(fp, "%[^;];", (ptr_data)->name);
				fscanf(fp, "%d\n", &((ptr_data)->sets));
				players[i] = ptr_data;
				i++;
			}
			else
			{
				for (j = i; j >= 0; j--)
					free(players[j]);
				return(ERROR);
			}
		}
		fclose(fp);
		return(OK);
	}
	else
		return(ERROR);
}

BOOLEAN isLeaf(BTREE node1)
{
	if (LEFT(node1) == NULL && RIGHT(node1) == NULL)
		return(TRUE);
	else if (LEFT(node1) != NULL || RIGHT(node1) != NULL)
		return(FALSE);
}

void showLeafs(BTREE node1)
{
	if(node1 == NULL)
		return;

	if (node1 != NULL)
	{
		if (isLeaf(node1))
			showPlayers(node1);
		else
		{
			showLeafs(LEFT(node1));
			showLeafs(RIGHT(node1));
		}
	}
}

// b) Mostrar jogos
void showGames(BTREE node1)
{
	int i = 4;

	if (node1 == NULL)
		return;

	if(isLeaf(node1) == FALSE)
	{
		showGames(LEFT(node1));
		showGames(RIGHT(node1));

		printf("%s - %d : %d - %s\n", DATA(LEFT(node1))->name, DATA(LEFT(node1))->sets, DATA(RIGHT(node1))->sets, DATA(RIGHT(node1))->name);		
	}		
}

// d) Mostrar vencedor
void showWinner(BTREE node1)
{
	if (node1 == NULL)
		return;

	char vencedor[MAX_NAME] = "Jogador4";

	//if (isLeaf(LEFT(node1)) == FALSE && isLeaf(RIGHT(node1)) == FALSE)
	if (!isLeaf(node1))
	{
		if (strcmp(DATA(LEFT(node1))->name, vencedor)==0)
		{
			printf("\n %s - %d : %d - %s\n", DATA(LEFT(node1))->name, DATA(LEFT(node1))->sets, DATA(RIGHT(node1))->sets, DATA(RIGHT(node1))->name);
			showWinner(LEFT(node1));
		}
		
		if (strcmp(DATA(RIGHT(node1))->name, vencedor)==0)
		{
			printf("\n %s - %d : %d - %s\n", DATA(LEFT(node1))->name, DATA(LEFT(node1))->sets, DATA(RIGHT(node1))->sets, DATA(RIGHT(node1))->name);
			showWinner(RIGHT(node1));
		}
	}

}

// c) Mostrar numero de eliminatorias
int getDepth(BTREE node1)
{
	int depth = 0, left_depth = 0, right_depth = 0;
	
	if (node1 == NULL)
		return 0;

	if(isLeaf(node1))
		return 1;
	
	if (LEFT(node1) != NULL)
		left_depth++;
	if (RIGHT(node1) != NULL)
		right_depth++;

	left_depth = getDepth(LEFT(node1));
	right_depth = getDepth(RIGHT(node1));

	if(left_depth > right_depth)
		depth = left_depth + 1;
	else
		depth = right_depth + 1;

	return(depth);
}

// a) Mostrar jogadores
void showPlayers(BTREE_NODE* node1)

{
	if(LEFT(node1) == NULL)
		printf("%s\n", DATA(node1)->name);
	else
	{
		printf("%s\n", DATA(node1)->name);
		showPlayers(LEFT(node1));
		showPlayers(RIGHT(node1));
	}
}	

BTREE_NODE* newBtreeNode(PLAYER* data)
{
	BTREE_NODE* tmp_pt;
	if ((tmp_pt = (BTREE_NODE*)malloc(sizeof(BTREE_NODE))) != NULL)
	{
		DATA(tmp_pt) = data;
		LEFT(tmp_pt) = RIGHT(tmp_pt) = NULL;
	}
	return tmp_pt;
}

BTREE_NODE* initNode(PLAYER* ptr_data, BTREE_NODE* node1, BTREE_NODE* node2)
{
	BTREE_NODE* tmp_pt = NULL;
	tmp_pt = newBtreeNode(ptr_data);
	LEFT(tmp_pt) = node1;
	RIGHT(tmp_pt) = node2;
	return(tmp_pt);
}

BTREE_NODE* createBtree(PLAYER** v, int i, int size)
{
	if (i >= size)
		return(NULL);
	else
		return(initNode(*(v + i), createBtree(v, 2 * i + 1, size), createBtree(v, 2 * i + 2, size)));
}

void freeBtree(BTREE btree)
{
	if (btree != NULL)
	{
		freeBtree(LEFT(btree));
		freeBtree(RIGHT(btree));
		free(DATA(btree));
		free(btree);
	}
}