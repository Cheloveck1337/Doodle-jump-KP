#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <fstream>
// Sprite - �������� ������������� �������� � ������������ ����������������, ������ � �. �. (������� ��� ��������)
// Texture - ����� ����������� �� ����������, ������� ����� ������������ ��� ���������.
// IntRect - ��������������� ����� ��� ���������� ���������� ����������������, ������������ �� ����.
class Game {
private:

	//�����
	sf::Sprite Player;
	sf::Texture PlayerModel_R;
	sf::Texture PlayerModel_L;
	sf::IntRect PlayerRect;

	//���������
	std::vector<sf::Sprite*> PlatVector;
	sf::Texture T_Plat;
	sf::IntRect PlatRect;

	//��������� ���������
	std::vector<sf::Sprite*> BrokenPlatVector;
	std::vector<sf::IntRect*> BrokenPlatRectVector;
	std::vector<bool> CheckBrokenPlatVector;

	//������� ������
	std::vector<sf::Sprite*> DoublePlatVector;
	std::vector<sf::IntRect*> DoublePlatRectVector;

	//�����
	std::vector<sf::Sprite*> BoomPlatVector;
	std::vector<sf::IntRect*> BoomPlatRectVector;

	//��� ����
	sf::Sprite MenuBackground;
	sf::Texture T_MenuBackground;

	//��� ����
	std::vector<sf::Sprite*> BackgroundVector;
	sf::Texture T_Background;
	sf::IntRect BackgroundRect;

	//��� ���� �����
	sf::Sprite PauseBackground;
	sf::Texture T_PauseBackground;

	//��� ���������
	sf::Sprite GOBackground;
	sf::Texture T_GOBackground;

	//����
	sf::Text Score;
	sf::Text ScoreGameOver;
	sf::Font font;

	//����� ����� �������
	sf::Clock clockForPlats, clockForBackground, clockForPause, clockForGameOver, clockForMenu, clockForBrokenPlats, clockForBrokenPlats2, clockForBrokenPlatsAnimation, clockForDoublePlat, clockForBoomPlat;

	sf::View View, CurrentView;

	//���� ������
	sf::SoundBuffer jumpBuffer;
	sf::Sound jumpSound;

	//���� ���������
	sf::SoundBuffer lostBuffer;
	sf::Sound lostSound;

	//���� �������
	sf::SoundBuffer breakBuffer; 
	sf::Sound breakSound; 

	//���� �������� ������
	sf::SoundBuffer djumpBuffer;
	sf::Sound djumpSound;

	//���� boom
	sf::SoundBuffer boomBuffer;
	sf::Sound boomSound;


	//���������� ��� ��������
	float yUp, yDown, xSpeed;
	bool isJumping, isFalling, isLost, isPause, isMenu, isMoving, canMove, looksLeft, looksRight, soundOn, playJumpSound;
	int BetweenPlat, scoreInt, highscoreInt;
	int pixelLength = 540, pixelHeight = 970;

public:
	//������ ��������� ��������� ��� ����
	Game(sf::RenderWindow& win) : yDown{ 0 }, yUp{ 15 }, xSpeed{ 7 }, scoreInt{ 0 }, BetweenPlat{ 60 }, isFalling{ false }, playJumpSound{ true }, soundOn{ true }, canMove{ true }, isJumping{ true }, isLost{ false }, isMenu{ true }, isMoving{ false }, isPause{ false }, looksRight{ true }, looksLeft{ false } {

		//��������� �������� ��������� � ������ ������� ������� ��� ��������� � ������� ��������� �� ������� ����
		PlayerModel_R.loadFromFile("images/gjR.png");
		PlayerModel_L.loadFromFile("images/gjL.png");


		PlayerRect.left = 0;
		PlayerRect.top = 0;
		PlayerRect.width = 46;
		PlayerRect.height = 44;

		Player.setTexture(PlayerModel_R);
		Player.setTextureRect(PlayerRect);
		Player.setPosition(pixelLength / 2, 0);
		Player.setScale(0.8, 0.8);

		//����� ������� ��� ����
		CurrentView = win.getView();

		//��������� ��� � ������ ��������� ������� ���� �� ������� ����
		T_Background.loadFromFile("images/Background.png");
		BackgroundRect.left = 0;
		BackgroundRect.top = 0;
		BackgroundRect.width = pixelLength;
		BackgroundRect.height = pixelHeight;

		//��������� ��������� � ������ ��������� ������� ��������� �� ������� ����
		T_Plat.loadFromFile("images/Plats.png");
		PlatRect.left = 0;
		PlatRect.top = 0;
		PlatRect.width = 59;
		PlatRect.height = 17;

		//������ ��������� ������ ���� � ��� ������
		View.setCenter(pixelLength / 2, pixelHeight / 2);
		View.setSize(pixelLength, pixelHeight);

		//��������� ����� � ������ ������� ������ � ��������� �����
		font.loadFromFile("fonts/al-seana.ttf");
		Score.setString(std::to_string(scoreInt));
		Score.setFont(font);
		Score.setPosition(30, 300);
		Score.setCharacterSize(40);
		Score.setFillColor(sf::Color::Black);

		//������ ��� ��������� ����� ����� ���������� ����, ����� �� ������ �-�����
		ScoreGameOver.setString(std::to_string(scoreInt));
		ScoreGameOver.setFont(font);
		ScoreGameOver.setPosition(400, 50);
		ScoreGameOver.setCharacterSize(40);
		ScoreGameOver.setFillColor(sf::Color::Black);

		//��������� ��� ��� ����� � ������ ��� ������� � ����
		T_PauseBackground.loadFromFile("images/PauseBackground.png");
		PauseBackground.setTexture(T_PauseBackground);
		PauseBackground.setPosition(0, 0);

		//��������� ��� ����
		T_MenuBackground.loadFromFile("images/MenuBackground.png");
		MenuBackground.setTexture(T_MenuBackground);
		MenuBackground.setPosition(0, 0);

		//��������� ��� ������ ���������
		T_GOBackground.loadFromFile("images/GOBackground.png");
		GOBackground.setTexture(T_GOBackground);
		GOBackground.setPosition(0, 0);

		//��������� ���� ��� ���������
		lostBuffer.loadFromFile("sounds/lost.wav");
		lostSound.setBuffer(lostBuffer);
		lostSound.setVolume(10);

		//��������� ���� ��� ������
		jumpBuffer.loadFromFile("sounds/jump.wav");
		jumpSound.setBuffer(jumpBuffer);
		jumpSound.setVolume(10);

		//��������� ���� ��� ������
		breakBuffer.loadFromFile("sounds/break.wav");
		breakSound.setBuffer(breakBuffer);
		breakSound.setVolume(10);

		//��������� ���� ��� �������� ������
		djumpBuffer.loadFromFile("sounds/djump.wav");
		djumpSound.setBuffer(djumpBuffer);
		djumpSound.setVolume(10);

		//��������� ���� boom
		boomBuffer.loadFromFile("sounds/boom2.wav");
		boomSound.setBuffer(boomBuffer);
		boomSound.setVolume(10);
	}
	//������� ������� ��� ���������� �� private ����� � ��� ����� ���� ���������� � main

	sf::Sprite player() const { return Player; }
	sf::Sprite pauseBackground() const { return PauseBackground; }
	sf::Sprite menuBackground() const { return MenuBackground; }
	sf::Sprite goBackground() const { return GOBackground; }

	std::vector<sf::Sprite*> backgroundVector() const { return BackgroundVector; }

	std::vector<sf::Sprite*> platVector() const { return PlatVector; }	
	std::vector<sf::Sprite*> getBrokenPlatVector() const { return BrokenPlatVector; }
	std::vector<sf::Sprite*> getDoublePlatVector() const { return DoublePlatVector; }
	std::vector<sf::Sprite*> getBoomPlatVector() const { return BoomPlatVector; }

	sf::View getView() const { return View; }
	sf::Text getscore() const { return Score; }
	sf::Text getScoreGameOver() const { return ScoreGameOver; }

	bool getIsPause() const { return isPause; }
	bool getIsMenu() const { return isMenu; }
	bool getIsLost() const { return isLost; }

	~Game();

	//������� ������� ������� �������� 

	//���������
	void GetPlatBackground();
	void NewPlats();
	void DelPlats();
	void DrawPlats(sf::RenderWindow&);

	//��������� ���������
	void BrokenPlat();
	void BrokenPlatCollision();

	//������� ������
	void Doublejump(); 

	//�����
	void BoomJump(); 

	//���������� ����������� � ����
	void UpdateView(sf::RenderWindow&); 

	//��������
	void Del();

	//���������� �����
	void newScore();

	// ���������� ����������
	void UpdatePar();

	//����
	void Menu();

	//�����
	void Pause();

	//������
	void Phisics();

	//�������� �� ��������
	void IfLost();

	// �������
	void Borders();

	// ���
	void addBackground();
	void delBackground();

	//�������� 
	void Jump();
	void Move();

	//��������
	void GameOver(sf::RenderWindow&);

};

//������ ���������� ����������� ������, ���������� ��� ������ �� ���� ��������� �������� PlatsVector, BackgroundVector � ��.
Game::~Game() {
	Del();
}

//�������� �������� �� �������� ��� �������� � ���� ��� ������������ ������
void Game::Del() {
	for (int i = 0; i < BackgroundVector.size(); i++) {
		delete BackgroundVector[i];
	}
	for (int i = 0; i < PlatVector.size(); i++) {
		delete PlatVector[i];
	}
	for (int i = 0; i < BrokenPlatRectVector.size(); i++) {
		delete BrokenPlatRectVector[i];
	}
	for (int i = 0; i < BrokenPlatVector.size(); i++) {
		delete BrokenPlatVector[i];
	}
	for (int i = 0; i < DoublePlatRectVector.size(); i++) {
		delete DoublePlatRectVector[i];
	}
	for (int i = 0; i < DoublePlatVector.size(); i++) {
		delete DoublePlatVector[i];
	}
	for (int i = 0; i < BoomPlatRectVector.size(); i++) {
		delete BoomPlatRectVector[i];
	}
	for (int i = 0; i < BoomPlatVector.size(); i++) {
		delete BoomPlatVector[i];
	}
}

//������������ ��������� ����� �� ������ � ����������� �� ���� ��������� �����
void Game::UpdatePar() {
	PauseBackground.setPosition(CurrentView.getCenter().x - (pixelLength / 2), CurrentView.getCenter().y - (pixelHeight / 2));
	MenuBackground.setPosition(CurrentView.getCenter().x - (pixelLength / 2), CurrentView.getCenter().y - (pixelHeight / 2));
	GOBackground.setPosition(CurrentView.getCenter().x - (pixelLength / 2), CurrentView.getCenter().y - (pixelHeight / 2));

	Score.setPosition(10, CurrentView.getCenter().y - (pixelHeight / 2));
	ScoreGameOver.setPosition(CurrentView.getCenter().x + 90, CurrentView.getCenter().y - 18);
}

//���� ������� ������ ����� ���� ������� ����� ������ ��������� � ��� + 200 �������� (�� ���� ���� ��������� ��������� �� 200 ��������) � ����� ���� ��� �� ����, �� �������� ������ ��������� � ������ ���� �������
void Game::IfLost() {
	if (Player.getPosition().y > PlatVector[0]->getPosition().y + 200 && !isLost) {
		isLost = true;
		if (soundOn)
			lostSound.play();
	}
}

//���� ����� �� ����, �� ����� ������ ��������������� �� ������� ������ �� ��� X, � �� ��� Y ������ �� ����������.
void Game::UpdateView(sf::RenderWindow& win) {
	if (!isLost)
		View.setCenter(pixelLength / 2, Player.getPosition().y);
	CurrentView = win.getView();
}

//�������� ��������� ��������� ��� ������� ����. ��������� � ���
void Game::GetPlatBackground() {
	//��������� ������ ����������� ��������� ���� � ����. ��� �������� �������� ��������� �� ������, ��� �������� ����, � ����� ������ ������ ������� �� ������� ������, �� ������������
	//� ����� ������� � ���������� ����� �� ������ ������.
	//��������� ������ ������� � ������� (0,500), ���� ������� �� ������ �� �� 500 �������� ���� �����������

	for (int i = 0; i < 4; i++) {
		sf::Sprite* newBackground = new sf::Sprite;
		newBackground->setTexture(T_Background);
		newBackground->setTextureRect(BackgroundRect);
		if (i == 0) {
			newBackground->setPosition(0, 500);
		}
		else {
			newBackground->setPosition(0, BackgroundVector[i - 1]->getPosition().y - 500);
		}

		BackgroundVector.push_back(newBackground); //����������� �������� � ����� �������
	}
	//�������� �������� ��������� � �� ������������ �� ������ (��������, �� � �������� ������� ����). 
	//���� ��� �� ������ �������� �����, �� ������� ��������� ����������� �� ������ ������� ���������� ��������� � ������ ����� ����. �������� ������ 15 ��������� ��������
	for (int i = 0; i < 15; i++) {
		float PlatPositionX = rand() % (pixelLength - PlatRect.width);

		sf::Sprite* newPlat = new sf::Sprite;
		newPlat->setTexture(T_Plat);
		newPlat->setTextureRect(PlatRect);

		if (i == 0) {
			newPlat->setPosition(PlatPositionX, 0);
		}
		else {

			newPlat->setPosition(PlatPositionX, PlatVector[i - 1]->getPosition().y - BetweenPlat);
		}

		PlatVector.push_back(newPlat);
	}
}

//������� ������� ����� ���������. ���������� ��������� ������� �� �, � � ����� (BetweenPlat) �� ����� ������� ��������� � �������� �����. 
//����� ����������� ������ � ������������ ���� ����� ��������
void Game::NewPlats() {
	if (!isLost) {
		float PlatPositionX = rand() % (pixelLength - PlatRect.width);

		sf::Sprite* newPlat = new sf::Sprite;
		newPlat->setTexture(T_Plat);
		newPlat->setTextureRect(PlatRect);
		newPlat->setPosition(PlatPositionX, PlatVector[PlatVector.size() - 1]->getPosition().y - BetweenPlat);

		PlatVector.push_back(newPlat);

	}
	//�������� ���������� ��������
	if (!isLost && clockForBrokenPlats.getElapsedTime().asSeconds() > 3) {
		float PlatPositionX = rand() % (pixelLength - PlatRect.width);
		int PlatPositionY = rand() % 200 + 970;

		sf::Sprite* newBrokenPlat = new sf::Sprite;
		sf::IntRect* newBrokenPlatRect = new sf::IntRect;

		newBrokenPlatRect->left = 0;
		newBrokenPlatRect->width = 61;
		newBrokenPlatRect->height = 16;
		newBrokenPlatRect->top = 72;

		newBrokenPlat->setTexture(T_Plat);
		newBrokenPlat->setTextureRect(*newBrokenPlatRect);
		newBrokenPlat->setPosition(PlatPositionX, Player.getPosition().y - PlatPositionY);

		for (int i = 0; i < PlatVector.size(); i++) {
			if (PlatVector[i]->getGlobalBounds().intersects(newBrokenPlat->getGlobalBounds())) { //.intersects - �������� �� ����������� ������ ���� ��������
				delete newBrokenPlat;
				delete newBrokenPlatRect;
				return;
			}
		}
		for (int i = 0; i < DoublePlatVector.size(); i++) {
			if (DoublePlatVector[i]->getGlobalBounds().intersects(newBrokenPlat->getGlobalBounds())) {
				delete newBrokenPlat;
				delete newBrokenPlatRect;
				return;
			}
		}
		for (int i = 0; i < BoomPlatVector.size(); i++) {
			if (BoomPlatVector[i]->getGlobalBounds().intersects(newBrokenPlat->getGlobalBounds())) {
				delete newBrokenPlat;
				delete newBrokenPlatRect;
				return;
			}
		}

		BrokenPlatVector.push_back(newBrokenPlat);
		BrokenPlatRectVector.push_back(newBrokenPlatRect);
		CheckBrokenPlatVector.push_back(false);
		clockForBrokenPlats.restart();
	}
	//�������� �������� � ������� �������
	if (!isLost && clockForDoublePlat.getElapsedTime().asSeconds() > 5) {
		float PlatPositionX = rand() % (pixelLength - PlatRect.width);
		int PlatPositionY = rand() % 200 + 970;
		
		sf::Sprite* newDoublePlat = new sf::Sprite;
		sf::IntRect* newDoublePlatRect = new sf::IntRect;

		newDoublePlatRect->left = 0;
		newDoublePlatRect->width = 61;
		newDoublePlatRect->height = 16;
		newDoublePlatRect->top = 19;

		newDoublePlat->setTexture(T_Plat);
		newDoublePlat->setTextureRect(*newDoublePlatRect);
		newDoublePlat->setPosition(PlatPositionX, Player.getPosition().y - PlatPositionY);

		for (int i = 0; i < PlatVector.size(); i++) {
			if (PlatVector[i]->getGlobalBounds().intersects(newDoublePlat->getGlobalBounds())) {
				delete newDoublePlat;
				delete newDoublePlatRect;
				return;
			}
		}
		for (int i = 0; i < BoomPlatVector.size(); i++) {
			if (BoomPlatVector[i]->getGlobalBounds().intersects(newDoublePlat->getGlobalBounds())) {
				delete newDoublePlat;
				delete newDoublePlatRect;
				return;
			}
		}
		for (int i = 0; i < BrokenPlatVector.size(); i++) {
			if (BrokenPlatVector[i]->getGlobalBounds().intersects(newDoublePlat->getGlobalBounds())) {
				delete newDoublePlat;
				delete newDoublePlatRect;
				return;
			}
		}

		DoublePlatVector.push_back(newDoublePlat);
		DoublePlatRectVector.push_back(newDoublePlatRect);
		clockForDoublePlat.restart(); 

	}
	//�������� �������� ������� ����������� 
	if (!isLost && clockForBoomPlat.getElapsedTime().asSeconds() > 1.5) {
		float PlatPositionX = rand() % (pixelLength - PlatRect.width);
		int PlatPositionY = rand() % 200 + 970;

		sf::Sprite* newBoomPlat = new sf::Sprite;
		sf::IntRect* newBoomPlatRect = new sf::IntRect;

		newBoomPlatRect->left = 0;
		newBoomPlatRect->width = 61;
		newBoomPlatRect->height = 16;
		newBoomPlatRect->top = 257;

		newBoomPlat->setTexture(T_Plat);
		newBoomPlat->setTextureRect(*newBoomPlatRect);
		newBoomPlat->setPosition(PlatPositionX, Player.getPosition().y - PlatPositionY);

		for (int i = 0; i < PlatVector.size(); i++) {
			if (PlatVector[i]->getGlobalBounds().intersects(newBoomPlat->getGlobalBounds())) {
				delete newBoomPlat;
				delete newBoomPlatRect;
				return;
			}
		}
		for (int i = 0; i < DoublePlatVector.size(); i++) {
			if (DoublePlatVector[i]->getGlobalBounds().intersects(newBoomPlat->getGlobalBounds())) {
				delete newBoomPlat;
				delete newBoomPlatRect;
				return;
			}
		}
		for (int i = 0; i < BrokenPlatVector.size(); i++) {
			if (BrokenPlatVector[i]->getGlobalBounds().intersects(newBoomPlat->getGlobalBounds())) {
				delete newBoomPlat;
				delete newBoomPlatRect;
				return;
			}
		}

		BoomPlatVector.push_back(newBoomPlat);
		BoomPlatRectVector.push_back(newBoomPlatRect);
		clockForBoomPlat.restart();

	}

}

//������� ������� ���������, ������� ��������� ���� ������ �� ����� ��� 500 ��������.
void Game::DelPlats() {
	for (int i = 0; i < PlatVector.size(); i++) {
		if (!isLost && Player.getPosition().y < PlatVector[i]->getPosition().y - 500) {
			delete PlatVector[i];
			PlatVector.erase(PlatVector.begin() + i);// begin - ��������� �� ������; erase - �������� �������� �� �������
		}
	}
	for (int i = 0; i < BrokenPlatVector.size(); i++) {
		if (!isLost && Player.getPosition().y < BrokenPlatVector[i]->getPosition().y - 500) {
			delete BrokenPlatVector[i];
			delete BrokenPlatRectVector[i];
			BrokenPlatVector.erase(BrokenPlatVector.begin() + i);
			BrokenPlatRectVector.erase(BrokenPlatRectVector.begin() + i);
			CheckBrokenPlatVector.erase(CheckBrokenPlatVector.begin() + i);
		}
	}
	for (int i = 0; i < DoublePlatVector.size(); i++) {
		if (!isLost && Player.getPosition().y < DoublePlatVector[i]->getPosition().y - 500) {
			delete DoublePlatVector[i];
			delete DoublePlatRectVector[i];
			DoublePlatVector.erase(DoublePlatVector.begin() + i);
			DoublePlatRectVector.erase(DoublePlatRectVector.begin() + i);
		}
	}
	for (int i = 0; i < BoomPlatVector.size(); i++) {
		if (!isLost && Player.getPosition().y < BoomPlatVector[i]->getPosition().y - 500 ) {
			delete BoomPlatVector[i];
			delete BoomPlatRectVector[i];
			BoomPlatVector.erase(BoomPlatVector.begin() + i);
			BoomPlatRectVector.erase(BoomPlatRectVector.begin() + i);
		}
	}
}

//���������� ��������� �� ������
void Game::DrawPlats(sf::RenderWindow& window) {
	float viewY = CurrentView.getCenter().y - (pixelHeight / 2);
	// ���� �������� �������� ���� �� ����� (�������� ��� ������������), �� ��� ������� ��������� ��������� ����� ������������
	// ���� ���, �� ��������� ��������� �� ������������
	for (int i = 0; i < PlatVector.size(); i++) {
		if (PlatVector[i]->getPosition().y > viewY - 100) {
			window.draw(*PlatVector[i]);
		}
		else if (PlatVector[i]->getPosition().y < viewY - 100)
			break;
	}
}

//������� ������� ���� ��������� ������� ������ ��� ����������� � ����� ����������, ������������� ���� ��������
void Game::Doublejump() {
	for (int i = 0; i < DoublePlatVector.size(); i++) {
		if (!isJumping && Player.getGlobalBounds().intersects(DoublePlatVector[i]->getGlobalBounds())) {
			Player.setPosition(Player.getPosition().x, DoublePlatVector[i]->getPosition().y - yUp);
			isJumping = true;
			if (soundOn)
				djumpSound.play();
			yUp = 30;
		}
	}
}

//������� ��������� ����������� ��������� � ������� ���������� � ���� �� ����� �� ��� ������� 2 ����������� ��������� � ���� ������� ��������� ����, ������������� ���� ������
void Game::BoomJump(){
	for (int i = 0; i < BoomPlatVector.size(); i++) {
		if (!isJumping && Player.getGlobalBounds().intersects(BoomPlatVector[i]->getGlobalBounds())) {
			Player.setPosition(Player.getPosition().x, BoomPlatVector[i]->getPosition().y - yUp);
			isJumping = true;

			for (int j = 0; j < PlatVector.size(); j++) {
				if ((PlatVector[j]->getPosition().y <= BoomPlatVector[i]->getPosition().y)) {
					delete PlatVector[j];
					PlatVector.erase(PlatVector.begin() + j);
					delete PlatVector[j + 1];
					PlatVector.erase(PlatVector.begin() + (j+1));
					delete BoomPlatVector[i];
					BoomPlatVector.erase(BoomPlatVector.begin() + i);
					break;
				}
			}
			if (soundOn) {
				boomSound.play();
			}
			yUp = 15;
		}
	}
}

//����������� ������� ��������� ������ ����
void Game::BrokenPlat() {
	for (int i = 0; i < CheckBrokenPlatVector.size(); i++) {
		if (!isLost && CheckBrokenPlatVector[i]) {
			BrokenPlatVector[i]->setPosition(BrokenPlatVector[i]->getPosition().x, BrokenPlatVector[i]->getPosition().y + 5);
		}
	}
}

//�������� ������������ ��������� � ���������� � ��� ����������� ����������� ���� ������
void Game::BrokenPlatCollision(){
	for (int i = 0; i < BrokenPlatVector.size(); i++) {
		if (!isLost && Player.getGlobalBounds().intersects(BrokenPlatVector[i]->getGlobalBounds()) && !isJumping) {
			CheckBrokenPlatVector[i] = true;
			if (clockForBrokenPlats2.getElapsedTime().asSeconds() > 0.5 && soundOn) {
				breakSound.play();
				clockForBrokenPlats2.restart();
			}
		}
	}
}

//����������� ����� ������ ���� � ������, ���� ����� ������������ � ����� ���������� ������� � ������� (�� ���������� ����� 800 ��������), � ����� ��������������� �������� ������� � 0.2 ������� ����� ����������� ����� ��������.
void Game::addBackground() {
	if (!isLost && Player.getPosition().y < BackgroundVector[BackgroundVector.size() - 1]->getPosition().y + 800 && clockForBackground.getElapsedTime().asSeconds() > 0.5) {
		sf::Sprite* newBackground = new sf::Sprite;
		newBackground->setTexture(T_Background);
		newBackground->setTextureRect(BackgroundRect);
		newBackground->setPosition(0, BackgroundVector[BackgroundVector.size() - 1]->getPosition().y - 500);

		BackgroundVector.push_back(newBackground);
		clockForBackground.restart(); // restsrt - ���������� (�������)
	}
}

//������� �� ������� ������� �������� �� �������, ������� ��������� �� ����� (�� ���������� ����� 500 �������� �� ������� ������), ����� �� ����������� ������� ����� �������� � ������.
void Game::delBackground() {
	for (int i = 0; i < BackgroundVector.size(); i++) {
		if (!isLost && Player.getPosition().y < BackgroundVector[i]->getPosition().y - 500) {
			delete BackgroundVector[i];
			BackgroundVector.erase(BackgroundVector.begin() + i);
		}
	}
}

//�����������, ��������� �� ����� �� ��������� ������ �� ��� X. ���� ����� ��������� �� ��������� ������, ��� ������� ����������� ����� �������, ����� �� �������� �� ��������������� ������� ������ 
//� ��� ��� ���������� �������� � ��� �� �����������.
void Game::Borders() {
	if (Player.getPosition().x < -PlayerRect.width && !isLost) {
		Player.setPosition(pixelLength, Player.getPosition().y);
	}
	if (Player.getPosition().x > pixelLength && !isLost) {
		Player.setPosition(-PlayerRect.width, Player.getPosition().y);
	}
}

// ������� ������������ ��� ���������� ����� ������ � ����. ��� ���������� ������� �������� ����� � �������� ������ �� ������ , � ���� ������� ������ ������, �� �������� ����� �����������.
void Game::newScore() {
	if (scoreInt > Player.getPosition().y) {
		scoreInt = Player.getPosition().y;
	}
	Score.setString(std::to_string(scoreInt * -1));
	ScoreGameOver.setString(std::to_string(scoreInt * -1));
}

//���� ������� "�" ��� "Left" ������, �� ������� �� � ������ ���������� � ����������� �� �������� (6 ��������) �����, � ��������������� ����������� �������� ����� ��������� ��� ������� �� ������� "D" ��� "Right"
//����� �������� �������� ������� ���������� isMoving (����������� ������� �� ������), looksLeft (����� ������� ��������� �����), looksRight (����� ������� ��������� ������)
void Game::Move() {
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && canMove) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && canMove)) {
		Player.setPosition(Player.getPosition().x - xSpeed, Player.getPosition().y);
		isMoving = true;
		looksLeft = true;
		looksRight = false;
	}
	else isMoving = false;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && canMove) or (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && canMove)) {
		Player.setPosition(Player.getPosition().x + xSpeed, Player.getPosition().y);
		isMoving = true;
		looksLeft = false;
		looksRight = true;
	}
	else isMoving = false;
	
	if (!isLost ) {
		if (looksLeft) {
			Player.setTexture(PlayerModel_L);
		}
		else {
			Player.setTexture(PlayerModel_R);
		}
	}
}

//������� ��������� ��������� ���������, ���� �� ��������� � ������, �� ��� ������� �� Y ���������� �� yUP
//����� yUP ��������� 0 �������� ������ �� ���������, ��������� ���� ������, �������� yUP ����������� �������� �� 15
void Game::Jump() {
	if (isJumping && !isLost) {
		Player.setPosition(Player.getPosition().x, Player.getPosition().y - yUp);
		yUp -= 0.5;//�������� �������

		if (yUp <= 0) {
			isJumping = false;
			isFalling = true;
			playJumpSound = true;
			yUp = 15; //������ ������
		}
	}
}

// ���� ����� �� ��������� �� ��������� � �� ��������� � ������, �� ��� ������� �������� � ������������ �� ��������� ������� yDown, ����� �� ����� ����.
void Game::Phisics() {
	for (int i = 0; i < PlatVector.size(); i++) {
		if (isLost || !PlatVector[i]->getGlobalBounds().intersects(Player.getGlobalBounds()) && !isJumping) {
			Player.setPosition(Player.getPosition().x, Player.getPosition().y + yDown);
			yDown += 0.55;//�������� �������
			isFalling = true;
			break;
		}
		// ���� ����� �������� ���������, �� �� ��������� ������ � �������� �������. ���������� yDown ��������������� �� 0, ��� ��������, ��� ����� ������ �� ������
		else if (!isLost) {
			yDown = 0;
			isFalling = false;
			isJumping = true;
			if (playJumpSound && soundOn) {
				jumpSound.play();
				playJumpSound = false;
			}
		}
	}
	// ���� �� ������ � ���������� ������� � ����������, �� �������� �������� ������� ������������� ������ 0, ����������� ���� ������
	for (int i = 0; i < PlatVector.size(); i++) {
		if (!isLost && PlatVector[i]->getGlobalBounds().intersects(Player.getGlobalBounds())) {
			yDown = 0;
			isFalling = false;
			isJumping = true;
			break;
		}
	}
}

//���� ���� isLost ���������� �� true, �� �� ��������, ��� ����� ��������, � ��������� ���������� ����� ���������. 
//��� ������� �� ������� "R" ��� "Enter" ���� ������������ � �������� ���������, ����� ����� ��� ������ ������. 
//��� ������� �� ������� "Escape" ���� ������������ � ������� ����. 
//������� ����� ��������� �������� �� ������ �������� ����� � ������� �������� ���� � ��������.
void Game::GameOver(sf::RenderWindow& win) {
	if (isLost) {
		canMove = false;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::R) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) && clockForGameOver.getElapsedTime().asSeconds() > 1)) {
			isLost = false;
			isMenu = false;
			canMove = true;
			Player.setPosition(pixelLength / 2, 0);
			yUp = 15;
			yDown = 0;
			isJumping = true;
			scoreInt = 0;

			Del();
			lostSound.stop();

			BackgroundVector.resize(0); // resize - ��������� ������ ������� (�� ����)
			PlatVector.resize(0);
			
			BrokenPlatRectVector.resize(0);
			BrokenPlatVector.resize(0);
			CheckBrokenPlatVector.resize(0);

			DoublePlatVector.resize(0);
			DoublePlatRectVector.resize(0);

			BoomPlatVector.resize(0);
			BoomPlatRectVector.resize(0);

			GetPlatBackground();
			clockForGameOver.restart();

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && clockForGameOver.getElapsedTime().asSeconds() > 1) {

			isLost = false;
			isMenu = true;
			clockForMenu.restart();
			canMove = false;
			Player.setPosition(pixelLength / 2, 0);
			yUp = 15;
			yDown = 0;
			isJumping = false;
			scoreInt = 0;

			Del();
			lostSound.stop();

			BackgroundVector.resize(0); 
			PlatVector.resize(0);
			
			BrokenPlatRectVector.resize(0); 
			BrokenPlatVector.resize(0); 
			CheckBrokenPlatVector.resize(0); 
			
			DoublePlatVector.resize(0);
			DoublePlatRectVector.resize(0);

			BoomPlatVector.resize(0);
			BoomPlatRectVector.resize(0);
			 
			GetPlatBackground();
			clockForGameOver.restart();
		}
	}
}

//���� ������������ ��������� � ���� (������� ������) ����, �� ����������� ������� �� ������ "Enter". ���� ��� ������ ���������� ������� �� ���� � ���� 
void Game::Menu() {
	if (isMenu && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		isMenu = false;
		canMove = true;
	}
}

//���� ������������ ��������� � ����, ��� �� ��������, ����� �� ������� "�" � ������ ����� 0,2 ������ ����� ������� �� ���� ��������������� (Pause)
//��� ��������� ������� ������� "�" ���� ��������������
void Game::Pause() {
	if (!isLost && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
		if (!isPause && clockForPause.getElapsedTime().asSeconds() > 0.2) {
			isPause = true;
			clockForPause.restart();
		}
		else if (isPause && clockForPause.getElapsedTime().asSeconds() > 0.2) {
			isPause = false;
			clockForPause.restart();
		}
	}
}

//������� main ������� ����, ������������� ��� ��������� � ��������� ���� � ����������� �����, ���� ���� �� ����� �������.
//������ ����� ��������� ��������� �������, ����� ��� �������� ����, � �������� ��������������� ������ ������ Game.
//���� ���� �� ��������� � ������ ����� ��� ����, ��������� ��������� ���, ���������, �� �������� �� �����, ������������ ������ �������� � ����, ��������� ������������, ��������� ����� ��������� � �.�.
//����� ��������� ������ ��� ������� �� ������, ������� ���������, ���, ������ � ����, � ����� ��������� ������, ����� ��� ����� ��������� � ����� �����.
int main(){
	// ������� ���������� ������ � ������
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::RenderWindow window(sf::VideoMode(540, 970), "Doodle Jump!");
	window.setFramerateLimit(60);
	srand(time(NULL));
	Game game(window);
	game.GetPlatBackground();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				window.close();
		}
		game.Menu();
		game.Pause();
		game.GameOver(window);

		if (!game.getIsPause() && !game.getIsMenu()) {

			window.setView(game.getView());

			game.UpdateView(window);

			game.IfLost();

			game.BrokenPlat();

			game.Doublejump();

			game.BoomJump();

			game.BrokenPlatCollision();

			game.Borders();

			game.NewPlats();

			game.newScore();

			game.UpdatePar();

			game.addBackground();

			game.delBackground();

			game.DelPlats();

			game.Phisics();

			game.Jump();

			game.Move();

		}
		window.clear();

		std::vector<sf::Sprite*> PlatVector = game.platVector();
		std::vector<sf::Sprite*> backgroundVector = game.backgroundVector();
		std::vector<sf::Sprite*> brokenPlatVector = game.getBrokenPlatVector();
		std::vector<sf::Sprite*> DoublePlatVector = game.getDoublePlatVector();
		std::vector<sf::Sprite*> BoomPlatVector = game.getBoomPlatVector(); 

		for (int i = 0; i < backgroundVector.size(); i++) {
			window.draw((*backgroundVector[i]));
		}
		for (int i = 0; i < brokenPlatVector.size(); i++) {
			window.draw(*(brokenPlatVector[i]));
		}
		for (int i = 0; i < DoublePlatVector.size(); i++) { 
			window.draw(*(DoublePlatVector[i])); 
		}
		for (int i = 0; i < BoomPlatVector.size(); i++) { 
			window.draw(*(BoomPlatVector[i]));
		}

		game.DrawPlats(window);
		window.draw(game.getscore());

		if (game.getIsLost()) {
			window.draw(game.goBackground());
			window.draw(game.getScoreGameOver());
		}

		window.draw(game.player());

		if (game.getIsPause()) {
			window.draw(game.pauseBackground());
		}

		if (game.getIsMenu()) {
			window.draw(game.menuBackground());
		}

		window.display();

	}
	return 0;
}