//  Student:    Yousra Smits
//  Rolnummer:  20201267
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

Game::Game() {
    setStartBord();
}

Game::~Game() {}

// Zet het bord klaar; voeg de stukken op de juiste plaats toe
void Game::setStartBord() {
    zw kleur = zwart;
    for (int r = 0; r < 8; r++) {
        if (r == 3) { kleur = wit; }            // volgende kleur
        if (1 < r && r < 6) {
            for(int k = 0; k < 8; k++){
                setPiece(r,k,nullptr);
            }
            continue;
        }

        if (r == 1 || r == 6){                  // pionnen plaatsen
            for(int k = 0; k < 8; k++){
                Pion *new_pio = new Pion(kleur);
                setPiece(r, k, new_pio);
            }
            continue;
        }
        else {
            for (int k = 0; k < 8; k++) {
                if (k == 0 || k == 7) {
                    Toren *new_tor = new Toren(kleur);
                    setPiece(r, k, new_tor);
                    continue;
                } else if (k == 1 || k == 6) {
                    Paard *new_pa = new Paard(kleur);
                    setPiece(r, k, new_pa);
                    continue;
                } else if (k == 2 || k == 5) {
                    Loper *new_lop = new Loper(kleur);
                    setPiece(r, k, new_lop);
                    continue;
                } else if (k == 3) {
                    Koningin *new_queen = new Koningin(kleur);
                    setPiece(r, k, new_queen);
                    continue;
                } else if (k == 4) {
                    Koning *new_king = new Koning(kleur);
                    setPiece(r, k, new_king);
                    continue;
                }
            }
        }
    }
}

// Verplaats stuk s naar positie (r,k)
// Als deze move niet mogelijk is, wordt false teruggegeven
// en verandert er niets aan het schaakbord.
// Anders wordt de move uitgevoerd en wordt true teruggegeven
bool Game::move(SchaakStuk* s, int r, int k) {
    pair<int, int> new_move = {r,k};
    for(auto pos_move : s->geldige_zetten(this)){
        if(pos_move == new_move){
            pair<int, int> lastpos = this->findPiece(s);
            setPiece(lastpos.first, lastpos.second, nullptr);
            setPiece(r, k, nullptr);
            setPiece(new_move.first, new_move.second, s);
            return true;
        }
    }
    return false;
}

// Geeft true als kleur schaak staat
bool Game::schaak(zw kleur) {
    return false;
}

// Geeft true als kleur schaakmat staat
bool Game::schaakmat(zw kleur) {
    return false;
}

// Geeft true als kleur pat staat
// (pat = geen geldige zet mogelijk, maar kleur staat niet schaak;
// dit resulteert in een gelijkspel)
bool Game::pat(zw kleur) {
    return false;
}

pair<int, int> Game::findPiece(SchaakStuk *stuk){
    for(int r = 0; r < 8; r++){
        for(int k = 0; k < 8; k++){
            if(bord[r][k] == stuk){
                return {r, k};
            }
        }
    }
}

vector<SchaakStuk *> Game::witte_stukken() {
    vector<SchaakStuk*> stukken;
    for(int r = 0; r < 8 ; r ++){
        for(int k = 0 ; k < 8 ; k ++){
            if(bord[r][k] != nullptr && bord[r][k]->getKleur() == wit){
                stukken.emplace_back(bord[r][k]);
            }
        }
    }
    return stukken;
}

vector<SchaakStuk *> Game::zwarte_stukken() {
    vector<SchaakStuk*> stukken;
    for(int r = 0; r < 8 ; r ++){
        for(int k = 0 ; k < 8 ; k ++){
            if(bord[r][k] != nullptr && bord[r][k]->getKleur() == zwart){
                stukken.emplace_back(bord[r][k]);
            }
        }
    }
    return stukken;
}
