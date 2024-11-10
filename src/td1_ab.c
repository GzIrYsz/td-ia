#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_LIGNES 10
#define NB_COLONNES 10
#define INFINI 10000
#define MAX_DEPTH 3

// #define DEBUG 1

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

long nb_nodes = 0;
long nb_coupures = 0;

typedef struct pion_s {
    int couleur;
    int valeur;
} Pion;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Move;

typedef struct {
    int nb_moves;
    Move *moves;
} MoveTab;

typedef struct {
    int eval;
    Move m;
} MoveEval;

Pion *plateauDeJeu;

void f_affiche_plateau(Pion *plateau);
int f_convert_char2int(char c);
char f_convert_int2char(int i);

int f_convert_char2int(char c) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    if (c >= 'A' && c <= 'Z')
        return (int)(c - 'A');
    if (c >= 'a' && c <= 'z')
        return (int)(c - 'a');
    return -1;
}

char f_convert_int2char(int i) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    return (char)i + 'A';
}

Pion* f_init_plateau() {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    int i, j;
    Pion *plateau = NULL;

    plateau = (Pion*)malloc(NB_LIGNES * NB_COLONNES * sizeof(Pion));
    if (plateau == NULL) {
        fprintf(stderr, "error: unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NB_LIGNES; i++) {
        for (j = 0; j < NB_COLONNES; j++) {
            plateau[i * NB_COLONNES + j].couleur = 0;
            plateau[i * NB_COLONNES + j].valeur = 0;
        }
    }

    plateau[9 * NB_COLONNES + 5].couleur = 1;
    plateau[9 * NB_COLONNES + 5].valeur = 1;

    plateau[9 * NB_COLONNES + 6].couleur = 1;
    plateau[9 * NB_COLONNES + 6].valeur = 2;

    plateau[9 * NB_COLONNES + 7].couleur = 1;
    plateau[9 * NB_COLONNES + 7].valeur = 3;

    plateau[9 * NB_COLONNES + 8].couleur = 1;
    plateau[9 * NB_COLONNES + 8].valeur = 2;

    plateau[9 * NB_COLONNES + 9].couleur = 1;
    plateau[9 * NB_COLONNES + 9].valeur = 1;

    plateau[8 * NB_COLONNES + 0].couleur = 1;
    plateau[8 * NB_COLONNES + 0].valeur = 1;

    plateau[8 * NB_COLONNES + 1].couleur = 1;
    plateau[8 * NB_COLONNES + 1].valeur = 3;

    plateau[8 * NB_COLONNES + 2].couleur = 1;
    plateau[8 * NB_COLONNES + 2].valeur = 3;

    plateau[8 * NB_COLONNES + 3].couleur = 1;
    plateau[8 * NB_COLONNES + 3].valeur = 1;

    plateau[8 * NB_COLONNES + 6].couleur = 1;
    plateau[8 * NB_COLONNES + 6].valeur = 1;

    plateau[8 * NB_COLONNES + 7].couleur = 1;
    plateau[8 * NB_COLONNES + 7].valeur = 1;

    plateau[8 * NB_COLONNES + 8].couleur = 1;
    plateau[8 * NB_COLONNES + 8].valeur = 1;

    plateau[7 * NB_COLONNES + 1].couleur = 1;
    plateau[7 * NB_COLONNES + 1].valeur = 1;

    plateau[7 * NB_COLONNES + 2].couleur = 1;
    plateau[7 * NB_COLONNES + 2].valeur = 1;

    plateau[2 * NB_COLONNES + 7].couleur = -1;
    plateau[2 * NB_COLONNES + 7].valeur = 1;

    plateau[2 * NB_COLONNES + 8].couleur = -1;
    plateau[2 * NB_COLONNES + 8].valeur = 1;

    plateau[1 * NB_COLONNES + 1].couleur = -1;
    plateau[1 * NB_COLONNES + 1].valeur = 1;

    plateau[1 * NB_COLONNES + 2].couleur = -1;
    plateau[1 * NB_COLONNES + 2].valeur = 1;

    plateau[1 * NB_COLONNES + 3].couleur = -1;
    plateau[1 * NB_COLONNES + 3].valeur = 1;

    plateau[1 * NB_COLONNES + 6].couleur = -1;
    plateau[1 * NB_COLONNES + 6].valeur = 1;

    plateau[1 * NB_COLONNES + 7].couleur = -1;
    plateau[1 * NB_COLONNES + 7].valeur = 3;

    plateau[1 * NB_COLONNES + 8].couleur = -1;
    plateau[1 * NB_COLONNES + 8].valeur = 3;

    plateau[1 * NB_COLONNES + 9].couleur = -1;
    plateau[1 * NB_COLONNES + 9].valeur = 1;

    plateau[0 * NB_COLONNES + 0].couleur = -1;
    plateau[0 * NB_COLONNES + 0].valeur = 1;

    plateau[0 * NB_COLONNES + 1].couleur = -1;
    plateau[0 * NB_COLONNES + 1].valeur = 2;

    plateau[0 * NB_COLONNES + 2].couleur = -1;
    plateau[0 * NB_COLONNES + 2].valeur = 3;

    plateau[0 * NB_COLONNES + 3].couleur = -1;
    plateau[0 * NB_COLONNES + 3].valeur = 2;

    plateau[0 * NB_COLONNES + 4].couleur = -1;
    plateau[0 * NB_COLONNES + 4].valeur = 1;

#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif

    return plateau;
}

void f_affiche_plateau(Pion *plateau) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    int i, j, k;

    printf("\n    ");
    for (k = 0; k < NB_COLONNES; k++)
        printf("%2c ", f_convert_int2char(k));
    printf("\n    ");
    for (k = 0; k < NB_COLONNES; k++)
        printf("-- ");
    printf("\n");
    for (i = NB_LIGNES - 1; i >= 0; i--) {
        printf("%2d ", i);
        for (j = 0; j < NB_COLONNES; j++) {
            printf("|");
            switch (plateau[i * NB_COLONNES + j].couleur) {
                case -1:
                    printf("%do", plateau[i * NB_COLONNES + j].valeur);
                    break;
                case 1:
                    printf("%dx", plateau[i * NB_COLONNES + j].valeur);
                    break;
                default:
                    printf("  ");
            }
        }
        printf("|\n    ");
        for (k = 0; k < NB_COLONNES; k++)
            printf("-- ");
        printf("\n");
    }
    printf("    ");

#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

int f_gagnant() {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    int i, j, somme1 = 0, somme2 = 0;

    // Quelqu'un est-il arrive sur la ligne de l'autre
    for (i = 0; i < NB_COLONNES; i++) {
        if (plateauDeJeu[i].couleur == 1)
            return 1;
        if (plateauDeJeu[(NB_LIGNES - 1) * NB_COLONNES + i].couleur == -1)
            return -1;
    }

    // taille des armees
    for (i = 0; i < NB_LIGNES; i++) {
        for (j = 0; j < NB_COLONNES; j++) {
            if (plateauDeJeu[i * NB_COLONNES + j].couleur == 1)
                somme1++;
            if (plateauDeJeu[i * NB_COLONNES + j].couleur == -1)
                somme2++;
        }
    }
    if (somme1 == 0)
        return -1;
    if (somme2 == 0)
        return 1;

#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    return 0;
}

/**
 * Prend comme argument la ligne et la colonne de la case
 * 	pour laquelle la bataille a lieu
 * Renvoie le couleur du gagnant
 * */
int f_bataille(int l, int c) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    int i, j, mini, maxi, minj, maxj;
    int somme = 0;

    mini = l - 1 < 0 ? 0 : l - 1;
    maxi = l + 1 > NB_LIGNES - 1 ? NB_LIGNES - 1 : l + 1;
    minj = c - 1 < 0 ? 0 : c - 1;
    maxj = c + 1 > NB_COLONNES - 1 ? NB_COLONNES - 1 : c + 1;

    for (i = mini; i <= maxi; i++) {
        for (j = minj; j <= maxj; j++) {
            somme += plateauDeJeu[i * NB_COLONNES + j].couleur * plateauDeJeu[i * NB_COLONNES + j].valeur;
        }
    }
    somme -= plateauDeJeu[l * NB_COLONNES + c].couleur * plateauDeJeu[l * NB_COLONNES + c].valeur;

#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    if (somme < 0)
        return -1;
    if (somme > 0)
        return 1;

    return plateauDeJeu[l * NB_COLONNES + c].couleur;
}

/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_test_mouvement(Pion *plateau, int l1, int c1, int l2, int c2, int couleur) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
	printf("de (%d,%d) vers (%d,%d)\n", l1, c1, l2, c2);
#endif
    /* Erreur, hors du plateau */
    if (l1 < 0 || l1 >= NB_LIGNES || l2 < 0 || l2 >= NB_LIGNES ||
        c1 < 0 || c1 >= NB_COLONNES || c2 < 0 || c2 >= NB_COLONNES)
        return 1;
    /* Erreur, il n'y a pas de pion a deplacer ou le pion n'appartient pas au joueur*/
    if (plateau[l1 * NB_COLONNES + c1].valeur == 0 || plateau[l1 * NB_COLONNES + c1].couleur != couleur)
        return 1;
    /* Erreur, tentative de tir fratricide */
    if (plateau[l2 * NB_COLONNES + c2].couleur == plateau[l1 * NB_COLONNES + c1].couleur)
        return 1;

    if (l1 - l2 > 1 || l2 - l1 > 1 || c1 - c2 > 1 || c2 - c1 > 1 || (l1 == l2 && c1 == c2))
        return 1;

#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    return 0;
}

/**
 * Prend la ligne et colonne de la case d'origine
 * 	et la ligne et colonne de la case de destination
 *  et effectue le trantement de l'operation demandée
 * Renvoie 1 en cas d'erreur
 * Renvoie 0 sinon
 * */
int f_bouge_piece(Pion *plateau, int l1, int c1, int l2, int c2, int couleur) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    int gagnant = 0;

    if (f_test_mouvement(plateau, l1, c1, l2, c2, couleur) != 0)
        return 1;

    /* Cas ou il n'y a personne a l'arrivee */
    if (plateau[l2 * NB_COLONNES + c2].valeur == 0) {
        plateau[l2 * NB_COLONNES + c2].couleur = plateau[l1 * NB_COLONNES + c1].couleur;
        plateau[l2 * NB_COLONNES + c2].valeur = plateau[l1 * NB_COLONNES + c1].valeur;
        plateau[l1 * NB_COLONNES + c1].couleur = 0;
        plateau[l1 * NB_COLONNES + c1].valeur = 0;
    }
    else {
        gagnant = f_bataille(l2, c2);
        /* victoire */
        if (gagnant == couleur) {
            plateau[l2 * NB_COLONNES + c2].couleur = plateau[l1 * NB_COLONNES + c1].couleur;
            plateau[l2 * NB_COLONNES + c2].valeur = plateau[l1 * NB_COLONNES + c1].valeur;
            plateau[l1 * NB_COLONNES + c1].couleur = 0;
            plateau[l1 * NB_COLONNES + c1].valeur = 0;
        }
            /* defaite */
        else if (gagnant != 0) {
            plateau[l1 * NB_COLONNES + c1].couleur = 0;
            plateau[l1 * NB_COLONNES + c1].valeur = 0;
        }
    }

#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    return 0;
}

// Calcul du nombre de pions sur le plateau du joueur
int f_nbPions(Pion *jeu, int joueur) {
    int nbPion = 0;
    int i, j;
    for (i = 0; i < NB_COLONNES; ++i) {
        for (j = 0; j < NB_LIGNES; ++j) {
            if (jeu[i * NB_COLONNES + j].couleur == joueur) {
                ++nbPion;
            }
        }
    }
    return nbPion;
}

// Calcul de la valeur de tous les pions du joueur
int f_valeur(Pion *jeu, int joueur) {
    int i, j;
    int valeur = 0;
    for (i = 0; i < NB_COLONNES; ++i) {
        for (j = 0; j < NB_LIGNES; ++j) {
            if (jeu[i * NB_COLONNES + j].couleur == joueur) {
                valeur += jeu[i * NB_COLONNES + j].valeur;
            }
        }
    }
    return valeur;
}

int f_distance(Pion *game_board, int player) {
    int qg_index = player == 1 ? 0 : NB_LIGNES - 1;
    int dist = 0;
    for (int i = 0; i < NB_LIGNES; ++i) {
        for (int j = 0; j < NB_COLONNES; ++j) {
            if (game_board[i * NB_COLONNES + j].couleur == player) {
                dist += abs(qg_index - i);
            }
        }
    }
    return dist;
}

// fonction d'évaluation
int f_eval(Pion *jeu, int joueur) {
    int winner = f_gagnant();
    if (winner == joueur) {
        return INFINI;
    } else if (winner == -joueur) {
        return -INFINI;
    }

    int W1 = 10, W2 = 5, W3 = 3, W4 = 7, W5 = 8;
    int materialAdvantage = f_nbPions(jeu,joueur) - f_nbPions(jeu, -joueur);
    int positionalAdvantage = f_distance(jeu, joueur) - f_distance(jeu, -joueur);
    // int supportAdvantage = sumSupport(state, player) - sumSupport(state, opponent(player));
    int supportAdvantage = 0;
    // int capturePotential = countPotentialCaptures(state, player);
    int capturePotential = 0;
    // int threats = countPotentialCaptures(state, opponent(player));
    int threats = 0;

    int h = W1 * materialAdvantage
            + W2 * positionalAdvantage
            + W3 * supportAdvantage
            + W4 * capturePotential
            - W5 * threats;
    return h;
}

// copie du plateau
void f_copie_plateau(Pion *source, Pion *destination) {
    int i, j;
    for (i = 0; i < NB_LIGNES; i++) {
        for (j = 0; j < NB_COLONNES; j++) {
            destination[i * NB_COLONNES + j].couleur = source[i * NB_COLONNES + j].couleur;
            destination[i * NB_COLONNES + j].valeur = source[i * NB_COLONNES + j].valeur;
        }
    }
}

// mise a zero du plateau
Pion* f_raz_plateau() {
    Pion *jeu = NULL;
    int i, j;
    jeu = (Pion *)malloc(NB_LIGNES * NB_COLONNES * sizeof(Pion));
    for (i = 0; i < NB_LIGNES; i++) {
        for (j = 0; j < NB_COLONNES; j++) {
            jeu[i * NB_COLONNES + j].couleur = 0;
            jeu[i * NB_COLONNES + j].valeur = 0;
        }
    }
    return jeu;
}

Pion** f_mouvements_suivants(Pion *jeu, int joueur, int *nb_moves) {
    int moves[] = {-1, 0, 1};
    int pions_vivants = f_nbPions(jeu, joueur);
    int nb_mov = 0;
    Pion **mov_suiv = (Pion**)malloc(8 * pions_vivants * sizeof(Pion*));

    for (int i = 0; i < NB_LIGNES; ++i) {
        for (int j = 0; j < NB_COLONNES; ++j) {
            if (jeu[i * NB_COLONNES + j].couleur == joueur) {
                for (int k = 0; k < 3; ++k) {
                    int deplX = moves[k];
                    for (int l = 0; l < 3; ++l) {
                        int deplY = moves[l];
                        if (deplX == 0 && deplY == 0)
                            continue;
                        Pion *m = f_init_plateau();
                        f_copie_plateau(jeu, m);
                        f_bouge_piece(m, i, j, i + deplX, j + deplY, joueur);
                        mov_suiv[nb_mov] = m;
                        nb_mov++;
                    }
                }
            }
        }
    }
    return mov_suiv;
}

void f_add_move(int x, int y, MoveTab *moves, int *possible_moves, Pion *game_board, int player) {
    for (int i = 0; i < 3; ++i) {
        int deplX = possible_moves[i];
        for (int j = 0; j < 3; ++j) {
            int deplY = possible_moves[j];

            if (0 == f_test_mouvement(game_board, x, y, x + deplX, y + deplY, player)) {
                moves->moves[moves->nb_moves].x1 = x;
                moves->moves[moves->nb_moves].y1 = y;
                moves->moves[moves->nb_moves].x2 = x + deplX;
                moves->moves[moves->nb_moves].y2 = y + deplY;
                moves->nb_moves++;
            }
        }
    }
}

MoveTab f_next_moves(Pion *jeu, int joueur) {
    int possible_moves[] = {-1, 0, 1};
    MoveTab moves = {
            0,
            (Move*)malloc(8 * f_nbPions(jeu, joueur) * sizeof(Move))
    };

    for (int i = 0; i < NB_LIGNES; ++i) {
        for (int j = 0; j < NB_COLONNES; ++j) {
            if (jeu[i * NB_COLONNES + j].couleur == joueur) {
                f_add_move(i, j, &moves, possible_moves, jeu, joueur);
            }
        }
    }
    return moves;
}

MoveEval f_max(Pion*, int, int, int, int);

// Fonction min trouve le minimum des noeuds fils
MoveEval f_min(Pion *game_board, int player, int alpha, int beta, int depth) {
    nb_nodes++;
    MoveEval bestMove = {
            INFINI,
            {-1, -1, -1, -1}
    };

    if ((0 == depth) || (0 != f_gagnant())) {
        int eval = f_eval(game_board, player);
        bestMove.eval = eval;
        return bestMove;
    }

    MoveTab moves = f_next_moves(game_board, player);

    for (int i = 0; i < moves.nb_moves; ++i) {
        Move m = moves.moves[i];
        Pion *new_board = f_init_plateau();
        f_copie_plateau(game_board, new_board);
        int has_not_changed = f_bouge_piece(new_board, m.x1, m.y1, m.x2, m.y2, player);
        if (has_not_changed) {
            free(new_board);
            continue;
        }
        int eval = f_max(new_board, -player, alpha, beta, depth - 1).eval;
        if (eval < beta) {
            beta = eval;
            bestMove.m.x1 = m.x1;
            bestMove.m.y1 = m.y1;
            bestMove.m.x2 = m.x2;
            bestMove.m.y2 = m.y2;
            bestMove.eval = eval;
        }
        if (alpha >= beta) {
            nb_coupures++;
            bestMove.eval = alpha;
            free(new_board);
            return bestMove;
        }
        free(new_board);
    }
    free(moves.moves);
    return bestMove;
}

// Fonction max trouve le maximum des noeuds fils
MoveEval f_max(Pion *game_board, int player, int alpha, int beta, int depth) {
    nb_nodes++;
    MoveEval bestMove = {
            -INFINI,
            {-1, -1, -1, -1}
    };

    if ((0 == depth) || (0 != f_gagnant())) {
        int eval = f_eval(game_board, player);
        bestMove.eval = eval;
        return bestMove;
    }

    MoveTab moves = f_next_moves(game_board, player);

    for (int i = 0; i < moves.nb_moves; ++i) {
        Move m = moves.moves[i];
        Pion *new_board = f_init_plateau();
        f_copie_plateau(game_board, new_board);
        int has_not_changed = f_bouge_piece(new_board, m.x1, m.y1, m.x2, m.y2, player);
        if (has_not_changed) {
            free(new_board);
            continue;
        }
        int eval = f_min(new_board, -player, alpha, beta, depth - 1).eval;
        if (eval > alpha) {
            alpha = eval;
            bestMove.m.x1 = m.x1;
            bestMove.m.y1 = m.y1;
            bestMove.m.x2 = m.x2;
            bestMove.m.y2 = m.y2;
            bestMove.eval = eval;
        }
        if (alpha >= beta) {
            nb_coupures++;
            bestMove.eval = beta;
            free(new_board);
            return bestMove;
        }
        free(new_board);
    }
    free(moves.moves);
    return bestMove;
}

/*int f_max(Pion *jeu, int joueur, int profondeur) {
    if (profondeur == 0)
        return f_eval(jeu, joueur);

    int val = -INFINI;
    int nb_moves = 0;
    Pion **nextMoves = f_mouvements_suivants(jeu, joueur, &nb_moves);
    for (int i = 0; i < nb_moves; ++i) {
        val = MAX(val, f_min(nextMoves[i], -joueur, profondeur - 1));
    }
    return val;
}*/

/**
 * Calcule et joue le meilleur cout
 * */
void f_IA(int joueur) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    nb_nodes = 0;
    nb_coupures = 0;
    MoveEval bestMove = f_max(plateauDeJeu, joueur, -INFINI, INFINI, MAX_DEPTH);
    if (0 != f_bouge_piece(plateauDeJeu, bestMove.m.x1, bestMove.m.y1, bestMove.m.x2, bestMove.m.y2, joueur)) {
        fprintf(stderr, "mauvais choix: (%d, %d) -> (%d, %d)\n", bestMove.m.x1, bestMove.m.y1, bestMove.m.x2, bestMove.m.y2);
        exit(EXIT_FAILURE);
    }
    FILE *f = fopen("../data/alphabeta.csv", "a+");
    fprintf(f, "%ld\n", nb_nodes);
    fclose(f);

    f = fopen("../data/coupures.csv", "a+");
    fprintf(f, "%ld\n", nb_coupures);
    fclose(f);
#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

/**
 * Demande le choix du joueur humain et calcule le coup demande
 * */
void f_humain(int joueur) {
#if DEBUG
    printf("dbg: entering %s %d\n", __FUNCTION__, __LINE__);
#endif
    char c1, c2;
    char buffer[32];
    int l1, l2;

    printf("joueur ");
    switch (joueur) {
        case -1:
            printf("o ");
            break;
        case 1:
            printf("x ");
            break;
        default:
            printf("inconnu ");
    }
    printf("joue:\n");
    while (1) {
        fgets(buffer, 32, stdin);
        if (sscanf(buffer, "%c%i%c%i\n", &c1, &l1, &c2, &l2) == 4) {
            if (f_bouge_piece(plateauDeJeu, l1, f_convert_char2int(c1), l2, f_convert_char2int(c2), joueur) == 0)
                break;
        }
        fflush(stdin);
        printf("mauvais choix\n");
    }

#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
}

int main(int argc, char **argv) {
    int fin = 0, mode = 0, ret, joueur = 1;
    printf("1 humain vs IA\n2 humain vs humain\n3 IA vs IA\n");
    scanf("%d", &mode);

    plateauDeJeu = f_init_plateau();
    while (!fin) {
        f_affiche_plateau(plateauDeJeu);
        if (mode == 1) {
            if (joueur > 0)
                f_humain(joueur);
            else
                f_IA(joueur);
        }
        else if (mode == 2) {
            f_humain(joueur);
        }
        else {
            f_IA(joueur);
        }

        if ((ret = f_gagnant()) != 0) {
            switch (ret) {
                case 1:
                    f_affiche_plateau(plateauDeJeu);
                    printf("joueur x gagne!\n");
                    fin = 1;
                    break;
                case -1:
                    f_affiche_plateau(plateauDeJeu);
                    printf("joueur o gagne!\n");
                    fin = 1;
                    break;
            }
        }
        joueur = -joueur;
    }
    printf("nb nodes = %ld\n", nb_nodes);
    printf("nb coupures = %ld\n", nb_coupures);
#if DEBUG
    printf("dbg: exiting %s %d\n", __FUNCTION__, __LINE__);
#endif
    return EXIT_SUCCESS;
}
