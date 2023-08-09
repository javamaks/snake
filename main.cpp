#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int N = 30, M = 20; //N - длина, M - ширина
int ts = 25; //Размер каждого плитки

int dir = 2, num = 4; //переменная dir отвечает за поворот, переменная num отвечает за длину змейки
/*Как поварачивается змейка при помощи переменной dir
dir = 1 -> лево
dir = 2 -> право
dir = 3 -> вверх
dir = 0 -> вниз*/

bool game = true; //переменная game отвечает за режима игры

//Структура змейки отвечает за её длину
struct Snake {
    int x, y;
    //В структуре указаны переменные x, y
    //они отвечают за координаты плитки
} s[600];

//s[] - это каждая плитка змейки

//Структура яблоки
struct Fruct
{
    int x, y;
    //Задаём координаты яблоки
} f;

//Функция Tick отвечает за саму игру
void Tick() {
    //Здесь находятся первичные координаты
    //Просто рисуется сама змейка
    for (int i = num; i > 0; i--) {
        s[i].x = s[i - 1].x; 
        s[i].y = s[i - 1].y;
    }

    //движение змейки
    if (dir == 0)
        s[0].y += 1; //вниз
    if (dir == 1)
        s[0].x -= 1; //влево
    if (dir == 2)
        s[0].x += 1; //право
    if (dir == 3)
        s[0].y -= 1; //вверх

    //Здесь отвечает, если змейка уходит через стенку экрана
    //то она возращается на через другую
    if (s[0].x > N)
        s[0].x = 0; 
    if (s[0].x < 0)
        s[0].x = N;
    if (s[0].y > M)
        s[0].y = 0;
    if (s[0].y < 0)
        s[0].y = M;

    //Если змейка поедает яблоко
    if ((s[0].x == f.x) && (s[0].y == f.y)) {
        num++; //Змейка растёт

        f.x = rand() % N; //Меняем рандомные координаты яблоки
        f.y = rand() % M;
    }

    //Здесь проигрышь
    for (int i = 1; i < num; i++) //цикл for для всех плиток змейки
        //Если змейка сталкивается со своим телом
        if ((s[0].x == s[i].x) && (s[0].y == s[i].y))
            game = false; //меняем значение переменной game на false
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(ts * N, ts * M), "Snake!");

    //Как ставляется картинка
   /*
   Texture t; <- Задаётся переменная для текстуры
    t.loadFromFile("Расположение картинки"); <- в переменную текстуры
    загружаем картинку
    Sprite tiles(t); <- Задаём следующию переменную спрайта и 
    загружаем текстуру
   */

    //Ставим картнинку titels
    Texture t;
    t.loadFromFile("C:/Users/Андрей/Desktop/Paint/tiles.png");
    Sprite tiles(t);

    //Ставим картинку змейки
    Texture sn;
    sn.loadFromFile("C:/Users/Андрей/Desktop/Paint/snake.png");
    Sprite snake(sn);

    //Ставим картинку яблоко
    Texture ap;
    ap.loadFromFile("C:/Users/Андрей/Desktop/Paint/apple.png");
    Sprite apple(ap);

    //Ставим надпись gameover
    Texture go;
    go.loadFromFile("C:/Users/Андрей/Desktop/Paint/gameover.png");
    Sprite gameover(go);
    gameover.setPosition(0, 175);

    f.x = 10;
    f.y = 10;
    //первичные координаты яблоки

    Clock clock; //переменная clock нужна для ирового времени
    float timer = 0, delay = 0.1;
    //timer и delay отвечают за скорость змейки
    //timer отвечает, через какое время змейка двигается
    //delay - за задержку

    while (window.isOpen())
    {
        //Игровое время
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //Управление змейкой через клавиатуру
        if (Keyboard::isKeyPressed(Keyboard::Left)) //Если стрелка влево нажата
            dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right)) //Если стрелка вправо нажата
            dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up)) //Если стрелка вверх нажата
            dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down))  //Если стрелка вниз нажата
            dir = 0;
        
        //Здесь отвечает за работу игры
        if (timer > delay && game) {
            timer = 0; //Возвращаем время
            Tick(); //Вызываем работу игры
        }

        window.clear();

        //Для прорисовки всего графичесского окна
        //Пишем цикл for
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) {
                tiles.setPosition(i * ts, j * ts); //Позиция
                window.draw(tiles); //Рисуем
            }

        //Рисуем змейку
        for (int i = 0; i < num; i++) {
            if (i != 0) //Рисуем тело змейки
                snake.setTextureRect(IntRect(0, 0, ts, ts));
            else //Рисуем голову змейки
                snake.setTextureRect(IntRect(dir * ts, ts, ts, ts));

            // Во время проигрыша голова изменяется
            if (!game && i == 1)
                snake.setTextureRect(IntRect(dir * ts, ts * 2, ts, ts));

            snake.setPosition(s[i].x * ts, s[i].y * ts); //Задаём позицию
        //для плиток змейки
            window.draw(snake);
        }

        apple.setPosition(f.x * ts, f.y * ts); //Позиция яблоки
        window.draw(apple); //Рисуем яблоко

        //Если мы проиграли, то будеть выдовать надпись gameover
        if (!game)
            window.draw(gameover);

        window.display();
    }

    return 0;
}
