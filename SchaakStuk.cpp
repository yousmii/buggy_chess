//  Student:    Yousra Smits
//  Rolnummer:  20201267
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "SchaakStuk.h"
#include "game.h"

vector<pair<int, int>> Pion::geldige_zetten(Game *game) {
    vector<pair<int, int>> zetten;
    pair<int, int> curpos = game->findPiece(this);
    int currow = curpos.first;
    int curkol = curpos.second;

    if(this->getKleur() == zwart){
        if (currow == 1 && game->getPiece(currow + 2, curkol) == nullptr){
            zetten = {{currow + 2, curkol}};
        }
        if (game->getPiece(currow + 1, curkol) == nullptr){
            zetten.emplace_back(currow + 1, curkol);
        }
        if (currow + 1 < 8 && curkol - 1 >= 0 && game->getPiece(currow + 1, curkol - 1) != nullptr && game->getPiece(currow + 1, curkol - 1)->getKleur() != this->getKleur()){
            zetten.emplace_back(currow + 1, curkol - 1);
        }
        if (currow + 1 < 8 && curkol + 1 < 8 && game->getPiece(currow + 1, curkol + 1) != nullptr && game->getPiece(currow + 1, curkol + 1)->getKleur() != this->getKleur()){
            zetten.emplace_back(currow + 1, curkol +  1);
        }
    }
    if(this->getKleur() == wit){
        if (currow == 6 && game->getPiece(currow - 2, curkol) == nullptr){
            zetten = {{currow - 2, curkol}};
        }
        if (game->getPiece(currow - 1, curkol) == nullptr){
            zetten.emplace_back(currow - 1, curkol);
        }
        if (currow - 1 >= 0 && curkol - 1 >= 0 && game->getPiece(currow - 1, curkol - 1) != nullptr && game->getPiece(currow - 1, curkol - 1)->getKleur() != this->getKleur()){
                zetten.emplace_back(currow - 1, curkol - 1);
        }
        if (currow - 1 >= 0 && curkol + 1 < 8 && game->getPiece(currow - 1, curkol + 1) != nullptr && game->getPiece(currow - 1, curkol + 1)->getKleur() != this->getKleur()){
            zetten.emplace_back(currow - 1, curkol +  1);
        }
    }
    return zetten;
}

vector<pair<int, int> > Toren::geldige_zetten(Game *g) {
    vector<pair<int, int>> zetten;
    int currow = g->findPiece(this).first;
    int curkol = g->findPiece(this).second;
    if(currow != 0){
        for (int r = currow+1; r >= 0 && r < 8; r--){
            if(g->getPiece(r, curkol) != nullptr){

                if (g->getPiece(r, curkol)->getKleur() != this->getKleur()) {
                    zetten.emplace_back(r, curkol);
                }
                break;
            }
            else{
                zetten.emplace_back(r, curkol);
            }
        }
    }
    for(int r = currow+1 ; r >= 0 && r < 8 ; r++){
        if(g->getPiece(r, curkol) != nullptr){
            if (g->getPiece(r, curkol)->getKleur() != this->getKleur()) {
                zetten.emplace_back(r, curkol);
            }
            break;
        }
        else{zetten.emplace_back(r, curkol);}
    }
    if(curkol != 0){
        for (int k = curkol+1; k >= 0 && k < 8; k--){
            if(g->getPiece(currow, k) != nullptr){
                if (g->getPiece(currow, k)->getKleur() != this->getKleur()) {
                    zetten.emplace_back(currow, k);
                }
                break;
            }
            else{zetten.emplace_back(currow, k);}
        }
    }
    for(int k = curkol+1 ;k >= 0 && k < 8 ; k++){
        if(g->getPiece(currow, k) != nullptr) {
            if (g->getPiece(currow, k)->getKleur() != this->getKleur()) {
                zetten.emplace_back(currow, k);
            }
            break;
        }
        else{zetten.emplace_back(currow, k);}
    }
    return zetten;
}

vector<pair<int, int> > Paard::geldige_zetten(Game *g) {
    vector<pair<int, int>> zetten;

    int currow = g->findPiece(this).first;
    int curkol = g->findPiece(this).second;

    vector<pair<int, int>> posmov{
        {currow + 2, curkol + 1},
        {currow - 2, curkol - 1},

        {currow + 1, curkol + 2},
        {currow - 1, curkol - 2},

        {currow + 2, curkol - 1},
        {currow - 2, curkol + 1},

        {currow + 1, curkol - 2},
        {currow - 1, curkol + 2}
    };
    for(auto move : posmov){
        int x = move.first;
        int y = move.second;
        if ( x < 8 && y < 8 && x >= 0 && y >= 0) {
            if (g->getPiece(x, y) == nullptr) {
                zetten.emplace_back(x, y);
            } else if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
        }
    }
    return zetten;
}

vector<pair<int, int> > Loper::geldige_zetten(Game *g) {
    vector<pair<int, int>> zetten;

    int x = g->findPiece(this).first +1;
    int y = g->findPiece(this).second +1;

    while (x < 7 && y < 7) {
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x++;
        y++;
    }
    x = g->findPiece(this).first - 1;
    y = g->findPiece(this).second - 1;

    while(x >= 0 && y >= 0){
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x--;
        y--;
    }

    x = g->findPiece(this).first + 1;
    y = g->findPiece(this).second - 1;

    while (x < 8 && y >= 0) {
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x++;
        y--;
    }
    x = g->findPiece(this).first - 1;
    y = g->findPiece(this).second + 1;
    while(x >= 0 && y < 8){
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x--;
        y++;
    }
    return zetten;
}

vector<pair<int, int> > Koning::geldige_zetten(Game *g) {
    vector<pair<int, int>> zetten;

    int x = g->findPiece(this).first;
    int y = g->findPiece(this).first;

    vector<pair<int, int>> posmov{
            {x + 1, y + 1},
            {x - 1, y - 1},

            {x - 1, y + 1},
            {x + 1, y - 1},

            {x + 1, y + 0},
            {x - 1, y + 0},

            {x + 0, y + 1},
            {x + 0, y - 1},
    };
    for(auto move : posmov){
        int r = move.first;
        int k = move.second;
        if(r < 8 && k < 8 && r >= 0 && k >= 0) {
            if (g->getPiece(r, y) == nullptr) {
                zetten.emplace_back(r, k);
            } else if (g->getPiece(r, k)->getKleur() != this->getKleur()) {
                zetten.emplace_back(r, k);
            }
        }
    }
    return zetten;
}


vector<pair<int, int> > Koningin::geldige_zetten(Game *g) {
    vector<pair<int, int>> zetten;

    int currow = g->findPiece(this).first;
    int curkol = g->findPiece(this).second;

    // beweegt als toren...

    if(currow != 0){
        for (int r = currow+1; r >= 0 && r < 8; r--){
            if(g->getPiece(r, curkol) != nullptr){

                if (g->getPiece(r, curkol)->getKleur() != this->getKleur()) {
                    zetten.emplace_back(r, curkol);
                }
                break;
            }
            else{
                zetten.emplace_back(r, curkol);
            }
        }
    }
    for(int r = currow+1 ; r >= 0 && r < 8 ; r++){
        if(g->getPiece(r, curkol) != nullptr){
            if (g->getPiece(r, curkol)->getKleur() != this->getKleur()) {
                zetten.emplace_back(r, curkol);
            }
            break;
        }
        else{zetten.emplace_back(r, curkol);}
    }
    if(curkol != 0){
        for (int k = curkol+1; k >= 0 && k < 8; k--){
            if(g->getPiece(currow, k) != nullptr){
                if (g->getPiece(currow, k)->getKleur() != this->getKleur()) {
                    zetten.emplace_back(currow, k);
                }
                break;
            }
            else{zetten.emplace_back(currow, k);}
        }
    }
    for(int k = curkol+1 ;k >= 0 && k < 8 ; k++){
        if(g->getPiece(currow, k) != nullptr) {
            if (g->getPiece(currow, k)->getKleur() != this->getKleur()) {
                zetten.emplace_back(currow, k);
            }
            break;
        }
        else{zetten.emplace_back(currow, k);}
    }

    // en als loper
    int x = g->findPiece(this).first +1;
    int y = g->findPiece(this).second +1;

    while (x < 7 && y < 7) {
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x++;
        y++;
    }
    x = g->findPiece(this).first - 1;
    y = g->findPiece(this).second - 1;

    while(x >= 0 && y >= 0){
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x--;
        y--;
    }

    x = g->findPiece(this).first + 1;
    y = g->findPiece(this).second - 1;

    while (x < 8 && y >= 0) {
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x++;
        y--;
    }
    x = g->findPiece(this).first - 1;
    y = g->findPiece(this).second + 1;
    while(x >= 0 && y < 8){
        if (g->getPiece(x, y) != nullptr) {
            if (g->getPiece(x, y)->getKleur() != this->getKleur()) {
                zetten.emplace_back(x, y);
            }
            break;
        }
        else{
            zetten.emplace_back(x,y);
        }
        x--;
        y++;
    }
    return zetten;
}
