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

#include <iostream>
#include <Windows.h>
#include <ctime>

#include "Capture.h"
#include "Time.h"
#include "Mouse.h"

using namespace std;

float MouseSensitivity = 15.00f; //Change this to your sensivitiy!!!
bool HumanLikeMovements = false; //Should we use human like mouse movements?
bool Headshots = false; //Should we aim at the head?

int main(void)
{
	Capture recorder("Overwatch");

	cout << "[========= SETTINGS ========]" << endl;
	cout << "Mouse sensitivity : " << MouseSensitivity << endl;
	cout << "Mouse movements : " << string(HumanLikeMovements ? "Human-like" : "Aimbot-like") << endl;
	cout << "Aiming for the : " << string(Headshots ? "Head" : "Body")  << endl << endl;

	cout << "[========= HOW TO USE & SETUP ========]" << endl;
	cout << "Make sure that the display mode is 'BORDERLESS WINDOWED'!!!" << endl;
	cout << "Aim assistance will work only on detection of a health bar." << endl;
	cout << "Press 'CAPSLOCK' to terminate the program at anytime!" << endl << endl;

	cout << "Scanning for Overwatch process handle.";
	while (!recorder.isWindowRunning())
	{
		cout << ".";
		Sleep(1000);
	}
	cout << endl << "Overwatch found!!!" << endl;
	
	recorder.switchToWindow(); //Switch to Overwatch
	Sleep(1000);
	Beep(1000, 250);

	//======    Main Code    ======//
	Screenshot screeny;
	Mouse mousey(recorder.getWidth(), recorder.getHeight(), MouseSensitivity);
	int x, y;
	bool run = true;

	recorder.screenshotGDI(screeny);
	while (run)
	{
		if (GetAsyncKeyState(VK_CAPITAL))
			run = false;

		//======   TRIGGER BOT   ======//
		//if (screeny.triggerBot())
		//	mousey.click(50);

		//======   AIMBOT   ======//
		if (screeny.findPlayer(x, y, Headshots)) //set to true if you aim headshots
		{
			if(HumanLikeMovements)
				mousey.moveSmoothAuto(x, y);
			else
			{
				mousey.moveTo(x, y); //use moveSmooth for human like movements and omitt waitTillNextFrame
				recorder.waitTillNextFrame(screeny); //eliminates mouse from 'overshoot' because the same frame is captured 2x in a row. Can be omitted if using moveSmooth, Sleep or click functions. 
			}
		}
			
		while (!recorder.screenshotGDI(screeny)) Sleep(100);  //Capture a new screeny until true
	}

	screeny.FreeMemory();
	Beep(1500, 500);
	cout << "Press 'Enter' to close!";
	cin.ignore();
	return 0;
}