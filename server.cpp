#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <map>

#pragma warning(disable : 4996)

using namespace std;
string currentDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%X", now);
	return buffer;
}

int main()
{
	setlocale(LC_ALL, "RU");
	cout << "s - server, c - client\n";
	sf::TcpSocket socket;
	sf::TcpSocket socket2;


	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	string type;
	cin >> type;
	if (type == "s") {
		sf::TcpListener listener;
		listener.listen(2000); 
		listener.accept(socket2);
		if (listener.accept(socket) != sf::Socket::Done) {
			if (listener.accept(socket2) != sf::Socket::Done) {
				cout << "Error";
			}
		}
	}
	else if (type == "c") {
		if (socket.connect(ip, 2000) != sf::Socket::Done) cout << "Error!\n";
	}
	else if (type == "c2") {
		if (socket2.connect(ip, 2000) != sf::Socket::Done) cout << "Error!\n";
	}
	string name;
	cout << "Enter your name:\n";
	cin >> name;
	socket.setBlocking(false); //нужно для того, чтобы при передаче данных программа не останавливалась
	socket2.setBlocking(false);
	string message = "";
	sf::Packet packet; //packet - форма записи данных которая посылается по сети в сформатированном виде
	string action;
	while (true) {
		cout << "Enter action: \n";
		string time;
		getline(cin, message);
		time = currentDateTime();
		if (message != "Get") {
			if (message != "") { 
				packet.clear();
				packet << name << message << time;
				if(type == "c")
				socket.send(packet);
				else
					socket2.send(packet);
				message = "";
			}
		}
		if (message == "Get") {
			vector <string> messages;
			while (socket.receive(packet) == sf::Socket::Done and socket2.receive(packet) == sf::Socket::Done) {
				string nameRec;
				string messageRec;
				string cur_t;
				string res;
				packet >> nameRec >> messageRec >> cur_t;
				res = nameRec + ": " + messageRec + " at " + cur_t;
				messages.push_back(res);
			}
			if (messages.size() == 0) {
				cout << "Новых сообщений пока нет!\n";
			}
			else {
				for (auto w : messages) cout << w << endl;
			}
		}
	}
}