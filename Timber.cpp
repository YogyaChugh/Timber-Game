#include<SFML\Graphics.hpp>
#include<sstream>
#include<SFML\Audio.hpp>

using namespace sf;

void updatebranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main() {
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Timber!!", Style::Fullscreen);
	

	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);



	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);



	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	bool Cloud1Active = false;
	bool Cloud2Active = false;
	bool Cloud3Active = false;
	float Cloud1Speed = 0.0f;
	float Cloud2Speed = 0.0f;
	float Cloud3Speed = 0.0f;



	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	bool BeeActive = false;
	float BeeSpeed = 0.0f;



	bool paused = true;

	Clock clock;


	int score = 0;

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	Text messageText;
	Text scoreText;

	messageText.setString("Press Enter to Start!");
	scoreText.setString("Score:- 0");

	messageText.setFont(font);
	scoreText.setFont(font);

	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f
	);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);


	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		branches[i].setOrigin(220, 20);
	}


	RectangleShape timeBar;
	float timeBarMaxWidth = 400;
	float timeBarMaxHeight = 80;
	timeBar.setSize(Vector2f(timeBarMaxWidth, timeBarMaxHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition(1920 / 2 - timeBarMaxWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarMaxWidth / timeRemaining;


	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580,720);

	side playerSide = side::LEFT;

	Texture textureGrave;
	textureGrave.loadFromFile("graphics/rip.png");
	Sprite spriteGrave;
	spriteGrave.setTexture(textureGrave);
	spriteGrave.setPosition(2000,300);

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(2000,0);

	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810,720);

	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	bool acceptInput = false;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer outBuffer;
	outBuffer.loadFromFile("sound/out_of_time.wav");
	Sound out;
	out.setBuffer(outBuffer);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				acceptInput = true;

				spriteAxe.setPosition(2000, 720);
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;

			score = 0;
			timeRemaining = 6.0f;

			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}

			spriteGrave.setPosition(2000,580);

			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}
		if (acceptInput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {

				playerSide = side::RIGHT;
				score++;

				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(
					AXE_POSITION_RIGHT, spriteAxe.getPosition().y
				);
				spritePlayer.setPosition(1200, 720);

				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				updatebranches(score);
				acceptInput = false;

				chop.play();
			}
			if (Keyboard::isKeyPressed(Keyboard::Left)) {

				playerSide = side::LEFT;

				score++;
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(
					AXE_POSITION_LEFT, spriteAxe.getPosition().y
				);

				spritePlayer.setPosition(580, 720);
					
				updatebranches(score);

				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				chop.play();

			}
		}

		Time dt = clock.restart();

		if (!paused) {

			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarMaxHeight));

			if (timeRemaining < 0.0f) {
				paused = true;

				messageText.setString("Out of Time!!");

				FloatRect textobject = messageText.getLocalBounds();

				messageText.setOrigin(
					textobject.left + textobject.width / 2, textobject.top + textobject.height / 2
				);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				out.play();

			}



			if (!BeeActive) {
				srand((int)time(0));
				BeeSpeed = (rand() % 200) + 200;

				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				BeeActive = true;
			}
			else {
				spriteBee.setPosition(
					spriteBee.getPosition().x - (BeeSpeed * dt.asSeconds()), spriteBee.getPosition().y
				);

				if (spriteBee.getPosition().x < -100) {
					BeeActive = false;
				}
			}



			if (!Cloud1Active) {
				srand((int)time(0));
				Cloud1Speed = (rand() % 200);

				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				Cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x + (Cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y
				);

				if (spriteCloud1.getPosition().x > 1920) {
					Cloud1Active = false;
				}
			}

			if (!Cloud2Active) {
				srand((int)time(0) * 20);
				Cloud2Speed = (rand() % 200);

				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				Cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x + (Cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y
				);

				if (spriteCloud2.getPosition().x > 1920) {
					Cloud2Active = false;
				}
			}

			if (!Cloud3Active) {
				srand((int)time(0) * 30);
				Cloud3Speed = (rand() % 200);

				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				Cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x + (Cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y
				);

				if (spriteCloud3.getPosition().x > 1920) {
					Cloud3Active = false;
				}
			}
			std::stringstream ss;
			ss << "Score:- " << score;
			scoreText.setString(ss.str());

			for (int i = 0; i < NUM_BRANCHES; i++) {
				if (branchPositions[i] == side::LEFT) {

					float height = i*150;
					branches[i].setPosition(610, height);
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {

					float height = i * 150;
					branches[i].setPosition(1330, height);
					branches[i].setRotation(0);
				}
				else {
					branches[i].setPosition(3000, 0);
				}
			}

			if (logActive) {
				spriteLog.setPosition(
					spriteLog.getPosition().x + logSpeedX * dt.asSeconds(),
					spriteLog.getPosition().y + logSpeedY * dt.asSeconds()
				);

				if (spriteLog.getPosition().x<-100 || spriteLog.getPosition().y>2000) {
					logActive = false;
					spriteLog.setPosition(810,720);
				}
			}

			if (branchPositions[5] == playerSide) {
				paused = true;
				acceptInput = false;

				spriteGrave.setPosition(525, 760);
				spritePlayer.setPosition(2000, 300);
				spriteAxe.setPosition(2000, 300);
				spriteLog.setPosition(2000, 400);

				messageText.setString("SQUISHED!!");

				FloatRect textobject = messageText.getLocalBounds();

				messageText.setOrigin(
					textobject.left + textobject.width / 2,
					textobject.top + textobject.height / 2
				);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				death.play();
			}

		}
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteLog);
		window.draw(spriteAxe);
		window.draw(spriteGrave);
		window.draw(spriteBee);
		window.draw(timeBar);
		if (paused) {
			window.draw(messageText);
		}
		window.draw(scoreText);
		window.display();
	}
	return 0;
}

void updatebranches(int seed) {
	for (int j = NUM_BRANCHES-1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	srand((int)time(0) + seed);
	int r = rand() % 5;

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}


