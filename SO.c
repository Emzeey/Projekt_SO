#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TOTAL_CARDS 24
#define NUM_CARDS_PER_PLAYER 6

typedef struct {
    int player_id;
    int cards[NUM_CARDS_PER_PLAYER];
} Player;

pthread_mutex_t lock;

void *distribute_cards(void *arg) {
    Player *player = (Player *)arg;
    printf("Player %d cards: ", player->player_id);
    for (int i = 0; i < NUM_CARDS_PER_PLAYER; i++) {
        printf("%d ", player->cards[i]);
    }
    printf("\n");
    return NULL;
}

void shuffle_deck(int *deck, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

int main() {
    int num_players;
    printf("Enter the number of players: ");
    scanf("%d", &num_players);

    if (num_players <= 0 || num_players > 4) {
        printf("Number of players must be between 1 and 4.\n");
        return 1;
    }

    int deck[TOTAL_CARDS];
    int index = 0;
    // 9, 10, J, Q, K, A are represented as 9, 10, 11, 12, 13, 14
    for (int i = 9; i <= 14; i++) {
        for (int j = 0; j < 4; j++) {
            deck[index++] = i;
        }
    }

    srand(time(NULL));
    shuffle_deck(deck, TOTAL_CARDS);

    pthread_t threads[num_players];
    Player players[num_players];

    for (int i = 0; i < num_players; i++) {
        players[i].player_id = i + 1;
        for (int j = 0; j < NUM_CARDS_PER_PLAYER; j++) {
            players[i].cards[j] = deck[i * NUM_CARDS_PER_PLAYER + j];
        }
    }

    for (int i = 0; i < num_players; i++) {
        pthread_create(&threads[i], NULL, distribute_cards, &players[i]);
    }

    for (int i = 0; i < num_players; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
