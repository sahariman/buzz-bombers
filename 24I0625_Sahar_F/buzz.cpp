
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstring>

using namespace std;
using namespace sf;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.

const int resolutionX = 960;
const int resolutionY = 640;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

//Function prototypes
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, float& spray2_y, float& spray3_y, float& spray2_x, float& spray3_x, Sprite& spray2Sprite, Sprite& spray3Sprite);
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock);
void drawBullet(RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite);
void beeMove(float bee_x[], bool bee_exists[], Clock beeClock[], int currentBee, float finalX, float y, float yPosition, float bee_y[], bool movingRight[], bool movingLeft[], bool leftMovingDown[], bool rightMovingDown[], float currentTier[], Sprite flowerSprite[], bool flower_exists[], int& flowerCount, int& newBee, float flower_x[],Sound& obstacleSound, int& deadBee, float& time, int limit);
bool checkCollision(float bullet_x, float bullet_y, float bee_x[], float bee_y[], float length, int i);
bool honeyCombCollision(float bee_x[], float bee_y[], float honeyComb_x[], float honeyComb_y[], float length, int i, int hc);
bool birdHoneyCombCollision(float bird_x, float bird_y, float honeyComb_x[], float honeyComb_y[], float length, int hc);
bool checkCollisionHoneyComb(float bullet_x, float bullet_y, float honeyComb_x[], float honeyComb_y[], float length, int hc);
bool checkCollisionBird(float bullet_x, float bullet_y, float bird_x, float bird_y, float length);

int main()
{	
	//Local variables for count
	int newBee=0;
	int fastNewBee=0;
	int currentBee;
	int hc=0;
	int fireCount=0;
	int beeKillCount=0;
	int count=0;
	int score=0;
	int flowerCount=0;
	int deadBee=0;
	int newBee_1=14;
	int newBee_2=11;
	int newBee_3=3;
	int newBee_4=16;
	int newBee_5=8;
	float flowerSpace=48; //Calculated by dividing the x-axis in 21 parts
	float yPosition=18;
	float length=32; //For Collisions
	float pauseDuration=0.5f; //Hummingbird pause before changing direction
	float regularBeeTime=40;
	float fastBeeTime=20;
	
	srand(time(0));
	srand(static_cast<unsigned>(time(nullptr))); //For random movement of bird

	// Declaring RenderWindow.
	RenderWindow window(VideoMode(resolutionX, resolutionY), "Buzz Bombers", Style::Close | Style::Titlebar);

	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(Vector2i(200, 150));

	// Initializing Background Music.
	Music bgMusic;
	if (!bgMusic.openFromFile("Music/Music3.ogg")) {
    cout << "Error: Could not load music file!" << endl;
	}
	bgMusic.setVolume(40);
	bgMusic.setLoop(true);
	bgMusic.play();
	
	SoundBuffer fireBuffer;
	if (!fireBuffer.loadFromFile("Sound Effects/fire.wav")) {
    cout << "Error: Could not load music file!" << endl;
	}
	Sound fireSound;
	fireSound.setBuffer(fireBuffer);
	fireSound.setVolume(40);
	
	SoundBuffer hitBuffer;
	if (!hitBuffer.loadFromFile("Sound Effects/hit.wav")) {
    cout << "Error: Could not load music file!" << endl;
	}
	Sound hitSound;
	hitSound.setBuffer(hitBuffer);
	hitSound.setVolume(40);
	
	SoundBuffer obstacleBuffer;
	if (!obstacleBuffer.loadFromFile("Sound Effects/Obstacle.wav")) {
    cout << "Error: Could not load music file!" << endl;
	}
	Sound obstacleSound;
	obstacleSound.setBuffer(obstacleBuffer);
	obstacleSound.setVolume(50);

	// Initial Positions
	float player_x = (gameRows - 17) * boxPixelsX;
	float player_y = (gameColumns - 4) * boxPixelsY;
	float spray2_x = (gameRows - 29.9) * boxPixelsX;
	float spray2_y = (gameColumns- 2) * boxPixelsY;
	float spray3_x = (gameRows - 28.7) * boxPixelsX;
	float spray3_y = (gameColumns - 2) * boxPixelsY;
	float bird_x = (gameRows - 29.7) * boxPixelsX;
	float bird_y = (gameColumns - 6) * boxPixelsY;
	
	float finalX = (gameRows - 2) * boxPixelsX; //Final X co-ordinate for the bees on the right side of the window
	float y = (gameColumns - yPosition) * boxPixelsY; //Initial y co-ordinate
	
					//---Initialization of textures---\\
	//Main Spray Texture
	Texture playerTexture;
	Sprite playerSprite;
	playerTexture.loadFromFile("Textures/spray.png");
	playerSprite.setTexture(playerTexture);
	
	//Two sprays in the left-bottom of the screen
	Sprite spray2Sprite;
	spray2Sprite.setTexture(playerTexture);
	Sprite spray3Sprite;
	spray3Sprite.setTexture(playerTexture);
	
	//Initializing texture of bees
	Texture beeTexture;
	beeTexture.loadFromFile("Textures/Regular_bee.png");
	Sprite beeSprite[6];
	for (int i=0; i<6; i++)
	{
		beeSprite[i].setTexture(beeTexture);
	}
	
	Texture redBeeTexture;
	redBeeTexture.loadFromFile("Textures/Fast_bee.png");
	Sprite fastBee[2];
	for (int i=0; i<2; i++)
	{
		fastBee[i].setTexture(redBeeTexture);
	}
	
	//Initializing texture of honeycombs
	Texture honeycombTexture;
	honeycombTexture.loadFromFile("Textures/honeycomb.png");
	Sprite honeyComb[35]; //Array for 20 honeycombs
	for (int i=0; i<35; i++)
	{
		honeyComb[i].setTexture(honeycombTexture);
	}
	
	Texture honeyComb_2Texture;
	honeyComb_2Texture.loadFromFile("Textures/honeycomb_red.png");
	Sprite honeyComb_2[10];
	for (int i=0; i<10; i++)
	{
		honeyComb_2[i].setTexture(honeyComb_2Texture);
	}
	
	//Initializing texture of hummingbird
	Texture birdTexture;
	birdTexture.loadFromFile("Textures/bird.png");
	Sprite birdSprite;
	birdSprite.setTexture(birdTexture);
	
	//Initializing texture for flower
	Texture flowerTexture;
	flowerTexture.loadFromFile("Textures/obstacles.png");
	Sprite flowerSprite[21];
	for (int i=0; i<21; i++){
		flowerSprite[i].setTexture(flowerTexture);
	}
	
	//Initializing texture for bullet
	Texture bulletTexture;
	Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setScale(3, 3);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY)); 
	
	Texture logoTexture;
	Sprite logoSprite;
	logoTexture.loadFromFile("BUZZ BOMBERS.png");
	logoSprite.setTexture(logoTexture);
	float logo_x = (resolutionX / 2) - (logoTexture.getSize().x / 2); // Center the sprite horizontally
	float logo_y = (resolutionY / 2.9) - (logoTexture.getSize().y / 2); // Center the sprite vertically
	
	Texture chooseLevelTexture;
	Sprite chooseLevelSprite;
	chooseLevelTexture.loadFromFile("level.png");
	chooseLevelSprite.setTexture(chooseLevelTexture);
	float level_x = (resolutionX / 2) - (chooseLevelTexture.getSize().x / 2); // Center the sprite horizontally
	float level_y = (resolutionY / 1.25) - (chooseLevelTexture.getSize().y / 2);
					
					//---Initialization of Positions---\\
	
	//Array for bees' initial x co-ordinates
	float bee_x[6];
	bee_x[0] = (gameRows - 29.7) * boxPixelsX;
	bee_x[1] = 1040;
	bee_x[2] = bee_x[0] - 160;
	bee_x[3] = bee_x[1] + 160;
	bee_x[4] = bee_x[2] - 160;
	bee_x[5] = bee_x[3] + 160;
	
	
	//Array for bees' initial y co-ordinates
	float bee_y[6];
	for (int i=0; i<6; i++){
	bee_y[i] = (gameColumns - yPosition) * boxPixelsY;
	}
	
	float fastBee_x[2];
	fastBee_x[0]=(gameRows - 29.7) * boxPixelsX;
	fastBee_x[1]=1040;
	float fastBee_y[2];
	fastBee_y[0]=(gameColumns - yPosition) * boxPixelsY;
	fastBee_y[1]=(gameColumns - yPosition) * boxPixelsY;
	
	//Flower Co-ordinates
	float flower_x[21];
	float flower_y[21];
	flower_x[0]=1;
	flower_x[1]=960-flowerSpace;
	for (int i=2; i<21; i++){
	if (i%2==0) flower_x[i]=flower_x[i-2]+flowerSpace;
	else flower_x[i]=flower_x[i-2]-flowerSpace;
	}
	for (int i=0; i<21; i++){
	flower_y[i]=550;
	} 
	Vector2f flowerPositions[21];
	
	//Hummingbird
	birdSprite.setPosition(bird_x, bird_y); 
	
	//Honeycomb co-ordinates
	float honeyComb_x[35];
	float honeyComb_y[35];
	honeyComb_x[20]=200;
	honeyComb_x[21]=500;
	honeyComb_x[22]=800;
	honeyComb_y[20]=120;
	honeyComb_y[21]=300;
	honeyComb_y[22]=500;
	Vector2f honeyCombPositions[35];
	honeyCombPositions[20]=Vector2f(honeyComb_x[20], honeyComb_y[20]);
	honeyCombPositions[21]=Vector2f(honeyComb_x[21], honeyComb_y[21]);
	honeyCombPositions[22]=Vector2f(honeyComb_x[22], honeyComb_y[22]);
	
	float honeyComb2_x[15];
	float honeyComb2_y[15];
	Vector2f honeyComb2Positions[15];
	
	//Bullet co-ordinates
	float bullet_x = player_x;
	float bullet_y = player_y;
	
	//For movement of bee
	float currentTier[9]={0, 0, 0, 0, 0, 0, 0, 0, 0};
	float fastCurrentTier[2]={0, 0};
	
					//---Initializing Speeds---\\
	
	//Speed of spray
	float spraySpeed=400.f;
	
	//Speed of bird
	float velocityX = static_cast<float>((rand() % 200 - 100) / 100.0) * 0.2f;
    	float velocityY = static_cast<float>((rand() % 200 - 100) / 100.0) * 0.2f;
    	
    					//---Initializing Existance---\\
	
	//Flowers
	bool flower_exists[21];
	for (int i=0; i<21; i++){
		flower_exists[i]=false;
	}
	
	//Honeycombs
	bool honeyComb_exists[35];
	for (int hc=0; hc<35; hc++){
		if (hc<20)
			honeyComb_exists[hc]=false;
		else if (hc>=20 && hc<=22) 
			honeyComb_exists[hc]=true;
		else honeyComb_exists[hc]=false;
	}
	
	bool honeyComb2_exists[15];
	for (int hc=0; hc<15; hc++){
			honeyComb2_exists[hc]=false;
	}
	
	//Bees
	bool bee_exists[6] = {1, 1, 1, 1, 1, 1};
	
	//Red Bee
	bool fastBee_exists[2] = {1, 1};
	
	//Bird
	bool bird_exists=true;
	
	//Bullet
	bool bullet_exists = false;
	
					//---Initializing Clocks---\\
	
	//Hummingbird
	Clock birdClock;
	Clock birdHitClock;
	Clock birdPauseClock;
	
	//Bees
	Clock beeClock[6];
	
	Clock fastBeeClock[2];
	
	//Bullet
	Clock bulletClock;
	
	//Spray
	Clock sprayClock;
	
	Clock delayClock;
	
						//---Movement---\\
	
	//Bees
	bool movingRight[6]={1, 0, 1, 0, 1, 0};
	bool movingLeft[6]={0, 1, 0, 1, 0, 1};
	bool leftMovingDown[6];
	bool rightMovingDown[6];
	
	//Killer Bees
	bool fastMovingRight[2]={1, 0};
	bool fastMovingLeft[2]={0, 1};
	bool fastLeftMovingDown[2]={0, 0};
	bool fastRightMovingDown[2]={0, 0};
	
						//---Font---//
	Font font;
	font.loadFromFile("SpaceMono-Bold.ttf");
	
	Font font2;
	font2.loadFromFile("PressStart2P-Regular.ttf");
	
	Text sprayCount;
	sprayCount.setFont(font);
	sprayCount.setCharacterSize(24);
	sprayCount.setFillColor(Color(255, 255, 255));
	sprayCount.setPosition(710, 590);
	
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(Color(255, 255, 255));
	scoreText.setPosition(333, 590);
	
	Text endText;
	endText.setFont(font2);
	endText.setCharacterSize(34);
	endText.setFillColor(Color(255, 234, 0));
	endText.setPosition(200, 280);
	
	Text level_1, level_2, level_3;
	level_1.setFont(font);
	level_2.setFont(font);
	level_3.setFont(font);
	level_1.setString("Level 1");
	level_2.setString("Level 2");
	level_3.setString("Level 3");
	level_1.setCharacterSize(30);
	level_2.setCharacterSize(30);
	level_3.setCharacterSize(30);
	level_1.setFillColor(Color(255, 255, 255));
	level_2.setFillColor(Color(255, 255, 255));
	level_3.setFillColor(Color(255, 255, 255));

	Text exitText;
	exitText.setFont(font);
	exitText.setString("Exit Game");
	exitText.setCharacterSize(30);
	exitText.setFillColor(Color(255, 255, 255));
	
	Text levelText;
	levelText.setFont(font);
	levelText.setCharacterSize(30);
	levelText.setFillColor(Color(137, 207, 240));
	levelText.setPosition(413, 12);
	
	Text nameText;
	nameText.setFont(font2);
	nameText.setCharacterSize(24);
	nameText.setPosition(280, 565);
	nameText.setFillColor(Color(255, 234, 0));
	nameText.setString("Enter Name: ");
	
	Text inputText;
	inputText.setFont(font2);
	inputText.setCharacterSize(24);
	inputText.setPosition(540, 565);
	inputText.setFillColor(Color(255, 234, 0));
	
						//---Drawing Ground---//
	RectangleShape groundRectangle(Vector2f(960, 64));
	groundRectangle.setPosition(0, (gameColumns - 2) * boxPixelsY);
	groundRectangle.setFillColor(Color::Green);
	
	int currentState=0;
	char name[100];

	
	
					//---Game Starts (Menu)---\\
	
	while (window.isOpen()) {
   		Event e;
    		while (window.pollEvent(e)) {
        		if (e.type == Event::Closed) {
            			window.close();
            			return 0;
        		}

        		if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
            			Vector2i mousePos = Mouse::getPosition(window);
				if (currentState == 0) {
                if (level_1.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentState = 1;
                } else if (level_2.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentState = 2;
                } else if (level_3.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentState = 3;
                } else if (exitText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) { 		    
                    window.close();
                    return 0;
                } 
            }
        }
    }

    		// Rendering logic
    		window.clear();

    		if (currentState == 0) {
        		logoSprite.setPosition(logo_x, logo_y);
        		chooseLevelSprite.setPosition(level_x, level_y);
        		level_1.setPosition(420, 355);
        		level_2.setPosition(420, 385);
        		level_3.setPosition(420, 415);
        		exitText.setPosition(398, 490);

        		window.draw(logoSprite);
        		window.draw(chooseLevelSprite);
        		window.draw(level_1);
        		window.draw(level_2);
        		window.draw(level_3);
        		window.draw(exitText);
        		window.display();
    		}
	 
					
					
						//---LEVEL 1---//
	
		if (currentState==1) {
		
		levelText.setString("LEVEL 1");
		window.draw(levelText);
		
		//Firing bullet with spacebar
		if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists && fireCount<=168){
			bullet_exists=true;
			fireSound.play();
			//Calculating the number of bullets fired
			fireCount++;
			//To display the number of bullets left
			int spray=56-fireCount;
			if (fireCount==56){
				count++;
				fireCount=0;
				if (count==1) spray3_y=641;
				else if (count==2) spray2_y=641;
			}
			sprayCount.setString(String("Bullets Left: ") + to_string(spray));
			bullet_x = player_x + 16;
			bullet_y = player_y;
			bulletClock.restart();
		}
		//Drawing and movement of bullet
		if (bullet_exists == true)
		{
			moveBullet(bullet_y, bullet_exists, bulletClock);
			drawBullet(window, bullet_x, bullet_y, bulletSprite);
		} 
		//Checking for collision between honeycomb and bullet
		for (int hc=0; hc<23; hc++){
			if (honeyComb_exists[hc] && checkCollisionHoneyComb(bullet_x, bullet_y, honeyComb_x, honeyComb_y, length, hc)){
				honeyComb_exists[hc]=false;
				bullet_exists=false;
			}
		} 
		 
		
		//Loop to check for collision, kill bees and introduce new bees
		for (int i=0; i<6; i++){
			if (bee_exists[i] && checkCollision(bullet_x, bullet_y, bee_x, bee_y, length, i) && hc<20){
				bee_x[i] = beeSprite[i].getPosition().x;
				bee_y[i] = beeSprite[i].getPosition().y;
				bee_exists[i]=false;
				deadBee++;
				score+=100;
				hitSound.play();
				scoreText.setString(String("Score: ") + to_string(score));
				bullet_exists=false;
				//Activating honeycomb if bee dies
				honeyComb_exists[hc]=true;
				honeyComb_x[hc]=bee_x[i];
				honeyComb_y[hc]=bee_y[i];
				honeyCombPositions[hc]=Vector2f(honeyComb_x[hc], honeyComb_y[hc]);
				hc++;
				bee_y[i]= (gameColumns - yPosition) * boxPixelsY;
				newBee++;
				if (newBee>14) break;
				bee_exists[i]=true;
				leftMovingDown[i]=false;
				rightMovingDown[i]=false;
				if (newBee%2==0){ 
					bee_x[i]=1040;
					movingLeft[i]=true;
					movingRight[i]=false;
				}
				else{
					bee_x[i]= (gameRows - 29.7) * boxPixelsX;
					movingLeft[i]=false;
					movingRight[i]=true;
				} 
				break;
			}
		}  
			
		//Checking for collision between bird and honeycomb
		for (int hc=0; hc<23; hc++){
			if (honeyComb_exists[hc] && birdHoneyCombCollision(bird_x, bird_y, honeyComb_x, honeyComb_y, length, hc))			 {			if (honeyComb_y[hc]<=160) score+=1000;
			else if (honeyComb_y[hc]<=352) score+=800;
			else score+=500;
			honeyComb_exists[hc]=false;
			scoreText.setString(String("Score: ") + to_string(score));
			}
		}

		//Hummingbird random movement
		if (newBee >= 4){
			if (bird_exists==true){
				birdSprite.move(velocityX, velocityY);
				if (birdSprite.getPosition().x<=0||
birdSprite.getPosition().x+birdSprite.getGlobalBounds().width >=window.getSize().x){
            				velocityX = -velocityX; // Reverse direction
        			}
        			if (birdSprite.getPosition().y<=0||birdSprite.getPosition().y+birdSprite.getGlobalBounds().height>=window.getSize().y){
            				velocityY = -velocityY; // Reverse direction
        			}
        		bird_x = birdSprite.getPosition().x;
			bird_y = birdSprite.getPosition().y;
				window.draw(birdSprite);
			}
		}  

		//Displaying active honeycombs
		for (int i=0; i<23; i++){
    			if (honeyComb_exists[i]){
        			honeyComb[i].setPosition(honeyCombPositions[i]);
        			window.draw(honeyComb[i]);
    			}
    		}
    		
    		//Displaying pollinated flowers
    		for (int i=0; i<20; i++){
			if (flower_exists[i]){
				flowerPositions[i]=Vector2f(flower_x[i], flower_y[i]);
				flowerSprite[i].setPosition(flowerPositions[i]);
				window.draw(flowerSprite[i]);
			}
		} 
    		
		//Checking for collisions between bees and honeycombs
		for (int i=0; i<6; i++){
			for (int hc=0; hc<23; hc++){
				if (bee_exists[i] && honeyComb_exists[hc] && honeyCombCollision(bee_x, bee_y, honeyComb_x, honeyComb_y, length, i, hc)){
					if (movingRight[i]){
						movingRight[i]=false;
						leftMovingDown[i]=true;
					}
					else if (movingLeft[i]){
						movingLeft[i]=false;
						rightMovingDown[i]=true;
					}
					else if (leftMovingDown[i]){
						leftMovingDown[i]=false;
						movingLeft[i]=true;
					}
					else if (rightMovingDown[i]){
						rightMovingDown[i]=false;
						movingRight[i]=true;
					} 
				}
			}
		} 	
		
		//Loop for initial movement of first six bees
		for (currentBee=0; currentBee<6; currentBee++){
			if (bee_exists[currentBee]==true){
				beeMove(bee_x, bee_exists, beeClock, currentBee, finalX, y, yPosition, bee_y, movingRight, movingLeft, leftMovingDown, rightMovingDown, currentTier, flowerSprite, flower_exists, flowerCount, newBee, flower_x, obstacleSound, deadBee, regularBeeTime, newBee_1);
				beeSprite[currentBee].setPosition(bee_x[currentBee], bee_y[currentBee]);	
				window.draw(beeSprite[currentBee]);
			}
		}

		//For movement of spray
		float deltaTime=sprayClock.restart().asSeconds();
		Vector2f movement(0.0f, 0.0f);
		if (Keyboard::isKeyPressed(Keyboard::Left)){
            		movement.x -= spraySpeed * deltaTime;
        	}
        	if (Keyboard::isKeyPressed(Keyboard::Right)) {
            		movement.x += spraySpeed * deltaTime;
        	}
        	
        	if (deadBee == 20) {
    			endText.setString("LEVEL 1 COMPLETE!\n\n ON TO LEVEL 2...");
    			window.clear();                 // Clear the screen
    			window.draw(endText);           // Draw the text
   			window.display();               // Display the updated content
    			sf::sleep(sf::seconds(3));      // Pause for 3 seconds
    			currentState = 2;               // Proceed to the next level
		}

		
		//Display
		window.draw(groundRectangle);
		drawPlayer(window, player_x, player_y, playerSprite, spray2_y, spray3_y, spray2_x, spray3_x, spray2Sprite, spray3Sprite);
		window.draw(sprayCount);
		window.draw(scoreText);
		playerSprite.move(movement);
        	player_x = playerSprite.getPosition().x;
		player_y = playerSprite.getPosition().y;
		
		//Bound Checking
		player_x>900?player_x=900:player_x;
		player_x<0?player_x=0:player_x;
		
		window.display();
		window.clear();
	}
	
	
	
						//---LEVEL 2---//
	
	
		
	if (currentState==2) {
	
	for (int i=0; i<4; i++){
		bee_exists[i]=true;
	}
	
	movingRight[0]=1;
	movingRight[1]=0;
	movingRight[2]=1;
	movingRight[3]=0;
	movingLeft[0]=0;
	movingLeft[1]=1;
	movingLeft[2]=0;
	movingLeft[3]=1;
	
	for (int i=0; i<4; i++){
		leftMovingDown[i]=0;
		rightMovingDown[i]=0;
	} 
	
	newBee=0;
	fastNewBee=0;
	hc=0;
	int fhc=0;
	fireCount=0;
	count=0;
	flowerCount=0;
	deadBee=0;
		
	spray2_x = (gameRows - 29.9) * boxPixelsX;
	spray2_y = (gameColumns- 2) * boxPixelsY;
	spray3_x = (gameRows - 28.7) * boxPixelsX;
	spray3_y = (gameColumns - 2) * boxPixelsY;
	
	bee_x[0] = (gameRows - 29.7) * boxPixelsX;
	bee_x[1] = 1040;
	bee_x[2] = bee_x[0] - 160;
	bee_x[3] = bee_x[1] + 160;
	
	for (int i=0; i<4; i++){
	bee_y[i] = (gameColumns - yPosition) * boxPixelsY;
	}
	
	birdSprite.setPosition(bird_x, bird_y);
	
	for (int i=0; i<4; i++){
		currentTier[i]=0;
	}
	
	for (int i=0; i<21; i++){
		flower_exists[i]=false;
	}
	for (int hc=0; hc<24; hc++){
		if (hc<15)
			honeyComb_exists[hc]=false;
		else if (hc>=15 && hc<=23) 
			honeyComb_exists[hc]=true;
		else
			honeyComb_exists[hc]=false;
	}

	
	while (currentState==2){	
	
		levelText.setString("LEVEL 2");
		window.draw(levelText);
		
		//Firing bullet with spacebar
		if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists && fireCount<=168){
			bullet_exists=true;
			fireSound.play();
			//Calculating the number of bullets fired
			fireCount++;
			//To display the number of bullets left
			int spray=56-fireCount;
			if (fireCount==56){
				count++;
				fireCount=0;
				if (count==1) spray3_y=641;
				else if (count==2) spray2_y=641;
			}
			sprayCount.setString(String("Bullets Left: ") + to_string(spray));
			bullet_x = player_x + 16;
			bullet_y = player_y;
			bulletClock.restart();
		}
		//Drawing and movement of bullet
		if (bullet_exists == true)
		{
			moveBullet(bullet_y, bullet_exists, bulletClock);
			drawBullet(window, bullet_x, bullet_y, bulletSprite);
		} 
		//Checking for collision between honeycomb and bullet
		for (int hc=0; hc<24; hc++){
			if (honeyComb_exists[hc] && checkCollisionHoneyComb(bullet_x, bullet_y, honeyComb_x, honeyComb_y, length, hc)){
				honeyComb_exists[hc]=false;
				bullet_exists=false;
			}
		} 
		
		for (int hc=0; hc<5; hc++){
			if (honeyComb2_exists[hc] && checkCollisionHoneyComb(bullet_x, bullet_y, honeyComb2_x, honeyComb2_y, length, hc)){
				honeyComb2_exists[hc]=false;
				bullet_exists=false;
			}
		}
		 
		
		//Loop to check for collision, kill bees and introduce new bees
		for (int i=0; i<4; i++){
			if (bee_exists[i] && checkCollision(bullet_x, bullet_y, bee_x, bee_y, length, i) && hc<15){
				bee_x[i] = beeSprite[i].getPosition().x;
				bee_y[i] = beeSprite[i].getPosition().y;
				bee_exists[i]=false;
				deadBee++;
				score+=100;
				hitSound.play();
				scoreText.setString(String("Score: ") + to_string(score));
				bullet_exists=false;
				//Activating honeycomb if bee dies
				honeyComb_exists[hc]=true;
				honeyComb_x[hc]=bee_x[i];
				honeyComb_y[hc]=bee_y[i];
				honeyCombPositions[hc]=Vector2f(honeyComb_x[hc], honeyComb_y[hc]);
				hc++;
				bee_y[i]= (gameColumns - yPosition) * boxPixelsY;
				newBee++;
				if (newBee>11) break;
				bee_exists[i]=true;
				leftMovingDown[i]=false;

				rightMovingDown[i]=false;
				if (newBee%2==0){ 
					bee_x[i]=1040;
					movingLeft[i]=true;
					movingRight[i]=false;
				}
				else{
					bee_x[i]= (gameRows - 29.7) * boxPixelsX;
					movingLeft[i]=false;
					movingRight[i]=true;
				} 
				break;
			}
		}
		
		for (int i=0; i<2; i++){
			if (fastBee_exists[i] && checkCollision(bullet_x, bullet_y, fastBee_x, fastBee_y, length, i) && fhc<5){
				fastBee_x[i] = fastBee[i].getPosition().x;
				fastBee_y[i] = fastBee[i].getPosition().y;
				fastBee_exists[i]=false;
				deadBee++;
				score+=1000;
				hitSound.play();
				scoreText.setString(String("Score: ") + to_string(score));
				bullet_exists=false;
				//Activating honeycomb if bee dies
				honeyComb2_exists[fhc]=true;
				honeyComb2_x[fhc]=fastBee_x[i];
				honeyComb2_y[fhc]=fastBee_y[i];
				honeyComb2Positions[fhc]=Vector2f(honeyComb2_x[fhc], honeyComb2_y[fhc]);
				fhc++;
				fastBee_y[i]= (gameColumns - yPosition) * boxPixelsY;
				fastNewBee++;
				if (fastNewBee>3) break;
				fastBee_exists[i]=true;
				fastLeftMovingDown[i]=false;
				fastRightMovingDown[i]=false;
				if (fastNewBee%2==0){ 
					fastBee_x[i]=1040;
					fastMovingLeft[i]=true;
					movingRight[i]=false;
				}
				else{
					fastBee_x[i]= (gameRows - 29.7) * boxPixelsX;
					fastMovingLeft[i]=false;
					fastMovingRight[i]=true;
				} 
				break;
			}
		}
		
		for (int hc=0; hc<24; hc++){
			if (honeyComb_exists[hc] && birdHoneyCombCollision(bird_x, bird_y, honeyComb_x, honeyComb_y, length, hc))			 {			if (honeyComb_y[hc]<=160) score+=1000;
			else if (honeyComb_y[hc]<=352) score+=800;
			else score+=500;
			honeyComb_exists[hc]=false;
			scoreText.setString(String("Score: ") + to_string(score));
			}
		}
		
		for (int hc=0; hc<5; hc++){
			if (honeyComb2_exists[hc] && birdHoneyCombCollision(bird_x, bird_y, honeyComb2_x, honeyComb2_y, length, hc)){			
			if (honeyComb2_y[hc]<=160) score+=2000;
			else if (honeyComb_y[hc]<=352) score+=1800;
			else score+=1500;
			honeyComb_exists[hc]=false;
			scoreText.setString(String("Score: ") + to_string(score));
			}
		}
		
		if (newBee >= 4){
			if (bird_exists==true){
				birdSprite.move(velocityX, velocityY);
				if (birdSprite.getPosition().x<=0||
birdSprite.getPosition().x+birdSprite.getGlobalBounds().width >=window.getSize().x){
            				velocityX = -velocityX; // Reverse direction
        			}
        			if (birdSprite.getPosition().y<=0||birdSprite.getPosition().y+birdSprite.getGlobalBounds().height>=window.getSize().y){
            				velocityY = -velocityY; // Reverse direction
        			}
        		bird_x = birdSprite.getPosition().x;
			bird_y = birdSprite.getPosition().y;
				window.draw(birdSprite);
			}
		}  

		//Displaying active honeycombs
		for (int i=0; i<24; i++){
    			if (honeyComb_exists[i]){
        			honeyComb[i].setPosition(honeyCombPositions[i]);
        			window.draw(honeyComb[i]);
    			}
    		}
    		
    		for (int i=0; i<5; i++){
    			if (honeyComb2_exists[i]){
        			honeyComb_2[i].setPosition(honeyComb2Positions[i]);
        			window.draw(honeyComb_2[i]);
    			}
    		}
    		
    		for (int i=0; i<20; i++){
			if (flower_exists[i]){
				flowerPositions[i]=Vector2f(flower_x[i], flower_y[i]);
				flowerSprite[i].setPosition(flowerPositions[i]);
				window.draw(flowerSprite[i]);
			}
		} 
    		
		//Checking for collisions between bees and honeycombs
		for (int i=0; i<4; i++){
			for (int hc=0; hc<24; hc++){
				if (bee_exists[i] && honeyComb_exists[hc] && honeyCombCollision(bee_x, bee_y, honeyComb_x, honeyComb_y, length, i, hc)){
					if (movingRight[i]){
						movingRight[i]=false;
						leftMovingDown[i]=true;
					}
					else if (movingLeft[i]){
						movingLeft[i]=false;
						rightMovingDown[i]=true;
					}
					else if (leftMovingDown[i]){
						leftMovingDown[i]=false;
						movingLeft[i]=true;
					}
					else if (rightMovingDown[i]){
						rightMovingDown[i]=false;
						movingRight[i]=true;
					} 
				}
			}
		}
		
		for (int i=0; i<4; i++){
			for (int hc=0; hc<5; hc++){
				if (bee_exists[i] && honeyComb2_exists[hc] && honeyCombCollision(bee_x, bee_y, honeyComb2_x, honeyComb2_y, length, i, hc)){
					if (movingRight[i]){
						movingRight[i]=false;
						leftMovingDown[i]=true;
					}
					else if (movingLeft[i]){
						movingLeft[i]=false;
						rightMovingDown[i]=true;
					}
					else if (leftMovingDown[i]){
						leftMovingDown[i]=false;
						movingLeft[i]=true;
					}
					else if (rightMovingDown[i]){
						rightMovingDown[i]=false;
						movingRight[i]=true;
					} 
				}
			}
		}  	
		
		//Loop for initial movement of first six bees
		for (currentBee=0; currentBee<4; currentBee++){
			if (bee_exists[currentBee]==true){
				beeMove(bee_x, bee_exists, beeClock, currentBee, finalX, y, yPosition, bee_y, movingRight, movingLeft, leftMovingDown, rightMovingDown, currentTier, flowerSprite, flower_exists, flowerCount, newBee, flower_x, obstacleSound, deadBee, regularBeeTime, newBee_2);
				beeSprite[currentBee].setPosition(bee_x[currentBee], bee_y[currentBee]);	
				window.draw(beeSprite[currentBee]);
			}
		}
		
		for (currentBee=0; currentBee<2; currentBee++){
			if (fastBee_exists[currentBee]==true){
				beeMove(fastBee_x, fastBee_exists, fastBeeClock, currentBee, finalX, y, yPosition, fastBee_y, fastMovingRight, fastMovingLeft, fastLeftMovingDown, fastRightMovingDown, fastCurrentTier, flowerSprite, flower_exists, flowerCount, fastNewBee, flower_x, obstacleSound, deadBee, fastBeeTime, newBee_3);
				fastBee[currentBee].setPosition(fastBee_x[currentBee], fastBee_y[currentBee]);	
				window.draw(fastBee[currentBee]);
			}
		}
		
		if (deadBee == 20) {
    			endText.setString("LEVEL 2 COMPLETE!\n\n ON TO LEVEL 3...");
    			window.clear();                 // Clear the screen
    			window.draw(endText);           // Draw the text
   			window.display();               // Display the updated content
    			sf::sleep(sf::seconds(3));      // Pause for 3 seconds
    			currentState = 3;   
    			break;            // Proceed to the next
		}
		
		float deltaTime=sprayClock.restart().asSeconds();
		Vector2f movement(0.0f, 0.0f);
		if (Keyboard::isKeyPressed(Keyboard::Left)){
            		movement.x -= spraySpeed * deltaTime;
        	}
        	if (Keyboard::isKeyPressed(Keyboard::Right)) {
            		movement.x += spraySpeed * deltaTime;
        	}
		
		window.draw(groundRectangle);
		drawPlayer(window, player_x, player_y, playerSprite, spray2_y, spray3_y, spray2_x, spray3_x, spray2Sprite, spray3Sprite);
		window.draw(sprayCount);
		window.draw(scoreText);
		playerSprite.move(movement);
        	player_x = playerSprite.getPosition().x;
		player_y = playerSprite.getPosition().y;
		
		//Bound Checking
		player_x>900?player_x=900:player_x;
		player_x<0?player_x=0:player_x;
		
		window.display();
		window.clear();
	}
	
	} 
	
						//---LEVEL 3---//
	
	
	
	
	if (currentState==3) {
	
	for (int i=0; i<4; i++){
		bee_exists[i]=true;
	}
	
	movingRight[0]=1;
	movingRight[1]=0;
	movingRight[2]=1;
	movingRight[3]=0;
	movingLeft[0]=0;
	movingLeft[1]=1;
	movingLeft[2]=0;
	movingLeft[3]=1;
	
	fastMovingRight[0]=1;
	fastMovingLeft[0]=0;
	fastMovingRight[1]=0;
	fastMovingLeft[1]=1;
	
	for (int i=0; i<2; i++){
		fastLeftMovingDown[i]=0;
		fastRightMovingDown[i]=0;
	}
	
	for (int i=0; i<4; i++){
		leftMovingDown[i]=0;
		rightMovingDown[i]=0;
	} 
	
	newBee=0;
	fastNewBee=0;
	hc=0;
	int fhc=0;
	fireCount=0;
	count=0;
	flowerCount=0;
	deadBee=0;
		
	spray2_x = (gameRows - 29.9) * boxPixelsX;
	spray2_y = (gameColumns- 2) * boxPixelsY;
	spray3_x = (gameRows - 28.7) * boxPixelsX;
	spray3_y = (gameColumns - 2) * boxPixelsY;
	
	bee_x[0] = (gameRows - 29.7) * boxPixelsX;
	bee_x[1] = 1040;
	bee_x[2] = bee_x[0] - 160;
	bee_x[3] = bee_x[1] + 160;
	
	fastBee_x[0]=(gameRows - 29.7) * boxPixelsX;
	fastBee_x[1]=1040;
	
	for (int i=0; i<4; i++){
	bee_y[i] = (gameColumns - yPosition) * boxPixelsY;
	}
	
	fastBee_y[0]=(gameColumns - yPosition) * boxPixelsY;
	fastBee_y[1]=(gameColumns - yPosition) * boxPixelsY;
	
	birdSprite.setPosition(bird_x, bird_y);
	
	for (int i=0; i<4; i++){
		currentTier[i]=0;
	}
	
	fastCurrentTier[0]=0;
	fastCurrentTier[1]=0;
	
	for (int i=0; i<21; i++){
		flower_exists[i]=false;
	}
	for (int hc=0; hc<35; hc++){
		if (hc<15)
			honeyComb_exists[hc]=false;
		else if (hc>=15) 
			honeyComb_exists[hc]=true;
	} 
	
	for (int hc=0; hc<15; hc++){
			honeyComb2_exists[hc]=false;
	}
	
	fastBee_exists[0]=1;
	fastBee_exists[1]=1;	
	
	while (currentState==3){	
	
		levelText.setString("LEVEL 3");
		window.draw(levelText);
		
		//Firing bullet with spacebar
		if (Keyboard::isKeyPressed(Keyboard::Space) && !bullet_exists && fireCount<=168){
			bullet_exists=true;
			fireSound.play();
			//Calculating the number of bullets fired
			fireCount++;
			//To display the number of bullets left
			int spray=56-fireCount;
			if (fireCount==56){
				count++;
				fireCount=0;
				if (count==1) spray3_y=641;
				else if (count==2) spray2_y=641;
			}
			sprayCount.setString(String("Bullets Left: ") + to_string(spray));
			bullet_x = player_x + 16;
			bullet_y = player_y;
			bulletClock.restart();
		}
		//Drawing and movement of bullet
		if (bullet_exists == true)
		{
			moveBullet(bullet_y, bullet_exists, bulletClock);
			drawBullet(window, bullet_x, bullet_y, bulletSprite);
		} 
		//Checking for collision between honeycomb and bullet
		for (int hc=0; hc<35; hc++){
			if (honeyComb_exists[hc] && checkCollisionHoneyComb(bullet_x, bullet_y, honeyComb_x, honeyComb_y, length, hc)){
				honeyComb_exists[hc]=false;
				bullet_exists=false;
			}
		} 
		
		for (int hc=0; hc<10; hc++){
			if (honeyComb2_exists[hc] && checkCollisionHoneyComb(bullet_x, bullet_y, honeyComb2_x, honeyComb2_y, length, hc)){
				honeyComb2_exists[hc]=false;
				bullet_exists=false;
			}
		}
		 
		
		//Loop to check for collision, kill bees and introduce new bees
		for (int i=0; i<4; i++){
			if (bee_exists[i] && checkCollision(bullet_x, bullet_y, bee_x, bee_y, length, i) && hc<20){
				bee_x[i] = beeSprite[i].getPosition().x;
				bee_y[i] = beeSprite[i].getPosition().y;
				bee_exists[i]=false;
				deadBee++;
				score+=100;
				hitSound.play();
				scoreText.setString(String("Score: ") + to_string(score));
				bullet_exists=false;
				//Activating honeycomb if bee dies
				honeyComb_exists[hc]=true;
				honeyComb_x[hc]=bee_x[i];
				honeyComb_y[hc]=bee_y[i];
				honeyCombPositions[hc]=Vector2f(honeyComb_x[hc], honeyComb_y[hc]);
				hc++;
				bee_y[i]= (gameColumns - yPosition) * boxPixelsY;
				newBee++;
				if (newBee>16) break;
				bee_exists[i]=true;
				leftMovingDown[i]=false;

				rightMovingDown[i]=false;
				if (newBee%2==0){ 
					bee_x[i]=1040;
					movingLeft[i]=true;
					movingRight[i]=false;
				}
				else{
					bee_x[i]= (gameRows - 29.7) * boxPixelsX;
					movingLeft[i]=false;
					movingRight[i]=true;
				} 
				break;
			}
		}
		
		for (int i=0; i<2; i++){
			if (fastBee_exists[i] && checkCollision(bullet_x, bullet_y, fastBee_x, fastBee_y, length, i) && fhc<10){
				fastBee_x[i] = fastBee[i].getPosition().x;
				fastBee_y[i] = fastBee[i].getPosition().y;
				fastBee_exists[i]=false;
				deadBee++;
				score+=1000;
				hitSound.play();
				scoreText.setString(String("Score: ") + to_string(score));
				bullet_exists=false;
				//Activating honeycomb if bee dies
				honeyComb2_exists[fhc]=true;
				honeyComb2_x[fhc]=fastBee_x[i];
				honeyComb2_y[fhc]=fastBee_y[i];
				honeyComb2Positions[fhc]=Vector2f(honeyComb2_x[fhc], honeyComb2_y[fhc]);
				fhc++;
				fastBee_y[i]= (gameColumns - yPosition) * boxPixelsY;
				fastNewBee++;
				if (fastNewBee>8) break;
				fastBee_exists[i]=true;
				fastLeftMovingDown[i]=false;
				fastRightMovingDown[i]=false;
				if (fastNewBee%2==0){ 
					fastBee_x[i]=1040;
					fastMovingLeft[i]=true;
					movingRight[i]=false;
				}
				else{
					fastBee_x[i]= (gameRows - 29.7) * boxPixelsX;
					fastMovingLeft[i]=false;
					fastMovingRight[i]=true;
				} 
				break;
			}
		}
		
		for (int hc=0; hc<35; hc++){
			if (honeyComb_exists[hc] && birdHoneyCombCollision(bird_x, bird_y, honeyComb_x, honeyComb_y, length, hc))			 {			if (honeyComb_y[hc]<=160) score+=1000;
			else if (honeyComb_y[hc]<=352) score+=800;
			else score+=500;
			honeyComb_exists[hc]=false;
			scoreText.setString(String("Score: ") + to_string(score));
			}
		}
		
		for (int hc=0; hc<10; hc++){
			if (honeyComb2_exists[hc] && birdHoneyCombCollision(bird_x, bird_y, honeyComb2_x, honeyComb2_y, length, hc)){			
			if (honeyComb2_y[hc]<=160) score+=2000;
			else if (honeyComb_y[hc]<=352) score+=1800;
			else score+=1500;
			honeyComb_exists[hc]=false;
			scoreText.setString(String("Score: ") + to_string(score));
			}
		}
		
		if (newBee >= 4){
			if (bird_exists==true){
				birdSprite.move(velocityX, velocityY);
				if (birdSprite.getPosition().x<=0||
birdSprite.getPosition().x+birdSprite.getGlobalBounds().width >=window.getSize().x){
            				velocityX = -velocityX; // Reverse direction
        			}
        			if (birdSprite.getPosition().y<=0||birdSprite.getPosition().y+birdSprite.getGlobalBounds().height>=window.getSize().y){
            				velocityY = -velocityY; // Reverse direction
        			}
        		bird_x = birdSprite.getPosition().x;
			bird_y = birdSprite.getPosition().y;
				window.draw(birdSprite);
			}
		}  

		//Displaying active honeycombs
		for (int i=0; i<35; i++){
    			if (honeyComb_exists[i]){
        			honeyComb[i].setPosition(honeyCombPositions[i]);
        			window.draw(honeyComb[i]);
    			}
    		}
    		
    		for (int i=0; i<10; i++){
    			if (honeyComb2_exists[i]){
        			honeyComb_2[i].setPosition(honeyComb2Positions[i]);
        			window.draw(honeyComb_2[i]);
    			}
    		}
    		
    		for (int i=0; i<20; i++){
			if (flower_exists[i]){
				flowerPositions[i]=Vector2f(flower_x[i], flower_y[i]);
				flowerSprite[i].setPosition(flowerPositions[i]);
				window.draw(flowerSprite[i]);
			}
		} 
    		
		//Checking for collisions between bees and honeycombs
		for (int i=0; i<4; i++){
			for (int hc=0; hc<35; hc++){
				if (bee_exists[i] && honeyComb_exists[hc] && honeyCombCollision(bee_x, bee_y, honeyComb_x, honeyComb_y, length, i, hc)){
					if (movingRight[i]){
						movingRight[i]=false;
						leftMovingDown[i]=true;
					}
					else if (movingLeft[i]){
						movingLeft[i]=false;
						rightMovingDown[i]=true;
					}
					else if (leftMovingDown[i]){
						leftMovingDown[i]=false;
						movingLeft[i]=true;
					}
					else if (rightMovingDown[i]){
						rightMovingDown[i]=false;
						movingRight[i]=true;
					} 
				}
			}
		}
		
		for (int i=0; i<4; i++){
			for (int hc=0; hc<10; hc++){
				if (bee_exists[i] && honeyComb2_exists[hc] && honeyCombCollision(bee_x, bee_y, honeyComb2_x, honeyComb2_y, length, i, hc)){
					if (movingRight[i]){
						movingRight[i]=false;
						leftMovingDown[i]=true;
					}
					else if (movingLeft[i]){
						movingLeft[i]=false;
						rightMovingDown[i]=true;
					}
					else if (leftMovingDown[i]){
						leftMovingDown[i]=false;
						movingLeft[i]=true;
					}
					else if (rightMovingDown[i]){
						rightMovingDown[i]=false;
						movingRight[i]=true;
					} 
				}
			}
		}  	
		
		//Loop for initial movement of first six bees
		for (currentBee=0; currentBee<4; currentBee++){
			if (bee_exists[currentBee]==true){
				beeMove(bee_x, bee_exists, beeClock, currentBee, finalX, y, yPosition, bee_y, movingRight, movingLeft, leftMovingDown, rightMovingDown, currentTier, flowerSprite, flower_exists, flowerCount, newBee, flower_x, obstacleSound, deadBee, regularBeeTime, newBee_4);
				beeSprite[currentBee].setPosition(bee_x[currentBee], bee_y[currentBee]);	
				window.draw(beeSprite[currentBee]);
			}
		}
		
		for (currentBee=0; currentBee<2; currentBee++){
			if (fastBee_exists[currentBee]==true){
				beeMove(fastBee_x, fastBee_exists, fastBeeClock, currentBee, finalX, y, yPosition, fastBee_y, fastMovingRight, fastMovingLeft, fastLeftMovingDown, fastRightMovingDown, fastCurrentTier, flowerSprite, flower_exists, flowerCount, fastNewBee, flower_x, obstacleSound, deadBee, fastBeeTime, newBee_5);
				fastBee[currentBee].setPosition(fastBee_x[currentBee], fastBee_y[currentBee]);	
				window.draw(fastBee[currentBee]);
			}
		}
		
		if (deadBee == 30) {
    			endText.setString("LEVEL 3 COMPLETE!\n\nSCORE: " + to_string(score));;
    			window.clear();                 // Clear the screen
    			window.draw(endText);           // Draw the text
   			window.display();               // Display the updated content
    			sf::sleep(sf::seconds(3));      // Pause for 3 seconds
    			currentState = 0;
    			break;               // Proceed to the next
		}
		
		float deltaTime=sprayClock.restart().asSeconds();
		Vector2f movement(0.0f, 0.0f);
		if (Keyboard::isKeyPressed(Keyboard::Left)){
            		movement.x -= spraySpeed * deltaTime;
        	}
        	if (Keyboard::isKeyPressed(Keyboard::Right)) {
            		movement.x += spraySpeed * deltaTime;
        	}
		
		window.draw(groundRectangle);
		drawPlayer(window, player_x, player_y, playerSprite, spray2_y, spray3_y, spray2_x, spray3_x, spray2Sprite, spray3Sprite);
		window.draw(sprayCount);
		window.draw(scoreText);
		playerSprite.move(movement);
        	player_x = playerSprite.getPosition().x;
		player_y = playerSprite.getPosition().y;
		
		//Bound Checking
		player_x>900?player_x=900:player_x;
		player_x<0?player_x=0:player_x;
		
		window.display();
		window.clear();
	}
	
	}
	
}}  

//end of main



					//---Function Declaration---//




//Movement of bees
void beeMove(float bee_x[], bool bee_exists[], Clock beeClock[], int currentBee, float finalX, float y, float yPosition, float bee_y[], bool movingRight[], bool movingLeft[], bool leftMovingDown[], bool rightMovingDown[], float currentTier[], Sprite flowerSprite[], bool flower_exists[], int& flowerCount, int& newBee, float flower_x[], Sound& obstacleSound, int& deadBee, float& time, int limit) {
	
	int i=currentBee;
	int fc=0;
    	
    	if (beeClock[i].getElapsedTime().asMilliseconds() < time)
        	return;

    	beeClock[i].restart();
	
	//Rightward movement for bees coming in from the left
    	if (movingRight[i]){
        	bee_x[i] += 10;
        	if (bee_x[i] >= finalX){
            		movingRight[i] = false;
            		leftMovingDown[i] = true;
        	}
    	}
    	//Leftward movement for bees coming in from the right 
    	else if (movingLeft[i]){
        	bee_x[i] -= 10;
        	if (bee_x[i] <= 0){
            		movingLeft[i] = false;
            		rightMovingDown[i] = true;
        	}
    	} 
    	
    	//Downward Movement
    	else if (leftMovingDown[i]||rightMovingDown[i]){
     		bee_y[i] += 10;
      		if (bee_y[i] >= currentTier[i] + 64) {
      			currentTier[i]+=64;
        		if (leftMovingDown[i]){
            			leftMovingDown[i] = false;
            			movingLeft[i] = true; 
        		}
			else if (rightMovingDown[i]){
				rightMovingDown[i] = false;
            			movingRight[i] = true;
			}
         	}
	}
	
	if (currentTier[i] >= 512){
		newBee++;
		deadBee++;
		if (flowerCount==0){
            		flower_exists[flowerCount] = true;
            		flower_exists[flowerCount+1] = true;
            		flowerCount+=2;
            		obstacleSound.play();
            	}	
            	else{
            		if (bee_x[i]==(gameRows/2)*boxPixelsX) flower_x[flowerCount]=(gameRows/2)*boxPixelsX;
            		flower_exists[flowerCount] = true;
            		flowerCount++;
            		obstacleSound.play();
            	} 
            	bee_exists[i] = false;
            	currentTier[i] = 0;
		if (newBee<=limit){
            		leftMovingDown[i]=false;
            		rightMovingDown[i]=false;
            		if (newBee%2==0){
            			bee_x[i] = 1040;
            			movingLeft[i]=true;
            			movingRight[i]=false;
            		}
            		else{
            			bee_x[i]=(gameRows - 29.7) * boxPixelsX;
            			movingLeft[i]=false;
            			movingRight[i]=true;
            		} 
            		bee_y[i] = (gameColumns - yPosition) * boxPixelsY;
            		bee_exists[i] = true;
            	} 
        } 
}

//Checking for collision of bee and spray
bool checkCollision(float bullet_x, float bullet_y, float bee_x[], float bee_y[], float length, int i){
	return !(bullet_x + length <= bee_x[i] || // Bullet is to the left of the bee
             	 bullet_x >= bee_x[i] + length || // Bullet is to the right of the bee
             	 bullet_y + length <= bee_y[i] || // Bullet is above the bee
             	 bullet_y >= bee_y[i] + length);
}

//Checking for collision of bullet and honeycomb
bool checkCollisionHoneyComb(float bullet_x, float bullet_y, float honeyComb_x[], float honeyComb_y[], float length, int hc){
	return !(bullet_x + length <= honeyComb_x[hc] || // Bullet is to the left of the honeycomb
             	 bullet_x >= honeyComb_x[hc] + length || // Bullet is to the right of the honeycomb
             	 bullet_y + length <= honeyComb_y[hc] || // Bullet is above the bee
             	 bullet_y >= honeyComb_y[hc] + length);
}

//Checking for collision of bee and honeycomb
bool honeyCombCollision(float bee_x[], float bee_y[], float honeyComb_x[], float honeyComb_y[], float length, int i, int hc){
	return !(bee_x[i] + length <= honeyComb_x[hc] || //Bee is to the left of honeycomb
		 bee_x[i] >= honeyComb_x[hc] + length || //Bee is the the right of honeycomb
		 bee_y[i] + length <= honeyComb_y[hc] || //Bee is above the honeycomb
		 bee_y[i] >= honeyComb_y[hc] + length);
} 

//Checking for collision of bird and honeycomb
bool birdHoneyCombCollision(float bird_x, float bird_y, float honeyComb_x[], float honeyComb_y[], float length, int hc){
	return !(bird_x + length <= honeyComb_x[hc] || //Bird is to the left of the honeycomb
		 bird_x >= honeyComb_x[hc] + length || //Bird is to the right of the honeycomb
		 bird_y + length <= honeyComb_y[hc] || //Bird is above the honeycomb
		 bird_y >= honeyComb_y[hc] + length);
} 

//Displaying the three sprays
void drawPlayer(RenderWindow& window, float& player_x, float& player_y, Sprite& playerSprite, float& spray2_y, float& spray3_y, float& spray2_x, float& spray3_x, Sprite& spray2Sprite, Sprite& spray3Sprite) {
	playerSprite.setPosition(player_x, player_y);
	spray2Sprite.setPosition(spray2_x, spray2_y);
	spray3Sprite.setPosition(spray3_x, spray3_y);
	window.draw(playerSprite);
	window.draw(spray2Sprite);
	window.draw(spray3Sprite);	
}

//Movement of bullet
void moveBullet(float& bullet_y, bool& bullet_exists, Clock& bulletClock) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 10)
		return;

	bulletClock.restart();
	bullet_y -= 10;
	if (bullet_y < -32)
		bullet_exists = false;
}

//Displaying bullet
void drawBullet(sf::RenderWindow& window, float& bullet_x, float& bullet_y, Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet_x, bullet_y);
	window.draw(bulletSprite);
}

