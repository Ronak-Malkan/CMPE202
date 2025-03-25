#include "mainwindow.h"
#include <QDebug>
#include <QFont>
#include <QPalette>
#include <QColor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      gameManager(std::make_unique<RPSGameManager>())
{
    // Make the window bigger by default
    resize(600, 400);

    // Create the central widget and set it for the main window
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create the main vertical layout with some spacing and margins
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // --- Settings Layout: Strategy and Rounds ---
    settingsLayout = new QHBoxLayout();
    strategyComboBox = new QComboBox();
    strategyComboBox->addItem("Random");
    strategyComboBox->addItem("Smart");

    QLabel *strategyLabel = new QLabel("<b>Strategy:</b>");
    strategyLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    settingsLayout->addWidget(strategyLabel);
    settingsLayout->addWidget(strategyComboBox);

    roundsSpinBox = new QSpinBox();
    roundsSpinBox->setRange(1, 100);
    roundsSpinBox->setValue(10);  // Default to 10 rounds

    QLabel *roundsLabel = new QLabel("<b>Rounds:</b>");
    roundsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    settingsLayout->addWidget(roundsLabel);
    settingsLayout->addWidget(roundsSpinBox);

    startGameButton = new QPushButton("Start New Game");
    settingsLayout->addWidget(startGameButton);

    mainLayout->addLayout(settingsLayout);

    // --- Move Buttons Layout ---
    moveLayout = new QHBoxLayout();
    rockButton = new QPushButton("Rock");
    paperButton = new QPushButton("Paper");
    scissorsButton = new QPushButton("Scissors");

    // Increase button sizes slightly
    rockButton->setFixedHeight(35);
    paperButton->setFixedHeight(35);
    scissorsButton->setFixedHeight(35);

    moveLayout->addWidget(rockButton);
    moveLayout->addWidget(paperButton);
    moveLayout->addWidget(scissorsButton);

    mainLayout->addLayout(moveLayout);

    // --- Display Layout ---
    displayLayout = new QVBoxLayout();

    // Turn label
    turnLabel = new QLabel("Your Turn");
    QFont turnFont = turnLabel->font();
    turnFont.setPointSize(14);
    turnFont.setBold(true);
    turnLabel->setFont(turnFont);
    turnLabel->setAlignment(Qt::AlignCenter);
    displayLayout->addWidget(turnLabel);

    // Round label
    roundLabel = new QLabel("Round: 0");
    roundLabel->setAlignment(Qt::AlignCenter);
    displayLayout->addWidget(roundLabel);

    // Computer move label
    computerMoveLabel = new QLabel("Computer Move: -");
    computerMoveLabel->setAlignment(Qt::AlignCenter);
    displayLayout->addWidget(computerMoveLabel);

    // NEW: Prediction label
    predictionLabel = new QLabel("Predicted Human Move: No Prediction");
    predictionLabel->setAlignment(Qt::AlignCenter);
    QFont predFont = predictionLabel->font();
    predFont.setPointSize(14);
    predFont.setBold(true);
    predictionLabel->setFont(predFont);
    displayLayout->addWidget(predictionLabel);

    // Result label
    resultLabel = new QLabel("Result: -");
    QFont resultFont = resultLabel->font();
    resultFont.setPointSize(16);
    resultFont.setBold(true);
    resultLabel->setFont(resultFont);
    resultLabel->setAlignment(Qt::AlignCenter);
    displayLayout->addWidget(resultLabel);

    // Score labels (in a sub-layout so they're grouped together nicely)
    QVBoxLayout *scoreLayout = new QVBoxLayout();

    yourScoreLabel = new QLabel("Your Score: 0");
    yourScoreLabel->setAlignment(Qt::AlignCenter);
    scoreLayout->addWidget(yourScoreLabel);

    computerScoreLabel = new QLabel("Computer Score: 0");
    computerScoreLabel->setAlignment(Qt::AlignCenter);
    scoreLayout->addWidget(computerScoreLabel);

    tiesLabel = new QLabel("Ties: 0");
    tiesLabel->setAlignment(Qt::AlignCenter);
    scoreLayout->addWidget(tiesLabel);

    winPercentageLabel = new QLabel("Win %: 0%");
    winPercentageLabel->setAlignment(Qt::AlignCenter);
    scoreLayout->addWidget(winPercentageLabel);

    displayLayout->addLayout(scoreLayout);

    mainLayout->addLayout(displayLayout);

    // --- Connect signals and slots ---
    connect(strategyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStrategyChanged);
    connect(roundsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::onRoundsChanged);
    connect(startGameButton, &QPushButton::clicked,
            this, &MainWindow::onStartGameClicked);

    connect(rockButton, &QPushButton::clicked, this, &MainWindow::onMoveButtonClicked);
    connect(paperButton, &QPushButton::clicked, this, &MainWindow::onMoveButtonClicked);
    connect(scissorsButton, &QPushButton::clicked, this, &MainWindow::onMoveButtonClicked);

    updateDisplay();
}

MainWindow::~MainWindow()
{
    // Qt automatically deletes child widgets
}

void MainWindow::onStrategyChanged(int index)
{
    gameManager->setStrategy(index);
}

void MainWindow::onRoundsChanged(int value)
{
    gameManager->setRounds(value);
}

void MainWindow::onStartGameClicked()
{
    gameManager->startNewGame();
    turnLabel->setText("Your Turn");
    updateDisplay();
}

void MainWindow::onMoveButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString text = button->text();
    if (text == "Rock")
        gameManager->playRound(Move::ROCK);
    else if (text == "Paper")
        gameManager->playRound(Move::PAPER);
    else if (text == "Scissors")
        gameManager->playRound(Move::SCISSORS);

    turnLabel->setText("Computer Turn Completed");
    updateDisplay();
}

void MainWindow::updateDisplay()
{
    int currentRound = gameManager->getCurrentRound();
    roundLabel->setText(QString("Round: %1").arg(currentRound));

    // Computer move
    Move compMove = gameManager->getLastComputerMove();
    computerMoveLabel->setText(QString("Computer Move: %1")
                               .arg(QString::fromStdString(moveToString(compMove))));

    // Prediction: if strategy is Smart and valid, display it; otherwise show No Prediction.
    if (gameManager->getStrategyName() == "Smart" && gameManager->isPredictionValid()) {
        predictionLabel->setText(QString("Predicted Human Move: %1")
            .arg(QString::fromStdString(moveToString(gameManager->getLastPredictedHumanMove()))));
    } else {
        predictionLabel->setText("Predicted Human Move: No Prediction");
    }

    // Round result
    std::string res = gameManager->getRoundResult();
    resultLabel->setText(QString("Result: %1").arg(QString::fromStdString(res)));

    // Scores
    int humanScore = gameManager->getHumanScore();
    int compScore  = gameManager->getComputerScore();
    int ties       = gameManager->getTies();
    int total      = humanScore + compScore + ties;
    int winPercent = (total > 0) ? (humanScore * 100 / total) : 0;

    yourScoreLabel->setText(QString("Your Score: %1").arg(humanScore));
    computerScoreLabel->setText(QString("Computer Score: %1").arg(compScore));
    tiesLabel->setText(QString("Ties: %1").arg(ties));
    winPercentageLabel->setText(QString("Win %: %1%").arg(winPercent));
}
