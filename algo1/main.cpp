#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"
#include "programmObject.hpp"
#include "button.hpp"

using namespace std;
using namespace sf;

bool isPressRight = 0;
bool isPressLeft = 0;
bool isPressU = 0;
bool isPressO = 0;

// Переменнные
vector<vector<int> *> Graph;            // вершины и связи
vector<vector<int> *> cycle;            // все простые циклы

vector<int> color;                      // список посещенных
vector<int> way;                        // путь
int ncycle = 0;                         // счетчик циклов

int numberOfCycle = 0;                  // номер цикла, который сейчас показывается

// Интерфейс
vector<int> x, y;                       // координаты вершин
int r = 500;                            // радиус круга

Color white = Color(255, 255, 255);
Color white_100 = Color(255, 255, 255, 100);
Color white_255 = Color(255, 255, 255, 255);
Color blue_background = Color(35, 95, 165);
Color line_color = Color(255, 255, 255, 50);
Color noCycle = Color(24, 67, 117);


// Прототипы функций
int readOut (string fileName);          // считывание данных с файла (имеено вершину и ребра)
void dfs (int vertex);                  // обход в глубину
void checkCycle(int countVertex);       // проверка графа на ацикличность и нахождение цикла
void showGraph ();       // вывод графа в консоле
int screen(int cycle_end, int cycle_st);// отсеивание ненужных циклов в графе
void findCycles();                      // поиск циклов



void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
    
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
    
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
    
    if (isNegative)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str, i);
    return str;
}

void work(int countVertex)
{
    readOut("input.txt");
    findCycles();
    showGraph();
}



int main ()
{
    // настройки окна
    ContextSettings settings;
    settings.antialiasingLevel = 100;
    VideoMode videoMode;
    videoMode.width = 2440;
    videoMode.height = 2160;
    string name = "Symple cycles";
    RenderWindow window(videoMode, name, Style::Close, settings);
    
    // настройки шрифта
    Font font;
    if (!font.loadFromFile("cour.ttf"))
    {
        return EXIT_FAILURE;
    }
    Text text("", font, 35);
    text.setFillColor(white);
    text.setStyle(Text::Bold);
    
    // настройки иконки
    Image icon;
    if (!icon.loadFromFile("icon.png"))
    {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    // найстройки кнопок
    vector<Button> button;
    string str = "";
    char numberCircle[1000] = "";
    
    Button next(1000, 1200, 80, 80, white_100, Color(0,0,0), ">");
    button.push_back(next);
    
    Button prev(320, 1200, 80, 80, white_100, Color(0,0,0), "<");
    button.push_back(prev);
    
    Button info(videoMode.width - 1040, 160, 1000, videoMode.height - 320, white_100, white_100, "INFORMATION\n", 1);
    button.push_back(info);
    
    // сделать бесцветным + добавить текст
    Button cycle_info(videoMode.width - 1040, 240, 1000, videoMode.height - 320, Color(255, 255, 255, 0), Color(255, 255, 255), str, 0);
    button.push_back(cycle_info);
    cycle_info.setFont(30);
    
    Button openFile(40, videoMode.height - 600, 640, 80, white_100, Color(0,0,0), "Open File");
    button.push_back(openFile);
    
    Button update(720, videoMode.height - 600, 640, 80, white_100, Color(0,0,0), "Update");
    button.push_back(update);
    
    Button infouser(40, videoMode.height - 480, 1320, 320, white_100, Color(0,0,0), "Info User\nOPEN FILE - Key O    UPDATE - Key U\nNEXT CYCLE - Key Right\nPREV CYCLE - Key Left", 0);
    button.push_back(infouser);

    // считывание вершины
    int countVertex = readOut("input.txt");
    
    system("clear");
    
    showGraph();
    
    way.assign(countVertex, -1);
    findCycles();
    
    // Вывод всех циклов текстом
    cout << "SIMPLE CYCLES" << endl;
    str = str + "SIMPLE CYCLES\n";
    cout << ncycle << " " << cycle.size() << endl;
    for(int i = 0; i < ncycle; i++)
    {
        int size = cycle[i]->size();
        cout << "CYCLE NO." << i + 1 << " -- ";
        itoa(i + 1, numberCircle, 10);
        str = str + "\n  Cycle NO." + numberCircle + ": ";
        cycle_info.setText(str);
    
        for(int j = 0; j < size; j++)
        {
            cout << (*cycle[i])[j] + 1 << " ";
            itoa((*cycle[i])[j] + 1, numberCircle, 10);
            str = str + numberCircle + " ";
            cycle_info.setText(str);
        }
        cout << endl;
    }
    
    
    
    
    // задаем круг для вершины
    int r = 30;
    
    // белый круг, используется в цикле
    CircleShape shape(r);
    shape.setOutlineColor(noCycle);
    shape.setOutlineThickness(5);
    shape.setFillColor(Color(0, 0, 0, 0));
    
    CircleShape shape2(r);
    shape2.setOutlineColor(white_255);
    shape2.setOutlineThickness(5);
    shape2.setFillColor(Color(0, 0, 0, 0));


    
    while (window.isOpen())
    {
        if (update.update(&window) || (Keyboard::isKeyPressed(Keyboard::U) && !isPressU))
        {
            work(countVertex);
            cout << "SIMPLE CYCLES" << endl;
            str = "";
            str = str + "SIMPLE CYCLES\n";
            for(int i = 0; i < ncycle; i++)
            {
                int size = cycle[i]->size();
                cout << "CYCLE NO." << i + 1 << " -- ";
                itoa(i + 1, numberCircle, 10);
                str = str + "\n  Cycle NO." + numberCircle + ": ";
                cycle_info.setText(str);
                
                for(int j = 0; j < size; j++)
                {
                    cout << (*cycle[i])[j] + 1 << " ";
                    itoa((*cycle[i])[j] + 1, numberCircle, 10);
                    str = str + numberCircle + " ";
                    cycle_info.setText(str);
                }
                cout << endl;
            }
            
            isPressU = 1;
        }
        
        if (openFile.update(&window) || (Keyboard::isKeyPressed(Keyboard::O) && !isPressO))
        {
            system("open -a TextEdit input.txt");
            
            isPressO = 1;
        }
        
        // кнопки ">" и "<"
        if (next.update(&window) || (Keyboard::isKeyPressed(Keyboard::Right) && !isPressRight))
        {
            if (numberOfCycle >= ncycle - 1)
            {
                numberOfCycle = 0;
            }
            else
                numberOfCycle++;
            
            isPressRight = 1;
        }
        
        if (prev.update(&window) || (Keyboard::isKeyPressed(Keyboard::Left) && !isPressLeft))
        {
            if (numberOfCycle <= 0)
            {
                numberOfCycle = ncycle - 1;
            }
            else
                numberOfCycle--;
            
            isPressLeft = 1;
        }
        
        if(!Keyboard::isKeyPressed(Keyboard::Right))
            isPressRight = 0;
        if(!Keyboard::isKeyPressed(Keyboard::Left))
            isPressLeft = 0;
        if(!Keyboard::isKeyPressed(Keyboard::U))
            isPressU = 0;
        if(!Keyboard::isKeyPressed(Keyboard::O))
            isPressO = 0;

        
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            
            
            // двигаем узлы мышкой
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                for (int i = 0; i < countVertex; i++)
                {
                    if (x[i] <= (Mouse::getPosition(window)).x && x[i] >= (Mouse::getPosition(window)).x - 2 * r &&
                        y[i] <= (Mouse::getPosition(window)).y && y[i] >= (Mouse::getPosition(window)).y - 2 * r)
                    {
                        x[i] = (Mouse::getPosition(window)).x - r;
                        y[i] = (Mouse::getPosition(window)).y - r;
                        break;
                    }
                }
            }
            
            window.clear(blue_background);
            
            
            // вывод ребер
            int edge = Graph.size();
            for (int i = 0; i < edge; i++)
            {
                int size = Graph[i]->size();
                for(int j = 0; j < size; j++)
                {
                    Vertex line[] =
                    {
                        Vertex(Vector2f(x[i] + r, y[i] + r), noCycle),
                        Vertex(Vector2f(x[(*Graph[i])[j]] + r, y[(*Graph[i])[j]] + r), noCycle)
                    };
                    window.draw(line, 2, Lines);
                }
            }
        

            
            
            // вывод вершин
            int sizeGraph = Graph.size();
            for (int i = 0; i < sizeGraph; i++)
            {
                shape.setPosition(x[i], y[i]);
                char s[5] = "";
                itoa(i + 1, s, 10);
                text.setString(s);
                text.setPosition(x[i] - 30, y[i] - 30);
                window.draw(shape);
                window.draw(text);
            }
            
            
            
        
            int size2 = cycle[numberOfCycle]->size();
            
            
            Vertex line[] =
            {
                Vertex(Vector2f(x[(*cycle[numberOfCycle])[0]] + r,
                                y[(*cycle[numberOfCycle])[0]] + r),
                                white_255),
                Vertex(Vector2f(x[(*cycle[numberOfCycle])[size2 - 1]] + r,
                                y[(*cycle[numberOfCycle])[size2 - 1]] + r),
                                white_255)
            };
            window.draw(line, 2, Lines);
            
            
            
            for(int j = 0; j < size2; j++)
            {
                int i = (*cycle[numberOfCycle])[j];
                int ii = (*cycle[numberOfCycle])[j + 1];
                shape2.setPosition(x[i], y[i]);
                char s[5] = "";
                itoa(i + 1, s, 10);
                text.setString(s);
                text.setPosition(x[i] - 30, y[i] - 30);
                
                if (j < size2 - 1)
                {
                    Vertex line[] =
                    {
                        Vertex(Vector2f(x[i] + r, y[i] + r), white_255),
                        Vertex(Vector2f(x[ii] + r, y[ii] + r), white_255)
                    };
                    window.draw(line, 2, Lines);
                }
                
                
                window.draw(shape2);
                window.draw(text);

            }
            

            
            int v = 40;
            int size = videoMode.width / v + 1;
            for(int j = 0; j < size; j++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(j * v, 0), line_color),
                    Vertex(Vector2f(j * v, videoMode.height), line_color)
                };
                window.draw(line, 2, Lines);
            }
            
            size = videoMode.height / v + 1;
            for(int j = 0; j < size; j++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(0, j * v), line_color),
                    Vertex(Vector2f(videoMode.width, j * v), line_color)
                };
                window.draw(line, 2, Lines);
            }
            
            
            // вывод кнопок
            size = button.size();
            for(int i = 0; i < size; i++)
                button[i].draw(window);
            

            window.display();
           
        }
    }
    return 0;
}




//////////////////////////////////////////////////////////////////////////////////////
//----------------------------------ПОДПРОГРАММЫ------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////

// Cчитывание данных с файла
int readOut (string fileName)
{
    
    ncycle = 0;     // сбрасываем счетчик циклов
    way.clear();    // очистка пути
    color.clear();  // очистка посещенных вершин
    x.clear();      // очищаем координаты x
    y.clear();      // очищаем координаты y
    
    
    int size = Graph.size(); // очистка вершин
    for (int i = 0; i < size; i++)
        Graph[i]->clear();
    Graph.clear();
    size = cycle.size(); // очистка циклов
    
    
    for (int i = 0; i < size; i++)
        cycle.pop_back();
    cycle.clear();
    
    // считываем вершину
    int n;
    int countVertex;
    ifstream inputFile(fileName);
    
    if (!inputFile.is_open())
        cout << "Error! File is not found!" << endl;
    else
    {
        inputFile >> n;
    }
    
    
    // инициализируем вершины
    for (int i = 0; i < n; i++)
    {
        Graph.push_back(new vector<int>());
        color.push_back(0);
    }
    
    // вывод по кругу
    for (int i = 0; i < n; i++)
    {
        int xx = 700 - 30 + r * cos(i * 360 / n * M_PI / 180);
        int yy = 700 - 30 + r * sin(i * 360 / n * M_PI / 180);
        x.push_back(xx);
        y.push_back(yy);
    }

    
    // считываем ребра
    int v_start, v_finish;
    bool flag = true;
    while(!inputFile.eof())
    {
        inputFile >> v_start;
        inputFile >> v_finish;
        for (int i = 0; i < (*Graph[v_start - 1]).size(); i++)
        {
            if ( (*Graph[v_start - 1])[i] == v_finish - 1)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            Graph[v_start - 1]->push_back(v_finish - 1);
            Graph[v_finish - 1]->push_back(v_start - 1);
        }
    }
    return n;
}


// Отсеивание ненужных циклов в графе
int screen (int cycle_end, int cycle_st)
{
    // добавляем новый цикл
    cycle.push_back(new vector<int>());
    cycle[ncycle]->clear();                             // очищаем
    cycle[ncycle]->push_back(cycle_st);                 // передаем стартовую вершину
    for(int v = cycle_end; v != cycle_st; v = way[v])   // передаем весь сохраненый путь
        cycle[ncycle]->push_back(v);
    
    // проверка на был ли такойже цикл
    int ret = cycle[ncycle]->size();
    if (ret < 3)
        cycle.pop_back();
    
    for (int i = 0; i < ncycle; i++)
    {
        bool found = false;
        int number;
        int size = cycle[i]->size();
        
        if (size == ret) // сравниваем длинну
        {
            
            // ищем одинаковую вершину
            for(int j = 0; j < size; j++)
            {
                if ((*cycle[i])[j] == (*cycle[ncycle])[0])
                {
                    found = true;
                    number = j;
                    break;
                }
            }
            
            if (found)
            {
                int num = 0;
                if(number != 0)
                {
                    if ((*cycle[i])[number - 1] == (*cycle[ncycle])[1])
                        num = -1;
                    else
                        if ((*cycle[i])[number - 1] == (*cycle[ncycle])[cycle[ncycle]->size() - 1])
                            num = 1;
                }
                else
                    if ((*cycle[i])[1] == (*cycle[ncycle])[1])
                        num = 1;
                    else
                        if ((*cycle[i])[1] == (*cycle[ncycle])[cycle[ncycle]->size() - 1])
                            num = -1;
                
                // проверка на совпадение всех вершин
                if (num != 0)
                {
                    int jj = number;
                    for(int j = 0; j < ret; j++)
                    {
                        if((*cycle[i])[jj] != (*cycle[ncycle])[0])
                        {
                            found = false;
                            ret = 0;
                            cycle.pop_back();
                        }
                        jj++;
                        int pp = cycle[i]->size();
                        if (jj == pp)
                            jj = 0;
                    }
                }
            }
        }
    }
    return ret;
}


// Обход в глубину
void dfs (int vertex)
{
    color[vertex] = 1;   // отметили как посещенную
    int size = Graph[vertex]->size();
    for (int i = 0; i < size; i++)
    {
        int to = (*Graph[vertex])[i];
        if (color[to] == 0)
        {
            way[to] = vertex;
            dfs(to);
        }
        else if (color[to] == 1)
            if( screen(vertex, to) > 2 )
                ncycle++;
    }
    color[vertex] = 0;
}


// Поиск циклов
void findCycles()
{
    int size = Graph.size();
    
    for (int i = 0; i < size; i++)
        if (color[i] == 0)
            dfs(i);
}


// Вывод графа в консоле
void showGraph ()
{
    cout << "Count of vertex: " << Graph.size() << endl;
    int sizeGraph = Graph.size();
    for (int i = 0; i < sizeGraph; i++)
    {
        int size = Graph[i]->size();
        cout << "Vertex NO. " << i + 1 << " color: " << color[i] << endl;
        for (int j = 0; j < size; j++)
        {
            cout << "    is connected with vertext NO. "  << (*Graph[i])[j] + 1 << endl;
        }
        cout << endl;
    }
}


