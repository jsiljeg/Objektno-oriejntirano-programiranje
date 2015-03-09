#include <iostream>
#include <vector>
#include <memory>

using namespace std;

template <class T> class Vektor;
template <class T> ostream& operator<<(ostream& out, const Vektor<T> &v);

template <class T>
class Vektor {
public:
	struct SmartVektor {
		T* polje;
		int n;
		int ref_count;
		SmartVektor() : ref_count(0), n(0), polje(new T[0]) {}
		SmartVektor(int m) : ref_count(1), n(m) {
			polje = new T[n];
			int i;
			for (i = 0; i < n; ++i)
				polje[i] = ((T)0);
		}
		SmartVektor(int m, T x) : ref_count(1), n(m) {
			polje = new T[n];
			int i;
			for (i = 0; i < n; ++i)
				polje[i] = x;
		}
		template <int m>
		SmartVektor(T(&v)[m]) : ref_count(1), n(m) {
			polje = new T[n];
			int i;
			for (i = 0; i < n; ++i)
				polje[i] = v[i];
		}
		SmartVektor(T *v, int m) : ref_count(1), n(m) {
			polje = new T[n];
			int i;
			for (i = 0; i < n; ++i)
				polje[i] = v[i];
		}

		template<class A, template<class S, class ALLOC = std::allocator<S> > class CONTAINER>
		SmartVektor(CONTAINER<A> &spremnik) :ref_count(1)
		{
			n = spremnik.size();
			polje = new T[n];
			int j = 0;
			CONTAINER<A>::iterator i;
			for (i = spremnik.begin(); i < spremnik.end(); i++)
			{
				polje[j] = (*i);
				j++;
			}
		}

		~SmartVektor() { delete polje; }
	};
	SmartVektor *vektor;
	struct pom { 
		Vektor &v;
		int x;
		pom(Vektor &a, const int &y) : v(a), x(y) {}
		operator T() { 
			if (v.vektor->n <= x) {
				cout << "Nedostupan element! " << endl;
				return ((T)0);
			}
			else return v.vektor->polje[x];
		}
		T& operator= (T z) { //std::vector popuni smecem ostali prostor, ja cu ga popuniti nulama
			if (v.vektor->polje[x] == z) return v.vektor->polje[x];
			int i, dulj, len;
			dulj = v.vektor->n;
			T* tmp = 0;
			(x >= dulj) ? len = x + 1 : len = dulj;
			tmp = new T[len];
			
			for (i = 0; i < len; ++i)
				(i < dulj) ? tmp[i] = v.vektor->polje[i] : tmp[i] = ((T)0);
			
			tmp[x] = z;
			if (!--v.vektor->ref_count) delete v.vektor;
			v.vektor = new SmartVektor(tmp, len);
			delete[] tmp;
			return v.vektor->polje[x];
		}
	};
	pom operator[] (const int &x){ return pom(*this, x); }

	Vektor() : vektor(new SmartVektor()) {}
	Vektor(int n) : vektor(new SmartVektor(n)) {}
	Vektor(int n, T x) : vektor(new SmartVektor(n, x)) {}
	template<class A, template<class S, class ALLOC = std::allocator<S> > class CONTAINER>
	Vektor(CONTAINER<A> &spremnik) : vektor(new SmartVektor(spremnik)){}
	template <int m>
	Vektor(T(&v)[m]) : vektor(new SmartVektor(v)) {}
	Vektor(T *v, int m) : vektor(new SmartVektor(v, m)) {}
	
	T& operator*() { return *vektor->polje; }
	T* operator->(){ return vektor->polje; }

	//kontrola kopiranja
	Vektor& operator=(const Vektor &a) {
		if (vektor != a.vektor) {
			++a.vektor->ref_count;
			if (!--vektor->ref_count) delete vektor;
			vektor = a.vektor;
			return *this;
		}
		return *this;
	}

	Vektor(const Vektor &a) {
		vektor = a.vektor;
		++vektor->ref_count;
	}
	~Vektor() { if (!--vektor->ref_count) delete vektor; }

	friend ostream& operator<< <T>(ostream& out, const Vektor &V);
	

	int size() { return vektor->n; }
	void push_back(T a) {
		int duljina = vektor->n;
		int i;
		T* tmp = new T[duljina + 1];
		for (i = 0; i < duljina; ++i)
			tmp[i] = vektor->polje[i];
		tmp[i] = a;
		delete[] this->vektor->polje;
		this->vektor->polje = new T[duljina + 1];		
		for (i = 0; i < duljina + 1; ++i)
			this->vektor->polje[i] = tmp[i];
		this->vektor->n = (duljina + 1);
		delete[] tmp;
	}
	void pop_back() {
		int duljina = vektor->n;
		int i;
		 if (duljina == 0) cout << "Nedozvoljena operacija (pokusavamo izbaciti nesto sto ne postoji)! Duljina je 0!" << endl; 
		 else if (!--duljina) --vektor->n; 
		 else {
			 T* tmp = new T[duljina];
			 for (i = 0; i < duljina; ++i)
				 tmp[i] = vektor->polje[i];

			 delete[] this->vektor->polje;
			 this->vektor->polje = new T[duljina];
			 for (i = 0; i < duljina + 1; ++i)
				 this->vektor->polje[i] = tmp[i];
			 this->vektor->n = duljina;
			 delete[] tmp;
		 }
		
	}
	T& front(){
		return this->vektor->polje[0];
	}
	T& back(){
		return this->vektor->polje[this->vektor->n - 1];
	}
};

template <class T>
ostream& operator<<(ostream& out, const Vektor<T>& v)
{
	out << "[ ";
	for (int i = 0; i<v.vektor->n; i++)
	{
		out << v.vektor->polje[i];
		out << " ";
	}
	out << "]";
	return out;
}

