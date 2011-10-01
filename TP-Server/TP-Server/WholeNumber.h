//WholeNumber.h
//class specifier for very long integer type
#ifndef WholeNumber_H
#define WholeNumber_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;

const int MAX =3000;											//Maximum digits in WholeNumber

class WholeNumber{
	private:
		char vlstr[MAX];
		int vlen;

		WholeNumber multiplyDigit(int);						//Multiply single digit
		WholeNumber multiply10(const WholeNumber &);			//Multiply by 10


	public:
		WholeNumber();											//Default Constructor
		WholeNumber(const char *);   							//One-arg constructor for String
		WholeNumber(unsigned long long);						//One-arg constructor for long long

		//Arithmatic operators
		WholeNumber operator + (const WholeNumber &);			//Add WholeNumbers
		WholeNumber operator - (const WholeNumber &);			//Subtract WholeNumbers
		WholeNumber operator * (const WholeNumber &);			//Multiply WholeNumbers
		WholeNumber operator / (const WholeNumber &);			//Divide WholeNumbers
		WholeNumber operator % (const WholeNumber &);			//Mod WholeNumbers

		//Increment and decrement operators
		/*WholeNumber operator ++ (const WholeNumber &);
		WholeNumber operator ++ (const WholeNumber &, int);
		WholeNumber operator -- (const WholeNumber &);
		WholeNumber operator -- (const WholeNumber &, int);*/


		//Conjugate operators
		/*WholeNumber &operator += (const WholeNumber &);
		WholeNumber &operator -= (const WholeNumber &);	
		WholeNumber &operator *= (const WholeNumber &);
		WholeNumber &operator /= (const WholeNumber &);
		WholeNumber &operator %= (const WholeNumber &);*/	

		//Comparison operators
		bool operator < (const WholeNumber &);				//Less than
		bool operator <= (const WholeNumber &);				//Less than equal to
		bool operator > (const WholeNumber &);				//Greater than 
		bool operator >= (const WholeNumber &);				//Greater than equal to
		bool operator == (const WholeNumber &);				//Equals to 
		bool operator != (const WholeNumber &);				//Not equal to 

		string toString() const;

		friend ostream &operator<<(ostream &, WholeNumber);
		friend istream &operator>>(istream &, WholeNumber &);
};

//Extractor
ostream &operator<<(ostream &, WholeNumber);
//Inserter
istream &operator>>(istream &, WholeNumber &);

#endif //WholeNumber_H
