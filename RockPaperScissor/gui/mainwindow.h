#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <memory>
#include "RPSGameManager.h"
#include "Move.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStrategyChanged(int index);
    void onRoundsChanged(int value);
    void onStartGameClicked();
    void onMoveButtonClicked();

private:
    // Central widget and main layouts
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    // Layouts for settings, move buttons, and display
    QHBoxLayout *settingsLayout;
    QHBoxLayout *moveLayout;
    QVBoxLayout *displayLayout;

    // Settings controls
    QComboBox   *strategyComboBox;
    QSpinBox    *roundsSpinBox;
    QPushButton *startGameButton;

    // Move buttons
    QPushButton *rockButton;
    QPushButton *paperButton;
    QPushButton *scissorsButton;

    // Display elements
    QLabel *turnLabel;
    QLabel *roundLabel;
    QLabel *computerMoveLabel;
    QLabel *resultLabel;

    // Score labels
    QLabel *yourScoreLabel;
    QLabel *computerScoreLabel;
    QLabel *tiesLabel;
    QLabel *winPercentageLabel;

    // NEW: Prediction label
    QLabel *predictionLabel;

    std::unique_ptr<RPSGameManager> gameManager;

    void updateDisplay();
};

#endif // MAINWINDOW_H
