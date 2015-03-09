#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "foo.h"

using namespace std;

class Broj {
public:
	struct BrojRep
	{
		int *broj;
		int sign, dulj, ref_count;

		BrojRep() : ref_count(0), sign(1), dulj(0), broj(new int [0])   {}; 
		BrojRep(int* newData, int Predznak, int len);
		~BrojRep();
	};

	struct sum
	{
		const Broj& prvi;
		const Broj& drugi;

		sum(const Broj *Prvi, const Broj *Drugi);

		operator Broj();
	};

	struct mult
	{
		const Broj& prvi;
		const Broj& drugi;

		mult(const Broj *Prvi, const Broj *Drugi);

		operator Broj();
	};

	struct div
	{
		const Broj& prvi;
		const Broj& drugi;

		div(const Broj *Prvi, const Broj *Drugi);

		operator Broj();
	};

	BrojRep* rep;

	struct sub
	{
		const Broj& prvi;
		const Broj& drugi;

		sub(const Broj *Prvi, const Broj *Drugi);

		operator Broj();
	};


	Broj() {}
	Broj(int broj);
	Broj(int brojevi[], int predz, int len);
	Broj(vector<char> polje);
	Broj(const string rijec);
	Broj(vector<int> brojevi); 
	template <int n>
	Broj(int(&brojevi)[n]);
	template<class T, template<class S, class A = std::allocator<S> > class Cont>
	Broj(Cont<T>& s);



	struct pom {
		Broj& num;
		const int& indeks;

		pom(Broj *b, const int& ind);

		operator int();
		int& operator= (int x);
	};

	pom operator [] (const int& br) { return pom(this, br); }
	
	void ispis();
	
	friend sum operator+ (const Broj& a, const Broj& b);
	friend sub operator- (const Broj& a, const Broj& b);
	friend mult operator* (const Broj& a, const Broj& b);
	friend div operator/ (const Broj& a, const Broj& b);
	int& operator*();
	int* operator->();
	friend ostream& operator<<(ostream& f, const Broj& b);

	// kontrola kopiranja
	Broj(const Broj& x);
	Broj& operator= (const Broj& x);
	~Broj();

	Broj& operator= (const string polje);
	Broj& operator= (int broj);
	Broj& operator= (vector<int> brojevi);
	Broj& operator= (vector<char> polje);
	template<int n>
	Broj& operator= (int (&polje)[n]);
	
	template<class T, template<class S, class A = std::allocator<S> > class Cont>
	Broj& operator= (Cont<T>& s);

	int duljina();
};


Broj::Broj(vector<int> brojevi) {
	int  duljina, i;
	duljina = brojevi.size();
	int* polje = new int[duljina];
	for (i = 0; i < duljina; ++i)
		polje[i] = brojevi[i];
	rep = (new BrojRep(polje, 1, duljina));

	delete[] polje;
}
Broj::Broj(int num) 
{
	int signum, duljina;
	if (num < 0) signum = -1;
	else signum = 1;
	num = abs(num);

	int i = 0, tmp = num, brojac = 0;

	while (1)
	{
		tmp = tmp / 10;
		++brojac;
		if (tmp == 0) break;
	}
	duljina = brojac;
	int* brojevi = new int[brojac];
	for (i = (brojac - 1); i >= 0; --i)
	{
		brojevi[i] = num % 10;
		num /= 10;
	}
	rep = (new BrojRep(brojevi, signum, duljina));

	delete[] brojevi;

}

Broj::Broj(int broje[], int sign, int len)
{
	int i, duljina, signum;
	duljina = len;
	signum = sign;
	int* brojevi = new int[duljina];
	for (i = 0; i<duljina; ++i)
		brojevi[i] = broje[i];

	rep = (new BrojRep(brojevi, signum, duljina));

	delete[] brojevi;
}

Broj::Broj(vector<char> polje) 
{
	int i, signum, duljina;
	if (polje[0] == '-') signum = -1;
	else signum = 1;
	if (signum == -1)
	{
		for (i = 0; i<(polje.size() - 1); ++i)
			polje[i] = polje[i + 1];

		polje.resize(polje.size() - 1);
	}
	duljina = polje.size();

	int* brojevi = new int[duljina];
	for (i = 0; i<duljina; ++i)
		brojevi[i] = polje[i] - '0';

	rep = (new BrojRep(brojevi, signum, duljina));

	delete[] brojevi;
}

Broj::Broj(const string rijec) 
{
	int i, j = 0, brojac = 0, signum, duljina;
	if (rijec[0] == '-') signum = -1;
	else signum = 1;

	while (1)
	{
		if (rijec[j++] != '\0') ++brojac;
		else break;
	}

	if (signum == 1) duljina = brojac;
	else duljina = brojac - 1;

	int* brojevi = new int[duljina];
	if (rijec[0] == '-')
	{
		for (i = 1; i <= duljina; ++i) brojevi[i - 1] = rijec[i] - '0';
	}
	else
	{
		for (i = 0; i<duljina; ++i)
			brojevi[i] = rijec[i] - '0';
	}

	rep = (new BrojRep(brojevi, signum, duljina));

	delete[] brojevi;
}
template <int n>
Broj::Broj(int(&brojevi)[n]) {
	rep = (new BrojRep(brojevi, 1, n));
}

template<class T, template<class S, class A = std::allocator<S> > class Cont>
Broj::Broj(Cont<T>& s)
{
	int* brojevi = new int[s.size()];
	int j = 0;
	Cont<T>::iterator i;
	for (i = s.begin(); i != s.end(); i++){
		brojevi[j] = ((int)*i);
		j++;
	}
	rep = (new BrojRep(brojevi, 1, s.size()));
	delete[] brojevi;
}


char* subtraction(int* broj1, int dulj1, int sign1, int* broj2, int dulj2, int sign2) 
{
	int* novi = 0;
	char* dodaj = 0;
	char* ddodaj = 0;
	int prvi, drugi, mala_opcija=0, opcija = 1, znam, i, kop1, kop2, zapamti = 0, kontrola = 0; 
	int konacna_duljina, brojac = 0, j, koliko_nula = 0;
	prvi = dulj1;
	drugi = dulj2;

	if (sign1 == 1 && sign2 == -1) 
	{
		return summing(broj1, dulj1, sign1, broj2, dulj2, 1);
	}

	if (sign1 == -1 && sign2 == 1)
	{
		return summing(broj1, dulj1, sign1, broj2, dulj2, -1);
	}

	if (prvi > drugi)
	{
		novi = new int[prvi]; konacna_duljina = prvi;
	}
	else if (drugi > prvi)
	{
		novi = new int[drugi]; opcija = 2; konacna_duljina = drugi;
	}
	else { opcija = 3; novi = new int[prvi]; konacna_duljina = prvi; }

	if (opcija == 3)
	{
		for (i = 0; i<prvi; ++i){
			if (broj1[i]>broj2[i]) { mala_opcija = 1; break; }
			else if (broj1[i]<broj2[i]) { mala_opcija = 2; break; }
			else mala_opcija = 3;
		}
	}
	if (opcija == 3 && mala_opcija == 3) { delete[] novi; return "0"; }
	
	if (opcija == 1 || mala_opcija == 1)
	{ //prvi dulji ili jednake duljine, a prvi veci
		kop2 = drugi;
		kop1 = prvi;

		for (i = 0; i<kop2; ++i)
		{
			znam = broj1[prvi - 1] - (broj2[drugi - 1] + zapamti);
			if (znam < 0)
			{
				novi[prvi - 1] = (znam + 10);
				zapamti = 1;
			}
			else {
				novi[prvi - 1] = (znam % 10);
				zapamti = 0;
			}
			--drugi;
			--prvi;
		}
		kop2 = prvi;
		for (i = 0; i<kop2; ++i)
		{
			znam = broj1[prvi - 1] - zapamti;
			if (znam < 0)
			{
				novi[prvi - 1] = (znam + 10);
				zapamti = 1;
			}
			else
			{
				novi[prvi - 1] = (znam % 10);
				zapamti = 0;
			}
			--prvi;

		}

		j = 0;
		for (i = 0; i<konacna_duljina; ++i)
		if (novi[i] != 0) break;
		else ++koliko_nula;

		dodaj = new char[konacna_duljina - koliko_nula + 1];
		for (i = koliko_nula; i<konacna_duljina; ++i)
			dodaj[j++] = (char)(((int)'0') + novi[i]);

		dodaj[konacna_duljina - koliko_nula] = '\0'; 

	}

	if (opcija == 2 || mala_opcija == 2) // drugi dulji od prvoga
	{ // ili drugi veci do prvoga, a jednakih duljina su
		kop2 = drugi;
		kop1 = prvi;
		for (i = 0; i<kop1; ++i)
		{
			znam = broj2[drugi - 1] - (broj1[prvi - 1] + zapamti);
			if (znam < 0)
			{
				novi[drugi - 1] = (znam + 10);
				zapamti = 1;
			}
			else {
				novi[drugi - 1] = (znam % 10);
				zapamti = 0;
			}
			--drugi;
			--prvi;
		}
		kop2 = drugi;
		for (i = 0; i<kop2; ++i)
		{
			znam = broj2[drugi - 1] - zapamti;
			if (znam < 0)
			{
				novi[drugi - 1] = (znam + 10);
				zapamti = 1;
			}
			else
			{
				novi[drugi - 1] = (znam % 10);
				zapamti = 0;
			}
			--drugi;

		}

		j = 0;
		for (i = 0; i<konacna_duljina; ++i)
		if (novi[i] != 0) break;
		else ++koliko_nula;

		dodaj = new char[konacna_duljina - koliko_nula + 2];
		for (i = (koliko_nula - 1); i<konacna_duljina; ++i)
		{
			if (kontrola == 0)
			{
				dodaj[j++] = '-';
				kontrola = 1;
			}
			else dodaj[j++] = (char)(((int)'0') + novi[i]);

		}

		dodaj[konacna_duljina - koliko_nula + 1] = '\0'; // .......
	}

	//ova zadnja 2 još testirat samo...samo
	if (sign1 == -1 && sign2 == -1)
	{
		if (dodaj[0] != '-')
		{
			i = 0;
			while (1)
			{
				if (dodaj[i++] == '\0') break;
				else ++brojac;
			}

			ddodaj = new char[brojac + 1];
			ddodaj[0] = '-';
			for (i = 1; i<brojac + 1; ++i)
				ddodaj[i] = dodaj[i - 1];

			ddodaj[i] = '\0';
		}
		else
		{
			i = 0;
			while (1)
			{
				if (dodaj[i++] == '\0') break;
				else ++brojac;
			}

			ddodaj = new char[brojac - 1];
			for (i = 0; i<(brojac - 1); ++i)
				ddodaj[i] = dodaj[i + 1];

			ddodaj[i] = '\0';
		}

		delete[] novi;
		delete[] dodaj;
		return ddodaj;
	}
	delete[] novi;
	return dodaj;
}


char* summing(int* broj1, int dulj1, int sign1, int* broj2, int dulj2, int sign2) 
{

	int* novi = 0;
	char* dodaj = 0;
	int prvi, drugi, mala_opcija=0, opcija = 1, znam, i, kop1, kop2, zapamti = 0; 
	int pazi = 0, ukupna_duljina, situacija = 4, k, j;
	prvi = dulj1;
	drugi = dulj2;

	if (prvi >= drugi) { novi = new int[prvi]; ukupna_duljina = prvi; }
	else { novi = new int[drugi]; opcija = 2; ukupna_duljina = drugi; }

	if (sign1 == -1 && sign2 == 1)
	{
		return subtraction(broj1, dulj1, sign1, broj2, dulj2, -1);
	}

	if (sign1 == 1 && sign2 == -1)
	{
		return subtraction(broj2, dulj2, sign2, broj1, dulj1, -1);
	}

	if (sign1 == -1 && sign2 == -1) mala_opcija = 5;
	if (sign1 == 1 && sign2 == 1) mala_opcija = 6;

	if (opcija == 2) // drugi broj je veci
	{
		kop2 = drugi;
		kop1 = prvi;
		for (i = 0; i<kop1; ++i)
		{
			znam = broj1[prvi - 1] + broj2[drugi - 1] + zapamti;
			if (znam > 9)
			{
				novi[drugi - 1] = (znam % 10);
				zapamti = znam / 10;
			} 
			else {
				novi[drugi - 1] = (znam % 10);
				zapamti = 0;
			}
			--drugi;
			--prvi;
		}

		kop2 = drugi;
		for (i = 0; i<kop2; ++i)
		{
			znam = broj2[drugi - 1] + zapamti;
			if (znam > 9)
			{ 
				novi[drugi - 1] = (znam % 10);
				zapamti = znam / 10;
			}
			else
			{
				novi[drugi - 1] = (znam % 10);
				zapamti = 0;
			}
			--drugi;

		}

		if (zapamti != 0)
		{
			if (mala_opcija == 6) //oba + carry .... radi
			{
				dodaj = new char[2 + ukupna_duljina]; 
				dodaj[0] = (char)(((int)'0') + 1);
				situacija = 1;
			}
			else if (mala_opcija == 5) // oba min i ima carry
			{
				dodaj = new char[ukupna_duljina + 3]; 
				dodaj[0] = '-';
				dodaj[1] = (char)(((int)'0') + 1);
				pazi = 1;
				situacija = 2;
			}
		}

		if (mala_opcija == 5 && pazi != 1)
		{
			dodaj = new char[ukupna_duljina + 2]; 
			dodaj[0] = '-'; // oba minusa bez carryja .. radi
			situacija = 3;
		}
		else if (situacija == 4) dodaj = new char[ukupna_duljina + 1]; // ... radi 


		if (situacija == 1 || situacija == 3) k = 1;
		else if (situacija == 2) k = 2;
		else if (situacija == 4) k = 0;
		j = 0;
		for (i = k; i<ukupna_duljina + k; ++i)
			dodaj[i] = (char)(((int)'0') + novi[j++]);

		dodaj[i] = '\0';
	}

	if (opcija == 1) // prvi broj je veci ili jednak
	{
		kop2 = drugi;
		kop1 = prvi;
		for (i = 0; i<kop2; ++i)
		{
			znam = broj1[prvi - 1] + broj2[drugi - 1] + zapamti;
			if (znam > 9)
			{
				novi[prvi - 1] = (znam % 10);
				zapamti = znam / 10;
			} 
			else {
				novi[prvi - 1] = (znam % 10);
				zapamti = 0;
			}
			--drugi;
			--prvi;
		}
		kop2 = prvi;
		for (i = 0; i<kop2; ++i)
		{
			znam = broj1[prvi - 1] + zapamti;
			if (znam > 9)
			{
				novi[prvi - 1] = (znam % 10);
				zapamti = znam / 10;
			}
			else
			{
				novi[prvi - 1] = (znam % 10);
				zapamti = 0;
			}
			--prvi;
		}
		if (zapamti != 0)
		{
			if (mala_opcija == 6) //oba + carry .... radi
			{
				dodaj = new char[2 + ukupna_duljina]; 
				dodaj[0] = (char)(((int)'0') + 1);
				situacija = 1;
			}
			else if (mala_opcija == 5) // oba min i ima carry
			{
				dodaj = new char[ukupna_duljina + 3]; 
				dodaj[0] = '-';
				dodaj[1] = (char)(((int)'0') + 1);
				pazi = 1;
				situacija = 2;
			}
		}

		if (mala_opcija == 5 && pazi != 1)
		{
			dodaj = new char[ukupna_duljina + 2]; 
			dodaj[0] = '-'; // oba minusa bez carryja .. radi
			situacija = 3;
		}
		else if (situacija == 4) dodaj = new char[ukupna_duljina + 1]; // ... radi 


		if (situacija == 1 || situacija == 3) k = 1;
		else if (situacija == 2) k = 2;
		else if (situacija == 4) k = 0;
		j = 0;
		for (i = k; i<ukupna_duljina + k; ++i)
			dodaj[i] = (char)(((int)'0') + novi[j++]);

		dodaj[i] = '\0';

	}
	delete[] novi;

	return dodaj;

}




Broj multiplication(int* broj1, int dulj1, int sign1, int* broj2, int dulj2, int sign2) 
{
	Broj ret("0");
	Broj tmp("0");
	Broj tmp1("0");
	int drugi = dulj2;
	int i, j, k, opcija = 0;
	int* novo = 0;
	if (sign1 == 1 && sign2 == -1) opcija = 2;
	else if (sign1 == -1 && sign2 == 1) opcija = 3;
	Broj aa(broj1, 1, dulj1);
	Broj bb(broj2, 1, dulj2);

	for (j = 0; j<drugi; ++j)
	{
		for (k = 0; k<broj2[j]; ++k)
			tmp = tmp + aa;


		for (k = 0; k<(drugi - 1 - j); ++k) //dodaju se nule
		{
			int duljina = ++(tmp.rep->dulj);
			novo = new int[duljina];
			for (i = 0; i<duljina - 1; ++i)
				novo[i] = tmp.rep->broj[i];
			novo[i] = 0;

			delete[] tmp.rep->broj;
			tmp.rep->broj = new int[duljina];
			for (i = 0; i<duljina; ++i)
				tmp.rep->broj[i] = novo[i];
			tmp.rep->dulj = duljina;
			tmp.rep->sign = 1;

		}
		ret = ret + tmp;
		tmp = tmp1;
	}
	while (1){
		if (ret.rep->broj[0] == 0) {
			for (i = 0; i < ret.rep->dulj - 1; ++i)
				ret.rep->broj[i] = ret.rep->broj[i + 1];
			ret.rep->dulj = ret.duljina() - 1;
		}
		else break;
	}

	if (opcija == 2 || opcija == 3)
	{
		ret.rep->sign = -1;
		delete[] novo;
		return ret;
	}
	else { delete[] novo; return ret; }
}

Broj division(int* broj1, int dulj1, int sign1, int* broj2, int dulj2, int sign2) 
{
	char nula[] = "0";
	int prvi = dulj1; 
	int drugi = dulj2;
	char* polje1 = 0;
	char* polje2 = 0;
	char* novi_broj = 0;
	char* novi = 0;
	char* ucitaj = 0;
	int duljina_novog = 0;
	if (prvi < drugi) return (Broj(nula));

	int i, slucaj = 2, opcija = 1, brojac = 0, prvi_put = 0;
	int pozicija;
	if ((sign1 == 1 && sign2 == 1) || (sign1 == -1 && sign2 == -1)) opcija = 0; 

	Broj br1(broj1, 1, dulj1);
	Broj br2(broj2, 1, dulj2);
	
	// u br1 i br2 se nalazi abs od a i b

	pozicija = drugi - 1;

	//uzme koliko je drugi velik znamenki od prvog i spremi u p i q
	polje1 = new char[drugi + 1];
	for (i = 0; i<drugi; ++i)
		polje1[i] = (char)(((int)'0') + br1.rep->broj[i]);
	polje1[i] = '\0';
	Broj p(polje1); // nenegativno
	Broj q = br2; //uvijek nenengativno
	delete[] polje1;

	while (1){
		Broj z = p - q;
		if (z.rep->sign == -1) slucaj = 1; // uzimam više od stringa
		if (slucaj == 1 && prvi_put == 0)
		{
			if (prvi == drugi)  return (Broj(nula));

			polje2 = new char[drugi + 2];
			for (i = 0; i<drugi + 1; ++i)
				polje2[i] = (char)(((int)'0') + br1.rep->broj[i]);

			polje2[i] = '\0';
			p = Broj(polje2);
			delete[] polje2;
			++pozicija;

		}
		if (slucaj == 1 && prvi_put == 1)
		{
			ucitaj = new char[p.rep->dulj + 2];
			for (i = 0; i<p.rep->dulj; ++i)
				ucitaj[i] = (char)(((int)'0') + p.rep->broj[i]);
			ucitaj[i++] = (char)(((int)'0') + br1.rep->broj[pozicija]);
			ucitaj[i] = '\0';

			p = Broj(ucitaj);
			delete[] ucitaj;
		}

		while (p.rep->sign == 1)
		{
			p = (p - q);

			++brojac;
		}
		++duljina_novog;
		--brojac;
		p = p + q;


		if (duljina_novog == 1)
		{
			novi_broj = new char[duljina_novog + 1];
			for (i = 0; i < duljina_novog; ++i)
				novi_broj[i] = (char)(((int)'0') + brojac);
			novi_broj[i] = '\0';
			novi = new char[duljina_novog + 1];
			for (i = 0; i < duljina_novog; ++i)
				novi[i] = (char)(((int)'0') + brojac);
			novi_broj[i] = '\0';
		}
		else
		{
			novi = new char[duljina_novog + 1];
			for (i = 0; i < duljina_novog - 1; ++i)
				novi[i] = novi_broj[i];
			novi[i++] = (char)(((int)'0') + brojac);
			novi[i] = '\0';

			novi_broj = new char[duljina_novog + 1];
			for (i = 0; i < duljina_novog + 1; ++i)
				novi_broj[i] = novi[i];
		}


		++pozicija;
		if (pozicija == prvi) break;
		slucaj = 2;
		brojac = 0;
		prvi_put = 1;
	}
	if (opcija == 1)
	{
		Broj c = Broj(novi_broj);
		c.rep->sign = -1;
		delete[] novi_broj;
		delete[] novi;
		return c;
	}
	delete[] novi;
	return Broj(novi_broj);
}

       //glavni cconstr
Broj& Broj::operator=(const Broj &b){
	++b.rep->ref_count;
	if (!--rep->ref_count) delete rep;
	rep = b.rep;
	return *this;
}

Broj::Broj(const Broj& b)
{
	rep = b.rep;
	++rep->ref_count;
}

Broj& Broj::operator=(const string polje)
{
	rep = (Broj(polje)).rep;
	return *this;
}

Broj& Broj::operator=(int broj){
	rep = (Broj(broj)).rep;
	return *this;
}

template<int n>
Broj& Broj::operator= (int(&polje)[n]) {
	rep = (Broj(polje)).rep;
	return *this;
}
template<class T, template<class S, class A = std::allocator<S> > class Cont>
Broj& Broj::operator= (Cont<T>& s) {
	rep = (Broj(s)).rep;
	return *this;
}

Broj& Broj::operator=(vector<int> brojevi){
	rep = (Broj(brojevi)).rep;
	return *this;
}

Broj& Broj::operator=(vector<char> polje) {
	rep = (Broj(polje)).rep;
	return *this;
}


Broj::~Broj()
{
	if (!--rep->ref_count) delete rep;
}

int Broj::duljina()
{
	return rep->dulj;
}

void Broj::ispis()
{
	cout << endl;
	int i;
	if (rep->sign == -1) cout << "-";
	for (i = 0; i<rep->dulj; ++i)
		cout << rep->broj[i];
}

Broj::pom::pom(Broj *b, const int& ind) : num(*b), indeks(ind) {}
Broj::pom::operator int() //broji se od indeksa 1
{
	if (num.duljina() < indeks) return 0;
	return num.rep->broj[num.duljina() - indeks];
}
int& Broj::pom::operator=(int x) //od 1 brojim, ali je svejedno
{
	int k = 0;
	int& r = k;
	if (x<0) { cout << "Pogrjesan unos!" << endl; return r; }
	else if (x == 0)
	{
		if (num.duljina() < indeks) return num.rep->broj[0] = num.rep->broj[0];
		else if (num.duljina() == indeks) {
			int i;
			while (1) {
				for (i = 0; i < (num.duljina() - 1); ++i)
					num.rep->broj[i] = num.rep->broj[i + 1]; 
				num.rep->dulj = num.duljina() - 1;
				if (num.rep->broj[0] != 0 || num.duljina()==1 ) break;
			}
			return num.rep->broj[0] = num.rep->broj[0];
		}
		else return num.rep->broj[num.duljina() - indeks] = x;


	}
	else
	{
		if (num.duljina() < indeks) //na pocetak guram
		{
			int dugo = num.duljina();
			int *polje = new int[indeks]; 
			polje[0] = x; 
			int i, j = 0;
			for (i = 1; i < indeks - dugo; ++i)
				polje[i] = 0; 
			for (i = indeks - dugo; i < indeks; ++i)
				polje[i] = num.rep->broj[j++];
			delete[] num.rep->broj;
			num.rep->broj = new int[indeks];
			for (i = 0; i < indeks; ++i)
				num.rep->broj[i] = polje[i];

			delete[] polje;
			num.rep->dulj=indeks;
			return num.rep->broj[0] = x;
		}
		else return num.rep->broj[num.duljina() - indeks] = x; //bilo gdje drugo


	}
}

Broj::BrojRep::BrojRep(int* newData, int Predznak, int len) : ref_count(1), sign(Predznak), dulj(len){
	broj = new int[dulj];
	for (int i = 0; i<dulj; ++i)
		broj[i] = newData[i];
}
Broj::BrojRep::~BrojRep(){ delete broj; }

int& Broj::operator*() { return *rep->broj; }

int* Broj::operator->(){ return rep->broj; }

Broj::sum::sum(const Broj *Prvi, const Broj *Drugi) : prvi(*Prvi), drugi(*Drugi){}

Broj::sum::operator Broj(){
	return Broj(summing(prvi.rep->broj, prvi.rep->dulj, prvi.rep->sign, drugi.rep->broj, drugi.rep->dulj, drugi.rep->sign));
}
Broj::sum operator+(const Broj& a, const Broj& b){
	return Broj::sum(&a, &b);
}

Broj::sub::sub(const Broj *Prvi, const Broj *Drugi) : prvi(*Prvi), drugi(*Drugi){}

Broj::sub::operator Broj(){
	return Broj(subtraction(prvi.rep->broj, prvi.rep->dulj, prvi.rep->sign, drugi.rep->broj, drugi.rep->dulj, drugi.rep->sign));
}
Broj::sub operator-(const Broj& a, const Broj& b){
	return Broj::sub(&a, &b);
}

Broj::mult::mult(const Broj *Prvi, const Broj *Drugi) : prvi(*Prvi), drugi(*Drugi){}

Broj::mult::operator Broj(){
	return multiplication(prvi.rep->broj, prvi.rep->dulj, prvi.rep->sign, drugi.rep->broj, drugi.rep->dulj, drugi.rep->sign);
}

Broj::mult operator* (const Broj& a, const Broj& b){
	return Broj::mult(&a, &b);
}

Broj::div::div(const Broj *Prvi, const Broj *Drugi) : prvi(*Prvi), drugi(*Drugi){}

Broj::div::operator Broj(){
	return division(prvi.rep->broj, prvi.rep->dulj, prvi.rep->sign, drugi.rep->broj, drugi.rep->dulj, drugi.rep->sign);
}

Broj::div operator/ (const Broj& a, const Broj& b){
	return Broj::div(&a, &b);
}

ostream& operator<<(ostream& f, const Broj& b){
	char* polje = new char[b.rep->dulj + 1];
	int i, dugo;
	dugo = b.rep->dulj;
	for (i = 0; i<dugo; ++i)
		polje[i] = (char)(((int)'0') + b.rep->broj[i]);
	polje[i] = '\0';


	if (b.rep->sign == 1) f << polje;
	if (b.rep->sign == -1) f << "-" << polje;
	return f;
}
