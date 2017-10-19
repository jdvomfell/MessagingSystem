/* File  : MessagingSystem.h                                         */
/* Author: Jesse Vomfell                                             */
/* Date Created: 9th September 2017                                  */
/* Messaging system used to send events across a game architecture	 */
/* to potentially trigger an action.                                 */

#pragma once

#include <vector>
#include <string>
#include <set>

// Maximum amount of messages that can be held on a message terminal or bus. 
#define MAX_MESSAGES 30

// Message
// Desc: Sent between terminals and the bus.
// Type: Related to an event that happend and that other systems may want to respond to.
// Data: May contain an object that should be acted on as a result of the event.
// Active: How many terminals are actively using the message, must be 0 before deleting.
class Message {

public:

	std::string type;
	void * data;
	int active;

	Message(std::string t, void * d) { Message::type = t; Message::data = d; Message::active = 0; }
	void clean();

};

// MessageArray
// Desc: Circular array that holds the messages for a terminal or bus and limits the number of messages that can be active.
// Messages: List of messages to be stored in a terminal or a bus
// Front: The oldest message on the list
// Back: The newest message on the list
struct MessageArray {

	Message * messages[MAX_MESSAGES];
	int front;
	int back;

public:

	MessageArray() { front = 0; back = 0; for (int i = 0; i < MAX_MESSAGES; i++) messages[i] = nullptr; }

	void insert(Message *);
	void print();
	Message * getMessage();

};

// MessageBus
// Desc: Where terminals send their messages to be posted to subscribed terminals.
// Messages: List of messages to be posted.
// Terminals: List of terminals the messages can be posted to.
class MessageBus {

	std::vector<class MessageTerminal *> terminals;

public:

	MessageArray messages;

	void post();
	void post(Message *);
	void addTerminal(class MessageTerminal *);

};

// MessageTerminal
// Desc: Where the message bus will post messages.
// Bus: A reference To The message bus for posting.
// Messages: A list of messages the system has yet to process.
// Subscriptions: Message types that this terminal will recieve.
class MessageTerminal {

	class MessageBus * bus;
	std::set<std::string> subscriptions;

public:

	MessageArray messages;

	MessageTerminal(MessageBus *);

	void post(Message *);
	void subscribe(std::string type);
	void unsubscribe(std::string type);
	bool isSubscribed(std::string type);

};