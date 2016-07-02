#pragma once

#include <iostream>
#include <string>

#include "Helper.h"
#include "Room.h"
#include "Game.h"

using namespace std;

class Room;
class Game;

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

		bool createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime);
		bool joinRoom(Room * room);

		void leaveRoom();
		int closeRoom();
		bool leaveGame();
};