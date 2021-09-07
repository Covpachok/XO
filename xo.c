#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char field[3][3] = {
	{ ' ', ' ', ' ' },
	{ ' ', ' ', ' ' },
	{ ' ', ' ', ' ' }
};

void write_gamefield()
{
	printf("\n+---+\n|%3.3s|\n|%3.3s|\n|%3.3s|\n+---+\n\n", field[0], field[1], field[2]);
}

// 1 - ok, 0 - not ok
int is_coord_correct(int x, int y)
{
	return !((x <= 0) || (x > 3) || (y <= 0) || (y > 3) ||
			(field[y-1][x-1] == 'O') || (field[y-1][x-1] == 'X'));
}

// 0 - noone, 1 - X, -1 - O
char who_won()
{
	int pX =
			((field[0][0] == 'X') && (field[0][1] == 'X') && (field[0][2] == 'X')) ||
			((field[1][0] == 'X') && (field[1][1] == 'X') && (field[1][2] == 'X')) ||
			((field[2][0] == 'X') && (field[2][1] == 'X') && (field[2][2] == 'X')) ||
			((field[0][0] == 'X') && (field[1][0] == 'X') && (field[2][0] == 'X')) ||
			((field[0][1] == 'X') && (field[1][1] == 'X') && (field[2][1] == 'X')) ||
			((field[0][2] == 'X') && (field[1][2] == 'X') && (field[2][2] == 'X')) ||
			((field[0][0] == 'X') && (field[1][1] == 'X') && (field[2][2] == 'X')) ||
			((field[0][2] == 'X') && (field[1][1] == 'X') && (field[2][0] == 'X'));
	int pO =
			((field[0][0] == 'O') && (field[0][1] == 'O') && (field[0][2] == 'O')) ||
			((field[1][0] == 'O') && (field[1][1] == 'O') && (field[1][2] == 'O')) ||
			((field[2][0] == 'O') && (field[2][1] == 'O') && (field[2][2] == 'O')) ||
			((field[0][0] == 'O') && (field[1][0] == 'O') && (field[2][0] == 'O')) ||
			((field[0][1] == 'O') && (field[1][1] == 'O') && (field[2][1] == 'O')) ||
			((field[0][2] == 'O') && (field[1][2] == 'O') && (field[2][2] == 'O')) ||
			((field[0][0] == 'O') && (field[1][1] == 'O') && (field[2][2] == 'O')) ||
			((field[0][2] == 'O') && (field[1][1] == 'O') && (field[2][0] == 'O'));

	int draw = 0;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			draw += (field[i][j] != ' ');
		}
	}

	if(pX) {
		return 'X';
	} 
	else if (pO) {
		return 'O';
	}
	return draw;
}

void p_turn(int cond, char p1_char, char p2_char)
{
	int x = 0;
	int y = 0;
	char p_char;

	if(!cond) {
		p_char = p1_char;

		// str sample: '1 3\n\0'
		// 1, 3 - coordinates
		// \n - enter
		// \0 - end of line
		char answ[5];
		int i;
		printf("Your turn.\n");
		do {
			printf("Enter coordinates(x 1-3; y 1-3): ");
			fgets(answ, 5, stdin);
			
			int nlpos = -1;
			for(i = 0; i < 5 && answ[i]; i++) {
				if(answ[i] == '\n') {
					nlpos = i;
					break;
				}
			}
			if(nlpos == -1) {
				int c;
				while((c = fgetc(stdin)) != EOF) {
					if(c == '\n')
						break;
				}
			} else {
				x = atoi(&answ[0]);
				y = atoi(&answ[2]);
				printf("%d, %d\n", x, y);
			}
		} while(!is_coord_correct(x, y));
	}

	else {
		p_char = p2_char;
		printf("Enemy turn.\n");
		do {
			x = (rand() % 3) + 1;
			y = (rand() % 3) + 1;
		} while(!is_coord_correct(x, y));
	}

	field[y-1][x-1] = p_char;
	write_gamefield();
}

int main()
{
	time_t t;
	srand((unsigned) time(&t));

	char p1_char, p2_char, pansw, wchar;
	int turn_count, turn_cond;
	int p1_wins_count = 0;
	int p2_wins_count = 0;
	int round_count = 1;

	while(1) {
		printf("@GAME ROUND #%03d\n", round_count);
		turn_count = 1;
		write_gamefield();

		if(round_count % 2) {
			p1_char = 'X';
			p2_char = 'O';
		}
		else {
			p1_char = 'O';
			p2_char = 'X';
		}
		printf("You are [%c].\n", p1_char);

		while(1) {
			// if cond == 2 or 0 then this is p2 turn
			turn_cond = ((turn_count % 2) + (round_count % 2)) == 1;
			p_turn(turn_cond, p1_char, p2_char);
			turn_count++;

			wchar = who_won();
			if(wchar == p1_char) {
				printf("PLAYER1 WINS!\n");
				p1_wins_count++;
				break;
			}
			else if(wchar == p2_char) {
				printf("PLAYER2 WINS!\n");
				p2_wins_count++;
				break;
			}
			else if(wchar == 9) {
				printf("DRAW.\n");
				break;
			}
		}

		if(round_count == 999) {
			printf("Game is over. [999] round reached.\n");
			printf("[Player1 wins: %03d times]\n", p1_wins_count);
			printf("[Player2 wins: %03d times]\n", p2_wins_count);
			return 0;
		}

		printf("Do you want to continue? (Y/N)\n");
		scanf("\n%c", &pansw);
		if((pansw == 'N') || (pansw == 'n')) {
			printf("[Player1 wins: %03d times]\n", p1_wins_count);
			printf("[Player2 wins: %03d times]\n", p2_wins_count);
			return 0;
		}

		// clear field
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				field[i][j] = ' ';
			}
		}
		round_count++;
	}
}
