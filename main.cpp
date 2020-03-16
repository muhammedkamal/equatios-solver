

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


class Variable {
public:
	string Factor;
	string Var;
	float IntegerFactor = 1;

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
			for (unsigned int i = 0; i < NewVariable.size(); i++) {
				if (isdigit(NewVariable[i])) {
					Factor += NewVariable[i];
				}
				else {
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
		else {
			Factor = "1";
			for (unsigned int i = 0; i < NewVariable.size(); i++) {
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
vector<string> vars;

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
	for (unsigned int i = 0; i < Equations.size(); i++)
		for (int j = 0; j < Equations[i].Variables.size(); j++)
			if (!count(vars.begin(), vars.end(), Equations[i].Variables[j].Var))
				vars.push_back(Equations[i].Variables[j].Var);
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
void PrintMatrix(int** Matrix, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << Matrix[i][j] << "\t";
		}
		cout << "\n";
	}
	cout << "\n";
}

int** PrintCramersCoefficientMatrixD() {
	int rows = Equations.size();
	int cols = vars.size();
	int** Matrix = new int*[rows];
	for (int i = 0; i < rows; i++) {
		Matrix[i] = new int[cols];
	}
	int Counter;
	for (int i = 0; i < rows; i++) {
		Counter = 0;
		for (int j = 0; j < cols; j++) {
			if (Counter < Equations[i].Variables.size())
			{
				if (Equations[i].Variables[Counter].Var == vars[j]) {
					if (Equations[i].Operators[Counter] == '-') {
						Matrix[i][j] = Equations[i].Variables[Counter].IntegerFactor * -1;
						Counter++;
					}
					else {
						Matrix[i][j] = Equations[i].Variables[Counter].IntegerFactor;
						Counter++;
					}

				}
				else {
					Matrix[i][j] = 0;
				}
			}
			else {
				Matrix[i][j] = 0;
			}
		}
	}

	return Matrix;
}

int** PrintCramersCoefficientMatrixDx1(int** Matrix) {
	for (int i = 0; i < Equations.size(); i++) {
		Matrix[i][0] = Equations[i].IntegerResult;
	}
	return Matrix;
}





int main()
{
	int Number_of_Equations;
	unsigned int counter = 0;
	cin >> Number_of_Equations;
	cin.ignore();
	string dummy;

	while (counter < Number_of_Equations) {
		getline(cin, dummy);
		Equation eq(dummy);
		Equations.push_back(eq);
		counter++;
	}
	int i, j;
	string MainMenuSelector;
	while (MainMenuSelector != "quit") {
		getline(cin, MainMenuSelector);
		if (MainMenuSelector == "num_vars")
		{
			cout << Num_Vars() << "\n";
		}
		else if (MainMenuSelector.substr(0, 8) == "equation")
		{
			remove_if(MainMenuSelector.begin(), MainMenuSelector.end(), isspace);
			string s = MainMenuSelector.substr(8, 1);
			int v = stoi(s.c_str());
			PrintEquation(getEquation(v - 1));
		}
		else if (MainMenuSelector.substr(0, 6) == "column")
		{

			string s = MainMenuSelector.substr(7);
			PrintColumn(s);
		}
		else if (MainMenuSelector.substr(0, 3) == "add")
		{
			remove_if(MainMenuSelector.begin(), MainMenuSelector.end(), isspace);
			string s = MainMenuSelector.substr(3, 1);
			int i = stoi(s.c_str());
			string a = MainMenuSelector.substr(4, 1);
			int j = stoi(a.c_str());
			PrintEquation(AddEuations(getEquation(i - 1), getEquation(j - 1)));
		}
		else if (MainMenuSelector.substr(0, 8) == "subtract")
		{
			remove_if(MainMenuSelector.begin(), MainMenuSelector.end(), isspace);
			string s = MainMenuSelector.substr(8, 1);
			int i = stoi(s.c_str());
			string a = MainMenuSelector.substr(9, 1);
			int j = stoi(a.c_str());

			PrintEquation(SubEuations(getEquation(i - 1), getEquation(j - 1)));
		}

		else if (MainMenuSelector.substr(0, 11) == "substitute")
		{
			remove_if(MainMenuSelector.begin(), MainMenuSelector.end(), isspace);
			string s = MainMenuSelector.substr(14, 1);
			int i = stoi(s.c_str());
			string a = MainMenuSelector.substr(15, 1);
			int j = stoi(a.c_str());
			PrintEquation(SubstitEuations(getEquation(i - 1), getEquation(j - 1)));
		}
		else if (MainMenuSelector == "D")
		{
			PrintMatrix(PrintCramersCoefficientMatrixD(), Equations.size(), vars.size());
		}

		else if (MainMenuSelector == "D x1")
		{
			PrintMatrix(PrintCramersCoefficientMatrixDx1(PrintCramersCoefficientMatrixD()), Equations.size(), vars.size());
		}

	}
	return 0;
}






