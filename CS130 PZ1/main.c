#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef int Bool;

#define True 1
#define False 0
#define FILE_PATH "data.txt"
#define MAX_ERRORS 3

#define VAR_CHAR 50
struct Question {
	char original[VAR_CHAR];
	char prefix[5];
	char translated[VAR_CHAR];
};

struct Game{
	int currentQuestion;
	int currentErrors;
	int questionCount;
	struct Question* questions;
};

struct Question* getNext(struct Game* game) {
	if (game->currentQuestion >= game->questionCount) {
		return NULL;
	}
	struct Question* result = &game->questions[game->currentQuestion];
	game->currentQuestion += 1;
	return result;
}

void toLowerCase(char* str) {
	while (*str) {
		*str = tolower(*str);
		str++;
	}
}

Bool checkQuestion(const struct Question* question, const char* buffer) {
	char a[VAR_CHAR];
	char b[VAR_CHAR];

	strcpy(a, question->translated);
	strcpy(b, buffer);
	
	toLowerCase(a);
	toLowerCase(b);

	if (strcmp(a, b) == 0) {
		return True;
	}
	return False;
}

Bool loadFromFile(const char* fileName, struct Game* game) {
	if (game == NULL) {
		printf("Couldn't load from file!\n");
		return False;
	}
	FILE* file = fopen(fileName, "rb");
	if (file == NULL) {
		printf("Couldn't open file\n");
		return False;
	}
	fread(&game->questionCount, sizeof(int), 1, file);
	game->questions = malloc(sizeof(struct Question) * game->questionCount);
	fread(game->questions, sizeof(struct Question), game->questionCount, file);
	fclose(file);
	printf("File loaded successfully!\n");
	return True;
}

void saveToFile(const char* fileName, const struct Game* game) {
	if (game == NULL || game->questions == NULL || game->questionCount == NULL) {
		printf("Couldn't load from file!\n");
		return False;
	}
	FILE* file = fopen(fileName, "wb");
	if (file == NULL) {
		printf("Couldn't open file\n");
		return False;
	}
	fwrite(&game->questionCount, sizeof(int), 1, file);
	fwrite(game->questions, sizeof(struct Question), game->questionCount, file);
	fclose(file);
	printf("File loaded successfully!\n");
}

void addQuestion(int index, struct Game* game, const char* original, const char* prefix,const char* translated) {
	strcpy(game->questions[index].original, original);
	strcpy(game->questions[index].prefix, prefix);
	strcpy(game->questions[index].translated, translated);
	game->questionCount = index + 1;
}

void createFile(const char* filePath, struct Game* game) {
	game->questions = malloc(sizeof(struct Question) * 10);
	
	addQuestion(0, game, "Livre", "FRA", "Book");
	addQuestion(1, game, "Playa", "ESP", "Beach");
	addQuestion(2, game, "Scuola", "ITA", "School");
	addQuestion(3, game, "Hund", "GER", "Dog");
	addQuestion(4, game, "Bonjour", "FRA", "Hello");
	addQuestion(5, game, "Casa", "ESP", "House");
	addQuestion(6, game, "Amore", "ITA", "Love");
	addQuestion(7, game, "Katze", "GER", "Cat");
	addQuestion(8, game, "Musique", "FRA", "Music");
	addQuestion(9, game, "Meer", "GER", "Sea");

	saveToFile(FILE_PATH, game);
	free(game->questions);
	game->questionCount = 0;
}

int main() {
	struct Game mainGame;
	
	mainGame.currentErrors = 0;
	mainGame.currentQuestion = 0;
	
	//kada zelimo da kreiramo novi fajl sa pitanjima pozivamo funkciju ispod 
	//createFile(FILE_PATH, &mainGame);

	loadFromFile(FILE_PATH, &mainGame);

	char input[VAR_CHAR];

	while (True) {

		const struct Question* current = getNext(&mainGame);

		if (current == NULL) {
			printf("Igra je gotova!\n");
			break;
		}

		printf("Prevedi datu rec: %s (%s)\n", current->original, current->prefix);

		scanf("%s", input);

		if (strcmp(input, "EXIT") == 0) {
			printf("Izlazim iz igre!\n");
			break;
		}

		printf("\033[2J");
		printf("\033[H");
		
		if (checkQuestion(current, input) == False) {
			mainGame.currentErrors += 1;

			printf("Odgovor je netacan, napravili ste %d od %d mogucih gresaka.\n", mainGame.currentErrors, MAX_ERRORS);
		}
		else {
			printf("Odgovor je tacan!\n");
		}

		if (mainGame.currentErrors >= MAX_ERRORS) {
			printf("Izgubili ste igru!\n");
			break;
		}
	}

	printf("Osvojili ste rezultat: %d\n", mainGame.currentQuestion - mainGame.currentErrors);

	free(mainGame.questions);

	return 0;
}