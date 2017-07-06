#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

const int MAXDEG = 4;
const int RES = 6;

int parseFunction(string function, double constants[], int cExp = MAXDEG) {
	string current;
	int type = 0;
	int strIndex = 0;
	while (current != "x" && current != "s" && current != "c") {
		current = function[strIndex];
		strIndex++;
	}
	if (function.substr(0, strIndex - 1) == "")
		constants[MAXDEG-cExp] = 1;
	else
		constants[MAXDEG-cExp] = atoi(function.substr(0, strIndex - 1).c_str());
	if (current == "x") {
		if (cExp != 1)
			parseFunction(function.substr(strIndex + 2, function.length()), constants, cExp - 1);
		else
			constants[MAXDEG] = atoi(function.substr(strIndex, function.length()).c_str());
	}
	else if (current == "s") {
		type = 1;
		double tempPoly[5] = {0};
		parseFunction(function.substr(strIndex + 3, function.length()), tempPoly, 1);
		constants[1] = tempPoly[3];
		constants[2] = tempPoly[4];
		while (current != ")") {
			current = function[strIndex];
			strIndex++;
		}
		constants[3] = atoi(function.substr(strIndex + 1, function.length()).c_str());
	}
	else if (current == "c") {
		type = 2;
		double tempPoly[5] = {0};
		parseFunction(function.substr(strIndex + 3, function.length()), tempPoly, 1);
		constants[1] = tempPoly[3];
		constants[2] = tempPoly[4];
		while (current != ")") {
			current = function[strIndex];
			strIndex++;
		}
		constants[3] = atoi(function.substr(strIndex + 1, function.length()).c_str());
	}
	return type;
}

void findYValues(double yVals[RES], double xMin, double xMax, int type, double con[5]) {
	double counter = (xMax - xMin) / RES;
	double x = xMin;
	cout << endl;
	for (int i = 0; i < MAXDEG + 1; i++) {
		cout << con[i] << " ";
	}
	for (int y = 0; y < RES; y++) {
		if (type == 0)
			yVals[y] = con[0] * pow(x, 4) + con[1] * pow(x, 3) + con[2] * pow(x, 2) + con[3] * pow(x, 1) + con[4];
		else if (type == 1)
			yVals[y] = con[0] * sin(con[1] * x + con[2]) + con[3];
		else if (type == 2) 
			yVals[y] = con[0] * cos(con[1] * x + con[2]) + con[3];
		x += counter;
	}
}

void findMinMax(double yVals[RES], double & min, double & max) {
	for (int i = 0; i<RES; i++) {
		if (yVals[i] < min)
			min = yVals[i];
		if (yVals[i] > max)
			max = yVals[i];
	}
}

int main() {
	string functionToParse = "2x^4+2x^3+0x^2+5x+1";
	double xMin = 0, xMax = 2*M_PI, yMin = 0, yMax = 10;
	ofstream fout("output.txt");
	double constants[MAXDEG+1] = {0};
	double yVals[RES] = {0};
	int type = parseFunction(functionToParse, constants, MAXDEG);
	cout << type;
	findYValues(yVals, xMin, xMax, type, constants);
	findMinMax(yVals, yMin, yMax);
	cout << endl;
	for (int i = 0; i < MAXDEG + 1; i++) {
		cout << constants[i] << " ";
	}
	fout << RES << endl << yMin << endl << yMax << endl;
	
	for (int i = 0; i < RES; i++) {
		fout << yVals[i] << " ";
	}
	fout.close();
	return 0;
}
