#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <fstream>
// Sprite - Рисуемое представление текстуры с собственными преобразованиями, цветом и т. д. (область без текстуры)
// Texture - Живое изображение на видеокарте, которое можно использовать для рисования.
// IntRect - Вспомогательный класс для управления двумерными прямоугольниками, выровненными по осям.
class Game {
private:

	//Игрок
	sf::Sprite Player;
	sf::Texture PlayerModel_R;
	sf::Texture PlayerModel_L;
	sf::IntRect PlayerRect;

	//Платформы
	std::vector<sf::Sprite*> PlatVector;
	sf::Texture T_Plat;
	sf::IntRect PlatRect;

	//Невидимые платформы
	std::vector<sf::Sprite*> BrokenPlatVector;
	std::vector<sf::IntRect*> BrokenPlatRectVector;
	std::vector<bool> CheckBrokenPlatVector;

	//Двойной прыжок
	std::vector<sf::Sprite*> DoublePlatVector;
	std::vector<sf::IntRect*> DoublePlatRectVector;

	//Взрыв
	std::vector<sf::Sprite*> BoomPlatVector;
	std::vector<sf::IntRect*> BoomPlatRectVector;

	//Фон меню
	sf::Sprite MenuBackground;
	sf::Texture T_MenuBackground;

	//Фон игры
	std::vector<sf::Sprite*> BackgroundVector;
	sf::Texture T_Background;
	sf::IntRect BackgroundRect;

	//Фон меню паузы
	sf::Sprite PauseBackground;
	sf::Texture T_PauseBackground;

	//Фон проигрыша
	sf::Sprite GOBackground;
	sf::Texture T_GOBackground;

	//Очки
	sf::Text Score;
	sf::Text ScoreGameOver;
	sf::Font font;

	//Время между кадрами
	sf::Clock clockForPlats, clockForBackground, clockForPause, clockForGameOver, clockForMenu, clockForBrokenPlats, clockForBrokenPlats2, clockForBrokenPlatsAnimation, clockForDoublePlat, clockForBoomPlat;

	sf::View View, CurrentView;

	//Звук прыжка
	sf::SoundBuffer jumpBuffer;
	sf::Sound jumpSound;

	//Звук проигрыша
	sf::SoundBuffer lostBuffer;
	sf::Sound lostSound;

	//Звук поломки
	sf::SoundBuffer breakBuffer; 
	sf::Sound breakSound; 

	//Звук двойного прыжка
	sf::SoundBuffer djumpBuffer;
	sf::Sound djumpSound;

	//Звук boom
	sf::SoundBuffer boomBuffer;
	sf::Sound boomSound;


	//Переменные для расчетов
	float yUp, yDown, xSpeed;
	bool isJumping, isFalling, isLost, isPause, isMenu, isMoving, canMove, looksLeft, looksRight, soundOn, playJumpSound;
	int BetweenPlat, scoreInt, highscoreInt;
	int pixelLength = 540, pixelHeight = 970;

public:
	//Задаем начальные параметры для игры
	Game(sf::RenderWindow& win) : yDown{ 0 }, yUp{ 15 }, xSpeed{ 7 }, scoreInt{ 0 }, BetweenPlat{ 60 }, isFalling{ false }, playJumpSound{ true }, soundOn{ true }, canMove{ true }, isJumping{ true }, isLost{ false }, isMenu{ true }, isMoving{ false }, isPause{ false }, looksRight{ true }, looksLeft{ false } {

		//Загружаем модельку персонажа и задаем размеры области для персонажа и позицию персонажа на игровом поле
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

		//Берем текущий вид окна
		CurrentView = win.getView();

		//Загружаем фон и задаем положение области фона на игровом поле
		T_Background.loadFromFile("images/Background.png");
		BackgroundRect.left = 0;
		BackgroundRect.top = 0;
		BackgroundRect.width = pixelLength;
		BackgroundRect.height = pixelHeight;

		//Загружаем платформу и задаем положение области платформы на игровом поле
		T_Plat.loadFromFile("images/Plats.png");
		PlatRect.left = 0;
		PlatRect.top = 0;
		PlatRect.width = 59;
		PlatRect.height = 17;

		//задаем положение внутри окна и его размер
		View.setCenter(pixelLength / 2, pixelHeight / 2);
		View.setSize(pixelLength, pixelHeight);

		//Загружаем шрифт и задпем позицию текста с подсчетом очков
		font.loadFromFile("fonts/al-seana.ttf");
		Score.setString(std::to_string(scoreInt));
		Score.setFont(font);
		Score.setPosition(30, 300);
		Score.setCharacterSize(40);
		Score.setFillColor(sf::Color::Black);

		//Меняем тип параметра счета после завершения игры, чтобы не писало е-шками
		ScoreGameOver.setString(std::to_string(scoreInt));
		ScoreGameOver.setFont(font);
		ScoreGameOver.setPosition(400, 50);
		ScoreGameOver.setCharacterSize(40);
		ScoreGameOver.setFillColor(sf::Color::Black);

		//Загружаем фон для паузы и задаем его позицию в окне
		T_PauseBackground.loadFromFile("images/PauseBackground.png");
		PauseBackground.setTexture(T_PauseBackground);
		PauseBackground.setPosition(0, 0);

		//Загружаем фон меню
		T_MenuBackground.loadFromFile("images/MenuBackground.png");
		MenuBackground.setTexture(T_MenuBackground);
		MenuBackground.setPosition(0, 0);

		//Загружаем фон экрана проигрыша
		T_GOBackground.loadFromFile("images/GOBackground.png");
		GOBackground.setTexture(T_GOBackground);
		GOBackground.setPosition(0, 0);

		//Загружаем звук для проигрыша
		lostBuffer.loadFromFile("sounds/lost.wav");
		lostSound.setBuffer(lostBuffer);
		lostSound.setVolume(10);

		//Загружаем звук для прыжка
		jumpBuffer.loadFromFile("sounds/jump.wav");
		jumpSound.setBuffer(jumpBuffer);
		jumpSound.setVolume(10);

		//Загружаем звук для хруста
		breakBuffer.loadFromFile("sounds/break.wav");
		breakSound.setBuffer(breakBuffer);
		breakSound.setVolume(10);

		//Загружаем звук для двойного прыжка
		djumpBuffer.loadFromFile("sounds/djump.wav");
		djumpSound.setBuffer(djumpBuffer);
		djumpSound.setVolume(10);

		//Загружаем звук boom
		boomBuffer.loadFromFile("sounds/boom2.wav");
		boomSound.setBuffer(boomBuffer);
		boomSound.setVolume(10);
	}
	//Создаем функции для переменных из private чтобы к ним можно было обращаться в main

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

	//Создаем функции игровый действий 

	//Платформы
	void GetPlatBackground();
	void NewPlats();
	void DelPlats();
	void DrawPlats(sf::RenderWindow&);

	//Невидимые платформы
	void BrokenPlat();
	void BrokenPlatCollision();

	//Двойной прыжок
	void Doublejump(); 

	//Взрыв
	void BoomJump(); 

	//Обновление изображения в окне
	void UpdateView(sf::RenderWindow&); 

	//Отчистка
	void Del();

	//Обновление счета
	void newScore();

	// Обновления параметров
	void UpdatePar();

	//Меню
	void Menu();

	//Пауза
	void Pause();

	//Физика
	void Phisics();

	//Проверка на проигрыш
	void IfLost();

	// Границы
	void Borders();

	// Фон
	void addBackground();
	void delBackground();

	//Движения 
	void Jump();
	void Move();

	//Проигрыш
	void GameOver(sf::RenderWindow&);

};

//Данный деструктор освобождает память, выделенную под память от всех элементов векторов PlatsVector, BackgroundVector и др.
Game::~Game() {
	Del();
}

//Отчистка спрайтов из векторов для перехода в меню или перезагрузки уровня
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

//Выставляются положение фонов по центру и подгоняются по виду положение очков
void Game::UpdatePar() {
	PauseBackground.setPosition(CurrentView.getCenter().x - (pixelLength / 2), CurrentView.getCenter().y - (pixelHeight / 2));
	MenuBackground.setPosition(CurrentView.getCenter().x - (pixelLength / 2), CurrentView.getCenter().y - (pixelHeight / 2));
	GOBackground.setPosition(CurrentView.getCenter().x - (pixelLength / 2), CurrentView.getCenter().y - (pixelHeight / 2));

	Score.setPosition(10, CurrentView.getCenter().y - (pixelHeight / 2));
	ScoreGameOver.setPosition(CurrentView.getCenter().x + 90, CurrentView.getCenter().y - 18);
}

//Если позиция игрока стала ниже позиции самой нижней платформы и еще + 200 пикселей (то есть ниже последней платформы на 200 пикселей) и игрок ПОКА еще не упал, то ставится статус проигрыша и играет звук падения
void Game::IfLost() {
	if (Player.getPosition().y > PlatVector[0]->getPosition().y + 200 && !isLost) {
		isLost = true;
		if (soundOn)
			lostSound.play();
	}
}

//Если игрок не упал, то центр камеры устанавливается на позицию игрока по оси X, а по оси Y камера не изменяется.
void Game::UpdateView(sf::RenderWindow& win) {
	if (!isLost)
		View.setCenter(pixelLength / 2, Player.getPosition().y);
	CurrentView = win.getView();
}

//Задаются начальные параметры для запуска игры. Платформы и фон
void Game::GetPlatBackground() {
	//Создается эффект бесконечной прокрутки фона в игре. При движении игрового персонажа на экране, фон движется вниз, и когда первый спрайт выходит за границу экрана, он перемещается
	//в конец вектора и появляется снова на экране сверху.
	//Создается первый элемент в позиции (0,500), если элемент не первый то на 500 пикселей выше предыдущего

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

		BackgroundVector.push_back(newBackground); //перемещение элемента в конец вектора
	}
	//Задаются текстуры платформы и ее расположение на экране (рандомно, но в пределах границы окна). 
	//Если это не первая итерация цикла, то позиция платформы вычисляется на основе позиции предыдущей платформы и зазора между ними. Задаются первые 15 начальных платформ
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

//Функция создает новые платформы. Выбирается рандомная позиция на Х, и с шагом (BetweenPlat) от самой верхней платформы и ставится новая. 
//Также проверяются случаи с пересечением друг друга платформ
void Game::NewPlats() {
	if (!isLost) {
		float PlatPositionX = rand() % (pixelLength - PlatRect.width);

		sf::Sprite* newPlat = new sf::Sprite;
		newPlat->setTexture(T_Plat);
		newPlat->setTextureRect(PlatRect);
		newPlat->setPosition(PlatPositionX, PlatVector[PlatVector.size() - 1]->getPosition().y - BetweenPlat);

		PlatVector.push_back(newPlat);

	}
	//Создание ломающихся платформ
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
			if (PlatVector[i]->getGlobalBounds().intersects(newBrokenPlat->getGlobalBounds())) { //.intersects - проверка на пересечение границ двух объектов
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
	//Создание платформ с двойным прыжком
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
	//Создание платформ которые взрвываются 
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

//Функция удаляет платформы, которые находятся ниже игрока на более чем 500 пикселей.
void Game::DelPlats() {
	for (int i = 0; i < PlatVector.size(); i++) {
		if (!isLost && Player.getPosition().y < PlatVector[i]->getPosition().y - 500) {
			delete PlatVector[i];
			PlatVector.erase(PlatVector.begin() + i);// begin - указатель на начало; erase - удаление элемента из вектора
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

//Отображаем платформы на экране
void Game::DrawPlats(sf::RenderWindow& window) {
	float viewY = CurrentView.getCenter().y - (pixelHeight / 2);
	// Если создание платформ идет за окном (невидиво для пользователя), то при подъеме персонажа платформы будут показываться
	// Если нет, то созданная платформа не показывается
	for (int i = 0; i < PlatVector.size(); i++) {
		if (PlatVector[i]->getPosition().y > viewY - 100) {
			window.draw(*PlatVector[i]);
		}
		else if (PlatVector[i]->getPosition().y < viewY - 100)
			break;
	}
}

//Функция которая дает персонажу двойной прыжок при пересечении с синей платформой, проигрывается звук пружинки
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

//Функция проверяет пересечение персонажа с красной платформой и если он попал на нее удаляет 2 вышестоящие платформы и саму красную платформу тоже, проигрывается звук взрыва
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

//Перемещение хрупкой платформы плавно вниз
void Game::BrokenPlat() {
	for (int i = 0; i < CheckBrokenPlatVector.size(); i++) {
		if (!isLost && CheckBrokenPlatVector[i]) {
			BrokenPlatVector[i]->setPosition(BrokenPlatVector[i]->getPosition().x, BrokenPlatVector[i]->getPosition().y + 5);
		}
	}
}

//Проверка столкновения персонажа с платформой и при пересечении проигрывает звук хруста
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

//Добавляется новый спрайт фона в вектор, если игрок приближается к концу последнего спрайта в векторе (на расстоянии менее 800 пикселей), а также устанавливается задержка времени в 0.2 секунды между добавлением новых спрайтов.
void Game::addBackground() {
	if (!isLost && Player.getPosition().y < BackgroundVector[BackgroundVector.size() - 1]->getPosition().y + 800 && clockForBackground.getElapsedTime().asSeconds() > 0.5) {
		sf::Sprite* newBackground = new sf::Sprite;
		newBackground->setTexture(T_Background);
		newBackground->setTextureRect(BackgroundRect);
		newBackground->setPosition(0, BackgroundVector[BackgroundVector.size() - 1]->getPosition().y - 500);

		BackgroundVector.push_back(newBackground);
		clockForBackground.restart(); // restsrt - перезапуск (таймера)
	}
}

//Удаляет из вектора фоновых спрайтов те спрайты, которые находятся за окном (на расстоянии более 500 пикселей от позиции игрока), чтобы не накапливать слишком много спрайтов в памяти.
void Game::delBackground() {
	for (int i = 0; i < BackgroundVector.size(); i++) {
		if (!isLost && Player.getPosition().y < BackgroundVector[i]->getPosition().y - 500) {
			delete BackgroundVector[i];
			BackgroundVector.erase(BackgroundVector.begin() + i);
		}
	}
}

//Проверяется, находится ли игрок за пределами экрана по оси X. Если игрок находится за пределами экрана, его позиция обновляется таким образом, чтобы он появился на противоположной стороне экрана 
//и тот мог продолжить движение в том же направлении.
void Game::Borders() {
	if (Player.getPosition().x < -PlayerRect.width && !isLost) {
		Player.setPosition(pixelLength, Player.getPosition().y);
	}
	if (Player.getPosition().x > pixelLength && !isLost) {
		Player.setPosition(-PlayerRect.width, Player.getPosition().y);
	}
}

// Функция используется для обновления счета игрока в игре. Она сравнивает текущее значение счета с позицией игрока на экране , и если позиция игрока больше, то значение счета обновляется.
void Game::newScore() {
	if (scoreInt > Player.getPosition().y) {
		scoreInt = Player.getPosition().y;
	}
	Score.setString(std::to_string(scoreInt * -1));
	ScoreGameOver.setString(std::to_string(scoreInt * -1));
}

//Если клавиша "А" или "Left" нажата, то позиция по Х игрока изменяется в зависимости от скорости (6 пикселей) влево, в противоположном направлении персонаж будет двигаться при нажатии на клавишу "D" или "Right"
//Также меняются значения булевых переменных isMoving (проверяющая нажатие на кнопки), looksLeft (смену взгляда персонажа влево), looksRight (смену взгляда персонажа вправо)
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

//Фунцция проверяет состояние персонажа, если он находится в прыжке, то его позиция по Y изменяется по yUP
//Когда yUP достигнет 0 персонаж упадет на платформу, проиграет звук прыжка, параметр yUP восстановит значение до 15
void Game::Jump() {
	if (isJumping && !isLost) {
		Player.setPosition(Player.getPosition().x, Player.getPosition().y - yUp);
		yUp -= 0.5;//Скорость подъема

		if (yUp <= 0) {
			isJumping = false;
			isFalling = true;
			playJumpSound = true;
			yUp = 15; //Высота прыжка
		}
	}
}

// Если игрок не находится на платформе и не находится в прыжке, то его позиция меняется в соответствии со скоростью падения yDown, чтобы он падал вниз.
void Game::Phisics() {
	for (int i = 0; i < PlatVector.size(); i++) {
		if (isLost || !PlatVector[i]->getGlobalBounds().intersects(Player.getGlobalBounds()) && !isJumping) {
			Player.setPosition(Player.getPosition().x, Player.getPosition().y + yDown);
			yDown += 0.55;//Скорость падения
			isFalling = true;
			break;
		}
		// если игрок коснулся платформы, то он перестает падать и начинает прыгать. Переменная yDown устанавливается на 0, что означает, что игрок больше не падает
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
	// Если не падает и происходит касание с платформой, то значение скорости падения присваивается равной 0, проигрывает звук прыжка
	for (int i = 0; i < PlatVector.size(); i++) {
		if (!isLost && PlatVector[i]->getGlobalBounds().intersects(Player.getGlobalBounds())) {
			yDown = 0;
			isFalling = false;
			isJumping = true;
			break;
		}
	}
}

//Если флаг isLost установлен на true, то он означает, что игрок проиграл, и программа отображает экран проигрыша. 
//При нажатии на клавишу "R" или "Enter" игра сбрасывается в исходное состояние, чтобы игрок мог начать заново. 
//При нажатии на клавишу "Escape" игра возвращается в главное меню. 
//Функция также выполняет операции по сбросу значения счета и очистки векторов фона и платформ.
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

			BackgroundVector.resize(0); // resize - обновляет размер вектора (до нуля)
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

//Если пользователь находится в меню (главном экране) игры, то проверяется нажание на кнопку "Enter". Если она нажата происходит переход из меню в игру 
void Game::Menu() {
	if (isMenu && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		isMenu = false;
		canMove = true;
	}
}

//Если пользователь находится в игре, еще не проиграл, нажал на клавишу "Р" и прошло более 0,2 секунд после нажатия то игра останавливается (Pause)
//При повторном нажатии клавиши "Р" игра возобновляется
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

//Функция main создает окно, устанавливает его настройки и запускает игру в бесконечном цикле, пока окно не будет закрыто.
//Внутри цикла программа проверяет события, такие как закрытие окна, и вызывает соответствующие методы класса Game.
//Если игра не находится в режиме паузы или меню, программа обновляет вид, проверяет, не проиграл ли игрок, обрабатывает физику объектов в игре, проверяет столкновения, добавляет новые платформы и т.д.
//Затем программа рисует все объекты на экране, включая платформы, фон, игрока и счет, а также различные экраны, такие как экран проигрыша и экран паузы.
int main(){
	// Функция нахождения утечек в памяти
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