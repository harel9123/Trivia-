#pragma once

//Client codes
#define SIGNIN_CODE 200
#define SIGNOUT_CODE 201
#define SIGNUP_CODE 203

#define ROOM_LIST_REQ 205
#define ROOM_USER_REQ 207
#define ROOM_JOIN_REQ 209
#define ROOM_LEAVE_REQ 211
#define CREATE_ROOM_REQ 213
#define CLOSE_ROOM_REQ 215
#define START_GAME_REQ 217

#define ANSWER_CODE 219

#define HIGHSCORE_REQ 223
#define INFO_REQ 225

#define LEAVE_GAME_REQ 222
#define CLOSING_GAME_REQ 299
//Client codes

//Server codes
#define SIGNIN_RESP 102
#define SIGNIN_SUCCESS 0
#define SIGNIN_WRONG 1
#define SIGNIN_CONNECTED 2

#define SIGNUP_RESP 104
#define SIGNUP_SUCCESS 0
#define PASS_ILLEGAL 1
#define USER_EXISTS 2
#define NAME_ILLEGAL 3
#define OTHER 4

#define ROOM_LIST_RESP 106
#define ROOM_USER_RESP 108
#define LEAVE_ROOM_RESP 1120

#define JOIN_ROOM_RESP 110
#define JOIN_ROOM_SUC 0
#define JOIN_ROOM_FULL 1
#define JOIN_ROOM_OTHER 2

#define CREATE_ROOM_RESP 114
#define CREATE_ROOM_SUC 0
#define CREATE_ROOM_FAIL 1

#define CLOSE_ROOM_RESP 116
#define SEND_QUESTION 118
#define SEND_QUESTION_FAILED 0

#define ANSWER_RELATE 120
#define CORRECT 1
#define INCORRECT 0

#define GAME_OVER 121
#define BEST_SCORES_RESP 124
#define INFO_RESP 126
//Server codes