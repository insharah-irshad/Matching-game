#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector> //dynamic array
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <algorithm> 

using namespace std;
using namespace sf;

//for colors
Color getColorById(int id) {
    static vector<Color> colors = {
        Color::Red, Color::Green, Color::Yellow, Color::Magenta,
        Color::Cyan, Color(255, 165, 0), Color(255, 20, 147), Color(128, 0, 128)
    };
    return colors[id % colors.size()];
}

//for Fonts
class FontManager {   //class1
private:
    Font font;
public:
    bool load(const string& filename) {
        if (!font.loadFromFile(filename)) {
            cout << "Failed to load font: " << filename << endl;
            return false;
        }
        return true;
    }
    Font& getFont() {
        return font;
    }
};

//for Card 
class Card {   //class 2
private:
    RectangleShape shape;
    int id;
    bool isRevealed = false;
    bool isMatched = false;

public:
    Card(Vector2f size, Vector2f pos, int id) : id(id) {
        shape.setSize(size);
        shape.setPosition(pos);
        shape.setFillColor(Color::White);
    }

    void draw(RenderWindow& window) {
        if (isRevealed || isMatched)
            shape.setFillColor(getColorById(id));
        else
            shape.setFillColor(Color(245, 245, 245));
        window.draw(shape);
    }

    bool contains(Vector2f point) const {
        return shape.getGlobalBounds().contains(point);
    }

    void reveal() { isRevealed = true; }
    void hide() { if (!isMatched) isRevealed = false; }
    void match() { isMatched = true; }

    bool revealed() const { return isRevealed; }
    bool matched() const { return isMatched; }
    int getId() const { return id; }
};

//for Game 
class Game {    //class2
private:
    RenderWindow window;
    FontManager fontManager;
    Text titleText, startBtn, exitBtn, resultText, scoreText, timeText, playAgainBtn, creditText;
    vector<Card> cards;
    Card* first = nullptr;
    Card* second = nullptr;
    Clock flipClock, gameClock;
    bool flipping = false;
    bool gameOver = false;
    int score = 0;
    bool showStartMenu = true;
    bool showEndScreen = false;
    int finalTime = 0; 

public:
    Game() : window(VideoMode(800, 600), "Memory Matching Game") {
        if (!fontManager.load("arial.ttf"))
            exit(EXIT_FAILURE);

        setupTexts();
        srand(static_cast<unsigned>(time(nullptr))); //randomness
    }

    void setupTexts() {
        Font& font = fontManager.getFont();

        titleText.setFont(font);
        titleText.setCharacterSize(40);
        titleText.setString("Memory Matching Game");
        titleText.setFillColor(Color::White);
        titleText.setPosition(180, 100);

        startBtn.setFont(font);
        startBtn.setCharacterSize(30);
        startBtn.setString("Start Game");
        startBtn.setFillColor(Color(50, 205, 50));
        startBtn.setPosition(300, 220);

        exitBtn.setFont(font);
        exitBtn.setCharacterSize(30);
        exitBtn.setString("Exit Game");
        exitBtn.setFillColor(Color(220, 20, 60));
        exitBtn.setPosition(300, 280);

        resultText.setFont(font);
        resultText.setCharacterSize(36);
        resultText.setFillColor(Color(230, 230, 230));
        resultText.setPosition(200, 150);

        playAgainBtn.setFont(font);
        playAgainBtn.setCharacterSize(30);
        playAgainBtn.setString("Play Again");
        playAgainBtn.setFillColor(Color::Green);
        playAgainBtn.setPosition(300, 350);

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color(230, 230, 230));
        scoreText.setPosition(20, 540);

        timeText.setFont(font);
        timeText.setCharacterSize(24);
        timeText.setFillColor(Color(230, 230, 230));
        timeText.setPosition(650, 540);

        creditText.setFont(font);
        creditText.setCharacterSize(18);
        creditText.setFillColor(Color(180, 180, 180)); // soft gray
        creditText.setString("Made by Insharah Irshad & Ashweena Khurram");
        creditText.setPosition(200, 500);
    }

    void run() {
        while (window.isOpen()) {
            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed)
                    window.close();

                if (showStartMenu && e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(e.mouseButton.x, e.mouseButton.y);
                    if (startBtn.getGlobalBounds().contains(mousePos)) {
                        showStartMenu = false;
                        initCards();
                        gameClock.restart();
                    } else if (exitBtn.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                } else if (showEndScreen && e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    Vector2f mousePos(e.mouseButton.x, e.mouseButton.y);
                    if (playAgainBtn.getGlobalBounds().contains(mousePos)) {
                        resetGame();
                    }
                } else if (!gameOver && e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left && !flipping) {
                    handleClick(Vector2f(e.mouseButton.x, e.mouseButton.y));
                }
            }

            if (flipping && flipClock.getElapsedTime().asSeconds() > 1.0f) {
                checkMatch();
                flipping = false;
            }

            draw();
        }
    }

    void draw() {
        window.clear(showEndScreen ? Color(36, 52, 71): Color(36, 52, 71));

        if (showStartMenu) {
            window.draw(titleText);
            window.draw(startBtn);
            window.draw(exitBtn);
            window.draw(creditText);  
        } else if (showEndScreen) {
            window.draw(resultText);

           
            stringstream ssScore, ssTime;
            ssScore << "Score: " << score;
            ssTime << "Time: " << finalTime << "s";

            Text finalScoreText(ssScore.str(), fontManager.getFont(), 28);
            finalScoreText.setFillColor(Color::White);
            finalScoreText.setPosition(300, 220);

            Text finalTimeText(ssTime.str(), fontManager.getFont(), 28);
            finalTimeText.setFillColor(Color::White);
            finalTimeText.setPosition(300, 270);

            window.draw(finalScoreText);
            window.draw(finalTimeText);

            window.draw(playAgainBtn);
        } else {
            drawCards();
            window.draw(scoreText);
            updateTime();
            window.draw(timeText);
        }

        window.display();
    }

    void initCards() {
        cards.clear();
        vector<int> ids;
        for (int i = 0; i < 8; ++i) {
            ids.push_back(i);
            ids.push_back(i);
        }
        random_shuffle(ids.begin(), ids.end());

        Vector2f cardSize(100, 100);
        float startX = 150, startY = 50, padding = 20;

        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                Vector2f pos(startX + col * (cardSize.x + padding), startY + row * (cardSize.y + padding));
                cards.emplace_back(cardSize, pos, ids[row * 4 + col]);
            }
        }
        score = 0;
    }

    void drawCards() {
        for (auto& card : cards)
            card.draw(window);
        scoreText.setString("Score: " + to_string(score));
    }

    void updateTime() {
        int seconds = static_cast<int>(gameClock.getElapsedTime().asSeconds());
        stringstream ss;
        ss << "Time: " << seconds << "s";
        timeText.setString(ss.str());
    }

    void handleClick(Vector2f pos) {
        for (auto& card : cards) {
            if (card.contains(pos) && !card.revealed() && !card.matched()) {
                card.reveal();
                if (!first)
                    first = &card;
                else if (!second) {
                    second = &card;
                    flipping = true;
                    flipClock.restart();
                }
                break;
            }
        }
    }

    void checkMatch() {
        if (first && second) {
            if (first->getId() == second->getId()) {
                first->match();
                second->match();
                score += 10;
            } else {
                first->hide();
                second->hide();
            }

            first = nullptr;
            second = nullptr;

            if (allMatched()) {
                gameOver = true;
                showEndScreen = true;
                finalTime = static_cast<int>(gameClock.getElapsedTime().asSeconds()); // save final time
                resultText.setString("You Won! Congrats!");
            }
        }
    }

    bool allMatched() {
        for (const auto& card : cards)
            if (!card.matched())
                return false;
        return true;
    }

    void resetGame() {
        showEndScreen = false;
        gameOver = false;
        first = second = nullptr;
        initCards();
        gameClock.restart();
    }
};

int main() {
    Game g;
    g.run();
    return 0;
}
