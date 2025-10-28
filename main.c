#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ===========================
// Configuração do shoe (fixo)
// ===========================
int total_cards = 312;

// ===========================
// Tipos e globais
// ===========================
typedef struct {
    int rank;
    int suit;
} Card;

Card  all_cards[312];
Card* deck_work[312];
int   deck_top = 0;

char* rank_labels[] = {
    "?", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
};
char* suit_labels[] = { "S", "H", "D", "C" };

// ===========================
// Funções auxiliares
// ===========================
int index_of_card(int rank, int suit) {
    int index = suit * 13 + (rank - 1);
    return index;
}

char* rank_label(int r) {
    if (r >= 1 && r <= 13) {
        return rank_labels[r];
    } else {
        return rank_labels[0];
    }
}

char* suit_label(int s) {
    if (s >= 0 && s <= 3) {
        return suit_labels[s];
    } else {
        return "?";
    }
}

// ===========================
// Shoe de 6 baralhos (fixo)
// ===========================
void init_shoe() {
    int index = 0;

    for (int d = 0; d < 6; d = d + 1) {
        for (int suit = 0; suit < 4; suit = suit + 1) {
            for (int r = 1; r <= 13; r = r + 1) {
                all_cards[index].rank = r;
                all_cards[index].suit = suit;
                deck_work[index] = &all_cards[index];
                index = index + 1;
            }
        }
    }

    total_cards = index;
    deck_top = 0;
}

void deck_shuffle() {
    for (int i = total_cards - 1; i > 0; i = i - 1) {
        int   j = GetRandomValue(0, i);
        Card* t = deck_work[i];
        deck_work[i] = deck_work[j];
        deck_work[j] = t;
    }
    deck_top = 0;
}

Card* deck_draw() {
    if (deck_top >= total_cards) {
        deck_shuffle();
    }

    Card* c = deck_work[deck_top];
    deck_top = deck_top + 1;
    return c;
}

// ===========================
// Renderização de carta
// ===========================
void draw_card(int x, int y, int w, int h, Card* c) {
    DrawRectangleRounded((Rectangle){ x, y, w, h }, 0.15f, 8, RAYWHITE);
    DrawRectangleRoundedLines((Rectangle){ x, y, w, h }, 0.15f, 8, BLACK);
    DrawText(rank_label(c->rank), x + 10, y + 8, 24, BLACK);
    DrawText(suit_label(c->suit), x + w - 26, y + 8, 22, DARKGRAY);
}
