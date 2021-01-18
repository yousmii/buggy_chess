#include "mainwindow.h"
#include "SchaakStuk.h"
#include <QMessageBox>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    scene = new ChessBoard;
    g = new Game;
    beurt = wit;
    focused = nullptr;

    QGraphicsView *view = new QGraphicsView(scene);
    setCentralWidget(view);

    connect(scene,SIGNAL(clicked(int,int)),this,SLOT(clicked(int,int)));

    createActions();
    createMenus();
    newGame();
}

void MainWindow::clicked(int r, int k) {
    update();
    SchaakStuk* selected = g->getPiece(r, k);

    if(selected != nullptr && selected->getKleur() != beurt) {
        QMessageBox box1;
        box1.setText(QString("Dit zijn jouw stukken niet! >:("));
        box1.exec();
        return;
    }

    scene->setTileSelect(r,k,true);
    pair<vector<SchaakStuk*>, vector<SchaakStuk*>> allpcs = {g->witte_stukken(), g->zwarte_stukken()};

    vector<SchaakStuk*> enemies;
    vector<SchaakStuk*> allies;

    if (beurt == wit){
        enemies = allpcs.second;
        allies = allpcs.first;
    }
    else if (beurt == zwart)  {
        enemies = allpcs.first;
        allies = allpcs.second;
    }
    if (display_kills->isChecked()) {
        for(auto ally :allies) {
            for (auto move : ally->geldige_zetten(g)) {
                for (SchaakStuk *enemy : enemies) {
                    if (g->findPiece(enemy) == move) {
                        scene->setPieceThreat(g->findPiece(enemy).first, g->findPiece(enemy).second, true);
                    }
                }
            }
        }
    }

    if (display_threats->isChecked()) {
        for(auto enemy : enemies){
            for(auto move : enemy->geldige_zetten(g)){
                for(auto ally : allies){
                    if (g->findPiece(ally) == move){
                        scene->setPieceThreat(g->findPiece(ally).first, g->findPiece(ally).second, true);
                    }
                }
            }
        }
    }

    if(selected != nullptr) {
        if (display_moves->isChecked()) {
            for (auto move: selected->geldige_zetten(g)) {
                int x = move.first;
                int y = move.second;
                for (auto enemy : enemies) {
                    for (auto enemymove : enemy->geldige_zetten(g)) {
                        if (enemymove == move) { scene->setTileThreat(x, y, true); }
                        else { scene->setTileFocus(x, y, true); }
                    }
                }
            }
        }
    }

    if(focused != nullptr){
        if(g->move(focused, r, k)) {
            if (beurt == wit) { beurt = zwart; }
            else { beurt = wit; }
            focused = nullptr;
            update();
        }
    }
    if (selected != nullptr){
        focused = selected;
    }

}

void MainWindow::newGame(){
    //for()
    g->setStartBord();
    update();
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save game"), "",
                                                    tr("Chess File (*.chs);;All Files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QDataStream out(&file);
        out << QString("Rb") << QString("Hb") << QString("Bb") << QString("Qb") << QString("Kb") << QString("Bb") << QString("Hb") << QString("Rb");
        for  (int i=0;i<8;i++) {
            out << QString("Pb");
        }
        for  (int r=3;r<7;r++) {
            for (int k=0;k<8;k++) {
                out << QString(".");
            }
        }
        for  (int i=0;i<8;i++) {
            out << QString("Pw");
        }
        out << QString("Rw") << QString("Hw") << QString("Bw") << QString("Qw") << QString("Kw") << QString("Bw") << QString("Hw") << QString("Rw");
    }
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load game"), "",
                                                    tr("Chess File (*.chs);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        try {
            QDataStream in(&file);
            QString debugstring;
            for (int r=0;r<8;r++) {
                for (int k=0;k<8;k++) {
                    QString piece;
                    in >> piece;
                    debugstring += "\t" + piece;
                    if (in.status()!=QDataStream::Ok) {
                        throw QString("Error reading file "+fileName);
                    }
                }
                debugstring += "\n";
            }
            QMessageBox::information(this, tr("Debug"),
                                     debugstring);
        } catch (QString& Q) {
            QMessageBox::information(this, tr("Error reading file"),
                                     Q);
        }
    }
    update();
}


void MainWindow::undo() {
    QMessageBox box;
    box.setText(QString("Je hebt undo gekozen"));
    box.exec();
}

void MainWindow::redo() {}


void MainWindow::visualizationChange() {
    QMessageBox box;
    QString visstring = QString(display_moves->isChecked()?"T":"F")+(display_kills->isChecked()?"T":"F")+(display_threats->isChecked()?"T":"F");
    box.setText(QString("Visualization changed : ")+visstring);
    box.exec();
}


// Update de inhoud van de grafische weergave van het schaakbord (scene)
// en maak het consistent met de game state in variabele g.
void MainWindow::update() {
    scene->clearBoard();
    scene->removeAllMarking();
    for (int r = 0 ; r < 8 ; r++){
        for (int k = 0 ; k < 8 ; k++){
            if(g->getPiece(r, k) != nullptr){
                scene->setItem(r, k, g->getPiece(r,k)->piece());
            }
        }
    }
}





////////////////






void MainWindow::createActions() {
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Start a new game"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newGame);

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Read game from disk"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save game to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Abandon game"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::on_actionExit_triggered);

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo last move"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    redoAct = new QAction(tr("&redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo last undone move"));
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    display_moves= new QAction(tr("&valid moves"), this);
    display_moves->setStatusTip(tr("Show valid moves"));
    display_moves->setCheckable(true);
    display_moves->setChecked(true);
    connect(display_moves, &QAction::triggered, this, &MainWindow::visualizationChange);

    display_kills= new QAction(tr("threathed &enemy"), this);
    display_kills->setStatusTip(tr("Highlight threathened pieces (enemy)"));
    display_kills->setCheckable(true);
    display_kills->setChecked(true);
    connect(display_kills, &QAction::triggered, this, &MainWindow::visualizationChange);

    display_threats= new QAction(tr("threathed &player"), this);
    display_threats->setStatusTip(tr("Highlight threathened pieces (player)"));
    display_threats->setCheckable(true);
    display_threats->setChecked(true);
    connect(display_threats, &QAction::triggered, this, &MainWindow::visualizationChange);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(exitAct);

    gameMenu = menuBar()->addMenu(tr("&Game"));
    gameMenu->addAction(undoAct);
    gameMenu->addAction(redoAct);

    visualizeMenu = menuBar()->addMenu(tr("&Visualize"));
    visualizeMenu->addAction(display_moves);
    visualizeMenu->addAction(display_kills);
    visualizeMenu->addAction(display_threats);
}

void MainWindow::on_actionExit_triggered() {
    if (QMessageBox::Yes == QMessageBox::question(this,
                                                  tr("Spel verlaten"),
                                                  tr("Bent u zeker dat u het spel wil verlaten?\nNiet opgeslagen wijzigingen gaan verloren.")))
    {
        QApplication::quit();
    }
}

