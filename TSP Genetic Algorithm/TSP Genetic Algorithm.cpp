/*
This program is written in C++
Goal of this program: Solve the Traveling-Salesman Problem
Approach: Genetic Algorithm
Some important note:
1. I set the population size for each generation = 31. 
	15 will be kept by Tournament selection, 
	15 will be generated by single-point crossover, 
	1 will be generated by swap mutation
2. After a generation is randomly produced, 100 runs of genetic algorithm will be applied
3. On top that, I also add another while loop to look for a local minimum, the step size = 5

For example, 
	if the shortest path after 100 runs of genetic algorithm is 12000 (path(100)), it'll also look the shortest path after 105 runs (path(105))
	if path(105) < path(100), look path(110)
	if path(110) < path(105), look path(115), etc
	if path(110) >= path(105), output path(105)

*/


#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

static vector<string> cities{ "Atlanta", "Boston", "Chicago", "Dallas", "Denver", "Houston", "Las Vegas",
					"Los Angeles", "Miami", "New Orleans", "New York", "Pheonix", "San Francisco",
					"Seattle", "Washington D.C"};

static int distance_value[15][15]={
	{0, 1095, 715, 805, 1437, 844, 1920, 2230, 675, 499, 884, 1832, 2537, 2730, 657},
	{1095, 0, 983, 1815, 1991, 1886, 2500, 3036, 1539, 1541, 213, 2664, 3179, 3043, 44},
	{715, 983, 0, 931, 1050, 1092, 1500, 2112, 1390, 947, 840, 1729, 2212, 2052, 695},
	{805, 1815, 931, 0, 801, 242, 1150, 1425, 1332, 504, 1604, 1027, 1765, 2122, 1372},
	{1437, 1991, 1050, 801, 0, 1032, 885, 1174, 2094, 1305, 1780, 836, 1266, 1373, 1635},
	{844, 1886, 1092, 242, 1032, 0, 1525, 1556, 1237, 365, 1675, 1158, 1958, 2348, 1143},
	{1920, 2500, 1500, 1150, 885, 1525, 0, 289, 2640, 1805, 2486, 294, 573, 1188, 2588},
	{2230, 3036, 2112, 1425, 1174, 1556, 289, 0, 2757, 1921, 2825, 398, 403, 1150, 2680},
	{675, 1539, 1390, 1332, 2094, 1237, 2640, 2757, 0, 892, 1328, 2359, 3097, 3389, 1101},
	{499, 1541, 947, 504, 1305, 365, 1805, 1921, 892, 0, 1330, 1523, 2269, 2626, 1098},
	{884, 213, 840, 1604, 1780, 1675, 2486, 2825, 1328, 1330, 0, 2442, 3036, 2900, 229},
	{1832, 2664, 1729, 1027, 836, 1158, 294, 398, 2359, 1523, 2442, 0, 800, 1482, 2278},
	{2537, 3179, 2212, 1765, 1266, 1958, 573, 403, 3097, 2269, 3036, 800, 0, 817, 2864},
	{2730, 3043, 2052, 2122, 1373, 2348, 1188, 1150, 3389, 2626, 3900, 1482, 817, 0, 2755},
	{657, 440, 695, 1372, 1635, 1443, 2568, 2680, 1101, 1098, 229, 2278, 2864, 2755, 0}
};

//use to randomize (initialize) cities. The approach is: generate a series of random number, sort it in order, and use the index as the order
void mysort(vector<pair<int, int>>& tosort) {
	int temp_rand;
	int temp_city;
	for (auto i = tosort.begin(); i != tosort.end(); i++) {
		for (auto j = i + 1; j != tosort.end(); j++) {
			if ((j->second) < (i->second)) {
				temp_rand = j->second;
				j->second = i->second;
				i->second = temp_rand;

				temp_city = j->first;
				j->first = i->first;
				i->first = temp_city;
			}
		}
	}
}

//chromosome class, basically a circuit of cities
class chromosome {
public:
	//path of cities, the second element of pair is for random generating and selection
	vector<pair<int, int>> path;
	int fitness = 0;

	chromosome(){
		for (int i = 0; i < 15; i++) {
			path.push_back(make_pair(i, 0));
		}
	}

	//generate random path and calculate the total distance
	void generate_path() {
		for (int i = 0; i < 15; i++) {
			path[i].second=rand();
		}
		mysort(path);
		calc_fitness();
	}

	//for calculating distance
	void calc_fitness() {
		fitness = 0;
		for (int i = 0; i < 15; i++) {
			if (i == 14) {
				fitness += distance_value[path[i].first][path[0].first];
				break;
			}
			fitness += distance_value[path[i].first][path[i + 1].first];
		}
	}

	int size() {
		return path.size();
	}

	void copy(chromosome to_copy) {
		path.clear();
		for (auto i = to_copy.path.begin(); i != to_copy.path.end(); i++) {
			path.push_back(*i);
		}
		fitness = to_copy.fitness;
	}

	void clear() {
		path.clear();
		fitness = 0;
	}

	//for mutation
	void swap_cities() {
		int rand1 = rand() % 15;
		int rand2 = rand() % 15;
		while (rand1 == rand2) {
			rand2 = rand() % 15;
		}
		int temp = path[rand1].first;
		path[rand1].first = path[rand2].first;
		path[rand2].first = temp;
		calc_fitness();
	}

	void add_city(int i) {
		path.push_back(make_pair(i, 0));
	}

	void reverse() {
		path.reserve(15);
	}

	int get_city(int i) {
		return path[i].first;
	}

	void print_path() {
		for (auto i = path.begin(); i != path.end(); i++) {
			cout << cities[i->first]<<"-->";
		}
		cout << cities[path[0].first];
		cout << endl << "Total distance: " << fitness<<endl;
	}

};

//for sorting the population based on fitness function
void sort_by_fit(vector<chromosome>& to_sort) {
	chromosome temp;
	for (auto i = to_sort.begin(); i != to_sort.end(); i++) {
		for (auto j = i + 1; j != to_sort.end(); j++) {
			if (((*j).fitness) < ((*i).fitness)) {
				temp.copy(*j);
				(*j).copy(*i);
				(*i).copy(temp);
			}
		}
	}
}

//basically a group of chromosomes
class population {
public:

	vector<chromosome> generation;
	vector<chromosome> generation_temp; //for selection only
	vector<chromosome> generation_next;

	//I set the initial population size = 31
	population() {
		for (int i = 0; i < 31; i++) {
			chromosome temp;
			temp.generate_path();
			generation.push_back(temp);
		}
	}

	void sort_population() {
		sort_by_fit(generation);
	}

	int get_first_fit() {
		return generation[0].fitness;
	}

	chromosome get_first_chromosome() {
		return generation[0];
	}

	void print_first_path() {
		generation[0].print_path();
	}

	//method: Tournament selection
	//choose 10 chromosomes randomly, keep 5 with smallest fitness value, repeat 3 times (selection rate 50%)
	void selection() {
		for (int count = 0; count < 3; count++) {

			//generate 31 random pairs
			vector<pair<int, int>> random_pair;
			for (int i = 0; i < 31; i++) {
				random_pair.push_back(make_pair(i, rand()));
			}
			mysort(random_pair);
			
			//select 10
			for (int i = 0; i < 10; i++) {
				generation_temp.push_back(generation[random_pair[i].first]);
			}
			sort_by_fit(generation_temp);

			//keep the top 5 with the smallest fitness function
			for (int i = 0; i < 5; i++) {
				generation_next.push_back(generation_temp[i]);
			}
			generation_temp.clear();
		}
	}

	//method: Single point crossover
	//randomly select 2 chromosome, randomly select 1 crossover points, repeat 15 times.
	void crossover() {
		for (int count = 0; count < 15; count++) {
			//selecting parents
			int parent1 = rand() % 31;
			int parent2 = rand() % 31;
			while (parent1 == parent2) {
				parent2 = rand() % 31;
			}

			//selecting crossover points
			int cross_point = rand() % 15;

			chromosome child;
			child.clear();
			map<int, int> copied;
			int p1_index = 0, p2_index = 0;

			//crossover
			while(child.size()<15) {
				if (p1_index < cross_point) {
					child.add_city(generation[parent1].get_city(p1_index));
					copied.insert(make_pair(generation[parent1].get_city(p1_index), 0));
					p1_index++;
				}
				else {
					// make sure no duplicate cities
					if (copied.find(generation[parent2].get_city(p2_index)) == copied.end()) {
						child.add_city(generation[parent2].get_city(p2_index));
						copied.insert(make_pair(generation[parent2].get_city(p2_index), 0));
						p2_index++;
					}
					else {
						p2_index++;
					}
				}
			
			}
			child.calc_fitness();
			generation_next.push_back(child);
		}
	}

	//swap 2 cities
	void mutation() {
		int temp = rand() % 15;
		chromosome mutated = generation[temp];
		mutated.swap_cities();
		generation_next.push_back(mutated);
	}

	void clear() {
		generation.clear();
	}

	void print_current() {
		for (auto i = generation.begin(); i != generation.end(); i++) {
			(*i).print_path();
			cout << endl;
		}
	}

	void print_next() {
		for (auto i = generation_next.begin(); i != generation_next.end(); i++) {
			(*i).print_path();
			cout << endl;
		}
	}

	//set generation = generation_next, generation_next = empty
	void generation_swap() {
		chromosome temp;
		for (int i = 0; i < 31; i++) {
			generation[i].copy(generation_next[i]);
		}
		generation_next.clear();
	}

};


int main()
{
	srand(time(NULL));
	population group;
	int generation_number = 100;

	group.sort_population();
	cout << "The minimum path in the initial population before applying genetic algorithm is: " <<endl;
	group.print_first_path();
	cout << endl;

	//genetic algorithm
	for (int i = 0; i < generation_number; i++) {
		group.selection();
		group.crossover();
		group.mutation();
		group.generation_swap();
	}
	group.sort_population();
	cout<< "The minimum path in the population after applying "<< generation_number<< " generations of genetic algorithm is: "<< endl;
	group.print_first_path();
	cout << endl;

	int before = group.get_first_fit();
	chromosome min_chromosome;
	min_chromosome.clear();
	min_chromosome.copy(group.get_first_chromosome());
	int after=0;

	//5 genetic algorithms per run, stop if after>=before
	do {

		if (min_chromosome.fitness > group.get_first_fit()) {
			min_chromosome.copy(group.get_first_chromosome());
		}

		for (int i = 0; i < 5; i++) {
			group.selection();
			group.crossover();
			group.mutation();
			group.generation_swap();
		}
		cout << "With 5 more generations, the minimum path in the population is: " << endl;
		group.sort_population();
		group.print_first_path();

		generation_number +=5;
		cout << endl;

	} while (group.get_first_fit() < min_chromosome.fitness);

	generation_number -= 5;

	cout << endl << "Final path:" << endl;
	min_chromosome.print_path();
	cout << "Total generation swaps: " << generation_number << endl;

}
