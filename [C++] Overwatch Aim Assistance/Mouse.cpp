/*
 *Overwatch Aim Assitance
 *Copyright (C) 2016  Juan Xuereb
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.

 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Mouse.h"

Mouse::Mouse(int captureWidth, int captureHeight, const float sensitivity)
{
	screenWidth = captureWidth;
	screenHeight = captureHeight;
	centreScreenX = captureWidth / 2;
	centreScreenY = captureHeight / 2;
	mouseSensitivity = sensitivity;

	modifier = mouseSensitivity*constant;
}

const float PI = 3.1415927;

/* Calculates actual coordinates for mouse movement based on sensitivity and a constant. */
void Mouse::calibrateCoordinates(int &x, int &y)
{
	x = x - centreScreenX;
	y = y - centreScreenY;

	x = (int)((float)x / modifier);
	y = (int)((float)y / modifier);
}

/* Moves to x,y coordinates after processed into actual coordinates (based on sensitivity and const). */
void Mouse::moveTo(int x, int y)
{
	calibrateCoordinates(x, y);
	mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	//std::cout << x << "," << y << std::endl;// << "      pred: " << predX << "," << predY << std::endl;
}

/* Mouse click @ 0,0 */
void Mouse::click(int leftDownMS)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(leftDownMS);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Mouse::moveSmooth(int x, int y, int aimSpeed)
{
	calibrateCoordinates(x, y);
	int pWidth = (0.5 * screenWidth) / 100;
	int pHeight = (0.5 * screenHeight) / 100;

	int movedX = 0;
	int movedY = 0;

	int steps = abs(x) + abs(y);
	int sleep = 10-aimSpeed;

	float divX = (float)x / (float)steps;
	float divY = (float)y / (float)steps;

	float X = 0.0f;
	float Y = 0.0f;
	for (int i = 0; i < steps; i++)
	{
		X += divX;
		Y += divY;
		if (X > 1.0f || X < 1.0f) //1 pixel is the smallest absolute value that we can move
		{
			movedX += abs(X);
			mouse_event(MOUSEEVENTF_MOVE, int(X), 0, 0, 0);
			X -= int(X); //we do not want to lose the remainder -> leads to inaccuracies of lost
		}

		if (Y > 1.0f || Y < 1.0f) //^
		{
			movedY += abs(Y);
			mouse_event(MOUSEEVENTF_MOVE, 0, int(Y), 0, 0);
			Y -= int(Y); //^
		}

		if (movedX >= pWidth)
		{
			movedX = 0;
			Sleep(sleep);
		}
		if (movedY >= pHeight)
		{
			movedY = 0;
			Sleep(sleep);
		}
	}
}