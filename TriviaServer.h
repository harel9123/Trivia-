#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <map>
#include <queue>
#include <exception>
#include <thread>
#include <condition_variable>

#include "Helper.h"
#include "User.h"
#include "Room.h"
#include "ReceivedMessage.h"
#include "DataBase.h"
#include "Protocol.h"
#include "Validator.h"

using namespace std;

class TriviaServer
{
	private:
		SOCKET _socket;
		sockaddr_in _client;
		map<SOCKET, User *> _connectedUsers;
		DataBase _db;
		map<int, Room *> _roomsList;

		mutex _mtxReceivedMessages;
		queue<ReceivedMessage *> _queRcvMessages;

		static int _roomIdSequence;

		void bindAndListen() throw();
		SOCKET acceptConnection() throw();
		void clientHandler(SOCKET client);
		void safeDeleteUser(ReceivedMessage *);

		User * handleSignin(ReceivedMessage *) throw();
		bool handleSignup(ReceivedMessage *) throw();
		void handleSignout(ReceivedMessage *) throw();

		void handleLeaveGame(ReceivedMessage *) throw();
		void handleStartGame(ReceivedMessage *) throw();
		void handlePlayerAnswer(ReceivedMessage *) throw();

		bool handleCreateRoom(ReceivedMessage *) throw();
		bool handleCloseRoom(ReceivedMessage *) throw();
		bool handleJoinRoom(ReceivedMessage *) throw();
		bool handleLeaveRoom(ReceivedMessage *) throw();
		void handleGetUsersInRoom(ReceivedMessage *) throw();
		void handleGetRooms(ReceivedMessage *) throw();

		void handleGetBestScores(ReceivedMessage *) throw();
		void handleGetPersonalStatus(ReceivedMessage *) throw();

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