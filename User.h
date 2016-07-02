#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <WinSock2.h>
#include "Room.h"
#include "Game.h"

#pragma comment(lib,"ws2_32.lib")
using namespace std;

class Room;

class User
{
	private:
		string _username;
		Room * _currRoom;
		Game * _currGame;
		SOCKET _sock;

	public:
		User(string name, SOCKET sock);
		void send(string);
		string getUsername() { return _username; };
		SOCKET getSocket() { return _sock; };
		Room * getRoom() { return _currRoom; };
		Game * getGame() { return _currGame; };

		void setGame(Game * game) { _currGame = game; _currRoom = nullptr; };
		void clearRoom();
		void clearGame();	

		bool createRoom(int, string, int, int, int);//Fill in
		bool joinRoom(Room * room);

		void leaveRoom();
		int closeRoom();
		bool leaveGame();
};