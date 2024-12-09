#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_CELLULE 10
#define LARGEUR_BOUTON 50
#define HAUTEUR_BOUTON 50

/**
 * @brief Crée une grille de cellules aléatoires.
 *
 * Cette fonction remplit une grille avec des cellules vivantes et des cellules mortes
 * Chaque cellule a 50% de chance d'être vivante.
 *
 * @param lignes Le nombre de lignes dans la grille.
 * @param colonnes Le nombre de colonnes dans la grille.
 * @param grille Tableau représentant la grille de cellules.
 */
void creer_grille(int lignes, int colonnes, int grille[lignes][colonnes])
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            grille[i][j] = rand() % 2;
        }
    }
}

/**
 * @brief Affiche la grille.
 *
 * Cette fonction parcourt la grille et dessine chaque cellule vivante à l'écran.
 *
 * @param lignes Nombre de lignes dans la grille.
 * @param colonnes Nombre de colonnes dans la grille.
 * @param grille Tableau représentant la grille de cellules.
 */
void afficher_grille(int lignes, int colonnes, int grille[lignes][colonnes])
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            if (grille[i][j] == 1)
            {
                DrawRectangle(j * TAILLE_CELLULE, i * TAILLE_CELLULE, TAILLE_CELLULE, TAILLE_CELLULE, BLUE);
            }
        }
    }
}

/**
 * @brief Met à jour la grille selon les règles du jeu de la vie.
 *
 * Cette fonction met à jour la grille de cellules en appliquant les règles du jeu de la vie :
 * - Une cellule vivante reste vivante si elle a 2 ou 3 voisins vivants, sinon elle meurt.
 * - Une cellule morte devient vivante si elle a exactement 3 voisins vivants.
 *
 * @param lignes Nombre de lignes dans la grille.
 * @param colonnes Nombre de colonnes dans la grille.
 * @param grille Tableau représentant la grille de cellules.
 */
void majgrille(int lignes, int colonnes, int grille[lignes][colonnes])
{
    int copie[lignes][colonnes]; // copie de la grille actuelle

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            int voisins_vivants = 0;

            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    if (!(di == 0 && dj == 0))
                    {
                        int ni = i + di;
                        int nj = j + dj;

                        if (ni >= 0 && ni < lignes && nj >= 0 && nj < colonnes)
                        {
                            voisins_vivants += grille[ni][nj];
                        }
                    }
                }
            }

            if (grille[i][j] == 1)
            {
                if (voisins_vivants == 2 || voisins_vivants == 3)
                {
                    copie[i][j] = 1;
                }
                else
                {
                    copie[i][j] = 0;
                }
            }
            else
            {
                if (voisins_vivants == 3)
                {
                    copie[i][j] = 1;
                }
                else
                {
                    copie[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            grille[i][j] = copie[i][j];
        }
    }
}

/**
 * @brief Fonction principale du programme.
 *
 * Crée une fenêtre, initialise la grille et permet d'interagir avec la simulation
 * via des boutons pour controler la vitesse, la pause, et redémarrer.
 *
 */
int main()
{
    const int largeur = 800;
    const int hauteur = 600;
    const int lignes = hauteur / TAILLE_CELLULE;
    const int colonnes = largeur / TAILLE_CELLULE;

    int vitesse_simulation = 10;
    bool simulation_pause = false;

    InitWindow(largeur, hauteur, "Jeu de la vie");
    SetTargetFPS(vitesse_simulation);

    int grille[lignes][colonnes];

    srand(time(NULL));
    creer_grille(lignes, colonnes, grille);

    Rectangle boutonAugmenter = {largeur - 110, 80, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonDiminuer = {largeur - 60, 80, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonPlay = {largeur - 110, 150, LARGEUR_BOUTON + 20, HAUTEUR_BOUTON};
    Rectangle boutonPause = {largeur - 110, 210, LARGEUR_BOUTON + 20, HAUTEUR_BOUTON};

    Rectangle boutonRestart = {largeur - 110, 270, LARGEUR_BOUTON + 20, HAUTEUR_BOUTON};

    Rectangle boutonRouge = {largeur - 110, hauteur - 110, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonVert = {largeur - 60, hauteur - 110, LARGEUR_BOUTON, HAUTEUR_BOUTON};

    Color couleur_fond = BLACK;

    while (!WindowShouldClose())
    {
        if (CheckCollisionPointRec(GetMousePosition(), boutonAugmenter) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            vitesse_simulation += 1;
            if (vitesse_simulation > 60)
                vitesse_simulation = 60;
            SetTargetFPS(vitesse_simulation);
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonDiminuer) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            vitesse_simulation -= 1;
            if (vitesse_simulation < 1)
                vitesse_simulation = 1;
            SetTargetFPS(vitesse_simulation);
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonPlay) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && simulation_pause)
        {
            simulation_pause = false;
            SetTargetFPS(vitesse_simulation);
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonPause) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !simulation_pause)
        {
            simulation_pause = true;
            SetTargetFPS(60);
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonRestart) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            creer_grille(lignes, colonnes, grille);
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonRouge) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            couleur_fond = RED;
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonVert) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            couleur_fond = GREEN;
        }

        BeginDrawing();
        ClearBackground(couleur_fond);

        afficher_grille(lignes, colonnes, grille);
        if (!simulation_pause)
        {
            majgrille(lignes, colonnes, grille);
        }

        DrawRectangle(largeur - 170, 0, 170, hauteur, DARKGRAY);
        DrawText("Vitesse", largeur - 150, 20, 20, WHITE);

        DrawText(TextFormat("%d", vitesse_simulation), largeur - 130, 50, 30, WHITE);

        DrawRectangleRec(boutonAugmenter, GREEN);
        DrawText("+", boutonAugmenter.x + 15, boutonAugmenter.y + 10, 30, WHITE);

        DrawRectangleRec(boutonDiminuer, RED);
        DrawText("-", boutonDiminuer.x + 15, boutonDiminuer.y + 10, 30, WHITE);

        DrawRectangleRec(boutonPlay, GRAY);
        DrawText("Play", boutonPlay.x + 10, boutonPlay.y + 10, 20, WHITE);

        DrawRectangleRec(boutonPause, GRAY);
        DrawText("Pause", boutonPause.x + 5, boutonPause.y + 10, 20, WHITE);

        DrawRectangleRec(boutonRestart, DARKBLUE);
        DrawText("Restart", boutonRestart.x + 5, boutonRestart.y + 10, 15, WHITE);

        DrawRectangleRec(boutonRouge, RED);
        DrawText("R", boutonRouge.x + 15, boutonRouge.y + 10, 30, WHITE);

        DrawRectangleRec(boutonVert, GREEN);
        DrawText("V", boutonVert.x + 15, boutonVert.y + 10, 30, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
