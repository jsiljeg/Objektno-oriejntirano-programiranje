#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <memory>

using namespace std;

class Matrica
{
	virtual int redak() = 0;
	virtual int stupac() = 0;
};

template<class T> class Normalna;

template <class T> ostream& operator<<(ostream& out, const Normalna<T>& A);
template <class T> Normalna<T> operator+(const Normalna<T>& A, const Normalna<T>& B);
template <class T> Normalna<T> operator-(const Normalna<T>& A, const Normalna<T>& B);
template <class T> Normalna<T> operator*(const Normalna<T>& A, const Normalna<T>& B);

template <class T> class Normalna : public Matrica
{
public:
	struct SmartNormalna
	{
		T* polje;
		int red, stup;
		int count_ref;

		SmartNormalna() : polje(new T[0]), red(0), stup(0), count_ref(0){}
		template <int m, int n>
		SmartNormalna(T(&mat)[m][n]) : count_ref(1), red(m), stup(n) { //ode
			polje = new T[red*stup];
			int i, j;
			for (i = 0; i<red; ++i)
			for (j = 0; j<stup; ++j)
				polje[i*red + j] = mat[i][j];
		}
		SmartNormalna(int m, int n) :count_ref(1), red(m), stup(n)
		{
			polje = new T[red*stup];
		}
		template<class A, template<class S, class ALLOC = std::allocator<S> > class CONTAINER>
		SmartNormalna(CONTAINER<A> &s) {
			int n = s.size(); // nadajmo se da smo dobili spremnik velicine n^2 jer drukcije nema smisla bas
			int tmp = n;
			int i, j, duljina, brojac=0;
			int l = (int)(sqrt(tmp));
			(l*l == n) ? duljina = l : duljina = l + 1;
			count_ref = 1;
			red = stup = duljina;
			polje = new T[duljina*duljina];
			CONTAINER<T>::iterator k = s.begin();
			for (i = 0; i<duljina; i++)
			for (j = 0; j<duljina; j++)
			{
				if (brojac >= n) polje[i*duljina + j] = ((T)0); //kopiranje nula u ostatak
				else {
					polje[i*duljina + j] = (*k);
					k++;
					++brojac;
				}
			}
		}

		~SmartNormalna(){ delete polje; }
	};

	SmartNormalna* matrix;

	struct pom {
	public:
		Normalna &A;
		int x;

		pom(Normalna &N, const int &y) : A(N), x(y) {}
		struct nova {
			Normalna &A;
			int M, N;
			nova(Normalna &B, const int &x, const int &y) : A(B), M(x), N(y) {}

			operator T() {
				return A.matrix->polje[M*(A.matrix->red) + N];
			}
			T& operator= (T c) {
				return A.matrix->polje[M*(A.matrix->red) + N] = c;
			}

		};

		nova operator[] (const int &y) { return nova(A, x, y); }

	};
	pom operator[] (const int &x) { return pom(*this, x); }

	//ctori...
	Normalna() : matrix(new SmartNormalna()) {}
	Normalna(int x, int y) : matrix(new SmartNormalna(x, y)) {}
	template <int m, int n>
	Normalna(T(&mat)[m][n]) : matrix(new SmartNormalna(mat)) {}
	template<class A, template<class S, class ALLOC = std::allocator<S> > class CONTAINER>
	Normalna(CONTAINER<A> &s) : matrix(new SmartNormalna(s)) {}

	//kontrola kopiranja
	Normalna& operator= (const Normalna &A) {

		if (matrix != A.matrix) {
			++A.matrix->count_ref;
			if (!--matrix->count_ref) delete matrix;
			matrix = A.matrix;
			return *this;
		}

		return *this;
	}

	Normalna(const Normalna &A) {
		matrix = A.matrix;
		++matrix->count_ref;
	}

	~Normalna() {
		if (!--matrix->count_ref) delete matrix;
	}
	// ode inverz obicne ubacit...algoritam je otprilike preuzet sa stranice :
	//	http://www.programming-techniques.com/2011/09/numerical-methods-inverse-of-nxn-matrix.html
	Normalna<T> inverz() {
		if (matrix->red != matrix->stup) {
			cout << "Matrica nije odgovarajucih dimenzija i ne postoji inverz!";
			return *this;
		}
		int redak = matrix->red;
		int stupac = matrix->stup;
		T ratio, a;
		int i, j, k;
		
		Normalna<T> N1(redak, stupac);
		Normalna<T> N2(redak, stupac);
		for (i = 0; i<redak; i++)
		for (j = 0; j<stupac; j++)
			N1.matrix->polje[i*redak + j] = matrix->polje[i*redak + j];
		for (i = 0; i<redak; i++)
		for (j = 0; j<stupac; j++)
		if (i == j) N2.matrix->polje[i*redak + j] = 1;
		else N2.matrix->polje[i*redak + j] = 0;
		for (i = 0; i<redak; i++)
		for (j = 0; j<stupac; j++)
		{
			if (i != j)
			{
				ratio = N1.matrix->polje[j*redak + i] / N1.matrix->polje[i*redak + i];
				for (k = 0; k<redak; k++)
				{
					N1.matrix->polje[j*redak + k] -= ratio*N1.matrix->polje[i*redak + k];
					N2.matrix->polje[j*redak + k] -= ratio*N2.matrix->polje[i*redak + k];
				}
			}
		}
		for (i = 0; i<redak; i++)
		{
			a = N1.matrix->polje[i*redak + i];
			for (j = 0; j<stupac; j++)
			{
				N1.matrix->polje[i*redak + j] /= a;
				N2.matrix->polje[i*redak + j] /= a;
			}
		}
		return N2;
	}

	int stupac()
	{
		return matrix->stup;
	}
	int redak()
	{
		return matrix->red;
	}
	
	friend ostream& operator<< <T>(ostream& out, const Normalna &A);
	friend Normalna operator+ <T>(const Normalna &A, const Normalna &B);
	friend Normalna operator- <T>(const Normalna &A, const Normalna &B);
	friend Normalna operator* <T>(const Normalna &A, const Normalna &B);



};

template <class T>
Normalna<T> operator+(const Normalna<T>& A, const Normalna<T>& B)
{
	int i, j;
	if ((A.matrix->red != B.matrix->red) || (A.matrix->stup != B.matrix->stup)) {
		cout << "Nedozvoljeno zbrajanje! Provjeriti dimenzije matrica!";
		return A;
	}
	int red = A.matrix->red;
	int stup = A.matrix->stup;
	Normalna<T> C(red, stup);
	for (i = 0; i<red; ++i)
	for (j = 0; j<stup; ++j)
		C.matrix->polje[red*i + j] = A.matrix->polje[red*i + j] + B.matrix->polje[red*i + j];
	return C;
}

template <class T>
Normalna<T> operator-(const Normalna<T>& A, const Normalna<T>& B)
{
	int i, j;
	if ((A.matrix->red != B.matrix->red) || (A.matrix->stup != B.matrix->stup)) {
		cout << "Nedozvoljeno oduzimanje! Provjeriti dimenzije matrica!";
		return A;
	}
	int red = A.matrix->red;
	int stup = A.matrix->stup;
	Normalna<T> C(red, stup);
	for (i = 0; i<red; ++i)
	for (j = 0; j<stup; ++j)
		C.matrix->polje[red*i + j] = A.matrix->polje[red*i + j] - B.matrix->polje[red*i + j];
	return C;
}

template <class T>
Normalna<T> operator*(const Normalna<T>& A, const Normalna<T>& B)
{
	int i, j, k;
	if (A.matrix->stup != B.matrix->red) {
		cout << "Nedozvoljeno mnozenje! Provjeriti dimenzije matrica!" << endl;
		return A;
	}

	int red = A.matrix->red;
	int stup = B.matrix->stup;
	int aaaaaaaaa = A.matrix->stup;
	Normalna<T> C(red, stup);

	for (i = 0; i<red; ++i)
	for (j = 0; j < stup; ++j)
	{
		C.matrix->polje[red*i + j] = ((T)0);
		for (k = 0; k < aaaaaaaaa; ++k)
			C.matrix->polje[red*i + j] = C.matrix->polje[red*i + j] + (A.matrix->polje[red*i + k] * B.matrix->polje[red*k + j]);
	}
	return C;
}

template <class T>
ostream& operator<<(ostream& out, const Normalna<T>& A)
{
	int i, j;
	int red = A.matrix->red;
	int stup = A.matrix->stup;
	for (i = 0; i<red; i++)
	{
		for (j = 0; j<stup; j++)
		{
			out << A.matrix->polje[i*red + j];
			out << " ";
		}
		out << endl;
	}
	return out;
}

template<class T> class Dijagonalna;

template <class T> ostream& operator<<(ostream& out, const Dijagonalna<T>& A);
template <class T> Dijagonalna<T> operator+(const Dijagonalna<T>& A, const Dijagonalna<T>& B);
template <class T> Dijagonalna<T> operator-(const Dijagonalna<T>& A, const Dijagonalna<T>& B);
template <class T> Dijagonalna<T> operator*(const Dijagonalna<T>& A, const Dijagonalna<T>& B);

template <class T>
class Dijagonalna : public Matrica
{
public:
	struct SmartDijagonalna{
	public:
		T* polje;
		int n, ref_count;
		SmartDijagonalna() : polje(new T[0]), n(0), ref_count(0) {}
		SmartDijagonalna(int m) : n(m), ref_count(1) { polje = new T[n]; }
		template <int a, int b>
		SmartDijagonalna(T(&mat)[a][b]) : n(a), ref_count(1) {//mozda ode svugdje jos napisati provjeru je li dobar unos
			polje = new T[n];
			int i;
			for (i = 0; i < n; ++i)
				polje[i] = mat[i][i];
		}

		template<class A, template<class S, class ALLOC = std::allocator<S> > class CONTAINER>
		SmartDijagonalna(CONTAINER<A> &s)
		{
			int l = s.size();
			int tmp=l;
			l = ((int)sqrt(l));
			if (tmp != (l*l)) cout << "Neispravno zadane dimenzije matrice!" << endl;
			else {
				ref_count = 1;
				n = l;
				polje = new T[n];
				int i;
				CONTAINER<T>::iterator k = s.begin();
				for (i = 0; i < n; )
				{
					if (*k != ((T)0)) {
						polje[i] = (*k); 
						++i;
					}
					k++;
				}
			}
		}
		~SmartDijagonalna() { delete polje; }
	};

	SmartDijagonalna *matrica;

	struct pomocna {
	public:
		Dijagonalna &A;
		int x;

		pomocna(Dijagonalna &B, const int &y) : A(B), x(y) {}
		struct unutarnja {
			Dijagonalna &A;
			int x, y;
			unutarnja(Dijagonalna &B, const int &a, const int&b) : A(B), x(a), y(b) {}
			operator T() {
				if (x != y) return 0;
				else return A.matrica->polje[y];
			}
			T& operator= (T c) {
				if (x != y) {
					cout << "Nije dozvoljeno promijeniti elemente koji nisu na dijagonali!";
					return A.matrica->polje[x];
				}

				else return A.matrica->polje[x] = c;
			}
		};
		
		unutarnja operator[] (const int &y) { return unutarnja(A, x, y); }

	};

	pomocna operator[] (const int &x) { return pomocna(*this, x); }

	//konstr za dijagonalnu
	Dijagonalna() : matrica(new SmartDijagonalna()) {}
	Dijagonalna(int n) : matrica(new SmartDijagonalna(n)) {}
	template <int a, int b>
	Dijagonalna(T(&m)[a][b]) : matrica(new SmartDijagonalna(m)) {}
	template<class A, template<class S, class ALLOC = std::allocator<S> > class CONTAINER>
	Dijagonalna(CONTAINER<A> &s) : matrica(new SmartDijagonalna(s)) {}

	//kontrola kopiranja...
	Dijagonalna& operator= (const Dijagonalna &A) {
		if (matrica != A.matrica)
		{
			A.matrica->ref_count++;
			if (!--matrica->ref_count) delete matrica;
			matrica = A.matrica;
			return *this;
		}
		return *this;
	}
	Dijagonalna(const Dijagonalna &A) {
		matrica = A.matrica;
		matrica->ref_count++;
	}
	~Dijagonalna() {
		if (!--matrica->ref_count) delete matrica;
	}

	Dijagonalna<T> inverz(){
		int length = matrica->n;
		int i;
		Dijagonalna<T> A(length);
		for (i = 0; i < length; ++i)
			A.matrica->polje[i] = (T) (((T)1)/matrica->polje[i]);
		return A;
	}

	int redak() {
		return matrica->n;
	}
	int stupac() {
		return matrica->n;
	}
	
	//operatori
	friend ostream& operator<< <T> (ostream& out, const Dijagonalna &A);
	friend Dijagonalna operator+ <T> (const Dijagonalna &A, const Dijagonalna &B);
	friend Dijagonalna operator- <T> (const Dijagonalna &A, const Dijagonalna &B);
	friend Dijagonalna operator* <T> (const Dijagonalna &A, const Dijagonalna &B);

};

template <class T>
ostream& operator<< (ostream & out, const Dijagonalna<T> &A) {
	int i, j;
	int veliko = A.matrica->n;
	for (i = 0; i < veliko; i++)
	{
		for (j = 0; j < veliko; j++)
		{
			if (i != j)
				out << "0  ";
			else
			{
				out << A.matrica->polje[i];
				out << "  ";
			}
		}
		out << endl;
	}
	return out;
}

template <class T>
Dijagonalna<T> operator+ (const Dijagonalna<T> &A, const Dijagonalna<T> &B){
	if (A.matrica->n != B.matrica->n) { cout << "Nedozvoljeno zbrajanje! Provjeriti dimenzije matrica!" << endl; return A; }
	int i;
	int length = A.matrica->n;
	Dijagonalna<T> C(length);
	for (i = 0; i < length; ++i)
		C.matrica->polje[i] = A.matrica->polje[i] + B.matrica->polje[i];
	return C;
}
template <class T>
Dijagonalna<T> operator- (const Dijagonalna<T> &A, const Dijagonalna<T> &B){
	if (A.matrica->n != B.matrica->n) { cout << "Nedozvoljeno oduzimanje! Provjeriti dimenzije matrica!" << endl; return A; }
	int i;
	int length = A.matrica->n;
	Dijagonalna<T> C(length);
	for (i = 0; i < length; ++i)
		C.matrica->polje[i] = A.matrica->polje[i] - B.matrica->polje[i];
	return C;
}
template <class T>
Dijagonalna<T> operator* (const Dijagonalna<T> &A, const Dijagonalna<T> &B){
	if (A.matrica->n != B.matrica->n) { cout << "Nedozvoljeno mnozenje! Provjeriti dimenzije matrica!" << endl; return A; }
	int i;
	int length = A.matrica->n;
	Dijagonalna<T> C(length);
	for (i = 0; i < length; ++i)
		C.matrica->polje[i] = ((T)0);
	for (i = 0; i < length; ++i)
		C.matrica->polje[i] = C.matrica->polje[i] + (A.matrica->polje[i] * B.matrica->polje[i]);
	return C;
}