#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define mC 26

void initializeBoard(char*, int);
void printBoard(char[26][26], int);
void playCpuTurn(char[26][26], int, char, bool);
void playPlayerTurn(char[26][26], int, char, bool);
bool checkMoveAvailable(char[26][26], int, char, char, char);
bool positionInBounds(int, char, char);
bool checkLegalInDirection(char[26][26], int, char, char, char, int, int);
void performMove(char[26][26], char*, int, char, char, char);
int performDummyMove(char[26][26], int, char, char, char);
bool checkEndGame(char[26][26], int);
void endGame(char[26][26], int);
void forceEndGame(char);

int main(int argc, char **argv)
{
	int n;
	char cpuClr, playerClr;
	printf("Enter the board dimension: ");
	scanf("%d", &n);
	char board[26][26];
	char *ptrBoard = &board[0][0];
	initializeBoard(ptrBoard, n);
	printf("Computer plays (B/W) : ");
	scanf(" %c", &cpuClr);
	playerClr = (cpuClr == 'B')? 'W' : 'B';
	printBoard(board, n);
	if (cpuClr == 'B') playCpuTurn(board, n, cpuClr, false);
	else playPlayerTurn(board, n, playerClr, false);
	return 0;
}

void initializeBoard(char *board, int n) {
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			if ((i == (n/2) - 1 || i == n/2) && (j == n/2 || j == (n/2) - 1))
				if (i == j) *(board+ mC*i + j) = 'W';
				else *(board + mC*i + j) = 'B';
			else
				*(board+ mC*i + j) = 'U';
		}
	}
}

void printBoard(char board[26][26], int n) {
	for (int row=-1; row<n; row++) {
		if (row == -1) {
			printf("  ");
			for (int i=0; i<n; i++) printf("%c", i+'a');
			printf("\n");
		} else {
			printf("%c ", row+'a');
			for (int i=0; i<n; i++) printf("%c", board[row][i]);
			printf("\n");
		}
	}
}

void playCpuTurn(char board[26][26], int n, char clr, bool passed) {
	int score = 0, maxScore = 0;
	char antiClr = (clr == 'B')? 'W' : 'B';
	char *ptrBoard = &board[0][0];
	char moveRow, moveCol;
	bool movesAvailable = false;
	
	int i,j;
	char row, col;
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			row = i + 'a';
			col = j + 'a';
			if (checkMoveAvailable(board, n, row, col, clr)) {
				movesAvailable = true;
				score = performDummyMove(board, n, row, col, clr);
				if (score > maxScore) {
				maxScore = score;
				moveRow = row;
				moveCol = col;
				}
			}
		}
	}
	
	if (movesAvailable) {
		printf("Computer places %c at %c%c.\n", clr, moveRow, moveCol);
		performMove(board, ptrBoard, n, moveRow, moveCol, clr);
		printBoard(board, n);
		return playPlayerTurn(board, n, antiClr, false);
	} else {
		if (checkEndGame(board, n)) return endGame(board, n);
		printf("%c player has no valid move.\n", clr);
		if (passed) return endGame(board, n);
		else return playPlayerTurn(board, n, antiClr, true);
	}
}

int performDummyMove(char board[26][26], int n, char row, char col, char clr) {
	int count=0;
	int i = row - 'a';
	int j = col - 'a';
	int dR, dC;
	for (dR=-1; dR<=1; dR++) {
		for (dC=-1; dC<=1; dC++) {
			if (!(dR == 0 && dC == 0)) {
				if (checkLegalInDirection(board, n, row, col, clr, dR, dC)) {
					for (int k=0, m=0; true; k+=dR, m+=dC) {
						if (board[i+k][j+m] != clr) count++;
						else break;
					}
				}
			}
		}
	}
	return count;
}

void playPlayerTurn(char board[26][26], int n, char clr, bool passed) {
	char antiClr = (clr == 'B')? 'W' : 'B';
	char *ptrBoard = &board[0][0];
	char moveRow, moveCol;
	bool movesAvailable = false;
	
	int i, j;
	char row, col;
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			row = i+'a';
			col = j+'a';
			if (checkMoveAvailable(board, n, row, col, clr)) {
				movesAvailable = true;
				break;
			}
		}
		if (movesAvailable) break;
	}
	
	if (movesAvailable) {
		printf("Enter move for colour %c (RowCol): ", clr);
		scanf(" %c%c", &moveRow, &moveCol);
	
		if (checkMoveAvailable(board, n, moveRow, moveCol, clr)) {
			performMove(board, ptrBoard, n, moveRow, moveCol, clr);
			printBoard(board, n);
			return playCpuTurn(board, n, antiClr, false);
		} else {
			printf("Invalid move.\n");
			return forceEndGame(antiClr);
		}
	} else {
		if (checkEndGame(board, n)) return endGame(board, n);
		printf("%c player has no valid move.\n", clr);
		if (passed) return endGame(board, n);
		else return playCpuTurn(board, n, antiClr, true);
	}
}

bool checkMoveAvailable(char board[26][26], int n, char row, char col, char clr) {
	int i=row-'a';
	int j=col-'a';
	if (clr != 'B' && clr != 'W') return false;
	if (!positionInBounds(n, row, col)) return false;
	if (board[i][j] != 'U') return false;
	bool available = false;
	int dR, dC;
	for (dR=-1; dR<=1; dR++) {
		for (dC=-1; dC<=1; dC++) {
			if (!(dR == 0 && dC == 0)) {
				if (checkLegalInDirection(board, n, row, col, clr, dR, dC)) {
						available = true;
						break;
				}
			}
		}
		if (available) break;
	}
	return available;
}

bool positionInBounds(int n, char row, char col){
	int i = row - 'a';
	int j = col - 'a';
	if (i>=n || i<0) return false;
	if (j>=n || j<0) return false;
	return true;
}

bool checkLegalInDirection(char board[26][26], int n, char row, char col, char colour, int deltaRow, int deltaCol) {
	row -= 'a';
	col -= 'a';
	char antiColour = (colour == 'W')? 'B' : 'W';
	int i, j;
	if (board[row+deltaRow][col+deltaCol] != antiColour) return false;
	bool foundColour = false;
	for (i=row+deltaRow, j=col+deltaCol; ((i+deltaRow<n+1) && (i+deltaRow>=-1)) && ((j+deltaCol)<n+1 && (j+deltaCol>=-1)); i+=deltaRow, j+=deltaCol) {
		if (board[i][j] == colour) {
			foundColour = true;
			break;
		}
	}
	return foundColour;
}

void performMove(char board[26][26], char *ptrBoard, int n, char row, char col, char clr) {
	int i = row - 'a';
	int j = col - 'a';
	int k,m;
	int dR, dC;
	*(ptrBoard + mC*i + j) = clr;
	for (dR=-1; dR<=1; dR++) {
		for (dC=-1; dC<=1; dC++) {
			if (!(dR == 0 && dC == 0)) {
				if (checkLegalInDirection(board, n, row, col, clr, dR, dC)) {
					for (k=dR, m=dC; true; k+=dR, m+=dC) {
						if (*(ptrBoard + mC*(i+k) + (j+m)) != clr) *(ptrBoard + mC*(i+k) + (j+m)) = clr;
						else break;
					}
				}
			}
		}
	}	
}

bool checkEndGame(char board[26][26], int n) {
	int i,j;
	bool over = true;
	for (i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			if (board[i][j] == 'U') {
				over = false;
				break;
			}
		}
		if (!over) break;
	}
	return over;
}

void endGame(char board[26][26], int n) {
	int wPts=0, bPts=0;
	int i,j;
	
	for (i=0; i<n; i++) {
		for(j=0; j<n; j++) {
			if (board[i][j] == 'W') wPts++;
			else if (board[i][j] == 'B') bPts++;
		}
	}
	
	if (wPts > bPts) printf("W player wins.");
	else if (bPts > wPts) printf("B player wins.");
	else printf("Draw!");
}

void forceEndGame(char clr) {
	printf("%c player wins", clr);
}
