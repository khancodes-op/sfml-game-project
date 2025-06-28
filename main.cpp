//without continous adjustement
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include<fstream>

using namespace sf;

const int M = 25;
const int N = 40;
int grid[M][N] = {0};
int ts = 18; // tile size
float totalelapsedtime = 0.0f;
bool freeze = false;
bool freeze2 = false;
bool moving =false;
bool moving2=false;
bool start=true;
long long int highscore[5]={0};
bool scoresadjusted=false;
Clock globalclock;
float globaltimer=0.0f;




void save() {
    
    for (int i=0;i<4;i++) {
    
        for (int j=0;j<4-i;j++) {
            if (highscore[j]<highscore[j + 1]) {
                
                long long int temp = highscore[j];		//swap to sort out in orderl.
                highscore[j] = highscore[j+1];
                highscore[j+1] = temp;
            }
        }
    }
    
    						//transferiing the score
    std::ofstream scores("scores.txt");
    if (scores.is_open()) {
        for (int i = 0; i < 5; i++) {
            if (highscore[i] > 0) {
                scores<< highscore[i] << std::endl;
            }
        }
        scores.close();
    } 
}


void loadhighscoresFromFile() {
    // Reset highscore array
    for (int i = 0; i < 5; i++) {
        highscore[i] = 0;
    }
    
    // Load from file
    std::ifstream load("scores.txt");
    if (load.is_open()) {
        for (int i = 0; i < 5; i++) {
            if (load >> highscore[i]) {
               
            } else {
                break; 
            }
        }
        load.close();
    } else {
        std::cout << "Unable to open file for reading" << std::endl;
    }
    
    for (int i = 0; i < 4; i++) {
    
        for (int j = 0; j < 4 - i; j++) {
            if (highscore[j] < highscore[j + 1]) {
                
                long long int temp = highscore[j];
                highscore[j] = highscore[j + 1];
                highscore[j + 1] = temp;
            }
        }
    }
}

void updateHighscore(long long int score) {
    // Load existing scores first
    loadhighscoresFromFile();
    
    // Add new score to array
    bool scoreAdded = false;
    for (int i = 0; i < 5; i++) {
        if (score > highscore[i]) {
            // Shift all lower scores down
            for (int j = 4; j > i; j--) {
                highscore[j] = highscore[j - 1];
            }
            // Insert the new score
            highscore[i] = score;
            scoreAdded = true;
            break;
        }
    }
    
    // If score wasn't added but there's room in the array
    if (!scoreAdded) {
        for (int i = 0; i < 5; i++) {
            if (highscore[i] == 0) {
                highscore[i] = score;
                break;
            }
        }
    }
    
    
    save();
}


std::string getScoreboardText() {
    loadhighscoresFromFile();
    std::string result ="=== SCOREBOARD ===\n\n\n";
    for (int i=0;i<5;i++) {
        if (highscore[i]>0) {		//only insert when the value is greater than 0
            result+=std::to_string(i + 1) + ". " + std::to_string(highscore[i]) + "\n";
        } else {
            result += std::to_string(i + 1) + ". ---\n";
        }
    }
    result += "\nPress B to go back";
    return result;
}


void scoremultiplier(int *tilescovered,int *rewardcount, int *score);
void counttotalscore_bfr(int grid[M][N],int *totalscore_bfr);
void counttotalscore_aftr(int grid[M][N],int *totalscore_aftr);
void counttotalscore_bfr2(int grid[M][N],int* totalscore_bfr2);
void counttotalscore_aftr2(int grid[M][N],int* totalscore_aftr2);
void calculatescore(int *totalscore_bfr,int *totalscore_aftr, int *score);
void calculatetotalscore(int *totalscore,int *score);
void powerup(int *totalscore, int *powerupcount,bool *bound50, bool *bound70, bool *bound100, bool *bound130,int *oldcount);
void resetgame_var(int &x,int &y,int &dx,int &dy,int &movecount, int &score, int &totalscore, int *totalscore_aftr,int *totalscore_bfr,int *tilescovered, bool *score_done, int *rewardcount,int *powerupcount, bool *bound50, bool *bound70, bool *bound100, bool *bound130,int &oldcount, bool &Game, bool &inendmenu, bool &freeze, int &enemyCount,int level,float &cont_lvl_timer, bool &levelselected, Clock &elapsedtime, Clock &freezetime, bool *player_selected, int &x2, int &y2, int &dx2, int &dy2, int &movecount2,int *totalscore_aftr2, int *totalscore_bfr2, int *tilescovered2, bool *score_done2,int *rewardcount2, int *powercount2, bool *bound50_2, bool *bound70_2,bool *bound100_2, bool *bound130_2, float &timer, float &timer2, int &totalscore2,Clock &freeze2,bool *p1_life,bool *p2_life,bool *start,bool *moving,bool *moving2,bool *scoresadjusted);
void reset_grid();

struct Enemy {
    int x, y, dx, dy;
    Enemy() {
        x = (rand() % (N-2) + 1) * ts;
        y = (rand() % (M-2) + 1) * ts;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }
    
    void zigzag(){
    	if(globaltimer>0.5f){
    		globaltimer=0;
    		dy=-dy;
    	}
    	 x += dx;
		    if (x / ts < 0 || x / ts >= N || grid[y / ts][x / ts] == 1 || grid[y / ts][x / ts] == 3) { dx = -dx; x += dx;}
		    y += dy;
		    if (y / ts < 0 || y / ts >= M || grid[y / ts][x / ts] == 1 || grid[y / ts][x / ts] == 3) { dy = -dy; y += dy; }
	}
    
    
    
    void move(int shape) {
	    if(shape==0){
		if (!freeze && !freeze2) { // Check both freeze states
		    x += dx;
		    if (x / ts < 0 || x / ts >= N || grid[y / ts][x / ts] == 1 || grid[y / ts][x / ts] == 3) { dx = -dx; x += dx; }
		    y += dy;
		    if (y / ts < 0 || y / ts >= M || grid[y / ts][x / ts] == 1 || grid[y / ts][x / ts] == 3) { dy = -dy; y += dy; }
		}
	    }
	    else if(shape==1){
	    	if(!freeze&&!freeze2){
	    		zigzag();
	    	
	    	}
	    	
	    }
    }
};

void drop(int y, int x) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    if (grid[y][x] != 0) return;

    grid[y][x] = -1;

    drop(y - 1, x);
    drop(y + 1, x);
    drop(y, x - 1);
    drop(y, x + 1);
}

void increaseSpeed(int* ptrx, int *ptry) {
    // Increase magnitude of dx
    if (*ptrx < 0){
     *ptrx -= 1; 	//move left
    }
    else if (*ptrx > 0){
     *ptrx += 1; 	//move right
    }
    else{
     *ptrx=(rand()%2==0)?1:-1; 
     }

    // changes in y 
    if (*ptry < 0){	//move up
     *ptry -= 1; 	
    }
    else if (*ptry > 0){	//move down
     *ptry += 1;
    } 
    else{
     *ptry = (rand() % 2 == 0) ? 1 : -1; 
    }
}

int main() {
    srand(time(0));
    const int total_playerchoice = 2;
    float cont_lvl_timer = 0;
    const int total_levels = 4;
    bool playerchoice = false;
    bool player_selected = false;
    bool inMenu = true, inLevelSelect = false;
    bool inScoreboard = false, Game = true;
    bool levelselected = false;
    bool inendmenu = false;
    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);

    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");
    t4.loadFromFile("images/tiles.png");
    
    // Load background textures
    Texture menuBgTexture, gameBgTexture, endmenuBgTexture, freezeBgTexture;
    if (!menuBgTexture.loadFromFile("fixedimages/Startmenu.jpg")) {
        std::cout << "Error loading menu background!" << std::endl;
        return -1;
    }
    if (!gameBgTexture.loadFromFile("fixedimages/NormalBackground.png")) {
        std::cout << "Error loading game background!" << std::endl;
        return -1;
    }
    if (!endmenuBgTexture.loadFromFile("fixedimages/Startmenu.jpg")) {
        std::cout << "Error loading end menu background!" << std::endl;
        return -1;
    }
    if (!freezeBgTexture.loadFromFile("fixedimages/Freezebackground.png")) {
        std::cout << "Error loading freeze background!" << std::endl;
        return -1;
    }
    

    

    
    Sprite menuBgSprite(menuBgTexture);
    Sprite gameBgSprite(gameBgTexture);
    Sprite endmenuBgSprite(endmenuBgTexture);
    Sprite freezeBgSprite(freezeBgTexture);



    Sprite sTile(t1), sGameover(t2), sEnemy(t3), sTile2(t4);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    Font font;
    if (!font.loadFromFile("arial.ttf")) { std::cout << "Error loading font!" << std::endl; return -1; }

    Text menuText;
    menuText.setFont(font);
    menuText.setString("Press Enter to Start\nPress Esc to Exit\nPress S for Scoreboard");
    menuText.setCharacterSize(30);
    menuText.setFillColor(Color::White);
    menuText.setPosition(100, 100);

    Text playeroptions[total_playerchoice];
    playeroptions[0].setFont(font);
    playeroptions[0].setString("1 PLAYER");
    playeroptions[0].setCharacterSize(30);
    playeroptions[0].setPosition(100, 350);
    playeroptions[1].setFont(font);
    playeroptions[1].setString("2 PLAYER");
    playeroptions[1].setCharacterSize(30);
    playeroptions[1].setPosition(100, 400);

    Text level_intro;
    level_intro.setFont(font);
    level_intro.setString("Choose the difficulty level");
    level_intro.setCharacterSize(30);
    level_intro.setFillColor(Color::White);
    level_intro.setPosition(100, 100);

    Text levels[total_levels];
    levels[0].setFont(font);
    levels[0].setString("Easy");
    levels[0].setCharacterSize(30);
    levels[0].setPosition(250, 150);
    levels[1].setFont(font);
    levels[1].setString("Medium");
    levels[1].setCharacterSize(30);
    levels[1].setPosition(250, 200);
    levels[2].setFont(font);
    levels[2].setString("Hard");
    levels[2].setCharacterSize(30);
    levels[2].setPosition(250, 250);
    levels[3].setFont(font);
    levels[3].setString("Continuous mode");
    levels[3].setCharacterSize(30);
    levels[3].setPosition(250, 300);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("=== SCOREBOARD ===\n\n1. ---\n2. ---\n3. ---\n4. ---\n5. ---\n\nPress B to go back");
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::Yellow);
    scoreText.setPosition(100, 100);

    Text continueText;
    continueText.setFont(font);
    continueText.setString("Press Enter to select Level");
    continueText.setCharacterSize(24);
    continueText.setFillColor(Color::Cyan);
    continueText.setPosition(100, 500);

    Text scorecounter;
    scorecounter.setFont(font);
    scorecounter.setCharacterSize(24);
    scorecounter.setFillColor(Color::White);
    scorecounter.setPosition(10, 10);
    
    Text scorecounter_2;
    scorecounter_2.setFont(font);
    scorecounter_2.setCharacterSize(24);
    scorecounter_2.setFillColor(Color::White);
    scorecounter_2.setPosition(600, 10);

    Text elapsedtime_txt;
    elapsedtime_txt.setFont(font);
    elapsedtime_txt.setCharacterSize(23);
    elapsedtime_txt.setFillColor(Color::Green);
    elapsedtime_txt.setPosition(350, 10);

    Text inventory_txt;
    inventory_txt.setFont(font);
    inventory_txt.setCharacterSize(23);
    inventory_txt.setFillColor(Color::Magenta);
    inventory_txt.setPosition(10, 60);
    
    Text inventory_txt_2;
    inventory_txt_2.setFont(font);
    inventory_txt_2.setCharacterSize(23);
    inventory_txt_2.setFillColor(Color::Magenta);
    inventory_txt_2.setPosition(570, 60);
    
    Text movecounter;
    movecounter.setFont(font);
    movecounter.setCharacterSize(23);
    movecounter.setFillColor(Color::Red);
    movecounter.setPosition(10, 90);
    
    Text movecounter_2;
    movecounter_2.setFont(font);
    movecounter_2.setCharacterSize(23);
    movecounter_2.setFillColor(Color::Red);
    movecounter_2.setPosition(600, 90);

    Text endmenuText;
    endmenuText.setFont(font);
    endmenuText.setString("Press R to ReStart\nPress E to Exit\nPress M for Main Menu");
    endmenuText.setCharacterSize(30);
    endmenuText.setFillColor(Color::Blue);
    endmenuText.setPosition(170, 250);

    Text endmenuscore1;
    endmenuscore1.setFont(font);
    endmenuscore1.setCharacterSize(30);
    endmenuscore1.setFillColor(Color::White);
    endmenuscore1.setPosition(170, 100);
    
    Text endmenuscore2;
    endmenuscore2.setFont(font);
    endmenuscore2.setCharacterSize(30);
    endmenuscore2.setFillColor(Color::White);
    endmenuscore2.setPosition(400, 100);
    
    Text endmenuscore1high;
    endmenuscore1high.setFont(font);
    endmenuscore1high.setCharacterSize(30);
    endmenuscore1high.setFillColor(Color::Yellow);
    endmenuscore1high.setPosition(170, 100);
    
    Text endmenuscore2high;
    endmenuscore2high.setFont(font);
    endmenuscore2high.setCharacterSize(30);
    endmenuscore2high.setFillColor(Color::Yellow);
    endmenuscore2high.setPosition(400, 100);
    
        Text winner_txt;
	winner_txt.setFont(font);
	winner_txt.setCharacterSize(80);
	winner_txt.setFillColor(Color::Yellow);
	winner_txt.setString("Player 1 Wins!"); // Temporary string for bounds
	FloatRect winnerBounds = winner_txt.getLocalBounds();
	winner_txt.setOrigin(winnerBounds.width / 2, winnerBounds.height / 2);
	winner_txt.setPosition(N * ts / 2, 50); // Center horizontally, 50 pixels from top
    
    
    
    // States
    int x = 0, y = 0, dx = 0, dy = 0;
    int x2 = N-1, y2 = 0, dx2 = 0, dy2 = 0;
    float timer = 0;
    float delay = 0.07;
    float timer2 = 0;
    float delay2 = 0.07;

    // Movement counter variables
    int prevdirectx = 0;
    int prevdirecty = 0;
    int prevdirectx2 = 0;
    int prevdirecty2 = 0;

    // Scoring variables
    int score = 0; int totalscore_aftr = 0;int  totalscore_bfr = 0;int  totalscore = 0;int tilescovered = 0;
    bool score_done = false;
    int score2 = 0;int totalscore_aftr2 = 0 ;int totalscore_bfr2 = 0 ;int totalscore2 = 0 ;int tilescovered2 = 0;
    bool score_done2 = false;

    // Powercount variables
    int rewardcount = 0, powerupcount = 0;
    int rewardcount2 = 0, powercount2 = 0;
    bool bound50 = false, bound70 = false;
    bool bound100 = false, bound130 = false;
    bool bound50_2 = false, bound70_2 = false;
    bool bound100_2 = false, bound130_2 = false;
    int oldcount = 130;
    int movecount = 0;
    int movecount2 = 0;

    // Clocks
    Clock clock, elapsedtime, speedclock, freezetime, freezetime2, keypress; 

    // Enemy count
    int level=0;
    int enemyCount=4;
    Enemy a[10];
    
    //playerlives
    bool p1_life=true;
    bool p2_life=false;		//depends on the palyerchouice shuru me is dead
    
    //winner
    int winner=0;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) grid[i][j] = 1;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        float elapsedtime_time = elapsedtime.getElapsedTime().asSeconds();
        float speedtimer = speedclock.getElapsedTime().asSeconds();
        float cont_delay = 20.0f, speed_delay = 20.0f;
        float freezetime_duration = 3.0f;
        float freezetime_time = freezetime.getElapsedTime().asSeconds();
        float freezetime_time2 = freezetime2.getElapsedTime().asSeconds();

        clock.restart();
        globaltimer+=time;
        timer +=time;
        timer2 +=time;
        cont_lvl_timer+=time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) window.close();
                if (inMenu) {
                    if (!player_selected) {
                        if (e.key.code == Keyboard::Down) playerchoice = true;
                        if (e.key.code == Keyboard::Up) playerchoice = false;
                        if (e.key.code == Keyboard::Enter) player_selected = true;
                    } else {
                        if (e.key.code == Keyboard::Enter) {
                            inMenu = false;
                            inLevelSelect = true;
                        }
                    }
                    if (e.key.code == Keyboard::S) {
                        inMenu = false;
                        inScoreboard = true;
                    }
                } else if (inLevelSelect) {
                    if (e.key.code == Keyboard::Down && level < total_levels-1) level++;
                    if (e.key.code == Keyboard::Up && level > 0) level--;
                    if (e.key.code == Keyboard::Enter) {
                        inLevelSelect = false;
                        Game = true;
                        levelselected = false;
                    }
                } else if (inScoreboard) {
                    if (e.key.code == Keyboard::B) {
                        inScoreboard = false;
                        inMenu = true;
                        player_selected = false;
                    }
                } else if (inendmenu) {
                    if (e.key.code == Keyboard::R) {
                        Game = true;
                        resetgame_var(x, y, dx, dy, movecount, score, totalscore, &totalscore_aftr, &totalscore_bfr, &tilescovered, &score_done, &rewardcount, &powerupcount, &bound50, &bound70, &bound100, &bound130, oldcount, Game, inendmenu, freeze, enemyCount, level, cont_lvl_timer, levelselected, elapsedtime, freezetime, &player_selected, x2, y2, dx2, dy2, movecount2, &totalscore_aftr2, &totalscore_bfr2, &tilescovered2, &score_done2, &rewardcount2, &powercount2, &bound50_2, &bound70_2, &bound100_2, &bound130_2, timer, timer2, totalscore2,freezetime2,&p1_life,&p2_life,&start,&moving,&moving2,&scoresadjusted);
                        player_selected = true;
                        p2_life=true;
                        reset_grid();
                    }
                    if (e.key.code == Keyboard::M) {
                        Game = true;
                        resetgame_var(x, y, dx, dy, movecount, score, totalscore, &totalscore_aftr, &totalscore_bfr, &tilescovered, &score_done, &rewardcount, &powerupcount, &bound50, &bound70, &bound100, &bound130, oldcount, Game, inendmenu, freeze, enemyCount, level, cont_lvl_timer, levelselected, elapsedtime, freezetime, &player_selected, x2, y2, dx2, dy2, movecount2, &totalscore_aftr2, &totalscore_bfr2, &tilescovered2, &score_done2, &rewardcount2, &powercount2, &bound50_2, &bound70_2, &bound100_2, &bound130_2, timer, timer2, totalscore2,freezetime2,&p1_life,&p2_life,&start,&moving,&moving2,&scoresadjusted);
                        inMenu = true;
                        reset_grid();
                    }
                    if (e.key.code == Keyboard::E) window.close();
                }
            }
        }

        window.clear();
        if (inMenu) {
            window.draw(menuBgSprite);
            window.draw(menuText);
            for (int i = 0; i < total_playerchoice; i++) {
                if (i == playerchoice) playeroptions[i].setFillColor(Color::Red);
                else playeroptions[i].setFillColor(Color::White);
                window.draw(playeroptions[i]);
                
            }
            if (player_selected){
             window.draw(continueText);
             if(playerchoice){
             	p2_life=true;
             }
            }
        } else if (inLevelSelect) {
            window.draw(menuBgSprite);
            for (int i = 0; i < total_levels; i++) {
                if (i == level) levels[i].setFillColor(Color::Green);
                else levels[i].setFillColor(Color::White);
                window.draw(level_intro);
                window.draw(levels[i]);
            }
        } else if (inScoreboard) {
            scoreText.setString(getScoreboardText());
            window.draw(menuBgSprite); 
            window.draw(scoreText);
        } else if (inendmenu) {
                
            if (!scoresadjusted) {
                if (totalscore > 0) {
                    updateHighscore(totalscore);
                    
                }
                if (playerchoice && totalscore2 > 0) {
                    updateHighscore(totalscore2);
                }
                scoresadjusted = true;
            }
            window.draw(menuBgSprite);
            window.draw(endmenuText);
            for(int i=0;i<5;i++){
            	if(highscore[i]==totalscore){
            		endmenuscore1high.setString("Score 1: " + std::to_string(totalscore));
            		window.draw(endmenuscore1high);
            		break;
            	}
            	else{
            		endmenuscore1.setString("Score 1: " + std::to_string(totalscore));
            		window.draw(endmenuscore1);
            	}
            
            }
            
            
            if (playerchoice) {
                for(int i=0;i<5;i++){
            	if(highscore[i]==totalscore2){
            		endmenuscore2high.setString("Score 2: " + std::to_string(totalscore2));
            		window.draw(endmenuscore2high);
            		break;
            	}
            	else{
            		endmenuscore2.setString("Score 2: " + std::to_string(totalscore2));
            		window.draw(endmenuscore2);
            	}
            
            }
                if (winner == 1) {
                    winner_txt.setString("PLAYER 1 WINS!");
                } else if (winner == 2) {
                    winner_txt.setString("PLAYER 2 WINS!");
                } else {
                    winner_txt.setString("It IS A TIE!");
                }
                window.draw(winner_txt);
            }
            if (e.type == Event::KeyPressed && (e.key.code == Keyboard::R || e.key.code == Keyboard::M || e.key.code == Keyboard::E)) {
                scoresadjusted = false;
            }
            
        } else if (Game) {
        	if(start){
        	elapsedtime.restart();
        	start=false;
        	}
        	
        	
        	if(freeze||freeze2){
        		window.draw(freezeBgSprite);
        	}
        	else{
        		window.draw(gameBgSprite);
        	}
            // Movement and freezing mechanism for player 1 and 2
            if(p1_life){
		    if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
		    if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; }
		    if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; }
		    if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; }
		    if (Keyboard::isKeyPressed(Keyboard::Enter) && powerupcount > 0 && !freeze) {
		        freeze = true;
		        powerupcount--;
		        freezetime.restart();
		    }
           }
		
	    
            if (playerchoice&&p2_life) {
                if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }
                if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }
                if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1; }
                if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1; }
                if (Keyboard::isKeyPressed(Keyboard::Q) && powercount2 > 0 && !freeze2) {
                    freeze2 = true;
                    powercount2--;
                    freezetime2.restart();
                }
            }

            // Movement counter for player 1 and player 2
            

            if (grid[y][x] != 1||grid[y][x]!=3) {		//when  out the blue tiles
                moving=true;
            }
            if(grid[y2][x2]!=1||grid[y2][x2]!=3){	//when out of the tiles
            	
            	
            	moving2=true;
            	
            }
            
            
            	
            
            if (playerchoice) {
                if (timer2 > delay2&&!freeze&&p2_life) {
                    x2 += dx2;
                    y2 += dy2;
                    if (x2 < 0) x2 = 0;
                    if (x2 > N - 1) x2 = N - 1;
                    if (y2 < 0) y2 = 0;
                    if (y2 > M - 1) y2 = M - 1;
                    if (grid[y2][x2] == 2||grid[y2][x2]==4){
                     p2_life=false;
                    	for(int i=0;i<M;i++){
                    		for(int j=0;j<N;j++){
                    			if(grid[i][j]==4){
                    				grid[i][j]=9;
                    			}
                    		
                    		}
                    	
                    	}
                    
                   }	
                    if (grid[y2][x2] == 0) grid[y2][x2] = 4;
                    timer2 = 0;
                }
            }

            if (timer > delay&&!freeze2&&p1_life) {	//only run when player is alive
                x += dx;
                y += dy;
                if (x < 0) x = 0;
                if (x > N - 1) x = N - 1;
                if (y < 0) y = 0;
                if (y > M - 1) y = M - 1;
                
                 if (grid[y][x] == 2||grid[y][x]==4){
                  p1_life=false;
                  for(int i=0;i<M;i++){
                    		for(int j=0;j<N;j++){
                    			if(grid[i][j]==2){
                    				grid[i][j]=9;
                    			}
                    		
                    		}
                    	
                    	}
                 }
                if (grid[y][x] == 0) grid[y][x] = 2;
                timer = 0;
            }
            
            if (playerchoice && p1_life && p2_life && x == x2 && y == y2 && 
    grid[y][x] != 1 && grid[y][x] != 3 && grid[y2][x2] != 1 && grid[y2][x2] != 3) {
    p1_life = false;
    p2_life = false;
    // Clear trails for both players
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 2 || grid[i][j] == 4) {
                grid[i][j] = 9;
            }
        }
    }
}

            
            if(p1_life==false&&p2_life==false&&playerchoice){
            	Game=false;
            	
            }
            if(p1_life==false&&playerchoice==false){
            	Game=false;
            }
            if (p1_life == false && p2_life == false && playerchoice) {
		    Game = false;
		    inendmenu = true;
		    if (totalscore > totalscore2) {
			winner = 1;
		    } else if (totalscore2 > totalscore) {
			winner = 2;
		    } else {
			winner = 0; // Tie
		    }
	    }
            

            // Freezing logic
            if (freeze && freezetime_time >= freezetime_duration&&p1_life) {
                freeze = false;
                freezetime.restart();
            }
            if (playerchoice && freeze2 && freezetime_time2 >= freezetime_duration&&p1_life) {
                freeze2 = false;
                freezetime2.restart();
            }

            // Enemy count decision over the levels
            if (!levelselected) {
                levelselected = true;
                if (level == 0) enemyCount = 2;
                else if (level == 1) enemyCount = 4;
                else if (level == 2) enemyCount = 6;
                else if (level == 3) enemyCount = 2;
                else enemyCount = 1;
                for (int i = 0; i < enemyCount; i++) {
                    a[i].x = (rand() % (N-2) + 1) * ts;
                    a[i].y = (rand() % (M-2) + 1) * ts;
                    a[i].dx = 4 - rand() % 8;
                    a[i].dy = 4 - rand() % 8;
                }
            }

            // Changing speed
            if (speedtimer >= speed_delay) {
                for (int i = 0; i < enemyCount; i++) {
                    increaseSpeed(&a[i].dx, &a[i].dy);
                }
                speedclock.restart();
            }

            for (int i = 0; i < enemyCount; i++){
             
             if(elapsedtime_time<30.0f){
             	a[i].move(0);
             }
             else{
             	if(i%2==0){
             		a[i].move(0);
             	}
             	else{
             		a[i].move(1);
             	}
             }
           }

            // Increasing the number of enemies after every 20 seconds
            if (level == 3) {
                if (cont_lvl_timer > cont_delay) {
                    if (enemyCount + 2 <= 10) {
                        int oldCount = enemyCount;
                        enemyCount += 2;
                        for (int i = oldCount; i < enemyCount; i++) {
                            a[i].x = (rand() % (N-2) + 1) * ts;
                            a[i].y = (rand() % (M-2) + 1) * ts;
                            a[i].dx = 4 - rand() % 8;
                            a[i].dy = 4 - rand() % 8;
                        }
                    }
                    cont_lvl_timer = 0;
                }
            }

            // Scoring logic
            if ((grid[y][x] == 1 || grid[y][x] == 3)&&moving) {
                dx = dy = 0;
                moving=false;
                if (!score_done) {
                    for (int i = 0; i < enemyCount; i++) drop(a[i].y / ts, a[i].x / ts);
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            if (grid[i][j] == -1) grid[i][j] = 0;
                            else if (grid[i][j] != 3) grid[i][j] = 1;
                        }
                    }
                    counttotalscore_aftr(grid, &totalscore_aftr);
                    tilescovered=totalscore_aftr - totalscore_bfr;
                    scoremultiplier(&tilescovered, &rewardcount, &score);
                    calculatetotalscore(&totalscore, &score);
                    movecount++;
                    score_done = true;
                    powerup(&totalscore, &powerupcount, &bound50, &bound70, &bound100, &bound130, &oldcount);
                }
            } else {
                score_done=false;
                counttotalscore_bfr(grid, &totalscore_bfr);
            }

            // Scoring for player 2
            if (playerchoice && (grid[y2][x2] == 1 || grid[y2][x2] == 3)&&moving2) {
                dx2 = dy2 = 0;
                moving2=false;
                if (!score_done2) {
                    for (int i = 0; i < enemyCount; i++) drop(a[i].y / ts, a[i].x / ts);
                    for (int i = 0; i < M; i++) {
                        for (int j = 0; j < N; j++) {
                            if (grid[i][j] == -1) grid[i][j] = 0;
                            else if (grid[i][j] != 1) grid[i][j] = 3;
                        }
                    }
                    counttotalscore_aftr2(grid, &totalscore_aftr2);
                    tilescovered2 = totalscore_aftr2 - totalscore_bfr2;
                    scoremultiplier(&tilescovered2, &rewardcount2, &score2);
                    calculatetotalscore(&totalscore2, &score2);
                    movecount2++;
                    score_done2 = true;
                    powerup(&totalscore2, &powercount2, &bound50_2, &bound70_2, &bound100_2, &bound130_2, &oldcount);
                }
            } else if (playerchoice) {
                score_done2 = false;
                counttotalscore_bfr2(grid, &totalscore_bfr2);
            }

            for (int i = 0; i < enemyCount; i++){
                if (grid[a[i].y / ts][a[i].x / ts] == 2){
                 p1_life = false;
                 	for(int m=0;m<M;m++){
                 		for(int n=0;n<N;n++){
                 			if(grid[m][n]==2){
                 				grid[m][n]=9;	//death tiles converted to 8
                 			}
                 		}	
                 	}
                }
                if (grid[a[i].y / ts][a[i].x / ts] == 4){
                 p2_life = false;
                 	for(int m=0;m<M;m++){
                 		for(int n=0;n<N;n++){
                 			if(grid[m][n]==4){
                 				grid[m][n]=9;	//death tiles converted to 9 
                 			}
                 		}	
                 	}
                 
                }
	    }
	    
            if (Game == false) inendmenu = true;

            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    if (grid[i][j] == 0) continue;
                    if (grid[i][j] == 1 || grid[i][j] == 3||grid[i][j]==9) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                    if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                    if (grid[i][j] == 4) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                    sTile.setPosition(j * ts, i * ts);
                    window.draw(sTile);
                    scorecounter.setString("Score: "+std::to_string(totalscore));
                    elapsedtime_txt.setString("Time: " + std::to_string((int)elapsedtime_time));
                    movecounter.setString("Moves: "+std::to_string(movecount-1));
                    inventory_txt.setString("PowerUps: "+ std::to_string(powerupcount));
                    window.draw(scorecounter);
                    window.draw(elapsedtime_txt);
                    window.draw(movecounter);
                    window.draw(inventory_txt);
                    if (playerchoice) {
                        scorecounter_2.setString("Score: "+ std::to_string(totalscore2));
                        movecounter_2.setString("Moves: "+ std::to_string(movecount2-1));
                        inventory_txt_2.setString("PowerUps: "+ std::to_string(powercount2));
                        window.draw(scorecounter_2);
                        window.draw(movecounter_2);
                        window.draw(inventory_txt_2);
                    }
                }
            }
	    if(p1_life){
		    sTile.setTextureRect(IntRect(36, 0, ts, ts));
		    sTile.setPosition(x * ts, y * ts);
		    window.draw(sTile);
            }

            if (playerchoice&&p2_life) {
                sTile2.setTextureRect(IntRect(108, 0, ts, ts));
                sTile2.setPosition(x2 * ts, y2 * ts);
                window.draw(sTile2);
            }

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++) {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }

            if (!Game) window.draw(sGameover);
        }
        window.display();
    }
    return 0;
}

void scoremultiplier(int *tilescovered, int *rewardcount, int *score) {
    if (*rewardcount>= 5 && *tilescovered > 5) {
        *tilescovered *= 4;
        *rewardcount += 1;
    }else if (*rewardcount>= 3 && *rewardcount < 5 && *tilescovered>5) {
        *tilescovered *= 2;
        *rewardcount += 1;
    }else if (*tilescovered> 10 && *rewardcount<=3) {
        *tilescovered *= 2;
        *rewardcount += 1;
    }
    *score = *tilescovered;
}

void counttotalscore_aftr(int grid[M][N], int *totalscore_aftr) {
    int count=0;
    for (int i=0;i<M;i++) {
        for (int j=0;j<N;j++) {
            if (grid[i][j]==1){
             count++;
            }
        }
    }
    *totalscore_aftr=count-126;		//to remove the 126 tiles alrdy made 
}

void counttotalscore_bfr(int grid[M][N], int* totalscore_bfr) {
    int count = 0;
    for (int i=0;i<M;i++) {
        for (int j=0;j<N;j++) {
            if (grid[i][j]==1){
             count++;
            }
        }
    }
    *totalscore_bfr=count-126;
}

void counttotalscore_aftr2(int grid[M][N], int* totalscore_aftr2) {
    int count=0;
    for (int i=0;i<M;i++) {
        for (int j=0;j<N;j++) {
            if (grid[i][j]==3){
             count++;
            }
        }
    }
    *totalscore_aftr2=count;
}

void counttotalscore_bfr2(int grid[M][N], int* totalscore_bfr2) {
    int count=0;
    for (int i=0;i<M;i++) {
        for (int j=0;j<N;j++) {
            if (grid[i][j]==3){
             count++;
            }
        }
    }
    *totalscore_bfr2=count;
}

void calculatescore(int *totalscore_bfr, int *totalscore_aftr, int *score) {
    *score=*totalscore_aftr-*totalscore_bfr;
}

void calculatetotalscore(int *totalscore, int *score) {
    *totalscore+=*score;
}

void powerup(int *totalscore, int *powerupcount, bool *bound50, bool *bound70, bool *bound100, bool *bound130, int *oldcount) {
    // Award power-ups at score milestones
    if (*totalscore >= 50 && !*bound50) {
        *powerupcount += 1;
        *bound50 = true;
    }
    if (*totalscore >= 70 && !*bound70) {
        *powerupcount += 1;
        *bound70 = true;
    }
    if (*totalscore >= 100 && !*bound100) {
        *powerupcount += 1;
        *bound100 = true;
    }
    if (*totalscore >= 130 && !*bound130) {
        *powerupcount += 1;
        *bound130 = true;
    }
    // Additional power-ups every 30 points after 130
    if (*totalscore >= 130) {
        int additional_powerups=(*totalscore-*oldcount) / 30;	//formula to calulcate additional powerups that are to be given on score being greater than 130
        if (additional_powerups > 0) {
            *powerupcount += additional_powerups;
            *oldcount += additional_powerups * 30;
        }
    }
    
    if (*powerupcount < 0) {
    	*powerupcount = 0;
    }
}

void resetgame_var(int &x, int &y, int &dx, int &dy, int &movecount, int &score, int &totalscore, int *totalscore_aftr, int *totalscore_bfr, int *tilescovered, bool *score_done, int *rewardcount, int *powerupcount, bool *bound50, bool *bound70, bool *bound100, bool *bound130, int &oldcount, bool &Game, bool &inendmenu, bool &freeze, int &enemyCount, int level, float &cont_lvl_timer, bool &levelselected, Clock &elapsedtime, Clock &freezetime, bool *player_selected, int &x2, int &y2, int &dx2, int &dy2, int &movecount2, int *totalscore_aftr2, int *totalscore_bfr2, int *tilescovered2, bool *score_done2, int *rewardcount2, int *powercount2, bool *bound50_2, bool *bound70_2, bool *bound100_2, bool *bound130_2, float &timer, float &timer2, int &totalscore2,Clock &freezetime2,bool *p1_life,bool *p2_life,bool *start,bool *moving,bool *moving2,bool *scoresadjusted) {
    x = 0; y = 0; dx = 0; dy = 0;
    movecount = 0;
    score = 0;
    totalscore = 0;
    totalscore2 = 0;
    *totalscore_aftr = 0;
    *totalscore_bfr = 0;
    *tilescovered = 0;
    *score_done = false;
    *rewardcount = 0;
    *powerupcount = 0;
    *bound50 = false;
    *bound70 = false;
    *bound100 = false;
    *bound130 = false;
    oldcount = 130;
    Game = true;
    inendmenu = false;
    freeze = false;
    cont_lvl_timer = 0;
    levelselected = false;
    *player_selected = false;
    x2 = N - 1; y2 = 0; dx2 = 0; dy2 = 0;
    movecount2 = 0;
    *totalscore_aftr2 = 0;
    *totalscore_bfr2 = 0;
    *tilescovered2 = 0;
    *score_done2 = false;
    *rewardcount2 = 0;
    *powercount2 = 0;
    *bound50_2 = false;
    *bound70_2 = false;
    *bound100_2 = false;
    *bound130_2 = false;
    timer = 0;
    timer2 = 0;
    *p1_life=true;
    *p2_life=false;
    *start=true;
    *moving=false;
    *moving2=false;
    *scoresadjusted=false;
	
    elapsedtime.restart();
    freezetime.restart();
    freezetime2.restart();
}

void reset_grid() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) grid[i][j] = 1;
            else grid[i][j] = 0;
        }
    }
} 
