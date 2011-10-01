//WholeNumber.cpp
//class specifier for very long integer type
#include "WholeNumber.h"

/*////////////////
///Constructors///
///////////////*/
//Default constructor
WholeNumber::WholeNumber()
{
	vlstr[0]='\0';
	vlen=0;
}

//One-arg constructor for String
WholeNumber::WholeNumber(const char *s)   
{ 
	strcpy(vlstr,s);
	vlen=strlen(s);
}

WholeNumber::WholeNumber(unsigned long long number)
{
	stringstream convert;
	convert << number;
	convert >> vlstr;
	strrev(vlstr);
	vlen=strlen(vlstr);
}

/////////////////////////////////Arithmatic Operators///////////////////////////////////
WholeNumber WholeNumber::operator + (const WholeNumber &v)  
{
	char temp[MAX];
	int maxlen=(vlen>v.vlen)?vlen:v.vlen;
	int carry=0, j=0;
	for(;j<maxlen;j++)
	{
		int d1= (j>vlen-1)?0:vlstr[j]-'0';
		int d2= (j>v.vlen-1)?0:v.vlstr[j]-'0';
		int digitsum =d1+d2+carry;
		if(digitsum>=10)
		{
			digitsum-=10;
			carry=1;
		}else	
			carry=0;

		temp[j]= digitsum+'0';
	}
	if(carry==1)
	{
		temp[j++]= '1';
	}
	temp[j]= '\0';
	return WholeNumber(temp);
}
WholeNumber WholeNumber::operator - (const WholeNumber &v)
{
	if(*this <= v)
		return (unsigned long long)0;

	char temp[MAX];
	int maxlen=(vlen>v.vlen)?vlen:v.vlen;
	int carry=0, j=0;
	for(;j<maxlen;j++)
	{
		int d1= (j>vlen-1)?0:vlstr[j]-'0';
		int d2= (j>v.vlen-1)?0:v.vlstr[j]-'0';
		int digitsub = d1-d2-carry;
		if(digitsub<0)
		{
			digitsub = 10 + digitsub;
			carry=1;
		}else	
			carry=0;

		temp[j]= digitsub+'0';
	}
	//Trim leading zeros
	while(temp[j - 1]=='0'){
		j--;
	}

	temp[j]='\0';

	return WholeNumber(temp);
}
WholeNumber WholeNumber::operator * (const WholeNumber &v)    
{
	WholeNumber pprod;
	WholeNumber tempsum;

	for(int j=0;j<v.vlen;j++)
	{
		int digit= v.vlstr[j]-'0';
		pprod= multiplyDigit(digit);
		for(int k=0; k<j; k++)
			pprod= multiply10(pprod);

		tempsum= tempsum + pprod;
	}
	return tempsum;
}
WholeNumber WholeNumber::operator / (const WholeNumber &v)    
{
	WholeNumber remainder= *this;
	WholeNumber quotient= (unsigned long long)0;

	while(remainder >= v){
		remainder = remainder - v;
		quotient = quotient + 1;
	}
	return quotient;
}

WholeNumber WholeNumber::operator % (const WholeNumber &v)
{
	WholeNumber remainder= *this;
	WholeNumber quotient= (unsigned long long)0;

	while(remainder >= v){
		remainder = remainder - v;
		quotient = quotient + 1;
	}
	return remainder;
}
/////////////////////////////////Comparison Operators///////////////////////////////////
bool WholeNumber::operator < (const WholeNumber &v)
{
	if(vlstr == v.vlstr)
		return false;
	
	if(vlen > v.vlen)
		return false;
	
	if(vlen < v.vlen)
		return true;
	
	char temp1[MAX];
	strcpy(temp1, vlstr);
	char temp2[MAX];
	strcpy(temp2, v.vlstr);

	strrev(temp1);
	strrev(temp2);

	if(strcmp(temp1, temp2)==-1)
		return true;

	return false;
}
bool WholeNumber::operator <= (const WholeNumber &v)
{
	if(*this == v)
		return true;
	
	if(vlen > v.vlen)
		return false;
	
	if(vlen < v.vlen)
		return true;
	
	char temp1[MAX];
	strcpy(temp1, vlstr);
	char temp2[MAX];
	strcpy(temp2, v.vlstr);

	strrev(temp1);
	strrev(temp2);

	if(strcmp(temp1, temp2)==-1)
		return true;

	return false;
}
bool WholeNumber::operator > (const WholeNumber &v)
{
	if(vlstr == v.vlstr)
		return false;
	
	if(vlen < v.vlen)
		return false;
	
	if(vlen > v.vlen)
		return true;
	
	char temp1[MAX];
	strcpy(temp1, vlstr);
	char temp2[MAX];
	strcpy(temp2, v.vlstr);

	strrev(temp1);
	strrev(temp2);

	if(strcmp(temp1, temp2)==1)
		return true;

	return false;
}

bool WholeNumber::operator >= (const WholeNumber &v)
{
	if(*this == v)
		return true;
	
	if(vlen < v.vlen)
		return false;
	
	if(vlen > v.vlen)
		return true;
	
	char temp1[MAX];
	strcpy(temp1, vlstr);
	char temp2[MAX];
	strcpy(temp2, v.vlstr);

	strrev(temp1);
	strrev(temp2);

	if(strcmp(temp1, temp2)==1)
		return true;

	return false;
}

bool WholeNumber::operator == (const WholeNumber &v)
{
	if(!strcmp(vlstr, v.vlstr))
		return true;

	return false;
}

bool WholeNumber::operator != (const WholeNumber &v)
{
	if(*this == v)
		return false;

	return true;
}

string WholeNumber::toString() const
{
	char temp[MAX];
	strcpy(temp, vlstr);
	strrev(temp);
	return temp;
}

////////////////////////////////////////////////////////////////////////////////////////

//Multiply argument by 10
WholeNumber WholeNumber::multiply10(const WholeNumber &v)  
{
	char temp[MAX];
	for(int j=v.vlen-1;j>=0;j--)
		temp[j+1]=v.vlstr[j];
	temp[0]='0';
	temp[v.vlen+1]='\0';
	return WholeNumber(temp);
}

WholeNumber WholeNumber::multiplyDigit(int d2)
{
	char temp[MAX];
	int carry=0, j=0;
	for(;j<vlen;j++)
	{
		int d1=vlstr[j]-'0';
		int digitprod=d1*d2;
		digitprod=d1*d2;
		digitprod +=carry;
		if(digitprod>=10)
		{
			carry=digitprod/10;
			digitprod -=carry*10;
		}else
			 carry=0;
		temp[j]=digitprod+'0';
	}
	if(carry!=0)
		temp[j++]=carry+'0';

	temp[j]='\0';
	return WholeNumber(temp);
}

//Extractor
ostream &operator<<(ostream &stream, WholeNumber number)
{
	char temp[MAX];
	strcpy(temp, number.vlstr);
	stream << strrev(temp);
	return stream;
}
//Inserter
istream &operator>>(istream &stream, WholeNumber &number)
{
	char temp[MAX];
	stream >> temp;
	number.vlen= strlen(temp);
	strcpy(number.vlstr, strrev(temp));
	return stream;
}


