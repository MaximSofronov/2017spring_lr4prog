/*
«Почтовые отделения». Вдоль прямой дороги расположены деревни.
Дорога представляется целочисленной осью, а расположение каждой деревни
задается целым числом – координатой на этой оси. Никакие две деревни не имеют
одинаковых координат. Расстояние между двумя деревнями вычисляется как
модуль разности их координат.
В некоторых, не обязательно во всех, деревнях будут построены почтовые
отделения. Деревня и расположенное в ней почтовое отделение имеют
одинаковые координаты. Почтовые отделения необходимо расположить в
деревнях таким образом, чтобы общая сумма расстояний от каждой деревни до
ближайшего к ней почтового отделения была минимальной.
Требуется по заданным координатам деревень и количеству почтовых
отделений найти такое их расположение по деревням, при котором общая сумма
расстояний от каждой деревни до её ближайшего почтового отделения будет
минимальной.
*/

using namespace std;

#include <fstream>
#include <vector>
#include <iostream>

struct vertex;
struct solution;

vertex find_min_vertex(const vector<vertex>& vertexs, const int begin);
vertex solve(solution& str);
void count_first_post(solution& str);
void count_last_post(solution& str);
vertex find_best_option(solution& str, int level, int cityNum);

struct vertex {
    vertex() {}
    vertex(int sum) : sum(sum) {}

    int sum;
    vector<int> way;

    void addwayCity(int city) {
        way.push_back(city);
    }
};

struct solution {
    vector<vector<vertex>> field;
    vector<int> cities;
    int postnum;
};

void count_first_post(solution& str) {
    str.field.push_back(vector<vertex>());
    str.field[0].push_back(vertex(0));
    str.field[0][0].addwayCity(str.cities[0]);
    for (int i = 1; i < str.cities.size(); i++) {
        int p = (str.cities[i] - str.cities[i - 1]) * i + str.field[0][i-1].sum;
        str.field[0].push_back(vertex(p));
        str.field[0][i].addwayCity(str.cities[i]);
    }
}

vertex find_min_vertex(const vector<vertex> &vertexs, const int begin) {
    int min = INT_MAX;
    vertex minvertex;

    for (int i = begin; i < vertexs.size(); i++) {
        if (vertexs[i].sum < min) {
            minvertex = vertexs[i];
            min = vertexs[i].sum;
        }
    }

    return minvertex;
}

vertex solve(solution& str) {
    count_first_post(str);
    for (int i = 1; i < str.postnum; i++) {
        str.field.push_back(vector<vertex>(str.cities.size()));
        for (int j = i; j < str.cities.size(); j++) {
            str.field[i][j] = find_best_option(str,i, j);
        }
    }
    count_last_post(str);

    vertex minvertex = find_min_vertex(str.field[str.postnum - 1], str.postnum);

    return minvertex;
}

void count_last_post(solution& str) {
    int q = 0;
    int last = str.postnum - 1;
    for (size_t i = str.cities.size() - 2; i >= str.postnum - 1 ; i--) {
        int p = (str.cities[i + 1] - str.cities[i]) * (int) (str.cities.size() - i - 1) + str.field[last][i].sum + q;
        q += (str.cities[i + 1] - str.cities[i]) * (int) (str.cities.size() - i - 1);
        str.field[last][i].sum = p;
    }
}

vertex find_best_option(solution& str, int level, int cityNum) {
    vector<vertex> vertexs;
    for (int j = level - 1; j < cityNum; j++) {
        int sum = str.field[level - 1][j].sum;
        int left = str.cities[j];
        int right = str.cities[cityNum];
        for (int i = j + 1; i < cityNum; i++) {
            int dLeft = str.cities[i] - left;
            int dRight = right - str.cities[i];
            sum += min(dLeft, dRight);
        }
        vertex p(sum);
        p.way = str.field[level - 1][j].way;
        vertexs.push_back(p);

    }
    vertex min = find_min_vertex(vertexs, 0);
    min.addwayCity(str.cities[cityNum]);

    return min;
}


int main() {
    std::ifstream fin ("input.txt");
    std::ofstream fout ("output.txt");

    int numberOfCities, numberOfPost;
    vector<int> cities;
    fin >> numberOfCities >> numberOfPost;

    for (int i = 0; i < numberOfCities; ++i) {
        int city;
        fin >> city;
        cities.push_back(city);
    }

    solution decision;
    decision.cities = cities;
    decision.postnum = numberOfPost;

    vertex result = solve(decision);

    fout << result.sum << endl;
    for (int j = 0; j < numberOfPost; ++j) {
        fout << result.way[j] << " ";
    }

    return 0;
}


