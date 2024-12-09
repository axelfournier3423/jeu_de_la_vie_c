#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_CELLULE 10
#define LARGEUR_BOUTON 50
#define HAUTEUR_BOUTON 50

// Fonction pour créer la grille aléatoirement
void creer_grille(int lignes, int colonnes, int grille[lignes][colonnes])
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            // Chaque cellule a 1 chance sur 2 de mourir
            grille[i][j] = rand() % 2;
        }
    }
}

// Fonction pour afficher la grille à l'écran
void afficher_grille(int lignes, int colonnes, int grille[lignes][colonnes])
{
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            // Création de carré en cas de cellule vivante
            if (grille[i][j] == 1)
            {
                DrawRectangle(j * TAILLE_CELLULE, i * TAILLE_CELLULE, TAILLE_CELLULE, TAILLE_CELLULE, BLUE);
            }
        }
    }
}

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
                    // Vérifier si la cellule à vérifier est différente de la cellule centrale
                    if (!(di == 0 && dj == 0))
                    {
                        int ni = i + di;
                        int nj = j + dj;

                        // Ajouter un voisin vivant si les coordonnées sont valides
                        if (ni >= 0 && ni < lignes && nj >= 0 && nj < colonnes)
                        {
                            voisins_vivants += grille[ni][nj];
                        }
                    }
                }
            }

            // Appliquer les règles du Jeu de la Vie
            if (grille[i][j] == 1)
            { // Cellule vivante
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
            { // Cellule morte
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

    // Copier les valeurs de la grille mise à jour dans la grille originale
    for (int i = 0; i < lignes; i++)
    {
        for (int j = 0; j < colonnes; j++)
        {
            grille[i][j] = copie[i][j];
        }
    }
}

int main()
{
    const int largeur = 800;
    const int hauteur = 600;
    const int lignes = hauteur / TAILLE_CELLULE;
    const int colonnes = largeur / TAILLE_CELLULE;

    // Vitesse de simulation (frames per second)
    int vitesse_simulation = 10;
    bool simulation_pause = false;

    // Création de la fenêtre
    InitWindow(largeur, hauteur, "Jeu de la vie");
    SetTargetFPS(vitesse_simulation);

    int grille[lignes][colonnes];

    srand(time(NULL));
    creer_grille(lignes, colonnes, grille);

    // Définir les boutons pour contrôler la vitesse et la pause
    Rectangle boutonAugmenter = {largeur - 110, 80, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonDiminuer = {largeur - 60, 80, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonPlay = {largeur - 110, 150, LARGEUR_BOUTON + 20, HAUTEUR_BOUTON};
    Rectangle boutonPause = {largeur - 110, 210, LARGEUR_BOUTON + 20, HAUTEUR_BOUTON};

    // Définir le bouton restart
    Rectangle boutonRestart = {largeur - 110, 270, LARGEUR_BOUTON + 20, HAUTEUR_BOUTON};

    // Définir les boutons pour changer la couleur de fond
    Rectangle boutonRouge = {largeur - 110, hauteur - 110, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonVert = {largeur - 60, hauteur - 110, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonBleu = {largeur - 110, hauteur - 160, LARGEUR_BOUTON, HAUTEUR_BOUTON};
    Rectangle boutonNoir = {largeur - 60, hauteur - 160, LARGEUR_BOUTON, HAUTEUR_BOUTON};

    // Variable pour stocker la couleur du fond
    Color couleur_fond = BLACK;

    while (!WindowShouldClose())
    {
        // Gestion des événements (clics sur les boutons)
        if (CheckCollisionPointRec(GetMousePosition(), boutonAugmenter) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            vitesse_simulation += 1;
            if (vitesse_simulation > 60) // Limite maximale de la vitesse
                vitesse_simulation = 60;
            SetTargetFPS(vitesse_simulation);
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonDiminuer) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            vitesse_simulation -= 1;
            if (vitesse_simulation < 1) // Limite minimale de la vitesse
                vitesse_simulation = 1;
            SetTargetFPS(vitesse_simulation);
        }

        // Gérer les boutons Play/Pause
        if (CheckCollisionPointRec(GetMousePosition(), boutonPlay) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && simulation_pause)
        {
            simulation_pause = false;
            SetTargetFPS(vitesse_simulation); // Reprendre la simulation à la vitesse définie
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonPause) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !simulation_pause)
        {
            simulation_pause = true;
            SetTargetFPS(60); // Arrêter l'animation en la mettant à un taux de rafraîchissement élevé (simule la pause)
        }

        // Gérer le bouton restart
        if (CheckCollisionPointRec(GetMousePosition(), boutonRestart) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            creer_grille(lignes, colonnes, grille); // Réinitialiser la grille à zéro
        }

        // Changer la couleur de fond si l'on clique sur un bouton de couleur
        if (CheckCollisionPointRec(GetMousePosition(), boutonRouge) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            couleur_fond = RED;
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonVert) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            couleur_fond = GREEN;
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonBleu) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            couleur_fond = BLUE;
        }

        if (CheckCollisionPointRec(GetMousePosition(), boutonNoir) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            couleur_fond = BLACK;
        }

        BeginDrawing();
        ClearBackground(couleur_fond); // Utiliser la couleur de fond choisie

        afficher_grille(lignes, colonnes, grille);
        if (!simulation_pause)
        {
            majgrille(lignes, colonnes, grille);
        }

        // Affichage de la barre de vitesse à droite
        DrawRectangle(largeur - 170, 0, 170, hauteur, DARKGRAY);
        DrawText("Vitesse", largeur - 150, 20, 20, WHITE);

        // Affichage de la vitesse actuelle au-dessus des boutons
        DrawText(TextFormat("%d", vitesse_simulation), largeur - 130, 50, 30, WHITE);

        // Dessiner les boutons pour augmenter et diminuer la vitesse
        DrawRectangleRec(boutonAugmenter, GREEN);
        DrawText("+", boutonAugmenter.x + 15, boutonAugmenter.y + 10, 30, WHITE);

        DrawRectangleRec(boutonDiminuer, RED);
        DrawText("-", boutonDiminuer.x + 15, boutonDiminuer.y + 10, 30, WHITE);

        // Dessiner les boutons Play et Pause
        DrawRectangleRec(boutonPlay, GRAY);
        DrawText("Play", boutonPlay.x + 10, boutonPlay.y + 10, 20, WHITE);

        DrawRectangleRec(boutonPause, GRAY);
        DrawText("Pause", boutonPause.x + 5, boutonPause.y + 10, 20, WHITE);

        // Dessiner le bouton restart
        DrawRectangleRec(boutonRestart, DARKBLUE);
        DrawText("Restart", boutonRestart.x + 5, boutonRestart.y + 10, 15, WHITE);

        // Dessiner les boutons de couleur
        DrawRectangleRec(boutonRouge, RED);
        DrawText("R", boutonRouge.x + 15, boutonRouge.y + 10, 30, WHITE);

        DrawRectangleRec(boutonVert, GREEN);
        DrawText("V", boutonVert.x + 15, boutonVert.y + 10, 30, WHITE);

        DrawRectangleRec(boutonBleu, BLUE);
        DrawText("B", boutonBleu.x + 15, boutonBleu.y + 10, 30, WHITE);

        DrawRectangleRec(boutonNoir, DARKGRAY);
        DrawText("N", boutonNoir.x + 15, boutonNoir.y + 10, 30, BLACK);

        EndDrawing();
    }

    CloseWindow(); // Fermer la fenêtre

    return 0;
}
