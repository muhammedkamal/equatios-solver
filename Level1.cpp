// Level1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


class Variable {
public:
	string Factor;
	string Var;
	float IntegerFactor;

	Variable() {

	}
	Variable(string NewVariable) {
		SetFactor(NewVariable);
		SetVariable(NewVariable);
		SetIntegerFactor();
	}
	void SetFactor(string NewVariable) {
		unsigned int i;
		for (i = 0; i < NewVariable.size() && isdigit(NewVariable[i]); i++);
		if (i == NewVariable.size()) {
			IntegerFactor = atoi(NewVariable.c_str());
		}
		else {
			for (unsigned int i = 0; i < NewVariable.size() - 2; i++) {
				if (NewVariable[i] >= '0' || NewVariable[i] <= '9') {
					Factor += NewVariable[i];
				}
				else {
					if (i == 0) {
						Factor = 1;
					}
					i = NewVariable.size();
				}
			}
		}
	}
	void SetIntegerFactor() {
		if (Factor.size() != 0) {
			IntegerFactor = atoi(Factor.c_str());
		}
	}
	void SetVariable(string NewVariable) {
		if (Factor.size() != 0) {
			for (unsigned int i = Factor.size(); i < NewVariable.size(); i++) {
				Var += NewVariable[i];
			}
		}
	}
};


class Equation {
public:
	vector<Variable> Variables;
	vector<char> Operators;
	string StringedEquation;
	string Result;
	float IntegerResult;

	Equation() {
	}
	Equation(string Equ) {
		if (Equ[0] != '-' && Equ[0] != '+') {
			StringedEquation = "+" + Equ;
		}
		else {
			StringedEquation = Equ;
		}


		SettingEquation();
		SortEquation(StringedEquation);
		SettingOneTimeVariable();
		OneConstantTerm();
	}

	void SettingEquation() {
		string Var;
		for (unsigned int i = 0; i < StringedEquation.size(); i++) {
			if (StringedEquation[i] == '+' || StringedEquation[i] == '-') {
				if (i != 0) {
					Variable newVariable(Var);
					Variables.push_back(newVariable);
					Var.clear();
				}
				Operators.push_back(StringedEquation[i]);
			}
			else if (StringedEquation[i] == '=') {
				Variable newVariable(Var);
				Variables.push_back(newVariable);
				Var.clear();

				for (unsigned int j = i + 1; j < StringedEquation.size(); j++) {
					Result += StringedEquation[j];
				}
				IntegerResult = atoi(Result.c_str());
				i = StringedEquation.size();
			}
			else {
				Var += StringedEquation[i];
			}

		}
	}

	void SortEquation(string Equation) {
		unsigned int j;
		Variable tempVar;
		char tempOperator;
		for (unsigned int i = 0; i < Variables.size(); i++) {
			j = i;
			while (j > 0 && Variables[j].Var < Variables[j - 1].Var) {

				tempVar = Variables[j];
				Variables[j] = Variables[j - 1];
				Variables[j - 1] = tempVar;
				tempOperator = Operators[j];
				Operators[j] = Operators[j - 1];
				Operators[j - 1] = tempOperator;

				j--;
			}
		}
	}
	void SettingOneTimeVariable() {
		for (unsigned int i = 0; i < Variables.size() - 1; i++) {
			for (unsigned int j = i + 1; j < Variables.size(); j++) {
				if (Variables[i].Var == Variables[j].Var) {
					if (Operators[i] == '+' && Operators[j] == '-') {
						Variables[i].IntegerFactor -= Variables[j].IntegerFactor;
						if (Variables[i].IntegerFactor < 0) {
							Variables[i].IntegerFactor *= -1;
							Operators[i] = '-';
						}
						Variables.erase(Variables.begin() + j);
						Operators.erase(Operators.begin() + j);
					}
					else if (Operators[i] == '-' && Operators[j] == '+') {
						Variables[i].IntegerFactor += Variables[j].IntegerFactor;
						if (Variables[i].IntegerFactor > 0) {
							Operators[i] = '+';
						}
						Variables.erase(Variables.begin() + j);
						Operators.erase(Operators.begin() + j);
					}
					else {
						Variables[i].IntegerFactor += Variables[j].IntegerFactor;
						Variables.erase(Variables.begin() + j);
						Operators.erase(Operators.begin() + j);
					}
				}
			}

		}
	}

	void OneConstantTerm() {
		for (int i = 0; i < Variables.size(); i++)
		{
			if (Variables[i].Var.size() == 0) {
				if (Operators[i] == '-') {
					IntegerResult += Variables[i].IntegerFactor;
				}
				else if (Operators[i] == '+') {
					IntegerResult -= Variables[i].IntegerFactor;
				}
				Variables.erase(Variables.begin() + i);
				Operators.erase(Operators.begin() + i);
			}
		}
	}
};

vector <Equation> Equations;

void PrintEquations() {
	for (unsigned int i = 0; i < Equations.size(); i++) {
		for (unsigned int j = 0; j < Equations[i].Variables.size(); j++) {
			cout << Equations[i].Operators[j] << " " << Equations[i].Variables[j].IntegerFactor << Equations[i].Variables[j].Var << " ";
		}
		cout << "= " + Equations[i].Result << "\n";
	}
}
Equation getEquation(unsigned int i) {
	return Equations[i];
}

void PrintEquation(Equation e) {
	for (unsigned int i = 0; i < e.Variables.size(); i++) {
		cout << e.Operators[i] << e.Variables[i].IntegerFactor << e.Variables[i].Var;
	}
	cout << "=" << e.IntegerResult << "\n";
}

void PrintColumn(string Var) {
	for (unsigned int i = 0; i < Equations.size(); i++) {
		for (unsigned int j = 0; j < Equations[i].Variables.size(); j++) {
			if (Equations[i].Variables[j].Var == Var) {
				cout << Equations[i].Variables[j].IntegerFactor << "\n";
			}
		}

	}
}

unsigned int Num_Vars() {
	vector<string> vars;
	for each (Equation e in Equations)
		for each(Variable v in e.Variables)
			if (!count(vars.begin(), vars.end(), v.Var))
				vars.push_back(v.Var);
	return vars.size();
}

Equation AddEuations(Equation FirstEquation, Equation SecondEquation) {

	Equation Result;
	Result.Variables.reserve(FirstEquation.Variables.size() + SecondEquation.Variables.size());
	Result.Operators.reserve(FirstEquation.Operators.size() + SecondEquation.Operators.size());
	Result.Variables.insert(Result.Variables.end(), FirstEquation.Variables.begin(), FirstEquation.Variables.end());
	Result.Operators.insert(Result.Operators.end(), FirstEquation.Operators.begin(), FirstEquation.Operators.end());
	Result.Variables.insert(Result.Variables.end(), SecondEquation.Variables.begin(), SecondEquation.Variables.end());
	Result.Operators.insert(Result.Operators.end(), SecondEquation.Operators.begin(), SecondEquation.Operators.end());
	Result.IntegerResult = FirstEquation.IntegerResult + SecondEquation.IntegerResult;
	Result.SettingOneTimeVariable();
	return Result;
}

Equation SubEuations(Equation FirstEquation, Equation SecondEquation) {
	Equation Result;
	Result.Variables.reserve(FirstEquation.Variables.size() + SecondEquation.Variables.size());
	Result.Operators.reserve(FirstEquation.Operators.size() + SecondEquation.Operators.size());
	Result.Variables.insert(Result.Variables.end(), FirstEquation.Variables.begin(), FirstEquation.Variables.end());
	Result.Operators.insert(Result.Operators.end(), FirstEquation.Operators.begin(), FirstEquation.Operators.end());
	for (unsigned int i = 0; i < SecondEquation.Operators.size(); i++) {
		if (SecondEquation.Operators[i] == '+')
			SecondEquation.Operators[i] = '-';
		else
			SecondEquation.Operators[i] = '+';
	}

	Result.Variables.insert(Result.Variables.end(), SecondEquation.Variables.begin(), SecondEquation.Variables.end());
	Result.Operators.insert(Result.Operators.end(), SecondEquation.Operators.begin(), SecondEquation.Operators.end());
	Result.IntegerResult = FirstEquation.IntegerResult - SecondEquation.IntegerResult;
	Result.SettingOneTimeVariable();
	return Result;
}




Equation SubstitEuations(Equation FirstEquation, Equation SecondEquation) {
	Variable x2VarEq1, x2VarEq2;
	char x2OperEq2;

	for (unsigned int i = 0; i < FirstEquation.Variables.size(); i++) {
		if (FirstEquation.Variables[i].Var != "x2") {
			if (FirstEquation.Operators[i] == '+')
				FirstEquation.Operators[i] = '-';
			else
				FirstEquation.Operators[i] = '+';
		}
		else {
			x2VarEq1 = FirstEquation.Variables[i];
		}
	}

	for (unsigned int i = 0; i < FirstEquation.Variables.size(); i++) {
		if (FirstEquation.Variables[i].Var != "x2") {
			FirstEquation.Variables[i].IntegerFactor /= x2VarEq1.IntegerFactor;
		}
	}

	FirstEquation.IntegerResult /= x2VarEq1.IntegerFactor;

	for (unsigned int i = 0; i < SecondEquation.Variables.size(); i++) {
		if (SecondEquation.Variables[i].Var == "x2") {
			x2VarEq2 = SecondEquation.Variables[i];
			x2OperEq2 = SecondEquation.Operators[i];
		}
	}

	for (unsigned int i = 0; i < FirstEquation.Variables.size(); i++) {
		if (FirstEquation.Variables[i].Var != "x2") {
			if (x2OperEq2 == '-') {
				if (FirstEquation.Operators[i] == '+')
					FirstEquation.Operators[i] = '-';
				else
					FirstEquation.Operators[i] = '+';
			}
			FirstEquation.Variables[i].IntegerFactor *= x2VarEq2.IntegerFactor;
		}
	}


	FirstEquation.IntegerResult *= x2VarEq2.IntegerFactor;

	if (x2OperEq2 == '-') {
		FirstEquation.IntegerResult *= -1;
	}


	for (unsigned int i = 0; i < FirstEquation.Variables.size(); i++) {
		if (FirstEquation.Variables[i].Var == "x2") {
			FirstEquation.Variables.erase(FirstEquation.Variables.begin() + i);
			FirstEquation.Operators.erase(FirstEquation.Operators.begin() + i);
		}
	}

	for (unsigned int i = 0; i < SecondEquation.Variables.size(); i++) {
		if (SecondEquation.Variables[i].Var == "x2") {
			SecondEquation.Variables.erase(SecondEquation.Variables.begin() + i);
			SecondEquation.Operators.erase(SecondEquation.Operators.begin() + i);
		}
	}

	return AddEuations(FirstEquation, SecondEquation);
}


int main()
{
	string MainMenu = "1- Print number of variables\n2- Print Equation i\n3- Print column coefficients\n4- Add 2 Equations\n5- Substract 2 Equations\n6- Substitute x2 from equation i to equation j\n 0- Exit\n";
	int Number_of_Equations;
	unsigned int counter = 0;
	cout << "Please enter number of equations\n";
	cin >> Number_of_Equations;
	string dummy;

	while (counter < Number_of_Equations) {
		cout << "Enter an equation\n";
		cin >> dummy;
		Equation eq(dummy);
		Equations.push_back(eq);
		counter++;
	}
	int i, j;
	int MainMenuSelector = -1;
	while (MainMenuSelector != 0) {
		cout << MainMenu;
		cin >> MainMenuSelector;
		string variable;
		switch (MainMenuSelector)
		{
		case(1):
			cout << Num_Vars();
			break;
		case(2):
			cout << "Please Enter Equation Number: ";
			int EquationNumber;
			cin >> EquationNumber;
			PrintEquation(getEquation(EquationNumber - 1));
			break;
		case(3):
			cout << "Please Enter a Variable: ";
			cin >> variable;
			PrintColumn(variable);
			break;
		case(4):
			cout << "Please Enter the number of the first equation: ";			
			cin >> i;
			cout << "Please Enter the number of the second equation: ";
			cin >> j;
			PrintEquation(AddEuations(getEquation(i - 1), getEquation(j - 1)));
			break;
		case(5):
			cout << "Please Enter the number of the first equation: ";
			cin >> i;
			cout << "Please Enter the number of the second equation: ";
			cin >> j;
			PrintEquation(SubEuations(getEquation(i - 1), getEquation(j - 1)));
			break;
		case(6):
			cout << "Please Enter the number of the first equation: ";
			cin >> i;
			cout << "Please Enter the number of the second equation: ";
			cin >> j;
			PrintEquation(SubstitEuations(getEquation(i - 1), getEquation(j - 1)));
		default:
			break;
		}
	}
	return 0;
}




