#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <map>

#pragma warning(disable : 4996)
#include <windows.h>
using namespace std;
std::string currentDateTime() {
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	char buffer[128];
	strftime(buffer, sizeof(buffer), "%X", now);
	return buffer;
}

int main()
{
	setlocale(LC_ALL, "RU");
	cout << "s - server, c - client" ;
	sf::TcpSocket socket;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	char type;
	cin >> type;
	if (type == 's') {
		sf::TcpListener listener;
		listener.listen(2000); //передаем виртуальный порт, через который будут передаваться данные
			if (listener.accept(socket) != sf::Socket::Done) {//через функцию accept принимаем клиента для обмена данными
			cout << "Error";
		}
	}
	else if (type == 'c') {
		if (socket.connect(ip, 2000) != sf::Socket::Done) cout << "Error!\n";
	}
	string name;
	cout << "Enter your name:\n";
	cin >> name;
	socket.setBlocking(false); //нужно для того, чтобы при передаче данных программа не останавливалась
	string message = "";
	sf::Packet packet; //packet - форма записи данных которая посылается по сети в сформатированном виде
	string action;
	while (true) {
		cout << "Enter action: \n";
		string time;
		getline(cin, message);
		time = currentDateTime();
		if (message != "Get") {
			if (message != "") { //отправка сообщения
				packet.clear();
				packet << name << message << time;
				socket.send(packet);
				message = "";
			}
		}
		if (message == "Get") {
			vector <string> messages;
			while (socket.receive(packet) == sf::Socket::Done) {
				string nameRec;
				string messageRec;
				string cur_t;
				string res;
				packet >> nameRec >> messageRec >> cur_t;
				res =  nameRec + ": " + messageRec + " at " + cur_t;
				messages.push_back(res);
			}
			if (messages.size() == 0) {
				cout << "Новых сообщений нет!\n";
			}
			else {
				for (auto w : messages) cout << w << endl;
			}
		}
	}
}