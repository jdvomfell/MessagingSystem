#include "MessagingSystem.h"

// If There is no more terminals that have acces to this message it will delete itself
void Message::clean() {

	if (--active <= 0) {
		if (data != nullptr)
			delete(data);
		delete(this);
	}

}

// Inserts a message onto the back of the circle array if there is space.
void MessageArray::insert(Message * message) {

	// If the message array is full clean and delete the message
	if (messages[back] != nullptr) {
		message->clean();
		return;
	}

	messages[back] = message;
	back = (++back % MAX_MESSAGES);

}

// Prints all message types in the list.
void MessageArray::print() {

	for (int i = 0; i < MAX_MESSAGES; i++) {
		if (messages[i] == nullptr) {
			printf("Message %d: NULL\n", i);
			continue;
		}
		printf("Message %d: %s\n", i, messages[i]->type.c_str());
	}
	printf("\n\n");

}

// Returns and removes the oldest message from the front of the circle array.
Message * MessageArray::getMessage() {

	Message * message;

	if (messages[front] == nullptr)
		return nullptr;

	message = messages[front];
	messages[front] = nullptr;
	front = (++front % MAX_MESSAGES);

	return message;

}

// Posts the next message on the bus to subscribers.
void MessageBus::post() {

	Message * message;
	bool posted = false;

	while (true) {

		message = messages.getMessage();

		if (message == nullptr)
			return;

		//for (size_t i = 0; i < terminals.size(); i++) {
		for(std::set<MessageTerminal *>::iterator it = terminals.begin(); it != terminals.end(); it++) {
			if ((*it)->isSubscribed(message->type) || (*it)->isSubscribed("All")) {
				message->active++;
				(*it)->messages.insert(message);
				posted = true;
			}
		}

		// If the message was not posted it has no where to go, clean and remove it
		if (posted == false) message->clean();

	}

}

// Posts the message passed to subscribers.
void MessageBus::post(Message * message) {

	bool posted = false;

	for (std::set<MessageTerminal *>::iterator it = terminals.begin(); it != terminals.end(); it++) {
		if ((*it)->isSubscribed(message->type) || (*it)->isSubscribed("All")) {
			message->active++;
			(*it)->messages.insert(message);
			posted = true;
		}
	}

	// If the message was not posted it has no where to go, clean and remove it
	if (posted == false) message->clean();

}

// Adds a terminal to the message bus for posting.
void MessageBus::addTerminal(MessageTerminal * terminal) {

	if (!terminals.insert(terminal).second)
		printf("ERROR: Trying to re-add terminal to bus\n");

}

// Creates a new message terminal and adds itself to the message bus.
MessageTerminal::MessageTerminal(MessageBus * bus) {

	this->bus = bus;
	this->bus->addTerminal(this);

}

// Posts the given message to the message bus.
void MessageTerminal::post(Message * message) {

	bus->messages.insert(message);

}

// Retrieves the first message on the list.
Message * MessageTerminal::getMessage() {

	return messages.getMessage();

}

// Subscribes to the given message type.
void MessageTerminal::subscribe(std::string type) {

	subscriptions.insert(type);

}

// Un-Subscribes to the given message type.
void MessageTerminal::unsubscribe(std::string type) {

	std::set<std::string>::iterator it;

	it = subscriptions.find(type);

	if (it != subscriptions.end())
		subscriptions.erase(it);

}

// Checks if the message terminal is subscribed to the given message type.
bool MessageTerminal::isSubscribed(std::string type) {

	if (subscriptions.find(type) == subscriptions.end())
		return false;
	else
		return true;

}