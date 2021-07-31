#include <iostream>
#include <math.h>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <Windows.h>
#include <fstream>
#include <vector>

using namespace std;
int countOfThr;

typedef double(*pointFunc)(double);

double f(double x) 
{
    Sleep(0.9);//имитация долгих и сложных вычислений :D
    return (10 - x);
}

double simpson_integral(pointFunc f, double a, double b, int n)
{
    const double h = (b - a) / n;
    double k1 = 0, k2 = 0;
#pragma omp parallel for reduction(+:k1, k2) num_threads(countOfThr)
        for (int i = 1; i < n; i += 2)
        {
            k1 += f(a + i * h);
            k2 += f(a + (i + 1) * h);
        }
    return h / 3 * (f(a) + 4 * k1 + 2 * k2);
}

int main() 
{
    setlocale(LC_ALL, "ru");
    double a = 0, b = 50, eps = 0.00001;
    double s1, s;
    int n = 1; //начальное число шагов
    int MaxCount = 8, MinCount = 1;
    vector<pair<int, double>> Rezults;

    //cout << "Введите левую границу интегрирования a = ";
    //cin >> a;
    //cout << "\nВведите правую границу интегрирования b = ";
    //cin >> b;
    //cout << "\nВведите требуемую точность eps = ";
    //cin >> eps;
    //cout << "\nВведите минимальное число потоков для эксперимента ";
    //cin >> MinCount;
    cout << "\nВведите максимальное число потоков для эксперимента ";
    cin >> MaxCount;

    countOfThr = MaxCount;
    //начало отсчета времени вычисления
    auto startTime = std::chrono::high_resolution_clock::now();

    s1 = simpson_integral(f, a, b, n); //первое приближение для интеграла
    do {
        s = s1;     //второе приближение
        n = 2 * n;  //увеличение числа шагов в два раза,
                    //т.е. уменьшение значения шага в два раза
        s1 = simpson_integral(f, a, b, n);

    } while (fabs(s1 - s) > eps);  //сравнение приближений с заданной точностью
    //окончание отсчета времениcicleLen
    auto endTime = std::chrono::high_resolution_clock::now() - startTime;
    //подсчет времени вычисления
    double elapseTime = std::chrono::duration<double>(endTime).count();

    cout << "Значение интеграла: " << s1 << endl;
    cout << "time " << elapseTime;

    return 0;
}