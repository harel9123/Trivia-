#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <map>
#include <queue>
#include <exception>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

#include "Socket.h"
#include "User.h"
#include "Room.h"
#include "ReceivedMessage.h"


using namespace std;

class TriviaServer
{
	private:
		Socket * _socket;
		map<SOCKET, User *> _connectedUsers;
		string _db;//string --> DataBase.
		map<int, Room *> _roomsList;

		mutex _mtxReceivedMessages;
		queue<ReceivedMessage *> _queRcvMessages;

		static int _roomIdSequence;

		void bindAndListen() throw();
		void accept();
		void clientHandler(SOCKET client);
		void safeDeleteUser(ReceivedMessage *);

		User * handleSignin(ReceivedMessage *);
		bool handleSignup(ReceivedMessage *);
		void handleSignout(ReceivedMessage *);

		void handleLeaveGame(ReceivedMessage *);
		void handleStartGame(ReceivedMessage *);
		void handlePlayerAnswer(ReceivedMessage *);

		bool handleCreateRoom(ReceivedMessage *);
		bool handleCloseRoom(ReceivedMessage *);
		bool handleJoinRoom(ReceivedMessage *);
		bool handleLeaveRoom(ReceivedMessage *);
		void handleGetUsersInRoom(ReceivedMessage *);
		void handleGetRooms(ReceivedMessage *);

		void handleGetBestScores(ReceivedMessage *);
		void handleGetPersonalStatus(ReceivedMessage *);

		void handleReceivedMessages();
		void addReceivedMessage(ReceivedMessage *);
		ReceivedMessage * buildReceiveMessage(SOCKET, int);

		User * getUserByName(string name);
		User * getUserBySocket(SOCKET sock);
		Room * getRoomById(int id);
	public:
		TriviaServer();
		~TriviaServer();

		void server();

		
};