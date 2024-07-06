#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WORD_SIZE 30
#define NO_WORDS 100
#define BIG_WORD_SIZE 300
#define GRAM_SIZE 5000

struct n_gram_freq {
	char word1[WORD_SIZE];
	char word2[WORD_SIZE];
	int freq;
};

struct words_freq {
	char word[WORD_SIZE];
	int freq;
};

void read_n_grams(struct n_gram_freq *grams, int *n, char *filename) {
	FILE *f = fopen(filename, "r");

	fscanf(f, "%d", n);

	for (int i = 0; i < *n; i++) {
		fscanf(f, "%s %s %d\n", grams[i].word1, grams[i].word2, &grams[i].freq);
	}
}

void read_words(struct words_freq *words, int *n, char *filename) {
	FILE *f = fopen(filename, "r");

	fscanf(f, "%d", n);

	for (int i = 0; i < *n; i++) {
		fscanf(f, "%s %d\n", words[i].word, &words[i].freq);
	}
}

double compute_ratio(struct n_gram_freq *grams, struct words_freq *words, int nr_grams, int nr_words, char *w1, char *w2) {
	char n_gram[2 * WORD_SIZE];
	strcpy(n_gram, w1);
	strcat(n_gram, " ");
	strcat(n_gram, w2);

	printf("%s\n", n_gram);

	double a = 0.0;
	double b = 0.0;

	for (int i = 0; i < nr_grams; i++) {
		char combined[2 * WORD_SIZE];
		strcpy(combined, grams[i].word1);
		strcat(combined, " ");
		strcat(combined, grams[i].word2);

		if (strcmp(combined, n_gram) == 0) {
			a = (double) grams[i].freq;
			//printf("a = %lf\n", a);
		}
	}

	for (int i = 0; i < nr_words; i++) {
		if (strcmp(w2, words[i].word) == 0) {
			b = (double) words[i].freq;
			//printf("b = %lf\n", b);
		}
	}

	return a / b;
}

void best_word(struct n_gram_freq *grams, struct words_freq *words, char *last_word, int nr_grams, int nr_words, char *best_word) {
	double max_ratio = 0, ratio = 0;
	char possible_words[NO_WORDS][WORD_SIZE];
	int k = 0;

	for (int i = 0; i < nr_grams; i++) {
		if (strcmp(last_word, grams[i].word1) == 0) {
			strcpy(possible_words[k], grams[i].word2);
			k++;
		}
	}

	for (int i = 0; i < k; i++) {
		printf("%s\n", possible_words[i]);
	}

	for (int i = 0; i < k; i++) {
		ratio = compute_ratio(grams, words, nr_grams, nr_words, last_word, possible_words[i]);
		printf("%lf\n", ratio);
		if (ratio >= max_ratio) {
			max_ratio = ratio;
			strcpy(best_word, possible_words[i]);
		}
	}

	return;
}

int main(int argc, char **argv) {
	int nr_grams = 0, nr_words = 0;
	struct n_gram_freq grams[GRAM_SIZE];
	for (int i = 0; i < GRAM_SIZE; i++) {
		grams[i].freq = 0;
		grams[i].word1[0] = '\0';
		grams[i].word2[0] = '\0';
	}
	struct words_freq words[GRAM_SIZE];
	for (int i = 0; i < GRAM_SIZE; i++) {
		words[i].freq = 0;
		words[i].word[0] = '\0';
	}

	read_n_grams(grams, &nr_grams, argv[1]);
	read_words(words, &nr_words, argv[2]);

	// Predict words
	int nr_predicted_words = atoi(argv[4]);
	if (nr_predicted_words > 3) {
		printf("Predicted words must be less or equal to 3.\n");
		return 0;
	}

	char sentence[BIG_WORD_SIZE];
	strcpy(sentence, argv[3]);

	char last_word[WORD_SIZE];
	strcpy(last_word, "to");

	char next_word[WORD_SIZE];

	for (int i = 0; i < atoi(argv[4]); i++) {
		best_word(grams, words, last_word, nr_grams, nr_words, next_word);

		strcat(sentence, next_word);
		strcat(sentence, " ");

		strcpy(last_word, next_word);
	}

	printf("\n%s\n", sentence);

	return 0;
}