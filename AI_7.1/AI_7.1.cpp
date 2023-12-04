// Решение задачи коммивояжера генетическим алгоритмом

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int Max = 100;           // максимальное число городов
const int K = 50;              // число особей в популяции
const int Mnogo = 30;            // длительность эволюции

struct TGorod {                  // описание города
    string Name;                 // название города
    double x, y;                 // координаты города
};

struct TOsob {                   // вариант маршрута
    int Path[Max];               // маршрут
    double dl;                   // длина маршрута
};

TGorod G[Max];                   // города
TOsob  pop[K];                   // популяция

int N;                           // число городов


double Random(double min, double max)  // случайное число от min до max
{
    return (double)(rand()) * (max - min) / RAND_MAX + min;
}


int LoadTowns(string FileName)  // загрузка городов из файла
{
    string s;          // строка из файла
    size_t j;          // индекс
    ifstream F;        // поток ввода
    F.open(FileName);  // окрываем файл для чтения    
    if (F) {
        N = 0;         // число городов
        while ((!F.eof()) && (N < Max)) {
            getline(F, s);
            j = s.find('\t');
            G[N].Name = s.substr(0, j);
            s.erase(0, j + 1);
            j = s.find('\t');
            G[N].x = atof(s.substr(0, j).c_str());
            s.erase(0, j + 1);
            G[N].y = atof(s.c_str());
            N++;
        }
    }
    F.close();     // закрываем файл    
    return N;
}


void Inic()  // инициализация
{
    int mas[Max];      // номера городов
    srand(time(0));
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) mas[j] = j;
        for (int j = 0; j < N; j++) {
            int sl = Random(0, N - j - 1);
            pop[i].Path[j] = mas[sl];
            for (int l = sl + 1; l < N; l++) mas[l - 1] = mas[l];
        }
    }
}


void Selec()  // селекция
{
    double x1, x2, y1, y2;   // координаты маршрута    
    // вычисление длин маршрутов
    for (int i = 0; i < K; i++) {
        pop[i].dl = 0;
        for (int j = 0; j < N - 1; j++) {
            x1 = G[pop[i].Path[j]].x;  x2 = G[pop[i].Path[j + 1]].x;
            y1 = G[pop[i].Path[j]].y;  y2 = G[pop[i].Path[j + 1]].y;
            pop[i].dl += sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
        }
    }
    // сортировка
    for (int i = 0; i < K - 1; i++) {
        double min = pop[i].dl;  int nm = i;
        for (int j = i + 1; j < K; j++)
            if (pop[j].dl < min) {
                min = pop[j].dl;  nm = j;
            }
        if (nm != i) {
            pop[nm].dl = pop[i].dl;  pop[i].dl = min;
            TOsob p = pop[nm];  pop[nm] = pop[i];  pop[i] = p;
        }
    }
    // подготовка к скрещиванию
    for (int i = K / 2; i < K; i++)
        for (int j = 0; j < N; j++) pop[i].Path[j] = -1;
}


void Papa(int p, int r)  // наследование от папы
{
    int mas[Max];      // номера городов
    for (int j = 0; j < N; j++) mas[j] = j;
    for (int j = 0; j < N / 2; j++) {
        int sl = Random(0, N - j - 1);
        int pos = mas[sl];
        pop[r].Path[pos] = pop[p].Path[pos];
        for (int i = sl + 1; i < N; i++) mas[i - 1] = mas[i];
    }
}


void Mama(int m, int r)  // наследование от мамы
{
    for (int i = 0; i < N; i++) {
        bool fExist = false;
        for (int j = 0; j < N; j++)
            if (pop[r].Path[j] == pop[m].Path[i]) fExist = true;
        if (!fExist) {
            int j = -1;
            do {
                j++;
            } while (pop[r].Path[j] != -1);
            pop[r].Path[j] = pop[m].Path[i];
        }
    }
}


void Mut(int r)  // мутация
{
    int p1 = Random(0, N - 1), p2;
    do {
        p2 = Random(0, N - 1);
    } while (p1 == p2);
    int c = pop[r].Path[p1];  pop[r].Path[p1] = pop[r].Path[p2];  pop[r].Path[p2] = c;
}


void Scresh()  // скрещивание
{
    int r, m, p;  // ребенок, мама, папа
    for (r = K / 2; r < K; r++) {
        p = Random(0, K / 2 - 1);
        do {
            m = Random(0, K / 2 - 1);
        } while (m == p);
        Papa(p, r);  Mama(m, r);  Mut(r);
    }
}


int main() {
    //setlocale(LC_ALL, "Russian");
    N = LoadTowns("goroda.txt");   Inic();
    for (int i = 0; i < Mnogo; i++) {
        Selec();  Scresh();
    }
    Selec();
    for (int i = 0; i < N - 1; i++) cout << pop[0].Path[i] << " - ";
    cout << pop[0].Path[N - 1] << endl << endl;
    for (int i = 0; i < N; i++) cout << G[pop[0].Path[i]].Name << endl;
    cout << endl << "Расстояние: " << pop[0].dl << endl;
    system("pause");
    return 0;
}