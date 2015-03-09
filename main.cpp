#include <iostream>
#include "br.h" 
#include "matrica.h" 
#include "vektor.h"
#include <vector>
#include <string>
#include <list>
#include <deque>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <memory>

using namespace std;
template<typename T>
Vektor<T> jednadzba(Dijagonalna<T> A, Vektor<T> v)
{
	int i, n = A.redak();
	if (n != v.size()) {
		cout << "Dimenzije nisu dobro zadane! " << endl; 
		return v;
	}
	Vektor<T> ret(n);
	for (i = 0; i < n; ++i)
		ret[i] = v[i] / A[i][i];
	return ret;
}

template<typename T>
Vektor<T> jednadzba(Normalna<T> A, Vektor<T> v)
{
	int i, j;
	int n = A.redak();
	int k = n;
	if (n != v.size()) {
		cout << "Dimenzije nisu dobro zadane! " << endl;
		return v;
	}
	Normalna<T> B;
	B = A.inverz();
	Vektor<T> ret(n);
	for (i = 0; i<n; i++)
	for (j = 0; j<k; j++)
	{
		T a = ret[i];
		T b = B[i][j];
		T c = v[j];
		T d = b*c;
		ret[i] = a + d;
	}
	return ret;
}

int main()
{
	cout << "______________klasa Vektor______________"<< endl;
	Vektor<char> v1;
	Vektor<double> v2(6, 2.2);
	int array[5] = { 1, 5, 4, -6, 7 };
	Vektor<int> v3=array;
	vector <int> v4 = { 1, -34, 5, 2, 25, 1 };
	Vektor<int> v5 = v4;
	deque<char> v7(10, '3');
	Vektor<char> v6(v7);
	cout << "Vektor<char> v1  = " << v1 << endl;
	cout << "v1.size() =  " << v1.size() << endl;
	cout << "Dodajmo par elemenata vektoru v1! " << endl;
	v1.push_back('f');
	v1.push_back('g');
	v1.push_back('h');
	v1.push_back('z');

	cout << "Novi vektor v1 =  " << v1 << endl;
	cout << "Vektori koji su nastali raznom primjenom konstruktora i operatora= su sljedeci : " << endl;
	cout << "v2 = " << v2 << endl;
	cout << "v3 = " << v3 << endl;
	cout << "v5 = " << v5 << endl;
	cout << "v6 = " << v6 << endl;
	
	Vektor<double>k;
	k = v2;
	cout << "k = v2 , k = " << k << endl;
	cout << "Potom cemo malo promijeniti elemente vektora k ..." << endl;
	k[3] = 5.1;
	k[1] = -8.7;
	cout << "...i dodati nove elemente na kraj pomocu push.back() ... " << endl;
	double x = k[1];
	k.push_back(-3);
	k.push_back(5);
	k.push_back(7);
	k.push_back(1);
	k.push_back(-2);
	cout << "Novonastali k izgleda ovako : " << endl;
	cout << k << endl;
	cout << "duljina = " << k.size() << endl;
	cout << endl;
	cout << "double x = k[1] ----> " << x << endl;
	cout << "front = " << k.front() << endl;
	cout << "back = " << k.back() << endl;
	cout << "k.front() += k.back() = ";
	k.front() += k.back();
	cout << "Novi front = " << k.front() << endl;
	cout << "Sada izbacimo par elemenata s kraja pomocu pop.back() ... " << endl;
	k.pop_back();
	k.pop_back();
	cout << "... i ispisimo novonastali k ... " << endl;
	cout << k << endl;
	cout <<"Duljina od k = "  << k.size() << endl;
	cout << "Pokusajmo izbaciti ostale elemente Vektora k ... " << endl;
	k.pop_back(); 
	k.pop_back(); 
	k.pop_back(); 
	k.pop_back(); 
	k.pop_back(); 
	k.pop_back();
	k.pop_back();
	k.pop_back();
	k.pop_back();
	k.pop_back();
	cout << "Na kraju dobijemo k = " << k << endl;
	cout << endl;
	cout << "Dodajmo opet neke elemente radi demonstracije kako se k ponasa kada pristupamo nepoznatoj poziciji... " << endl;
	cout << "k.push_back(2)" << endl;
	cout << "k[0] = 7" << endl;
	cout << "k[10] = 11" << endl;
	cout << "k[6] = 3" << endl;
	k.push_back(2);
	k[0] = 7;
	k[10] = 11;
	k[6] = 3;
	cout <<"Za kraj imamo k = " << k << endl;
	
	cout << endl << endl;
	cout<< "______________klasa Broj______________" << endl;
	string q("345342");
	list <int> l; 
	l.push_back(1);
	l.push_back(3);
	l.push_back(7);
	l.push_back(2);
	l.push_back(0);
	l.push_back(1);
	vector<char> ra;
	deque<double> de = { 1, 7, 5, 0 };
	int netipsko_polje[4] = { 1, 9, 5, 0 };
	ra.push_back('-');
	ra.push_back('2');
	ra.push_back('4');
	ra.push_back('3');
	Broj b1=l;
	Broj b2("-10946346356");
	Broj b3 = q;
	Broj b4 = netipsko_polje;
	Broj b5(ra);
	Broj b6 = de;

	cout << "Brojevi koji su nastali raznom primjenom konstruktora i operatora= su sljedeci : " << endl;
	cout << "Broj b1 = l  ----> " << b1 << endl;
	cout << "Broj b2((''-10946346356''))  ----> " << b2 << endl;
	cout << "Broj b3 = q  ----> " << b3 << endl;
	cout << "Broj b4 = netipsko_polje  ----> " << b4 << endl;
	cout << "Broj b5 (ra)  ----> " << b5 << endl;
	cout << "Broj b6 = de  ----> " << b6 << endl;
	
	cout << "b2.duljina() = " << b2.duljina() << endl;
	cout << "Demonstrirajmo racunske operacije nad klasom Broj : " << endl;
	cout << "Ulancano zbrajanje : b1+b2+b6 = " << b1 + b2 + b6 << endl;
	cout << "Ulancano oduzimanje : b1-b4-b5 = " << b1 - b4 -b5 << endl;
	cout << "Ulancano mnozenje : b1*b2*b6 = " << b1*b2*b6 << endl;
	cout << "Ulancano dijeljenje : b2/b1/b6 = " << b2 / b1 / b6 << endl;
	cout << "Ulancane operacije : b1*b5-b3+b6/b4+b2  = " << b1*b5 - b3 + b6 / b4 + b2<< endl;
	
	cout << "Provjerimo sada rad operatora[] : " << endl;
	cout << "b4 = " << b4 << endl;
	cout << "b4[1] = 7 ------>";
	b4[1] = 7;
	cout << b4 << endl;
	cout << "b4[7] = 2 ------>";
	b4[7] = 2; 
	cout << b4 << endl;
	cout << "b4[3] = 0 ------>";
	b4[3] = 0;
	cout << b4 << endl;
	cout << "b4[10] = 7 ------>";
	b4[10] = 7; 
	cout << b4 << endl;
	cout << "b4[10] = 0 ------>";
	b4[10] = 0; 
	cout << b4 << " ima duljinu " << b4.duljina() << endl;
	cout << "int y = b4[10] ------> y = ";
	int y = b4[10];
	cout << y << endl;
	cout << "int z = b4[1] ------> z = ";
	int z = b4[1];
	cout << z << endl;

	cout << endl << endl ;
	cout << "______________klasa Normalna______________" << endl;
	double m1[5][5];
	
	m1[0][0] = 3; m1[0][1] = 1; m1[0][2] = -5; m1[0][3] = 8; m1[0][4] = 8;
	m1[1][0] = -3; m1[1][1] = 14; m1[1][2] = 9; m1[1][3] = 24; m1[1][4] = 2;
	m1[2][0] = 2; m1[2][1] = -5; m1[2][2] = 34; m1[2][3] = 35; m1[2][4] =-28;
	m1[3][0] = 7; m1[3][1] = 6; m1[3][2] = 42; m1[3][3] = -6; m1[3][4] = 1;
	m1[4][0] = -3; m1[4][1] = 16; m1[4][2] = 2; m1[4][3] = 1; m1[4][4] = 0;

	list<double> o1 = { 3, 1, -5, 8, -3, 14, 9, 24, 2, -5, 34, 35, 7, 6, 42, -6};
	deque<double> o2 = { 4, 76, 7, 35, 5, 73, 12, 42, 4 };
	vector<double> o3 = { 1, 7, 4, 33, 2, 4, 5, 7, 9, 9, 6, 0 };
	Normalna<double> n1(o1);
	Normalna<double> n2(o2);
	Normalna<double> n3(o3);
	Normalna<double> n4(m1);
	Normalna<double> n5;
	
	cout<<"n1 = "<<endl;
	cout<<n1 <<endl;
	cout << "n2 = " << endl;
	cout << n2 << endl;
	cout << "n3 = " << endl;
	cout << n3 << endl;
	cout << "n4 = " << endl;
	cout << n4 << endl;
	cout << "Mnozenje matrica n1 i n3 : " << endl;
	cout << n1*n3 << endl;
	
	cout << "Mnozenje matrica n2 i n3 : " << endl;
	cout << n2*n3 << endl;
	
	cout << "Zbrajanje matrica n1 i n3 : " << endl;
	cout << n1+n3 << endl;

	cout << "Oduzimanje matrica n1 i n3 : " << endl;
	cout << n1-n3 << endl;

	cout << "Inverz matrice n1 =  : " << endl;
	cout << n1.inverz() << endl;
	n5 = n1;
	cout << "Provjera je li inverz dobar (  n1*ni.inverz()  ) : " << endl;
	cout << n5*n1.inverz() << endl;
	cout << "Ocito je da radi dobro ako izuzmemo nepreciznost u mnozenju double brojeva ... " << endl;

	cout << endl <<  endl;
	cout << "______________klasa Dijagonalna______________" << endl;
	double m2[5][5];

	m2[0][0] = 3; m2[0][1] = 0; m2[0][2] = 0; m2[0][3] = 0; m2[0][4] = 0;
	m2[1][0] = 0; m2[1][1] = 4; m2[1][2] = 0; m2[1][3] = 0; m2[1][4] = 0;
	m2[2][0] = 0; m2[2][1] = 0; m2[2][2] = 9; m2[2][3] = 0; m2[2][4] = 0;
	m2[3][0] = 0; m2[3][1] = 0; m2[3][2] = 0; m2[3][3] = -5; m2[3][4] = 0;
	m2[4][0] = 0; m2[4][1] = 0; m2[4][2] = 0; m2[4][3] = 0; m2[4][4] = 6;

	list<double> p1 = { 3, 0, 0, 0, 0, 14, 0, 0, 0, 0, 34, 0, 0, 0, 0, -6 };
	deque<double> p2 = { -4, 0, 0, 0, 5, 0, 0, 0, 4 };
	vector<double> p3 = { 1, 7, 4, 33, 2, 4, 5, 7, 9, 9, 6, 0 };
	Dijagonalna<double> d1(p1);
	Dijagonalna<double> d2(p2);
	Dijagonalna<double> d3(p3);
	Dijagonalna<double> d4(m2);
	Dijagonalna<double> d5=d4;
	Dijagonalna<double> d6;
	
	cout << "d1 = " << endl;
	cout << d1 << endl;
	cout << "d2 = " << endl;
	cout << d2 << endl;
	cout << "d3 = " << endl;
	cout << d3 << endl;
	cout << "d4 = " << endl;
	cout << d4 << endl;
	cout << "Mnozenje matrica d4 i d5 : " << endl;
	cout << d4*d5 << endl;

	cout << "Mnozenje matrica d2 i d4 : " << endl;
	cout << d2*d4 << endl;

	cout << "Zbrajanje matrica d4 i d5 : " << endl;
	cout << d4 + d5 << endl;

	cout << "Oduzimanje matrica d4 i d5 : " << endl;
	cout << d4 - d5 << endl;

	cout << "Inverz matrice d4 =  : " << endl;
	cout << d4.inverz() << endl;

	cout << "Provjera je li inverz dobar (   d4*d4.inverz()  ) : " << endl;
	cout << d5*d4.inverz() << endl;
	
	cout << endl << endl ;
	cout << "_________Rjesenje sustava jednadzbi zadanog matricom i vektorom_________" << endl;
	float m3[3][3];
	m3[0][0] = 2; m3[0][1] = 4; m3[0][2] = 1;
	m3[1][0] = 1; m3[1][1] = 1; m3[1][2] = 1;
	m3[2][0] = 0; m3[2][1] = 3; m3[2][2] = 2;
	Normalna<float> rj1(m3);
	float vek[3] = { 6, 2, 4 };
	Vektor<float> rj2(vek);
	Vektor<float> rj3;
	cout << "Normalna matrica jednadzbe : " << endl;
	cout << rj1 << endl;
	cout << "Vektor jednadzbe : " << endl;
	cout << rj2 << endl;
	rj3 = jednadzba(rj1, rj2);
	cout << "Rjesenje jdbe je vektor : " << rj3 << endl;
	cout << endl << endl;
	cout << "_________Rjesenje sustava jednadzbi zadanog matricom i vektorom_________" << endl;
	float m4[3][3];
	m4[0][0] = 2; m4[0][1] = 0; m4[0][2] = 0;
	m4[1][0] = 0; m4[1][1] = 1; m4[1][2] = 0;
	m4[2][0] = 0; m4[2][1] = 0; m4[2][2] = 2;
	Dijagonalna<float> rj4(m4);
	float vekt[3] = { 6, 2, 4 };
	Vektor<float> rj5(vekt);
	Vektor<float> rj6;
	cout << "Normalna matrica jednadzbe : " << endl;
	cout << rj4 << endl;
	cout << "Vektor jednadzbe : " << endl;
	cout << rj5 << endl;
	rj6 = jednadzba(rj4, rj5);
	cout << "Rjesenje jdbe je vektor : " << rj6 << endl;
	
	system("pause");
	return 0;
}
