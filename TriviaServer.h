#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <queue>
#include <thread>
#include <map>
#include <condition_variable>

#include "Helper.h"
#include "Socket.h"
#include "User.h"
#include "Room.h"
#include "DataBase.h"
#include "ReceivedMessage.h"
#include "Validator.h"

using namespace std;

class TriviaServer
{
	private:
		Socket * s;
		SOCKET _socket;
		map<SOCKET, User *> _connectedUsers;
		DataBase * _db;
		map<int, Room *> _roomsList;

		queue<ReceivedMessage *> _queRcvMessages;

		static int _roomIdSequence;

		void bindAndListen() throw();
		SOCKET acceptConnection() throw();
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