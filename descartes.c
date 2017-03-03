#include "NXT_FileIO.c"

const int XPOW = 10;
const int BOARD_WIDTH = 500;
const int BOARD_LENGTH = 500;

void turnMotorsOff() {
	motor[motorB] = 0;
	motor[motorC] = 0;
}

void waitForButton(int button) {
	while (nNxtButtonPressed != button) {}
	while (nNxtButtonPressed != -1) {}
}

float getYPower(float xLength, float yDist) {
	displayString(5, "x %d", (yDist / xLength) * XPOW);
	return ((yDist/xLength) * XPOW);
}

void toPoint(float currentY, float nextY, float xLength, float yLength) {
	nMotorEncoder[motorB] = 0;
	motor[motorB] = XPOW;
	motor[motorC] = getYPower(xLength, yLength * (nextY - currentY));
	while (fabs(nMotorEncoder[motorB]) < xLength) {
		if (SensorValue[S1] == 1)
			stopAllTasks();
	}
	turnMotorsOff();
}

float calibrate(float &xLength, float &yLength, TFileHandle &fin) {
	float resolution, yMin, yMax = 0;
	readFloatPC(fin, resolution);
	readFloatPC(fin, yMin);
	readFloatPC(fin, yMax);
	xLength = BOARD_WIDTH / resolution;
	yLength = BOARD_LENGTH / (yMax - yMin);
	return yMin;
}

void setReference() {
	motor[motorB] = -10;
	while (nNxtButtonPressed != 3 && SensorValue[S1] == 0) {}
	motor[motorB] = 0;
	nMotorEncoder[motorB] = 0;
	waitForButton(3);
	motor[motorC] = -10;
	while (nNxtButtonPressed != 3 && SensorValue[S1] == 0) {}
	motor[motorC] = 0;
	nMotorEncoder[motorC] = 0;
	waitForButton(3);
}

void liftPen(bool lift) {
	if (lift)
		motor[motorA] = -50;
	else
		motor[motorA] = 50;
	wait1Msec(300);
	motor[motorA] = 0;
}

task main() {
	TFileHandle fin;
	bool fileOkay = openReadPC(fin, "output.txt");
	if (!fileOkay) {
		displayString(0,"Could not open file.");
		wait1Msec(5000);
		stopAllTasks();
	}
	SensorType[S1] = sensorTouch;
	float xLength = 0, yLength = 0, yMin = 0, current = 0, next = 0;
	displayString(0, "Welcome!");
	displayString(1, "Press orange button");
	displayString(2, "to start calibration");
	waitForButton(3);
	eraseDisplay();
	displayString(0, "Calibrating...");
	setReference();
	yMin = calibrate(xLength,yLength,fin);
	readFloatPC(fin, current);
	toPoint(yMin, current, xLength, yLength);
	eraseDisplay();
	displayString(0, "Calibration finished");
	displayString(1, "Press orange button");
	displayString(2, "to begin!");
	waitForButton(3);
	eraseDisplay();
	displayString(0, "Working...");
	liftPen(false);
	while (readFloatPC(fin, next)) {
		toPoint(current, next, xLength, yLength);
		current = next;
	}
	liftPen(true);
	eraseDisplay();
	displayString(0, "Done!");
	displayString(1, "Press orange button");
	displayString(2, "to finish");
	waitForButton(3);
	closeFilePC(fin);
}
