// Lab1-Calculator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "string"
#include "stdio.h"
#include "istream"
#include "sstream"
using namespace std;

class err {};

int factorial(double);
int convert(char);
double mod(double, double);
double get_token(string, char);
double very_primary(string);
double primary(string);
double term(string);
double expression(string);
double str_to_num(string);
char convertback(int);
string del_space(string);
string input();
string init(string);
string cut_str(string, int, int);
string deal_with_negative(string);

double save = false,saved=false;//save represents whether the ans should be saved, while saved represents whether there is an ans in storage.
double saving = 0;//the number in storage.


int convert(char x) {
	if (x == '+') return 1;
	if (x == '-') return -1;
	else throw err();
}

char convertback(int x) {
	if (x == 1) return '+';
	if (x == -1)return '-';
	else throw err();
}

int find_number(string s, int start) {//to return the position of the char following the first number found after "start". 
	int num_of_dot = 0;
	s = s + ' ';//to avoid out of range.
	for (int i = start + 1; i < s.size();i++) {
		if (s[i] == '.') {
			if (num_of_dot = 0) num_of_dot++;
			else throw err();
		}
		else if (!(s[i] >= '0' && s[i] <= '9')) return i;
	}
	throw err();
	return 0;
}

string deal_with_negative(string equation) {
	//to automatically add '()' to negative numbers or numbers with '+' or '-' in the front.
	string temp;
	while (true) {
		bool modified = false;
		for (int i = 1; i < equation.size(); i++) {
			if (equation[i] == '+' || equation[i] == '-') {
				if (equation[i - 1] == '*' || equation[i - 1] == '/' || equation[i - 1] == '%') {//in cases that '+' or '-' is after '*','/','%'. 
					int right = find_number(equation, i);
					temp = cut_str(equation, 0, i - 1) + '(' + cut_str(equation, i, right - 1) + ')' + cut_str(equation, right, equation.size() - 1);
					equation = temp;
					modified = true;
					break;
				}
			}
		}
		if (!modified) break;
	}
	return equation;
}

string init(string equation){//to deal with consective '+', '-' and to deal with 'M', 'R' and ';'.
	//for instance, init("1++--1")="1+1", init("1---1")="1-1".
	if (equation == "") throw err();
	if (equation[equation.size() - 1] == 'M') {
		save = true;
		return init(cut_str(equation, 0, equation.size() - 2));
	}
	if (equation[equation.size() - 1] == ';') return init(cut_str(equation, 0, equation.size() - 2));
	//^ to deal with 'M' and ';'.
	int pon = 1;//positve or negative.
	int i = 0, j = 0;
	string temp;
	bool found = false, end = false;
	while (true) {
		found = false; //'+' or '-' not found yet.
		for (i = 0; i < equation.size(); i=i+1) {
			//cout << i << endl;
			bool modified = false;//to note whether in that loop the equation has been modified.
			if (equation[i] == '+'|| equation[i]=='-') {
				pon = convert(equation[i]);
				for (j = i+1; j < equation.size(); j=j+1) {
					if (equation[j] == '+' || equation[j] == '-') pon *= convert(equation[j]), modified = true;
					else {
						temp = cut_str(equation, 0, i - 1) + convertback(pon) + cut_str(equation, j, equation.size() - 1);
						equation = temp;
						found = true;
						break;
					}
				}
			}
			if (i == equation.size() - 1) end = true;
			if (found&&modified) break;
			//cout << i <<equation.size()<< endl;
		}
		if (end) break;
	}
	return equation;
}


string cut_str(string s, int left, int right) {// to return a string in the middle, starting at left and ending at right. left and right included.
	string result = "";
	for (int i = left; i <= right; i++) result += s[i];
	return result;
}

double mod(double a, double b) {
	if ((int)a != a || (int)b != b||b==0) throw err();
	return (int)a  % (int)b;
}

string del_space(string s) {//to delete the <space> and <tab> in the input to avoid error.
	string result="";
	for (int i = 0; i < s.size(); i++) {
		if (s[i] != ' ' && s[i] != '\t') result += s[i];
	}
	return result;
}

int factorial(double n) {
	//to calculate n!
	if ((int)n != n || n<0) throw err();//not a positive integer.
	if (n == 0) return 1;
	int result = 1;
	for (int i = 1; i <= n; i++) result *= i;
	return result;
}

double get_token(string s,char op) {
	//return the last position of a token in string s.
	//a positive number for '!', '+' or '*', a negative one for '-' or '/'.
	//To express '%', return the position+0.1 for differentiaton.
	//As for op, '+' represents '+' or '-', '*' for '*' or '/', '!' for itself.
	int inside[100000] = { 0 };//judge whether a specific char in the string is in a pair of '()'.0 for out, 1 for in.
	int l;
	for (int r = 0; r < s.size(); r++) {//find all ')'s.
		if (s[r] == ')') {
			for (l = r; l >= 0; l--) {
				if (s[l] == '(' && inside[l] == 0) {//search for the nearest '('
					inside[l] = 1;
					break;
				}
				inside[l] = 1;
			}
		}
	}
	for (int i = s.size(); i>=0; i--) {
		if (inside[i]==0) {//not indise "()"
			if (op == '+') {
				if (s[i] == '+') return i;
				if (s[i] == '-') return -i;
			}
			if (op == '*') {
				if (s[i] == '*') return i;
				if (s[i] == '/') return -i;
				if (s[i] == '%') return i + 0.1;//To express '%', return the position+0.1 for differentiaton.
			}
			if (op == '!'&&s[i] == '!') return i;
		}
	}
	return -0.1;//which means not found.
	//This function cannot differentiate '+(*)' and '-(/)' while it is on postion 0, so it should be specially considered later.
}

string input() {
	string equation;
	cout << '>';
	std::getline(cin, equation);
	return equation;
}

double str_to_num(string s) {
	if (s == "R") {
		if (saved) return saving;
		else throw err();
	}
	if (s == "")  throw err(); 
	int num_of_dot = 0;
	for (int i = 0; i < s.size(); i++) {//to judge whether it is a number.
		if (s[i]<'0' || s[i]>'9') {
			if (s[i] != '.') throw err();
			else if (s[i] == '.') {
				num_of_dot++;
				if (num_of_dot > 1) throw err();//a number should not include 2 dots.
			}
		}
	}
	istringstream iss(s);
	double  x;
	if (iss >> x) return x;
	return 0;
}

double very_primary(string equation) {
	//deal with numbers and expressions in '()'.
	int right = -1, left = equation.size();
	bool found = false;
	for (int j = 0; j < equation.size(); j++) {
		if (equation[j] == '('&& found == false) left = j, found = true;
		if (equation[j] == ')') right = j, found = true;
	}//to search for the outermost '()'
	string inside = "";//the equation indside the "()".
	if ((left != 0 || right != equation.size() - 1) && found) throw err();//if a couple of '()' has been found, it must be outermost.
	for (int i = left + 1; i < right; i++) inside += equation[i];
	if (found) return expression(inside);
	else return str_to_num(equation);
	return 0;
}

double primary(string equation) {
	//Different from the book, the function "primary" deals with '!'.
	//See more in "veryPrimary".
	double position = get_token(equation, '!');
	if (position == -0.1) return very_primary(equation);
	if (position+1 != equation.size()) throw err();
	else {
		string left=""; //which means the part in equation except '!'.
		for (int j = 0; j < equation.size() - 1; j++) left += equation[j];
		return factorial(very_primary(left));
	}
	return 0;
}

double term(string equation) {
	//The function returns the value of the string equation.
	//"term" only deals with '*','/', and '%'
	double position = get_token(equation, '*');
	int abspos = (int)abs(position);
	string l_equation = "", r_equation = "";//which represents the term(expression) on the left(right) side of the token.
	if (position != -0.1) {
		for (int i = 0; i < abspos; i++) l_equation += equation[i];
		for (int j = abspos + 1; j < equation.size(); j++) r_equation += equation[j];
	}
	if (position == 0||position==0.1) throw err();//as '*', '/' and '%' should never exist at the beginning of a term. 
	else if (position > 0) { 
		if ((int)position == position/*means it is a '*'. */) return term(l_equation) * primary(r_equation); 
		else return mod(term(l_equation),primary(r_equation));
	}
	else if (position == -0.1) return primary(equation);// which means that the equation doesn't include a '*', '/' or '%', in other words, it is a "primary".
	else if (primary(r_equation) == 0) throw err();//divided by 0.
	else return term(l_equation)/primary(r_equation);//find a '/'.
	return 0;
}

double expression(string equation) {
	//The funtion expression returns the value of the string equation.
	//"expression" only deals with '+' and '-'.
	//'*','/'or deeper, see term().
	//equation = "0+" + equation;//avoid error when '+' or '-' is in the front.
	int left = equation.size(), right = -1;//which represents the position of '(' and ')', as tokens in '()' should be specially considered.
	bool found = false; //whether '(' has been found.
	int numl = 0, numr = 0;//The number of '(' and ')'.
	for (int j = 0; j < equation.size(); j++) {
		if (equation[j] == '(') {
			numl++;
			if (!found)	left = j, found = true;
		}
		if (equation[j] == ')') right = j, found = true, numr++;
	}
	//if (numl != numr) throw err();//invalid input. 
	double position = get_token(equation, '+');
	int abspos = (int)abs(position);
	string l_equation="", r_equation="";//which represents the term(expression) on the left(right) side of the token.
	if (position != -0.1) {
		for (int i = 0; i < abspos; i++) l_equation += equation[i];
		for (int j = abspos + 1; j < equation.size(); j++) r_equation += equation[j];
	}

	if (position == 0) {//as the "get_token" fuction cannot differentiate '+' and '-' when it is at the position 0, it should be specially processed. 
		if (equation[0] == '+') return term(r_equation);
		if (equation[0] == '-') return 0 - term(r_equation);
	}
	else if (position > 0) return expression(l_equation) + term(r_equation);//find a '+'.
	else if (position == -0.1) return term(equation);// which means that the equation doesn't include a '+' or '-', in other words, it is a term.
	else return expression(l_equation) -term(r_equation);//find a '-'.
	return 0;
}

int main()
{
	string equation;
	while (true) {
		try{
			save = false;
			equation = del_space(input());
			double result = expression(deal_with_negative(init(equation)));
			if (save) saving = result, saved = true;//save while 'M'.
			cout << '=' << result << endl;
		}
		catch (err) {
			cerr << "error\n";
		}
	}
	return 0;
}

