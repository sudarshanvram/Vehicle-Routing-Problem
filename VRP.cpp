#include<iostream>
#include<cstdlib>
#include<fstream>
#include<time.h>


using namespace std;
float randomseed = 3;
int routed[600] = {0};
int extreme, a = 40;
float dist[601][601];
int demand[601];
int v;
const float BIG = 100000;
struct route {
	int R[40];
} routes[40], globalbest[40];
float globalcost = 999999;



//Please refer README for more info on the below functions:

int seeder(int array[]);
int calculatelength(route);
void routeinitializer(route&, int);
float calculatecost(route);
int feasibility(route);
float insert(route, int, int);
float permutation(route, int);
int mininsertionposition(route, int);
int minroute(route[], int);
float inspriority(route[], int);
int selectcustomer();
int check(int);
void routedcustomer(int);
void changeroute(route& , int, int);
float updateglobal(route&, route);
float calculatetimecost(route);
float times(route);
float demands(route);
void intrarouteimprovement(route&);
float calculategainintra(route, int, int);
void changer(route&, int, int);
void interrouteimprovement(route& p, route& q);
float calculategain(route p, route q, int petro1[], int petro2[], int edge1, int edge2);
void deletion(route&t, int edge );
void insert(route&t, int customer);
void changerinter(route &p, route &q, int petro1[], int petro2[], int edge1, int edge2);


int main() {
	ifstream infile, filein;
	ofstream fileout, filesout;
	fileout.open("outputone.txt");
	filesout.open("outputtwo.txt");
	infile.open("dataone.txt");
	for (int po = 0; po < 601; po++) {
		for (int qo = 0; qo < 601; qo++) {
			infile >> dist[po][qo];
		}
	}


	infile.close();

	filein.open("datatwo.txt");
	int jo;
	for (jo = 0; jo < 601; jo++) {
		filein >> demand[jo];
	}


	filein.close();
	int i, p, q, t, demand_sum = 0;
	float r = 0;
	for (i = 0; i < 601; i++) {
		demand_sum += demand[i];
	}
	v = (demand_sum / 1000) + 1;
	cout << "the number of seeds are " << v << endl;
	int seed[29] = {0};
	for (p = 0; p < 601; p++) {
		if (dist[0][p] > r) {
			r = dist[0][p];
		}
	}
	for (p = 1; p < 601; p++) {
		if (dist[0][p] == r) {
			seed[0] = p;
		}
	}
	routedcustomer(seed[0]);
	for (q = 1; q < v; q++) {
		seed[q] = seeder(seed);
	}
	cout << "the initial seeds are" << endl;
	for (int d = 0; d < v; d++) {
		cout << "seed" << d << "= " << seed[d] << endl;
	}
	//construction starts from here..
	//after calculation of seed customers..
	int count = 1;

	while (count != 10) {

		for (int e = 0; e < 600; e++) {
			routed[e] = 0;
		}
		for (int w = 0; w < v; w++) {
			routed[w] = seed[w];
		}


		for (int h = 0; h < v; h++) {
			routeinitializer(routes[h], seed[h]);  //for reference copy the struct variable should b global..
		}

		extreme = v;
		int cus;
		int oppa = 0;
		while (oppa != 571) {
			cus = selectcustomer();
			if (cus != 0) {
				int tion = minroute(routes, cus);
				int tata = mininsertionposition(routes[tion], cus);
				changeroute(routes[tion], tata, cus);

				routedcustomer(cus);
			}

			oppa++;
		}

		for (int me = 0; me < 600; me++) {
			filesout << routed[me] << endl;
		}

		float totaldistance = 0;
		for (int fiesta = 0; fiesta < extreme; fiesta++) {
			totaldistance = totaldistance + calculatecost(routes[fiesta]);
		}

		cout << " result of iteration " << count << " is " << totaldistance << " with feasibility " << check(0) << "and no of R's " << extreme << endl;



		for (int lo = 0; lo < 600; lo++) {
			for (int e = 0; e < extreme; e++) {

				for (int f = 0; f < extreme; f++)

				{
					if (e == f) {

						intrarouteimprovement(routes[e]);
					} else {
						int length1 = 0, length2 = 0, length3 = 0;
						length1 = calculatelength(routes[e]);
						length2 = calculatelength(routes[f]);
						length3 = length1 + length2;
						if (length3 < 39) {
							interrouteimprovement(routes[e], routes[f]);
						}
					}
				}
			}
		}



		totaldistance = 0;
		for (int fiesta = 0; fiesta < extreme; fiesta++) {
			totaldistance = totaldistance + calculatecost(routes[fiesta]);
		}
		cout << " result of iteration after improvement " << count << " is " << totaldistance << " with feasibility " << check(0) << "and no of R's " << extreme << endl;


		if (totaldistance < globalcost) {
			if (extreme <= a) {
				globalcost = 0;
				for (int benz = 0; benz < extreme; benz++) {
					a = extreme;
					globalcost = globalcost + updateglobal(globalbest[benz], routes[benz]);
				}
			}
		}
		count++;

	}
	cout << endl << "the global best route is" << endl;
	for (int aveo = 0; aveo < a; aveo++) {
		int ford = calculatelength(globalbest[aveo]);
		cout << endl;
		fileout << endl;
		cout << "the fees of route " << aveo << " is " << feasibility(globalbest[aveo]);
		cout << " the distance of route " << aveo << " is " << calculatecost(globalbest[aveo]);
		cout << " the length of route " << aveo << " is " << calculatelength(globalbest[aveo]);
		cout << " the time for route " << aveo << " is " << times(globalbest[aveo]);
		cout << " the demend for route " << aveo << " is " << demands(globalbest[aveo]);
		for (int uva = 0; uva < ford; uva++) {
			cout << "  " << globalbest[aveo].R[uva];
			fileout << "  " << globalbest[aveo].R[uva];
		}
	}

	cout << endl << "global best cost is " << globalcost;

	fileout.close();
	filesout.close();

	system("pause");
	return 0;
}





int seeder(int array[]) {       //function calculating and returning seeds..

	int q, n, j, m = 0, x, y = 0, s;
	float min, max_array[600] = {0}, max = 0;
	while (array[m] != 0) {
		y++;
		m++;
	}
	for (n = 1; n < 601; n++) {
		if (check(n) == 0) {
			min = 10000;
			for (j = 0; j < y; j++) {
				if (dist[array[j]][n] < min) {
					min = dist[array[j]][n];
				}
			}
			max_array[n - 1] = min;
		}
	}
	for (x = 0; x < 600; x++) {
		if (max_array[x] > max) {
			max = max_array[x];
		}
	}
	for (j = 0; j < y; j++) {
		for (q = 1; q < 601; q++) {
			if (check(q) == 0) {
				if (dist[array[j]][q] == max) {
					s = q;
				}
			}
		}
	}
	routedcustomer(s);
	return s;
}



int calculatelength(route l) { //a function to calculate length..
	int i = 1;
	int p = 2;
	while (l.R[i] != 0) {
		i++;
		p++;
	}
	return p;
}


void routeinitializer(route&o, int x) { //a func which initializes the routes.
	int i = 0;
	o.R[i] = 0;
	i++;
	o.R[i] = x;
	i++;
	o.R[i] = 0;

}


float calculatecost(route p) { // a function to calculate cost..
	int i, j, l;
	float cost = 0;
	l = calculatelength(p);


	for (i = 0, j = 1; i < (l - 1), j < l; i++, j++) {
		cost = cost + dist[p.R[i]][p.R[j]];
	}

	return cost;
}

float times(route t) {
	int l = calculatelength(t);
	float costs, time, timecost;
	timecost = calculatetimecost(t);
//costs=calculatecost(t);
	time = (0.08333333 * (l - 2) + 0.025 * timecost);
	return time;
}

float demands(route t) {
	float f = 0;
	int i;
	int l = calculatelength(t);
	for (i = 0; i < l; i++) {
		f = f + demand[t.R[i]];
	}
	return f;
}


int feasibility(route t) {  //a func to check feasibility..
	int f = 0;
	int i, spider, constraint1 = 0, constraint2 = 0, constraint3 = 0, constraint4 = 0;
	int l = calculatelength(t);
	float costs, time, timecost;
	timecost = calculatetimecost(t);
	costs = calculatecost(t);
	time = (0.08333333 * (l - 2) + 0.025 * timecost);
	for (i = 0; i < l; i++) {
		f = f + demand[t.R[i]];
	}
	if (f <= 1000) {
		constraint1 = 1;
	}
	if ((l - 2) <= 35) {
		constraint2 = 1;
	}
	if (costs <= 65) {
		constraint3 = 1;
	}
	if (time <= 3) {
		constraint4 = 1;
	}
	spider = (constraint1 + constraint2 + constraint3 + constraint4);
	if (spider == 4) {
		return 0;
	} else {
		return 1;
	}
}

float calculatetimecost(route p) { // a function to calculate cost..
	int i, j, l;
	float cost = 0;
	l = calculatelength(p);
	for (i = 0, j = 1; i < (l - 2), j < (l - 1); i++, j++) {
		cost = cost + dist[p.R[i]][p.R[j]];
	}
	return cost;
}



float insert(route t, int position, int customer) { //func to insert a customer
	//in agiven position in a
	//given route and returns
	float d1 = calculatecost(t);		     //insertion cost..
	int l = calculatelength(t);

	for (int c = (l - 1); c >= (position - 1); c--) {
		t.R[c + 1] = t.R[c];
	}
	t.R[position - 1] = customer;

	float d2 = calculatecost(t);
	int i = feasibility(t);
	float d3 = (d2 - d1);
	if (i == 0) {
		return d3;
	} else {
		return BIG;
	}
}



float permutation(route t, int customer) { //a func which tries all possible positions in a given route for a customer and returns the min insertion cost..
	int l = calculatelength(t);
	float cost[40] = {0};
	for (int position = 2; position <= l; position++) {
		cost[position - 2] = insert(t, position, customer);
	}
	float min = 1000000;
	for (int i = 0; i < (l - 1); i++) {
		if (cost[i] < min) {
			min = cost[i];
		}
	}
	return min;
}



int mininsertionposition(route t, int customer) { //a func which returns min insrtn cost position..
	int l = calculatelength(t);
	float cost[40] = {0};
	for (int position = 2; position <= l; position++) {
		cost[position - 2] = insert(t, position, customer);
	}
	float min = 1000000;
	for (int i = 0; i < (l - 1); i++) {
		if (cost[i] < min) {
			min = cost[i];
		}
	}
	int h;
	for (int j = 0; j < (l - 1); j++) {
		if (cost[j] == min) {
			h = j + 2;
		}
	}
	return h;
}



int minroute(route c[], int customer) { //wait a minute..brilliant!!

	float a[40];
	for (int h = 0; h < 40; h++) {
		a[h] = 100000;
	}

	for (int j = 0; j < extreme; j++) { // try replacing v with another global variable..for u know what..
		a[j] = permutation(c[j], customer);
	}
	float min = 999999;
	for (int q = 0; q < extreme; q++) {
		if (a[q] < min)
		{min = a[q];}

	}
	int y;
	for (int k = 0; k < 40; k++) {
		if (a[k] == min) {
			y = k;
		}
	}
	return y;
}



float inspriority(route c[], int customer) { //a func which returns inspriority..

	float a[40];
	for (int h = 0; h < 40; h++) {
		a[h] = 100000;
	}

	for (int j = 0; j < extreme; j++) { // try replacing v with another global variable..for u know what..
		a[j] = permutation(c[j], customer);
	}
	float min = 999999;
	for (int q = 0; q < extreme; q++) {
		if (a[q] < min)
		{min = a[q];}
	}
	float priority = 0;
	for (int n = 0; n < extreme; n++) {
		priority = priority + (a[n] - min);
	}
	return priority;
}



int check(int customer) {         //checks whether a customer is already routed..
	int flag = 0;
	for (int i = 0; i < 600; i++) {
		if (routed[i] == customer) {
			flag = 1;
		}
	}
	if (flag == 1) {
		return 1;
	} else {
		return 0;               //returns 0 if not routed..
	}
}



int selectcustomer() {    //function to select a customer...
	int c;
	int p = 0;
	float pro[601];
	for (int k = 0; k < 601; k++) {
		pro[k] = -99;
	}

	float a[601];
	for (int f = 0; f < 601; f++) {
		a[f] = -98;
	}
	float sumo = 1;
	int flag = 0;
	for (int customer = 1; customer <= 600; customer++) {
		c = check(customer);

		if (c == 0) {

			pro[p] = inspriority(routes, customer);
			a[customer] = pro[p];
			p++;
		}
	}

	if (p != 0) {
		int iteration, index;
		float temp;
		for (iteration = 1; iteration < p; iteration++) {
			for (index = 0; index < (p - iteration); index++) {
				if (pro[index] < pro[index + 1]) {
					temp = pro[index];
					pro[index] = pro[index + 1];
					pro[index + 1] = temp;
				}
			}
		}
		float ptr[600] = {0};
		int w;
		float ya;
//srand(randomseed);
		srand(time(NULL));
		int ra = (rand() % 100) + 1;
		ya = (ra * 0.01);
		w = (ya * p);
		if (w < 100) {
			w = p;
		}

		float random;

		for (int n = 0; n < w; n++) {
			ptr[n] = pro[n];
		}
		random = ptr[rand() % w];

		for (int l = 0; l < 601; l++) {
			if (a[l] == random) {
				flag = l;
			}
		}
		randomseed = randomseed++;
	}

	return flag;
}


void routedcustomer(int customer) {  //a function to insert a customer into routed list..
	for (int n = 0; n < 600; n++) {
		if (routed[n] == 0) {
			routed[n] = customer;
			break;
		}
	}
}



void changeroute(route& t, int position, int customer) { //func to make permanant changes..

	int l = calculatelength(t);

	for (int c = (l - 1); c >= (position - 1); c--) {
		t.R[c + 1] = t.R[c];
	}
	t.R[position - 1] = customer;

}



float updateglobal(route& global, route current) { //updates global best routes..

	int ford = calculatelength(current);
	for (int uva = 0; uva < ford; uva++) {
		global.R[uva] = current.R[uva];
	}

	float totaldistance = 0;
	totaldistance = calculatecost(global);

	return totaldistance;
}




void intrarouteimprovement(route&p) { //for intraroute improvements..
	int l = calculatelength(p);

	float darray[40][40];
	for (int row = 0; row < 40; row++) {
		for (int col = 0; col < 40; col++) {
			darray[row][col] = 0;
		}
	}
	float check;
	for (int i = 1; i < (l - 2); i++) {
		for (int j = 1; j < (l - 2); j++) {

			darray[i - 1][j - 1] = calculategainintra(p, i, j);

		}
	}
	int wow, now;
	float max = -999999;
	for (int row = 0; row < (l - 2); row++) {
		for (int col = 0; col < (l - 2); col++) {
			if (darray[row][col] > max) {
				max = darray[row][col];
				wow = row + 1;
				now = col + 1;
			}
		}
	}

	if (max > 0) {
		changer(p, wow, now);
	}
}




float calculategainintra(route t , int i, int j)

{
	int fees = 1;
	float d3 = 0;
	float d1 = 0;
	float d2 = 0;
	if ((j - i) > 1) {

		int temp;
		d1 = calculatecost(t);
		for (int k = i, g = j; (g - k) > 1; k++, g--) {

			temp = t.R[k + 1];
			t.R[k + 1] = t.R[g];
			t.R[g] = temp;

		}
		d2 = calculatecost(t);

		if ((d1 - d2) > 0) {

			fees = feasibility(t);

		}
		d3 = (d1 - d2);

	}
	if ((fees == 0)) {
		return d3;
	} else {
		return -9;
	}
}





void changer(route&t, int i, int j) {
	float temp;
	for (int k = i, g = j; (g - k) > 1; k++, g--) {

		temp = t.R[k + 1];
		t.R[k + 1] = t.R[g];
		t.R[g] = temp;
	}
}




void interrouteimprovement(route& p, route& q) {
	int l1, l2;
	float check;
	l1 = calculatelength(p);
//cout<<" length 1 "<<l1;
	l2 = calculatelength(q);
//cout<<" length 2 "<<l2;
	int ptr1[100] = {0};
	int ptr[100] = {0};
	float darray[100][100];
	for (int row = 0; row < 100; row++) {
		for (int col = 0; col < 100; col++) {
			darray[row][col] = -1000;
		}
	}


	for (int edge1 = 1; edge1 < l1 - 1; edge1++) {
		for (int edge2 = 1; edge2 < l2 - 1; edge2++)

		{


			for (int a = 0, b = (edge1 - 1); a<edge1, b>0; a++, b--) {
				ptr1[a] = p.R[b];
			}
			ptr1[edge1 - 1] = 0;
			for (int x = 0, z = (edge2 - 1); x<edge2, z>0; x++, z--) {
				ptr[x] = q.R[z];
			}
			ptr[edge2 - 1] = 0;

			darray[(edge1 - 1)][(edge2 - 1)] = calculategain(p, q, ptr1, ptr, edge1, edge2);

		}
	}



	int wow, now;
	float max = -999999;
	for (int row = 0; row < (l1 - 1); row++) {
		for (int col = 0; col < (l2 - 1); col++) {
			if (darray[row][col] > max) {
				max = darray[row][col];
				wow = row + 1;
				now = col + 1;
			}
		}
	}

	for (int a = 0, b = (wow - 1); a<wow, b>0; a++, b--) {
		ptr1[a] = p.R[b];
	}
	ptr1[wow - 1] = 0;
	for (int x = 0, z = (now - 1); x<now, z>0; x++, z--) {
		ptr[x] = q.R[z];
	}
	ptr[now - 1] = 0;

	changerinter(p, q, ptr1, ptr, wow, now);

}



float calculategain(route p, route q, int petro1[], int petro2[], int edge1, int edge2) {

	int l1, l2;
	float actualcost, c1, c2, modifiedcost, cm1, cm2, gain;
	c1 = calculatecost(p);
	c2 = calculatecost(q);
	l1 = calculatelength(p);
	l2 = calculatelength(q);

	actualcost = (c1 + c2);
	deletion(p, edge1);
	deletion(q, edge2);

	int c, d, length1, length2;

	for (int y = 0; y < edge2; y++) {
		c = petro2[y];
		insert(p, c);
	}

	for (int y = 0; y < edge1; y++) {
		d = petro1[y];
		insert(q, d);
	}

	length1 = calculatelength(p);
	length2 = calculatelength(q);
	cm1 = calculatecost(p);
	cm2 = calculatecost(q);
	modifiedcost = (cm1 + cm2);
	gain = (actualcost - modifiedcost);
	int fees1 = 0;
	int fees2 = 0;
	int summer = 0;
	fees1 = feasibility(p);
	fees2 = feasibility(q);
	summer = (fees1 + fees2);
	if (summer == 0) {
//cout<<"gain is "<<gain<<endl;
		return gain;
	} else {
		return -99;
	}


}


void deletion(route&t, int edge ) {
	int l;
	l = calculatelength(t);

	int count = 0;
	while (count != edge) {

		for (int c = 0; c < (l - 1); c++) {
			t.R[c] = t.R[c + 1];
		}
		count++;
	}
}



void insert(route&t, int customer) {
	int l = calculatelength(t);
	for (int c = (l - 1); c >= 0; c--) {
		t.R[c + 1] = t.R[c];
	}
	t.R[0] = customer;
}



void changerinter(route &p, route &q, int petro1[], int petro2[], int edge1, int edge2) {
	int l1, l2;
	float actualcost, c1, c2, modifiedcost, cm1, cm2, gain;
	c1 = calculatecost(p);
	c2 = calculatecost(q);
	l1 = calculatelength(p);
	l2 = calculatelength(q);

	actualcost = (c1 + c2);
	deletion(p, edge1);
	deletion(q, edge2);

	int c, d, length1, length2;

	for (int y = 0; y < edge2; y++) {
		c = petro2[y];
		insert(p, c);
	}

	for (int y = 0; y < edge1; y++) {
		d = petro1[y];
		insert(q, d);
	}
}


