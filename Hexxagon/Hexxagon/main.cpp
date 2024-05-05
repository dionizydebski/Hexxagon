#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <set>

enum STATE{FREE,BLUE,RED,BLACK};
enum TURN{BlueTurn,RedTurn};
enum GAMEMODE{PVP,PVC};
enum WINDOWSTATE{MENU,GAME,CHOSEGAMEMODE,SCOREBOARD,LOAD_GAME,SAVE_GAME,WON_GAME};

WINDOWSTATE windowstate = WINDOWSTATE::MENU;
GAMEMODE gamemode;
TURN turn = TURN::RedTurn;
STATE whoWon;
/**
 * \brief Klasa odpowiadająca za wpisywanie zapisywanie i wczytwanie plikow tekstowych gry
 */
class TextBox {
    sf::RectangleShape shape;
    sf::Text text;
    bool isSelected;
    std::string inputText;
public:
    TextBox(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font): text(font) {
        shape.setSize(size);
        shape.setPosition({position.x - (shape.getGlobalBounds().width/2), position.y - (shape.getGlobalBounds().height/2)});
        shape.setFillColor(sf::Color::White);

        text.setCharacterSize(20);
        text.setPosition({position.x - (shape.getGlobalBounds().width/2), position.y - (shape.getGlobalBounds().height/3)});
        text.setFillColor(sf::Color::Black);

        isSelected = false;
        inputText = "";
    }
    /**
     *\brief Metoda sprawdzająca czy tekst jest wybrany i dopisująca nastepne litery
     *
     * @param event przekazywane zdarzenie z okna
     * @param window okno
     */
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (shape.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                isSelected = true;
            } else {
                isSelected = false;
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            if (isSelected) {
                if (event.text.unicode == '\b' && !inputText.empty()) {
                    inputText.pop_back();
                } else if (event.text.unicode < 128) {
                    inputText += static_cast<char>(event.text.unicode);
                }
            }
        }
        text.setString(inputText);
    }
    /**
     * \brief Metoda rysyjąca textbox
     * @param window
     */
    void drawTo(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
    /**
     * \brief Metoda zabierająca nazwę pliku i usuwająca zawartość textbox'a
     * @return tekst z tekstbox'a
     */
    std::string getAndResetInputText(){
        std::string tmp = inputText;
        inputText = "";
        return tmp;
    }
};
/**
 * \brief Klasa tworzaca wyniki graczy
 */
class Score{
    sf::RectangleShape button;
    sf::Text text;

public:
    Score(const std::string &t, const sf::Vector2f &size, const sf::Color &bgColor,const sf::Font &font): text(font){
        text.setString(t);
        text.setFillColor(sf::Color::White);
        text.setFont(font);
        text.setCharacterSize(20);

        button.setSize(size);
        button.setFillColor(bgColor);
    }
    /**
     * \brief metoda zmienijąca kolor podkładu
     * @param color
     */
    void setBackColor(sf::Color color){
        button.setFillColor(color);
    }
    /**
     * \brief metoda zmieniajaca pozycje score
     * @param pos
     */
    void setPosition(sf::Vector2f pos){
        button.setPosition({pos.x - (button.getLocalBounds().width/2),pos.y - (button.getLocalBounds().height/2)});

        float xPos = ((pos.x - (button.getLocalBounds().width/2)) + button.getLocalBounds().width / 2) - (text.getLocalBounds().width / 2);
        float yPos = ((pos.y - (button.getLocalBounds().height/2)) + button.getLocalBounds().height / 3) - (text.getLocalBounds().height / 3);
        text.setPosition({xPos, yPos});
    }
    /**
     * \brief metoda rysująca score
     * @param window
     */
    void drawTo(sf::RenderWindow &window){
        window.draw(button);
        window.draw(text);
    }
    /**
     * \brief metoda zmienijąca napis na score
     * @param t
     */
    void changeText(const std::string &t){
        text.setString(t);
    }
};
/**
 * \brief klasa tworząca przyciski
 */
class Button {
    sf::RectangleShape button;
    sf::Text text;
    bool isClickeddd;

    WINDOWSTATE windowstateChange;
public:
    Button(const std::string &t, const sf::Vector2f &size, const sf::Color &bgColor,const sf::Font &font,const WINDOWSTATE &windowstateChange): text(font), windowstateChange(windowstateChange), isClickeddd(false){
        text.setString(t);
        text.setFillColor(sf::Color::Black);
        text.setFont(font);
        text.setCharacterSize(20);

        button.setSize(size);
        button.setFillColor(bgColor);
    }
    /**
     * \brief metoda zmieniajaca kolor przycisku
     * @param color
     */
    void setBackColor(sf::Color color){
        button.setFillColor(color);
    }
    /**
     * \brief metoda zmieniajaca pozycje przycisku
     * @param pos
     */
    void setPosition(sf::Vector2f pos){
        button.setPosition({pos.x - (button.getLocalBounds().width/2),pos.y - (button.getLocalBounds().height/2)});

        float xPos = ((pos.x - (button.getLocalBounds().width/2)) + button.getLocalBounds().width / 2) - (text.getLocalBounds().width / 2);
        float yPos = ((pos.y - (button.getLocalBounds().height/2)) + button.getLocalBounds().height / 3) - (text.getLocalBounds().height / 3);
        text.setPosition({xPos, yPos});
    }
    /**
     * \brief metoda rysujaca przycisk
     * @param window
     */
    void drawTo(sf::RenderWindow &window){
        window.draw(button);
        window.draw(text);
    }
    /**
     * \brief metoda okreslajaca dzialanie przycisku
     * @param event
     * @param window
     */
    void handleEvent(sf::Event &event, sf::RenderWindow &window){
        if (event.type == sf::Event::MouseMoved){
            if (button.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                this->setBackColor(sf::Color::Blue);
            else
                this->setBackColor(sf::Color::White);
        }
        else if(event.type == sf::Event::MouseButtonPressed){
            if (button.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
                windowstate = windowstateChange;
                isClickeddd = true;
            }
            else
                isClickeddd = false;
        }
    }
    /**
     * \brief metoda sprawdzajaca czy przycisk jest klikniety
     * @return
     */
    bool isClickeddd1() const {
        return isClickeddd;
    }
};
/**
 * \brief klasa pol gry
 */
class Hexagon {
    sf::CircleShape hexagon;
    sf::CircleShape circle;
    sf::Vector2f position;

    sf::CircleShape koloFriends;

    bool clicked;

    bool justMoved;

    int i,j;

    STATE state;
public:
    Hexagon(const sf::Vector2f &position,const int i,const int j) : position(position), i(i), j(j), state(STATE::FREE), clicked(false), justMoved(false){
        hexagon = sf::CircleShape(30,6);
        hexagon.setFillColor(sf::Color::Magenta);
        hexagon.rotate(sf::degrees(90));
        hexagon.setPosition({position.x + 30,position.y - 30});
        hexagon.setOutlineColor(sf::Color::Yellow);

        koloFriends = sf::CircleShape(65);
        koloFriends.setPosition({position.x + hexagon.getGeometricCenter().x - 95, position.y + hexagon.getGeometricCenter().y - 95});
        koloFriends.setFillColor(sf::Color::Green);

        circle = sf::CircleShape(15);
        circle.setFillColor(sf::Color::White);
        circle.setPosition({position.x + hexagon.getGeometricCenter().x - 45,position.y + hexagon.getGeometricCenter().y - 45});
    }
    /**
     * \brief metoda zmieniajaca kolor kola w srodku
     * @param color
     */
    void setCircleColor(sf::Color color){
        circle.setFillColor(color);
    }
    /**
     * \brief metoda rysujaca hexagon
     * @param window
     */
    void drawTo(sf::RenderWindow &window){
        window.draw(hexagon);
        window.draw(circle);
    }
    /**
     * \brief metoda sprawdzajaca czy w dalszych znajomych pol jest pole ktorego jest aktualnie ruch
     * @param window
     * @param gameboard
     * @return
     */
    bool containsMouseFriendsOfFriends(sf::RenderWindow &window, std::vector<std::vector<Hexagon>> &gameboard){
        for(auto i : this->FriendsOfFriends(gameboard)){
            if(i.hexagon.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))) && ((i.state == STATE::RED && turn == TURN::RedTurn) || (i.state == STATE::BLUE && turn == TURN::BlueTurn)))
                return true;
        }
        return false;
    }
    /**
     * \brief metoda sprawdzajaca czy w blizszych znajomych pol jest pole ktorego jest aktualnie ruch
     * @param window
     * @param gameboard
     * @return
     */
    bool containsMouseFriends(sf::RenderWindow &window, std::vector<std::vector<Hexagon>> &gameboard){
        for(auto i : this->Friends(gameboard)){
            if(i.hexagon.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))) && ((i.state == STATE::RED && turn == TURN::RedTurn) || (i.state == STATE::BLUE && turn == TURN::BlueTurn)))
                return true;
        }
        return false;
    }
    /**
     * \brief metoda sprawdzajaca czy w blizszych znajomych pol jest pole ktore wykonalo wlasnie ruch
     * @param window
     * @param gameboard
     * @return
     */
    bool containsMouseFriendsMove(sf::RenderWindow &window, std::vector<std::vector<Hexagon>> &gameboard){
        for(auto i : this->Friends(gameboard)){
            if(i.hexagon.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))) && i.justMoved)
                return true;
        }
        return false;
    }
    /**
     * \brief metoda sprawdzjaca czy wszystkie pola wokol sa tego samego koloru
     * @param gameboard
     * @return
     */
    bool areSameColor(std::vector<std::vector<Hexagon>> &gameboard){
        for(auto i : this->Friends(gameboard)){
            if(i.getState() != STATE::FREE && i.getState() != STATE::BLACK && i.getState() != this->state) {
                i.sayState();
                return false;
            }
        }
        return true;
    }
    /**
     * \brief metoda spawdzjaca czy dalszy znajomy zostal klikniety
     * @param window
     * @param gameboard
     * @return
     */
    bool containsMouseFriendsOfFriendsMove(sf::RenderWindow &window, std::vector<std::vector<Hexagon>> &gameboard){
        for(auto i : this->FriendsOfFriends(gameboard)){
            if(i.hexagon.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                return true;
        }
        return false;
    }
    /**
     * \brief metoda sprawdzajaca czy to pole zostalo klikniete oraz czy jest jego tura
     * @param window
     * @return
     */
    bool containsMouse(sf::RenderWindow &window){
        if(hexagon.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))) && ((this->state == STATE::RED && turn == TURN::RedTurn) || (this->state == STATE::BLUE && turn == TURN::BlueTurn) || this->isReady()))
            return true;
        return false;
    }
    /**
     * \brief zwraca pozycje pola
     * @return
     */
    const sf::Vector2f &getPosition() const {
        return position;
    }
    /**
     * \brief zwraca blizyszych znajomych
     * @param gameboard
     * @return
     */
    std::vector<Hexagon> Friends(std::vector<std::vector<Hexagon>> &gameboard){
        std::vector<Hexagon> friends;
        for(int i=0;i<gameboard.size();i++)
            for(int j=0;j<gameboard.at(i).size();j++){
                if(koloFriends.getGlobalBounds().contains(gameboard.at(i).at(j).getPosition())){
                    if(this->i != i || this->j != j) {
                        friends.push_back(gameboard.at(i).at(j));
                    }
                }
            }
        return friends;
    }
    /**
     * \brief zwraca dalszych znajomych
     * @param gameboard
     * @return
     */
    std::vector<Hexagon> FriendsOfFriends(std::vector<std::vector<Hexagon>> &gameboard){
        std::vector<Hexagon> fof;
        for(auto i : this->Friends(gameboard)){
            for(auto j : i.Friends(gameboard)){
                //j sprawdzam
                bool IsThere = false;
                for(auto k : this->Friends(gameboard)){
                    if((j.j == k.j && j.i == k.i) || (j.j == this->j && j.i == this->i))
                        IsThere = true;
                }
                if(!IsThere)
                    fof.push_back(j);
            }
        }
        return fof;
    }
    /**
     * \brief metoda sprawdzajaca czy podane pole jest znajomych tego pola
     * @param gameboard
     * @param h
     * @return
     */
    bool isFriend(std::vector<std::vector<Hexagon>> &gameboard,Hexagon &h){
        for(auto i : this->Friends(gameboard))
            if(i.getI() == h.getI() && i.getJ() == h.getJ())
                return true;
        return false;
    }
    /**
     * \brief metoda zmieniajaca stan na gotowy do ruchu
     */
    void beReady(){
        hexagon.setOutlineThickness(2);
    }
    /**
     * \brief metoda zmieniajaca stan na nie gotowy do ruchu
     */
    void dontBeReady(){
        hexagon.setOutlineThickness(0);
    }
    /**
     * \brief metoda sprawdzajaca czy pole jest gotowe do ruchu
     * @return
     */
    bool isReady(){
        if(hexagon.getOutlineThickness() == 2)
            return true;
        return false;
    }
    /**
     * \brief metoda ustawiajaca stan pola
     * @param s
     */
    void setState(STATE s){
        state = s;
        if(s == STATE::BLUE)
            this->setCircleColor(sf::Color::Blue);
        else if(s == STATE::RED)
            this->setCircleColor(sf::Color::Red);
        else if(s == STATE::BLACK) {
            this->setCircleColor(sf::Color::Black);
            hexagon.setFillColor(sf::Color::Black);
        }
        else if(s == STATE::FREE)
            this->setCircleColor(sf::Color::White);
    }
    /**
     * \brief metoda wyswietlajaca stan pola
     */
    void sayState(){
        if(state == STATE::BLUE)
            std::cout<<"blue ";
        else if(state == STATE::RED)
            std::cout<<"red ";
        else if(state == STATE::BLACK) {
            std::cout<<"black ";
        }
        else if(state == STATE::FREE)
            std::cout<<"free ";;
    }
    /**
     * \brief metoda zwracajaca stan pola
     * @return
     */
    STATE getState(){
        return state;
    }
    /**
     * \brief ustawiajaca ze pole zostalo klikniete
     * @param b
     */
    void setClicked(bool b){
        clicked = b;
    }
    /**
     * \brief metoda sprawdzajaca czy pole jest klikniete
     * @return
     */
    bool isClicked(){
        if(clicked)
            return true;
        return false;
    }
    /**
     * \brief metoda ustawiajaca ze pole wlasnie sie ruszylo
     * @param b
     */
    void setJustMoved(bool b){
        justMoved = b;
    }
    /**
     * \brief metoda sprawdzajaca czy pole sie ruszylo
     * @return
     */
    bool didHeMove(){
        if(justMoved)
            return true;
        return false;
    }
    /**
     * \brief metoda sprawdzajaca ile z blizych znajomych jest czerwonych pol
     * @param gameboard
     * @return
     */
    int howManyReds(std::vector<std::vector<Hexagon>> &gameboard){
        int howManyReds = 0;
        for(auto i : this->Friends(gameboard)){
            if(i.getState() == STATE::RED){
                howManyReds++;
            }
        }
        return howManyReds;
    }
    /**
     * \brief metoda zwracajaca wsp i pola
     * @return
     */
    int getI() const {
        return i;
    }
    /**
     * \brief metoda zwracajaca wsp j pola
     * @return
     */
    int getJ() const {
        return j;
    }
};
/**
 * \brief klasa gry odpowidajaca za pole do gry, logike gry i menu
 */
class Game {
    std::vector<Button> menu;
    std::vector<Button> chosegamemode;
    std::vector<Button> game;
    std::vector<Button> scoreboard;
    std::vector<Score> score;
    std::vector<Score> won_game;
    std::vector<Score> scoreboard_places;
    std::vector<Button> load_game;
    std::vector<Button> save_game;
    std::vector<TextBox> text_boxes;

    std::vector<std::vector<Hexagon>> gameboard;

    sf::Font font;
public:
    Game(sf::RenderWindow &window, const sf::Font &font): font(font){
        this->makeButtons(window);
        this->makeGameboardAlgo(window);
    }
    /**
     * \brief metoda tworzaca pole do gry
     * @param window
     */
    void makeGameboardAlgo(sf::RenderWindow &window){
        gameboard.erase(gameboard.begin(),gameboard.end());
        gameboard.resize(17);
        int tmp = 5;
        int y = 8;
        int x = 0;
        for(int i = 0;i < gameboard.size();i++){
            if(i < 4) {
                int temp = x;
                for (int j = 0; j < i+1; j++) {
                    gameboard.at(i).push_back(Hexagon({(float) window.getSize().x / 2 + temp, (float) window.getSize().y / 2 - y * 30},i,j));
                    temp = temp+104;
                }
                x=x-52;
                y--;
            }
            else if(i < 13){
                int temp = x;
                for (int j = 0; j < tmp; j++) {
                    gameboard.at(i).push_back(Hexagon({(float) window.getSize().x / 2 + temp, (float) window.getSize().y / 2 - y * 30},i,j));
                    temp = temp+104;
                }
                if(tmp == 5)
                    tmp--;
                else
                    tmp++;
                if(x==-208)
                    x=x+52;
                else
                    x=x-52;
                y--;
            }
            else {
                int temp = x;
                for (int j = 0; j < tmp; j++) {
                    gameboard.at(i).push_back(Hexagon({(float) window.getSize().x / 2 + temp, (float) window.getSize().y / 2 - y * 30},i,j));
                    temp = temp+104;
                }
                tmp--;
                x=x+52;
                y--;
            }
        }
        gameboard.at(0).at(0).setState(STATE::BLUE);
        gameboard.at(4).at(0).setState(STATE::RED);
        gameboard.at(4).at(4).setState(STATE::RED);
        gameboard.at(12).at(0).setState(STATE::BLUE);
        gameboard.at(12).at(4).setState(STATE::BLUE);
        gameboard.at(16).at(0).setState(STATE::RED);

        gameboard.at(6).at(2).setState(STATE::BLACK);
        gameboard.at(9).at(1).setState(STATE::BLACK);
        gameboard.at(9).at(2).setState(STATE::BLACK);
    }
    /**
     * \brief metoda tworzaca przyciski do menu
     * @param window
     */
    void makeButtons(sf::RenderWindow &window){
        Button NewGameButton("New Game", {200, 50}, sf::Color::White, font, WINDOWSTATE::CHOSEGAMEMODE);
        NewGameButton.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 - 75});

        Button LoadGameButton("Load Game", {200, 50}, sf::Color::White, font, WINDOWSTATE::LOAD_GAME);
        LoadGameButton.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2});

        Button ScoreboardButton("Scoreboard", {200, 50}, sf::Color::White, font, WINDOWSTATE::SCOREBOARD);
        ScoreboardButton.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 + 75});

        Button EscButton("Quit", {100, 50}, sf::Color::White, font, WINDOWSTATE::MENU);
        EscButton.setPosition({75, 50});

        Button PvcButton("Player vs. Computer", {300, 50}, sf::Color::White, font, WINDOWSTATE::GAME);
        PvcButton.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 - 40});

        Button PvpButton("Player vs. Player", {300, 50}, sf::Color::White, font, WINDOWSTATE::GAME);
        PvpButton.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 + 40});

        Button SaveButton("Save", {100, 50}, sf::Color::White, font, WINDOWSTATE::SAVE_GAME);
        SaveButton.setPosition({1200, 50});

        Score RedScore("0",{30,30},sf::Color::Red,font);
        RedScore.setPosition({1200,600});

        Score BlueScore("0",{30,30},sf::Color::Blue,font);
        BlueScore.setPosition({1200,650});

        Score RedWon("Red won!!!",{200,50},sf::Color::Red,font);
        RedWon.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2});

        Score BlueWon("Blue won!!!",{200,50},sf::Color::Blue,font);
        BlueWon.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2});

        TextBox textBox({(float)window.getSize().x / 2, (float)window.getSize().y / 2}, sf::Vector2f(200.f, 30.f), font);

        Score place1("0",{200,50},sf::Color::Magenta,font);
        place1.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 - 150});

        Score place2("0",{200,50},sf::Color::Magenta,font);
        place2.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 - 75});

        Score place3("0",{200,50},sf::Color::Magenta,font);
        place3.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2});

        Score place4("0",{200,50},sf::Color::Magenta,font);
        place4.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 + 75});

        Score place5("0",{200,50},sf::Color::Magenta,font);
        place5.setPosition({(float)window.getSize().x / 2, (float)window.getSize().y / 2 + 150});



        scoreboard_places = std::vector<Score>{
                place1,
                place2,
                place3,
                place4,
                place5
        };
        won_game = std::vector<Score>{
                BlueWon,
                RedWon
        };
        text_boxes = std::vector<TextBox>{
                textBox,
        };
        load_game = std::vector<Button>{
                EscButton,
        };
        save_game = std::vector<Button>{
                EscButton,
        };
        score = std::vector<Score>{
                RedScore,
                BlueScore
        };
        menu = std::vector<Button>{
                NewGameButton,
                LoadGameButton,
                ScoreboardButton
        };
        chosegamemode = std::vector<Button>{
                EscButton,
                PvcButton,
                PvpButton
        };
        game = std::vector<Button>{
                EscButton,
                SaveButton
        };
        scoreboard = std::vector<Button>{
                EscButton,
        };
    }
    /**
     * \brief metoda handlujaca eventy dla stanu okna
     * @param event
     * @param window
     */
    void handleEvent(sf::Event &event, sf::RenderWindow &window){
        switch(windowstate){
            case MENU: {
                for(auto & i : menu)
                    i.handleEvent(event,window);
                break;
            }
            case CHOSEGAMEMODE: {
                for(auto & i : chosegamemode)
                    i.handleEvent(event,window);
                if(chosegamemode.at(1).isClickeddd1()) {
                    gamemode = GAMEMODE::PVC;
                }
                else if(chosegamemode.at(2).isClickeddd1()) {
                    gamemode = GAMEMODE::PVP;
                }
                break;
            }
            case SCOREBOARD: {
                for(auto & i : scoreboard)
                    i.handleEvent(event,window);
                break;
            }
            case GAME: {
                for(auto & i : game)
                    i.handleEvent(event,window);
                for(int i=0;i<gameboard.size();i++)
                    for(int j=0;j<gameboard.at(i).size();j++) {
                        this->handleEventGameboard(event, window, gameboard.at(i).at(j));
                    }
                this->isGameWon(window);
                break;
            }
            case LOAD_GAME: {
                for(auto & i : load_game)
                    i.handleEvent(event,window);
                for(auto & i : text_boxes)
                    i.handleEvent(event,window);
                if(windowstate == WINDOWSTATE::MENU) {
                    this->loadGame();
                }
                break;
            }
            case SAVE_GAME: {
                for(auto & i : save_game)
                    i.handleEvent(event,window);
                for(auto & i : text_boxes)
                    i.handleEvent(event,window);
                if(windowstate == WINDOWSTATE::MENU)
                    this->saveGame();
                break;
            }
            case WON_GAME: {
                for(auto i : save_game)
                    i.handleEvent(event,window);
            }
        }
    }
    /**
     * \brief metoda rysyjaca menu i gre dla stanu okna
     * @param window
     */
    void drawTo(sf::RenderWindow &window){
        switch(windowstate){
            case MENU: {
                for(auto & i : menu)
                    i.drawTo(window);
                break;
            }
            case CHOSEGAMEMODE: {
                for(auto & i : chosegamemode)
                    i.drawTo(window);
                break;
            }
            case SCOREBOARD: {
                for(auto & i : scoreboard)
                    i.drawTo(window);
                for(auto i : scoreboard_places)
                    i.drawTo(window);
                break;
            }
            case GAME: {
                for(auto & i : game)
                    i.drawTo(window);
                for(int i=0;i<gameboard.size();i++)
                    for(int j=0;j<gameboard.at(i).size();j++)
                        gameboard.at(i).at(j).drawTo(window);
                this->updateScore();
                for(auto & i : score)
                    i.drawTo(window);
                break;
            }
            case LOAD_GAME: {
                for(auto & i : load_game)
                    i.drawTo(window);
                for(auto & i : text_boxes)
                    i.drawTo(window);
                break;
            }
            case SAVE_GAME: {
                for(auto & i : save_game)
                    i.drawTo(window);
                for(auto & i : text_boxes)
                    i.drawTo(window);
                break;
            }
            case WON_GAME: {
                for(auto i : save_game)
                    i.drawTo(window);
                if(whoWon == STATE::BLUE){
                    won_game.at(0).drawTo(window);
                }
                else{
                    won_game.at(1).drawTo(window);
                }
            }
        }
    }
    /**
     * \brief metoda odpowiadajca za logike gry
     * @param event
     * @param window
     * @param h
     */
    void handleEventGameboard(sf::Event &event, sf::RenderWindow &window, Hexagon &h){
        if(event.type == sf::Event::MouseButtonPressed){
            if(h.didHeMove()){
                if(h.areSameColor(gameboard)) {
                    h.setJustMoved(false);
                }
            }
            if(h.isClicked()){
                if(h.containsMouseFriendsOfFriendsMove(window,gameboard)){
                    h.setState(STATE::FREE);//podlepszyc bo jak sie kliknie na nie free to i tak znika :<
                    h.setClicked(false);
                }
                else
                    h.setClicked(false);
            }
            if (h.containsMouseFriendsOfFriends(window, gameboard) && h.getState() != STATE::BLACK){
                if(!h.isReady() && h.getState() != STATE::BLUE && h.getState() != STATE::RED){
                    h.beReady();
                }
            }
            else if(h.containsMouseFriends(window, gameboard) && h.getState() != STATE::BLACK){
                if(!h.isReady() && h.getState() != STATE::BLUE && h.getState() != STATE::RED){
                    h.beReady();
                }
            }
            else if(h.containsMouse(window) && h.getState() != STATE::BLACK){
                if(h.getState() == STATE::FREE && h.isReady()){
                    if(turn == TURN::RedTurn){
                        h.dontBeReady();
                        h.setState(STATE::RED);
                        if(gamemode == GAMEMODE::PVC) {
                            this->moveAi();
                        }
                        else{
                            turn = TURN::BlueTurn;
                            h.setJustMoved(true);
                        }
                    }
                    else if(turn == TURN::BlueTurn){
                        h.dontBeReady();
                        h.setState(STATE::BLUE);
                        turn = TURN::RedTurn;
                        h.setJustMoved(true);
                    }
                }
                else if((h.getState() == STATE::BLUE || h.getState() == STATE::RED) && !h.isReady()){
                        h.setClicked(true);
                }
            }
            else
                h.dontBeReady();
        }
        else if(event.type == sf::Event::MouseButtonReleased){
            if(h.containsMouseFriendsMove(window,gameboard) && (h.getState() == STATE::RED || h.getState() == STATE::BLUE)){
                if(turn == TURN::RedTurn){
                    h.setState(STATE::BLUE);
                }
                else if(turn == TURN::BlueTurn){
                    h.setState(STATE::RED);
                }
            }
        }
    }
    /**
     * \brief metoda odswiezajaca wynik
     */
    void updateScore(){
        int red = 0;
        int blue = 0;
        for(int i=0;i<gameboard.size();i++)
            for(int j=0;j<gameboard.at(i).size();j++) {
                if (gameboard.at(i).at(j).getState() == STATE::RED)
                    red++;
                else if(gameboard.at(i).at(j).getState() == STATE::BLUE)
                    blue++;
            }
        score.at(0).changeText(std::to_string(red));
        score.at(1).changeText(std::to_string(blue));
    }
    /**
     * \brief metoda sprawdzajaca czy gra zostala wygrana i tworzaca nowe pole do gry
     * @param window
     */
    void isGameWon(sf::RenderWindow &window){
        bool canRedMove = false;
        bool canBlueMove = false;
        int red = 0;
        int blue = 0;
        int free = 0;
        for(int i=0;i<gameboard.size();i++)
            for(int j=0;j<gameboard.at(i).size();j++){
                if (gameboard.at(i).at(j).getState() == STATE::RED) {
                    red++;
                    for(auto i : gameboard.at(i).at(j).Friends(gameboard)){
                        if(i.getState() == STATE::FREE)
                            canRedMove = true;
                    }
                    for(auto i : gameboard.at(i).at(j).FriendsOfFriends(gameboard)){
                        if(i.getState() == STATE::FREE)
                            canRedMove = true;
                    }
                }
                else if(gameboard.at(i).at(j).getState() == STATE::BLUE) {
                    blue++;
                    for(auto i : gameboard.at(i).at(j).Friends(gameboard)){
                        if(i.getState() == STATE::FREE)
                            canBlueMove = true;
                    }
                    for(auto i : gameboard.at(i).at(j).FriendsOfFriends(gameboard)){
                        if(i.getState() == STATE::FREE)
                            canBlueMove = true;
                    }
                }
                else if(gameboard.at(i).at(j).getState() == STATE::FREE)
                    free++;
            }

        if(!(canBlueMove && canRedMove)){
            if(red > blue){
                windowstate = WINDOWSTATE::WON_GAME;
                whoWon = RED;
            }
            else{
                windowstate = WINDOWSTATE::WON_GAME;
                whoWon = BLUE;
            }
        }
        else if(free == 0){
            if(red > blue){
                windowstate = WINDOWSTATE::WON_GAME;
                whoWon = RED;
            }
            else{
                windowstate = WINDOWSTATE::WON_GAME;
                whoWon = BLUE;
            }
        }
        else if(red == 0 || blue == 0){
            if(red == 0){
                windowstate = WINDOWSTATE::WON_GAME;
                whoWon = RED;
            }
            else{
                windowstate = WINDOWSTATE::WON_GAME;
                whoWon = BLUE;
            }
        }

        if(windowstate == WINDOWSTATE::WON_GAME){
            this->makeGameboardAlgo(window);
            this->updateScoreBoard(red);
        }
    }
    /**
     * \brief metoda odswiezajaca tablice wynikow oraz plik
     * @param red
     */
    void updateScoreBoard(int red){
        std::ifstream fileIn("Scoreboard.txt");
        int number;
        std::set<int> scores;
        while(fileIn >> number){
            scores.insert(number);
        }

        fileIn.close();
        std::ofstream fileOut("Scoreboard.txt");

        scores.insert(red);
        if(scores.size()>5) {
            scores.erase(scores.begin());
        }
        int iterator = 4;
        for(auto i : scores){
            fileOut << i <<" ";
            scoreboard_places[iterator].changeText(std::to_string(i));
            iterator--;
        }
        fileOut.close();
    }
    /**
     * \brief metoda zapisujaca gre
     */
    void saveGame(){
        std::ofstream fileOut(text_boxes.at(0).getAndResetInputText());
        if(gamemode == GAMEMODE::PVP)
            fileOut << "1";
        else
            fileOut << "0";
        fileOut << " ";

        if(turn = TURN::RedTurn)
            fileOut << "1";
        else
            fileOut << "0";
        fileOut << " ";

        for(auto i : gameboard){
            for(auto j : i){
                if(j.getState() == STATE::RED)
                    fileOut << "1";
                else if(j.getState() == STATE::FREE)
                    fileOut << "0";
                else if(j.getState() == STATE::BLUE)
                    fileOut << "-1";
                else if(j.getState() == STATE::BLACK)
                    fileOut << "-2";
                fileOut << " ";
            }
        }
        fileOut.close();
    }
    /**
     * \brief metoda ladujaca gre
     */
    void loadGame(){
        std::ifstream fileIn(text_boxes.at(0).getAndResetInputText());
        int whosTurn;
        fileIn >> whosTurn;

        if(whosTurn == 1)
            gamemode = GAMEMODE::PVP;
        else
            gamemode = GAMEMODE::PVC;

        fileIn >> whosTurn;
        if(whosTurn == 1)
            turn = TURN::RedTurn;
        else
            turn = TURN::BlueTurn;

        int whosField;
        fileIn >> whosField;
        for(auto &i : gameboard){
            for(auto &j : i){
                if(whosField == 1)
                    j.setState(STATE::RED);
                else if(whosField == 0)
                    j.setState(STATE::FREE);
                else if(whosField == -1)
                    j.setState(STATE::BLUE);
                else if(whosField == -2)
                    j.setState(STATE::BLACK);
                fileIn >> whosField;
            }
        }
        fileIn.close();
        windowstate = WINDOWSTATE::GAME;
    }
    /**
     * metoda ruchu Ai
     */
    void moveAi(){
        Hexagon bestMovesBase = Hexagon({0,0},-1,0);
        Hexagon bestMove = Hexagon({0,0},-1,0);
        bool isHeFar = false;
        for(auto &i : gameboard)
            for(auto &j : i){
                if(j.getState() == STATE::BLUE){
                    //sprawdzamy zolte pola
                    for(auto &k : j.Friends(gameboard)){
                        if(k.getState() == STATE::FREE){
                            if(k.howManyReds(gameboard) >= bestMove.howManyReds(gameboard)){
                                bestMove = k;
                                bestMovesBase = j;
                                isHeFar = false;
                            }
                        }
                    }
                    for(auto &k : j.FriendsOfFriends(gameboard)){
                        if(k.getState() == STATE::FREE){
                            if(k.howManyReds(gameboard) >= bestMove.howManyReds(gameboard)){
                                bestMove = k;
                                bestMovesBase = j;
                                isHeFar = true;
                            }
                        }
                    }

                }
            }
        for(auto &i : gameboard)
            for(auto &j : i){
                if(j.getJ() == bestMove.getJ() && j.getI() == bestMove.getI()){
                    j.setState(STATE::BLUE);
                }
                if(isHeFar){
                    if(j.getJ() == bestMovesBase.getJ() && j.getI() == bestMovesBase.getI()){
                        j.setState(STATE::FREE);
                    }
                }
                if(j.getState() == STATE::RED && j.isFriend(gameboard,bestMove)){
                    j.setState(STATE::BLUE);
                }
            }

    }
};

int main() {
    sf::Font arial;
    arial.loadFromFile("C:\\Users\\dioni\\CLionProjects\\Hexxagon\\Arialn.ttf");

    sf::RenderWindow window(sf::VideoMode({1280,720}),"Hexxagon");
    sf::Event event;

    Game game = Game(window,arial);


    std::vector<std::vector<Hexagon>> lol;

    while (window.isOpen()){
        window.clear();
        while (window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            game.handleEvent(event, window);
        }
        game.drawTo(window);
        window.display();
    }
    return 0;
}