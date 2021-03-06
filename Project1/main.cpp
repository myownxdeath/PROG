#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <limits>
#include <sstream>
#include <ctime>
#include <stdlib.h> // For getting windows command to clear console
#define NOMINMAX
#include <windows.h>
#include <bitset>

using namespace std;


struct client {
	unsigned int ID;
	string name;
	double ammount;
};
struct transaction {
	unsigned int ID;
	unsigned int day;
	unsigned int month;
	unsigned int year;
	vector<string> prodcts;
};
struct clientprod {
	unsigned int nummatches;
	unsigned int index;
	unsigned int total;
};
struct productpublicity {
	unsigned int index;
	unsigned int total;
};

enum DOS_COLORS
{
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN,
	LIGHT_GRAY, DARK_GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN,
	LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE
};

void textcolor(DOS_COLORS iColor)
{
	HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	BOOL b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
	bufferInfo.wAttributes &= 0x00F0;
	SetConsoleTextAttribute(hl, bufferInfo.wAttributes |= iColor);
}

void gotoxy(int x, int y)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	_COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hConsole, pos);
}

bool bissextile(int year)
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return true;
	else
		return false;
}

bool ComparebyDate(const transaction &a, const transaction &b)
{
	if (a.year < b.year)
		return true;
	if (a.year == b.year && a.month < b.month)
		return true;
	if (a.year == b.year && a.month == b.month && a.day < b.day)
		return true;

	return false;
}

bool ComparebyID(const client &a, const client &b)
{
	return a.ID <= b.ID;
}

bool ComparebyTotal(const productpublicity &a, const productpublicity &b)
{
	return a.total > b.total;
}

double ErrorInput2(string error, string error1, double &option)
{
	while (true) //Wrong input
	{

		string input;
		cin.clear();
		getline(cin, input);

		if (all_of(input.begin(), input.end(), isdigit) && input.size() != 0)
		{
			option = stoi(input);
			break;                         //input is integer
		} else {
			textcolor(LIGHT_RED);
			cout << error;
			textcolor(WHITE);
			cout << error1;
		}
	}
	return option;
}

void ErrorInput(string error, string error1, int &option)
{
	while (true) //Wrong input
	{

		string input;
		cin.clear();
		getline(cin, input);

		if (all_of(input.begin(), input.end(), isdigit) && input.size() != 0)
		{
			option = stoi(input);          // input is
			break;                         // integer
		} else {
			textcolor(LIGHT_RED);
			cout << error << endl;
			textcolor(WHITE);
			cout << error1;
		}
	}
}

bool ExitProgram()  // Are you sure you want exit procedure
{
	bool error = false;
	while (true)
	{
		int optionexit;
		system("CLS");
		cout << "\t#######################" << endl;
		cout << "\t#    EXIT PROGRAM     #" << endl;
		cout << "\t#######################" << endl << endl;
		if (error)
		{
			textcolor(LIGHT_RED);
			cout << endl << "Invalid Input!" << endl;
			textcolor(WHITE);
		}
		gotoxy(6, 7); cout << "=================================================";
		gotoxy(8, 8); cout << "Are you sure you want to exit?";
		gotoxy(8, 9); cout << "1. Yes";
		gotoxy(8, 10); cout << "2. No, I want to do some more things.";
		gotoxy(6, 11); cout << "=================================================";
		cout << endl << endl << " Choose one to proceed: ";
		while (true) //Wrong input
		{

			string wronginput;
			cin.clear();
			getline(cin, wronginput);

			if (all_of(wronginput.begin(), wronginput.end(), isdigit) && wronginput.size() != 0)
			{
				optionexit = stoi(wronginput);          // input is
				break;                                  // integer
			}
			else {
				system("CLS");
				cout << "\t#######################" << endl;
				cout << "\t#    EXIT PROGRAM     #" << endl;
				cout << "\t#######################" << endl << endl;
				textcolor(LIGHT_RED);
				cout << endl << "Invalid Input!" << endl;
				textcolor(WHITE);
				gotoxy(6, 7); cout << "=================================================";
				gotoxy(8, 8); cout << "Are you sure you want to exit?";
				gotoxy(8, 9); cout << "1. Yes";
				gotoxy(8, 10); cout << "2. No, I want to do some more things.";
				gotoxy(6, 11); cout << "=================================================";
				cout << endl << endl << " Choose one to proceed: ";
			}
		}

		switch (optionexit)
		{
		case 1:
			return false;
		case 2:
			return true;
		default:
			cin.clear();
			error = true;
		}
	}
}

void MakingClientsVector2(vector<client> &clientvec, int id, string name, double ammount, int i) //i is the index
{
	clientvec.push_back(client());
	clientvec[i].ID = id;
	clientvec[i].name = name;
	clientvec[i].ammount = ammount;
}

void MakingClientsVector(ifstream &clients, vector<client> &clientvec)
{
	int i = 0; //i is the index of the vector
	while (!clients.eof())
	{
		istringstream is;
		string line;
		int id;
		char c1;
		string trash;
		string name;
		double ammount;

		getline(clients, line);
		is.str(line);

		is >> id >> c1;
		getline(is, trash, ';');
		while (trash[0] == ' ')
		{
			trash.erase(0, 1);
		}
		name = trash;
		is >> ammount;
		setprecision(2);
		MakingClientsVector2(clientvec, id, name, ammount, i);
		i++;
	}
	sort(clientvec.begin(), clientvec.end(), ComparebyID);
}

void MakingTransactionsProductsVector(vector<transaction> &transactionsvec, string products, unsigned int i)
{
	istringstream iss(products);
	string product;
	while (iss)
	{
		getline(iss, product, ',');
		while (product[0] == ' ')
		{
			product.erase(0, 1);
		}
		transactionsvec[i].prodcts.push_back(product);
	}
	transactionsvec[i].prodcts.erase(transactionsvec[i].prodcts.begin() + (transactionsvec[i].prodcts.size() - 1));
}

void MakingTransactionsVector2(vector<transaction> &transactionsvec, int id, int day, int month, int year, string products, unsigned int i)
{
	transactionsvec.push_back(transaction());
	transactionsvec[i].ID = id;
	transactionsvec[i].day = day;
	transactionsvec[i].month = month;
	transactionsvec[i].year = year;
	MakingTransactionsProductsVector(transactionsvec, products, i);
}

void MakingTransactionsVector(ifstream &transactions, vector<transaction> &transactionsvec)
{
	unsigned int i = 0;
	while (!transactions.eof())
	{
		istringstream is;
		string line;
		unsigned int id, day, month, year;
		char c1, c2, c3, c4;
		string products;


		getline(transactions, line);
		is.str(line);

		is >> id >> c1 >> day >> c2 >> month >> c3 >> year >> c4;
		getline(is, products, '\n');
		while (products[0] == ' ')
		{
			products.erase(0, 1);
		}
		MakingTransactionsVector2(transactionsvec, id, day, month, year, products, i);

		i++;
	}
	sort(transactionsvec.begin(), transactionsvec.end(), ComparebyDate);
}

void MakingProductsVector(ifstream &products, vector<string> &productsvec)
{
	while (!products.eof())
	{
		string product;
		getline(products, product, '\n');
		productsvec.push_back(product);
	}
}

void ListOfProducts(vector<string> &productsvec)
{
	cout << fixed;
	cout << endl;
	cout << "\t###########################" << endl;
	cout << "\t#    LIST OF PRODUCTS     #" << endl;
	cout << "\t###########################" << endl;
	for (int i = 0; i < productsvec.size(); i++)
	{

		if (i == 0)
			cout << "\t" << i + 1 << ". " << productsvec[i];
		else
			cout << endl << "\t" << i + 1 << ". " << productsvec[i];
	}
	cout << endl;
}

void ListofClients(vector<client> &clientvec)
{
	cout << fixed;
	cout << endl;
	cout << "\t##########################" << endl;
	cout << "\t#    LIST OF CLIENTS     #" << endl;
	cout << "\t##########################" << endl;
	cout << "ID\t\tNAME\t\t\tAMMOUNT" << endl;
	for (int i = 0; i < clientvec.size(); i++)
	{
		//26
		if (i == 0)
		{
			cout << clientvec[i].ID << "\t"; //<< clientvec[i].name << "\t" << setprecision(2) << fixed << clientvec[i].ammount;
			if (clientvec[i].name.size() < 26)
			{
				cout << clientvec[i].name;
				for (unsigned int ii = clientvec[i].name.size(); ii < 27; ii++)
					cout << ' ';
			}
			else if (clientvec[i].name.size() > 26)
			{
				for (unsigned int ii = 0; ii < 27; ii++)
					cout << clientvec[i].name[ii];
			}
			cout << "\t" << setprecision(2) << fixed << clientvec[i].ammount;
		}
		else {
			cout << endl << clientvec[i].ID << "\t";// << clientvec[i].name << "\t" << setprecision(2) << fixed << clientvec[i].ammount;
			if (clientvec[i].name.size() < 26)
			{
				cout << clientvec[i].name;
				for (unsigned int ii = clientvec[i].name.size(); ii < 27; ii++)
					cout << ' ';
			}
			else if (clientvec[i].name.size() > 26)
			{
				for (unsigned int ii = 0; ii < 27; ii++)
					cout << clientvec[i].name[ii];
			}
			cout << "\t" << setprecision(2) << fixed << clientvec[i].ammount;
		}
	}
	cout << endl;
}

void SaveInfoInFiles(string &clientsfile, string &productsfile, string &transactionsfile, vector<client> &clientvec, vector<string> &productsvec, vector<transaction> &transactionsvec)
{
	ofstream clients;
	ofstream products;
	ofstream transactions;

	clients.open(clientsfile);

	for (int i = 0; i < clientvec.size(); i++)
	{

		if (i == 0)
			clients << clientvec[i].ID << " ; " << clientvec[i].name << "; " << setprecision(2) << fixed << clientvec[i].ammount;
		else
			clients << endl << clientvec[i].ID << " ; " << clientvec[i].name << "; " << setprecision(2) << fixed << clientvec[i].ammount;

	}
	clients.close();
	products.open(productsfile);
	for (int i = 0; i < productsvec.size(); i++)
	{

		if (i == 0)
			products << productsvec[i];
		else
			products << endl << productsvec[i];
	}
	products.close();
	transactions.open(transactionsfile);
	for (int i = 0; i < transactionsvec.size(); i++)
	{

		if (i == 0)
		{
			transactions << transactionsvec[i].ID << " ; " << transactionsvec[i].day << "/" << transactionsvec[i].month << "/" << transactionsvec[i].year << " ; ";
			for (int ii = 0; ii < transactionsvec[i].prodcts.size(); ii++)
			{
				if (ii == transactionsvec[i].prodcts.size() - 1)
					transactions << transactionsvec[i].prodcts[ii];
				else
					transactions << transactionsvec[i].prodcts[ii] << ", ";

			}
		}
		else
		{
			transactions << endl << transactionsvec[i].ID << " ; " << transactionsvec[i].day << "/" << transactionsvec[i].month << "/" << transactionsvec[i].year << " ; ";
			for (int ii = 0; ii < transactionsvec[i].prodcts.size(); ii++)
			{
				if (ii == transactionsvec[i].prodcts.size() - 1)
					transactions << transactionsvec[i].prodcts[ii];
				else
					transactions << transactionsvec[i].prodcts[ii] << ", ";

			}
		}
	}
	transactions.close();
}

void OpenFiles(string &clientsfile, string &productsfile, string &transactionsfile, vector<client> &clientvec, vector<string> &productsvec, vector<transaction> &transactionsvec)
{
	ifstream clients;
	ifstream products;
	ifstream transactions;
	while (true)
	{
		cout << "Enter the name of the Clients File: ";
		getline(cin, clientsfile);
		clients.open(clientsfile);
		if (clients.is_open())
		{
			MakingClientsVector(clients, clientvec);
			break;
		}
		else
		{
			textcolor(LIGHT_RED);
			cout << "Invalid File. Try Again!\n";
			textcolor(WHITE);
		}
	}
	while (true)
	{
		cout << "Enter the name of the Products File: ";
		getline(cin, productsfile);
		products.open(productsfile);
		if (products.is_open())
		{
			MakingProductsVector(products, productsvec);
			break;
		}
		else
		{
			textcolor(LIGHT_RED);
			cout << "Invalid File. Try Again!\n";
			textcolor(WHITE);
		}
	}

	while (true)
	{
		cout << "Enter the name of the Transactions File: ";
		getline(cin, transactionsfile);
		transactions.open(transactionsfile);
		if (transactions.is_open())
		{
			MakingTransactionsVector(transactions, transactionsvec);
			break;
		}
		else
		{
			textcolor(LIGHT_RED);
			cout << "Invalid File. Try Again!\n";
			textcolor(WHITE);
		}
	}
	clients.close();
	products.close();
	transactions.close();
	cin.clear();
}

void TransactionsInfo(int id, vector<transaction> &transactionsvec)
{
	bool breakwhile = false;
	int optioncase1;
	cout << "\t=================================================" << endl;
	cout << "\t1. See all Transactions" << endl;
	cout << "\t2. See the Transactions between two dates" << endl;
	cout << "\t=================================================";
	cout << endl << endl << " Choose one to proceed: ";
	while (true) //Wrong input
	{

		string wronginput;
		cin.clear();
		getline(cin, wronginput);

		if (all_of(wronginput.begin(), wronginput.end(), isdigit) && wronginput.size() != 0)
		{
			optioncase1 = stoi(wronginput);          // input is
			break;                                  // integer
		}
		else {
			textcolor(LIGHT_RED);
			cout << endl << "Invalid Input!" << endl;
			textcolor(WHITE);
			cout << "\t=================================================";
			cout << "\t1. See all Transactions";
			cout << "\t2. See the Transactions between two dates";
			cout << "\t=================================================";
			cout << endl << endl << " Choose one to proceed: ";
		}
	}
	int year1, month1, day1, year2, month2, day2;
	switch (optioncase1)
	{
	case 1:
		while (true)
		{
			for (int i = 0; i < transactionsvec.size(); i++)
			{
				if (id == transactionsvec[i].ID)
				{
					cout << transactionsvec[i].day << "/" << transactionsvec[i].month << "/" << transactionsvec[i].year << "\tProducts bought: ";
					for (int ii = 0; ii < transactionsvec[i].prodcts.size(); ii++)
					{
						if (ii == transactionsvec[i].prodcts.size() - 1)
							cout << transactionsvec[i].prodcts[ii];
						else
							cout << transactionsvec[i].prodcts[ii] << ", ";
					}
					cout << endl;
					breakwhile = true;
				}
			}
			if (breakwhile)
				break;
			else
			{
				cout << "That ID doesn't have transactions!";
				break;
			}
		}
		break;
	case 2:
		while (true)
		{
			cout << "\t=========================" << endl;
			cout << "\t  Enter the first date" << endl;
			cout << "\t=========================" << endl;
			cout << endl << "Enter the year: ";
			ErrorInput("That's not a number!", "Enter the year : ", year1);
			cout << "Enter the month: ";
			ErrorInput("That's not a number!", "Enter the month: ", month1);
			while (true)
			{
				if (month1 <= 12 || month1 >= 1)
					break;
				else
					ErrorInput("That's not a valid month!", "Enter the month: ", year1);
			}
			cout << "Enter the day: ";
			ErrorInput("That's not a number!", "Enter the day: ", day1);
			while (true)
			{
				if (day1 < 1 || (day1 > 30 && (month1 == 4 || month1 == 6 || month1 == 9 || month1 == 11)) || day1 > 31 || (bissextile(year1) && month1 == 2 && day1 > 29) || (!bissextile(year1) && month1 == 2 && day1 > 28))
				{
					textcolor(LIGHT_RED);
					cout << "That's not a valid day!";
					textcolor(WHITE);
					cout <<  " Enter the day: ";
					ErrorInput("That's not a valid day!", "Enter the day: ", day1);
				}
				else
					break;
			}
			cout << "\t=========================" << endl;
			cout << "\t  Enter the second date" << endl;
			cout << "\t=========================" << endl;
			cout << endl << "Enter the year: ";
			ErrorInput("That's not a number!", "Enter the year: ", year2);
			cout << "Enter the month: ";
			ErrorInput("That's not a number!", "Enter the month: ", month2);
			while (true)
			{
				if (month2 <= 12 || month2 >= 1)
					break;
				else
					ErrorInput("That's not a valid month!", "Enter the month: ", year2);
			}
			cout << "Enter the day: ";
			ErrorInput("That's not a number!", "Enter the day: ", day2);
			while (true)
			{
				if (day2 < 1 || (day2 > 30 && (month2 == 4 || month2 == 6 || month2 == 9 || month2 == 11)) || day2 > 31 || (bissextile(year2) && month2 == 2 && day2 > 29) || (!bissextile(year2) && month2 == 2 && day2 > 28))
				{
					cout << "That's not a valid day! Enter the day: ";
					ErrorInput("That's not a valid day!", "Enter the day: ", day2);
				}
				else
					break;
			}
			if (year1 < year2)
				break;
			if (year1 == year2 && month1 < month2)
				break;
			if (year1 == year2 && month1 == month2 && day1 < day2)
				break;
			cout << "The first date must be before the second date!\n";
		}
		for (int i = 0; i < transactionsvec.size(); i++)
		{
			if ((year1 < transactionsvec[i].year || (year1 == transactionsvec[i].year && month1 < transactionsvec[i].month) || (year1 == transactionsvec[i].year && month1 == transactionsvec[i].month && day1 <= transactionsvec[i].day)) && (year2 > transactionsvec[i].year || (year2 == transactionsvec[i].year && month2 > transactionsvec[i].month) || (year2 == transactionsvec[i].year && month2 == transactionsvec[i].month && day2 >= transactionsvec[i].day)))
			{
				cout << transactionsvec[i].day << "/" << transactionsvec[i].month << "/" << transactionsvec[i].year << "\tProducts bought: ";
				for (int ii = 0; ii < transactionsvec[i].prodcts.size(); ii++)
				{
					if (ii == transactionsvec[i].prodcts.size() - 1)
						cout << transactionsvec[i].prodcts[ii];
					else
						cout << transactionsvec[i].prodcts[ii] << ", ";
				}
				cout << endl;
			}
		}
		break;
	default:
		cin.clear();
		textcolor(LIGHT_RED);
		cout << "Invalid option! Enter a valid one!\n";
		textcolor(WHITE);
		TransactionsInfo(id, transactionsvec);
		break;
	}
}

void Information(vector<client> &clientvec, vector<string> &productsvec, vector<transaction> &transactionsvec, bool error)
{
		int optioninformation;
		system("CLS");
		cout << "\t######################" << endl;
		cout << "\t#    INFORMATION     #" << endl;
		cout << "\t######################" << endl << endl;
		if (error)
		{
			textcolor(LIGHT_RED);
			cout << endl << "Invalid Input!" << endl;
			textcolor(WHITE);
		}
		gotoxy(6, 7); cout << "======================================";
		gotoxy(8, 8); cout << " 1. Client Information";
		gotoxy(8, 9); cout << " 2. Products Information";
		gotoxy(8, 10); cout << " 3. Transactions Information";
		gotoxy(8, 11); cout << " 4. Back";
		gotoxy(6, 12); cout << "=====================================";
		cout << endl << endl << " Choose one to proceed: ";
		while (true) //Wrong input
		{

			string wronginput;
			cin.clear();
			getline(cin, wronginput);

			if (all_of(wronginput.begin(), wronginput.end(), isdigit) && wronginput.size() != 0)
			{
				optioninformation = stoi(wronginput);          // input is
				break;                                  // integer
			}
			else {
				system("CLS");
				cout << "\t######################" << endl;
				cout << "\t#    INFORMATION     #" << endl;
				cout << "\t######################" << endl << endl;
				textcolor(LIGHT_RED);
				cout << endl << "Invalid Input!" << endl;
				textcolor(WHITE);
				gotoxy(6, 7); cout << "======================================";
				gotoxy(8, 8); cout << " 1. Client Information";
				gotoxy(8, 9); cout << " 2. Products Information";
				gotoxy(8, 10); cout << " 3. Transactions Information";
				gotoxy(8, 11); cout << " 4. Back";
				gotoxy(6, 12); cout << "=====================================";
				cout << endl << endl << " Choose one to proceed: ";
			}
		}
		
		int optioncase1;
		bool breakwhile = false;
		int id;
		switch (optioninformation)
		{
		case 1:
			int optioncase1;
			cout << "\t===============================================" << endl;
			cout << "\t1. See all Clients list" << endl;
			cout << "\t2. See just one Client information" << endl;
			cout << "\t===============================================";
			cout << endl << endl << " Choose one to proceed: ";
			while (true) //Wrong input
			{

				string wronginput;
				cin.clear();
				getline(cin, wronginput);

				if (all_of(wronginput.begin(), wronginput.end(), isdigit) && wronginput.size() != 0)
				{
					optioncase1 = stoi(wronginput);          // input is
					break;                                  // integer
				}
				else {
					textcolor(LIGHT_RED);
					cout << endl << "Invalid Input!" << endl;
					textcolor(WHITE);
					cout << "\t===============================================" << endl;
					cout << "\t1. See all Clients list" << endl;
					cout << "\t2. See just one Client information" << endl;
					cout << "\t===============================================";
					cout << endl << endl << " Choose one to proceed: ";
				}
			}
			if (optioncase1 == 1)
				ListofClients(clientvec);
			else if (optioncase1 == 2)
			{
				cout << endl << "Enter the ID of the client: ";
				while (true)
				{
					ErrorInput("That's not a number! Enter a valid one!", "Enter the ID of the client: ", id);
					for (int i = 0; i < clientvec.size(); i++)
					{
						if (id == clientvec[i].ID)
						{
							cout << "ID\t\tNAME\t\t\tAMMOUNT" << endl;
							cout << clientvec[i].ID << "\t";
							if (clientvec[i].name.size() < 26)
							{
								cout << clientvec[i].name;
								for (unsigned int ii = clientvec[i].name.size(); ii < 27; ii++)
									cout << ' ';
							}
							else if (clientvec[i].name.size() > 26)
							{
								for (unsigned int ii = 0; ii < 27; ii++)
									cout << clientvec[i].name[ii];
							}
							cout << "\t" << setprecision(2) << fixed << clientvec[i].ammount;
							cout << endl;
							breakwhile = true;
							break;
						}
					}
					if (breakwhile)
						break;
					else
					{
						cout << "That ID doesn't exist! Enter another one: ";
					}
				}
			}
			break;
		case 2:
			ListOfProducts(productsvec);
			break;
		case 3:
			ListofClients(clientvec);
			cout << endl << "Enter the ID of the client you want to see the transactions: ";
			while (true)
			{
				ErrorInput("That's not a number! Enter a valid one!", "Enter the ID of the client you want to see the transactions: ", id);
				for (int i = 0; i < clientvec.size(); i++)
				{
					if (id == clientvec[i].ID)
					{
						breakwhile = true;
						break;
					}
				}
				if (breakwhile)
					break;
				else
				{
					cout << "That ID doesn't exist! Enter another one: ";
				}
			}
			TransactionsInfo(id, transactionsvec);
			break;
		case 4:
			return;
			break;
		default:
			cin.clear();
			textcolor(LIGHT_RED);
			cout << "Invalid option! Enter a valid one!\n";
			textcolor(WHITE);
			Information(clientvec, productsvec, transactionsvec ,true);
		}
}

void ChangeClientName(vector<client> &clientvec, unsigned int id)
{
	cout << "Enter the new name for the Client: ";
	cin.clear();

	string name;
	getline(cin, name);
	for (int i = 0; i < clientvec.size(); i++)
	{
		if (id == clientvec[i].ID)
		{
			clientvec[i].name = name + ' ';
			break;
		}
	}
}

void ChangeClientAmmount(vector<client> &clientvec, unsigned int id, double value)
{
	for (int i = 0; i < clientvec.size(); i++)
	{
		if (id == clientvec[i].ID)
		{
			clientvec[i].ammount = clientvec[i].ammount + value;
			break;
		}
	}
}

int CheckIDtoCreate(int id, vector<transaction> &transactionsvec)
{
	for (unsigned int i = 0; i < transactionsvec.size(); i++)
	{
		if (id < transactionsvec[i].ID)
			id = transactionsvec[i].ID + 1;
	}
	return id;
}

vector<string> CheckClientProducts(vector<client> &clientvec, vector<string> &productsvec, vector<transaction> &transactionsvec, unsigned int id)
{
	vector<string> productslist;
	bool alreadyexist = false;
	for (int i = 0; i < transactionsvec.size(); i++)
	{
		if (transactionsvec[i].ID == id)
		{
			for (int ii = 0; ii < transactionsvec[i].prodcts.size(); ii++)
			{
				for (int iii = 0; iii < productslist.size(); iii++)
				{

					if (productslist[iii] == transactionsvec[i].prodcts[ii])
					{
						alreadyexist = true;
						break;
					}
				}
				if (alreadyexist == false)
				{
					productslist.push_back(transactionsvec[i].prodcts[ii]);
				}
				else
				{
					alreadyexist = false;
				}
			}
		}
	}
	return productslist;
}

void Publicity(vector<client> &clientvec, vector<string> &productsvec, vector<transaction> &transactionsvec)
{
	vector<string> productslist;
	bool alreadyexist = false;
	for (int i = 0; i < transactionsvec.size(); i++)
	{
		for (int ii = 0; ii < transactionsvec[i].prodcts.size(); ii++)
		{
			for (int iii = 0; iii < productslist.size(); iii++)
			{

				if (productslist[iii] == transactionsvec[i].prodcts[ii])
				{
					alreadyexist = true;
					break;
				}
			}
			if (alreadyexist == false)
			{
				productslist.push_back(transactionsvec[i].prodcts[ii]);
			}
			else
			{
				alreadyexist = false;
			}
		}
	}

	typedef vector<bool> matrixLine;
	vector<matrixLine> matrixPublicity;
	matrixPublicity.resize(clientvec.size());

	for (int i = 0; i < matrixPublicity.size(); i++)
	{
		matrixPublicity[i].resize(productslist.size());
	}


	for (int i = 0; i < clientvec.size(); i++)
	{
		vector<string> clientproducts = CheckClientProducts(clientvec, productsvec, transactionsvec, clientvec[i].ID);
		for (int ii = 0; ii < productslist.size(); ii++)
		{
			for (int iii = 0; iii < clientproducts.size(); iii++)
			{
				if (clientproducts[iii] == productslist[ii])
				{
					matrixPublicity[i][ii] = true;

				}
			}
		}

	}

	int id;
	bool breakwhile = false;
	while (true)
	{
		cout << endl << "Enter the ID of the Client: ";
		ErrorInput("Invalid Input!", "Enter the ID of the Client: ", id);
		for (int i = 0; i < clientvec.size(); i++)
		{
			if (id == clientvec[i].ID)
			{
				breakwhile = true;
				break;
			}
		}
		if (breakwhile)
			break;
		else
			cout << "That ID doesn't exist!\n ";

	}
	vector<clientprod> vecsimilarclients;
	unsigned int index;
	for (int i = 0; i < clientvec.size(); i++)
	{
		if (id == clientvec[i].ID)
			index = i;
	}

	for (int i = 0; i < clientvec.size(); i++)
	{
		vecsimilarclients.push_back(clientprod());
		vecsimilarclients[i].index = i;
		vecsimilarclients[i].nummatches = 0;
		vecsimilarclients[i].total = 0;
		if (i != index)
		{
			for (int ii = 0; ii < productslist.size(); ii++)
			{
				if (matrixPublicity[i][ii] && matrixPublicity[index][ii])
				{
					vecsimilarclients[i].nummatches++;
				}
				if (matrixPublicity[i][ii])
				{
					vecsimilarclients[i].total++;
				}
			}
		}

	}

	vector<int> indexallsimilar;
	int max = 0;
	for (int i = 0; i < vecsimilarclients.size(); i++)
	{
		if (indexallsimilar.size() >= 1)
		{
			if ((vecsimilarclients[i].nummatches == vecsimilarclients[indexallsimilar[0]].nummatches && vecsimilarclients[i].nummatches >= max) && vecsimilarclients[i].total > vecsimilarclients[i].nummatches)
			{
				max = vecsimilarclients[i].nummatches;
				indexallsimilar.push_back(i);

			}
			else if (vecsimilarclients[i].nummatches > vecsimilarclients[indexallsimilar[0]].nummatches && vecsimilarclients[i].nummatches >= max && vecsimilarclients[i].total > vecsimilarclients[i].nummatches)
			{
				max = vecsimilarclients[i].nummatches;
				indexallsimilar.clear();
				indexallsimilar.push_back(i);
			}
		}
		else
		{
			if (vecsimilarclients[i].nummatches >= max && vecsimilarclients[i].total > vecsimilarclients[i].nummatches)
			{
				max = vecsimilarclients[i].nummatches;
				indexallsimilar.push_back(i);

			}
			else if (vecsimilarclients[i].nummatches >= max && vecsimilarclients[i].total > vecsimilarclients[i].nummatches)
			{
				max = vecsimilarclients[i].nummatches;
				indexallsimilar.resize(0);
				indexallsimilar.push_back(i);
			}
		}

	}
	vector<bool> indexbool;
	for (unsigned int i = 0; i < productslist.size(); i++)
	{
		indexbool.push_back(matrixPublicity[index][i]);
	}

	if (indexallsimilar.size() == 1)
	{
		for (int i = 0; i < productslist.size(); i++)
		{
			if (matrixPublicity[indexallsimilar[0]][i] == 1 && matrixPublicity[index][i] == 0)
			{
				cout << endl << "Maybe you want some " << productslist[i] << "?";
				break;
			}

		}
	}
	else if (indexallsimilar.size() > 1)
	{
		for (int i = 0; i < matrixPublicity.size(); i++)
		{
			bool exist = false;
			for (int ii = 0; ii < indexallsimilar.size(); ii++)
			{

				if (i == indexallsimilar[ii])
				{
					exist = true;
					break;
				}
			}
			if (!exist || i == index)
			{
				for (unsigned int j = 0; j < productslist.size(); j++)
				{
					matrixPublicity[i][j] = false;
				}

			}
		}

		for (unsigned int i = 0; i < productslist.size(); i++)
		{
			if (indexbool[i])
			{
				for (unsigned int ii = 0; ii < matrixPublicity.size(); ii++)
				{
					matrixPublicity[ii][i] = false;
				}

			}
		}


		vector<productpublicity> prodpublicityvec;
		for (int i = 0; i < productslist.size(); i++)
		{
			prodpublicityvec.push_back(productpublicity());
			prodpublicityvec[i].index = i;
			prodpublicityvec[i].total = 0;
			for (int ii = 0; ii < indexallsimilar.size(); ii++)
				if (matrixPublicity[indexallsimilar[ii]][i] == 1)
				{
					prodpublicityvec[i].total++;
				}

		}

		sort(prodpublicityvec.begin(), prodpublicityvec.end(), ComparebyTotal);
		cout << "Maybe you want some " << productslist[prodpublicityvec[0].index]<< "?";
		
	}
}


void MakeTransactions(vector<client> &clientvec, vector<string> &productsvec, vector<transaction> &transactionsvec)
{
	system("CLS");
	cout << "\t##########################" << endl;
	cout << "\t#    MAKE TRANSATION     #" << endl;
	cout << "\t##########################" << endl << endl;
	
	bool breakwhile = false;
	int id;
	ListofClients(clientvec);
	cout << "\nEnter the ID of the client you want to add a transactions: ";
	while (true)
	{
		ErrorInput("That's not a number! Enter a valid one!", "Enter the ID of the client you want to add a transactions: ", id);
		for (int i = 0; i < clientvec.size(); i++)
		{
			if (id == clientvec[i].ID)
			{
				breakwhile = true;
				break;
			}
		}
		if (breakwhile)
			break;
		else
		{
			cout << "That ID doesn't exist! Enter another one: ";
		}
	}
	int optiondate;

	int day, month, year;
	while (true)
	{

		cout << " 1. Use today's date\n 2. Use another date\n";
		cout << "Choose one: ";
		ErrorInput("That's not a number! Enter a valid one!\n"," 1. Use today's date\n 2. Use another date\nChoose one: ", optiondate);
		if (optiondate == 1)
		{
			time_t now = time(0);
			tm timeinfo;
			localtime_s(&timeinfo, &now);
			day = timeinfo.tm_mday;
			month = timeinfo.tm_mon + 1;
			year = timeinfo.tm_year + 1900;
			break;
		}
		else if (optiondate == 2)
		{
			cout << "Enter the year: ";
			ErrorInput("That's not a number!", "Enter the year: ", year);
			cout << "Enter the month: ";
			ErrorInput("That's not a number!", "Enter the month: ", month);
			while (true)
			{
				if (month <= 12 || month >= 1)
					break;
				else
					ErrorInput("That's not a valid month!", "Enter the month: ", year);
			}
			cout << "Enter the day: ";
			ErrorInput("That's not a number!", "Enter the day: ", day);
			while (true)
			{
				if (day < 1 || (day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) || day > 31 || (bissextile(year) && month == 2 && day > 29) || (!bissextile(year) && month == 2 && day > 28))
				{
					cout << "That's not a valid day! Enter the day: ";
					ErrorInput("That's not a valid day!", "Enter the day: ", day);
				}
				else
					break;
			}
			break;
		}
		else
		{
			cin.clear();
			cout << "That's not a valid option!\n";
		}
	}
	int optionnumberprod;
	string products;
	int product;
	cout << "\nEnter the number of products that were bought: ";
	ErrorInput("\nInvalid Input!", "Enter the number of products that were bought: ", optionnumberprod);
	ListOfProducts(productsvec);
	for (int i = 0; i < optionnumberprod; i++)
	{
		cout << "Enter the number of the product ID " << i + 1 << ": ";
		while (true) //Wrong input
		{
			string input;
			cin.clear();
			getline(cin, input);


			if (all_of(input.begin(), input.end(), isdigit) && stoi(input) - 1 < productsvec.size())
			{
				product = stoi(input);
				break;                         //input is integer
			}
			else
				textcolor(LIGHT_RED);
				cout << "\nInvalid Input!\n";
				textcolor(WHITE);
				cout << "Enter the number of the product ID " << i + 1 << ": ";

		}
		if (i == 0)
			products = productsvec[product - 1];
		else
			products = products + ", " + productsvec[product - 1];
	}

	MakingTransactionsVector2(transactionsvec, id, day, month, year, products, transactionsvec.size());
	sort(transactionsvec.begin(), transactionsvec.end(), ComparebyDate);
}

void ClientsManagement(vector<client> &clientvec, vector<transaction> &transactionsvec, bool error)
{
	int optionclients;
	system("CLS");
	cout << "\t#############################" << endl;
	cout << "\t#    CLIENTS MANAGEMENT     #" << endl;
	cout << "\t#############################" << endl << endl;
	if (error)
	{
		textcolor(LIGHT_RED);
		cout << endl << "Invalid Input!" << endl;
		textcolor(WHITE);
	}
	gotoxy(6, 7); cout << "======================================";
	gotoxy(8, 8); cout << " 1. Add Client";
	gotoxy(8, 9); cout << " 2. Remove Client";
	gotoxy(8, 10); cout << " 3. Change Client";
	gotoxy(8, 11); cout << " 4. Back";
	gotoxy(6, 12); cout << "=====================================";
	cout << endl << endl << " Choose one to proceed: ";
	while (true) //Wrong input
	{

		string wronginput;
		cin.clear();
		getline(cin, wronginput);

		if (all_of(wronginput.begin(), wronginput.end(), isdigit) && wronginput.size() != 0)
		{
			optionclients = stoi(wronginput);          // input is
			break;                                  // integer
		}
		else {
			system("CLS");
			cout << "\t#############################" << endl;
			cout << "\t#    CLIENTS MANAGEMENT     #" << endl;
			cout << "\t#############################" << endl << endl;
			textcolor(LIGHT_RED);
			cout << endl << "Invalid Input!" << endl;
			textcolor(WHITE);
			gotoxy(6, 7); cout << "======================================";
			gotoxy(8, 8); cout << " 1. Add Client";
			gotoxy(8, 9); cout << " 2. Remove Client";
			gotoxy(8, 10); cout << " 3. Change Client";
			gotoxy(8, 11); cout << " 4. Back";
			gotoxy(6, 12); cout << "=====================================";
			cout << endl << endl << " Choose one to proceed: ";
		}
	}
	string name;
	int number, id, id2 = 0; //id2 is for comparison to check an ID to create for a client
	int optioncase3;
	double value;
	bool breakwhile = false;
	switch (optionclients)
	{
	case 1:
		number = clientvec.size() - 1;
		id = clientvec[number].ID + 1;
		id2 = CheckIDtoCreate(id2, transactionsvec);
		if (id < id2)
			id = id2;
		cout << "ID: " << id << endl << "Enter the name of the client: ";
		cin.clear();
		getline(cin, name);
		MakingClientsVector2(clientvec, id, name + ' ', 0.00, clientvec.size());
		cout << "Client Added!\n";
		ListofClients(clientvec);
		break;
	case 2:
		ListofClients(clientvec);
		cout << "Enter the ID of the client: ";
		while (true)
		{
			ErrorInput("That's not a number! Enter a valid one!", "Enter the ID of the client: ", id);
			for (int i = 0; i < clientvec.size(); i++)
			{
				if (id == clientvec[i].ID)
				{
					clientvec.erase(clientvec.begin() + i);
					breakwhile = true;
					break;
				}
			}
			if (breakwhile)
				break;
			else
			{
				textcolor(LIGHT_RED);
				cout << "That ID doesn't exist!";
				textcolor(WHITE);
				cout << "Enter another one: ";
			}
		}
		cout << "Client Removed!\n";
		break;

	case 3:
		ListofClients(clientvec);
		cout << endl << "Enter the ID of the client you want to change: ";
		while (true)
		{
			ErrorInput("That's not a number! Enter a valid one!", "Enter the ID of the client you want to change: ", id);
			for (int i = 0; i < clientvec.size(); i++)
			{
				if (id == clientvec[i].ID)
				{
					breakwhile = true;
					break;
				}
			}
			if (breakwhile)
				break;
			else
			{
				textcolor(LIGHT_RED);
				cout << "That ID doesn't exist!";
				textcolor(WHITE);
				cout << "Enter another one : ";
				//ErrorInput("That's not a number! Enter a valid one!\nEnter the ID of the client:\n", id);
			}
		}

		cout << endl << "What you want to change?\n 1. Name\n 2. Add Ammount\nChoose One: ";
		ErrorInput("Invalid Input! Enter a valid one!", "\nWhat you want to change?\n 1. Name\n 2. Add Ammount\nChoose One: ", optioncase3);
		while (true)
		{
			if (optioncase3 == 1 || optioncase3 == 2)
				break;
			else
			{
				cout << "Invalid Input! \n";
				ErrorInput("Invalid Input! Enter a valid one!", "\nWhat you want to change?\n 1. Name\n 2. Add Ammount\nChoose One: ", optioncase3);
			}

			if (optioncase3 == 1)
				ChangeClientName(clientvec, id);
			else if (optioncase3 == 2)
			{
				cout << endl << "Enter the ammount to add to the client: ";
				ErrorInput2("Invalid Input!", "Enter the ammount to add to the client: ", value);
				ChangeClientAmmount(clientvec, id, value);
			}

			break;
	case 4:
		return;
		break;
	default:
		cin.clear();
		textcolor(LIGHT_RED);
		cout << "Invalid option! Enter a valid one!\n";
		textcolor(WHITE);
		ClientsManagement(clientvec, transactionsvec, true);
		}

	}
}


void super()
{
	textcolor(LIGHT_CYAN);
	gotoxy(6, 1); cout << "      #####    ###  ###   ########   ########   ########    " << endl;
	gotoxy(6, 2); cout << "  #####        ###  ###   ###  ###   ###       ###  ###       ##       ##" << endl;
	gotoxy(6, 3); cout << " #########    ###  ###   ########   #######    #######     #######  ####### " << endl;
	gotoxy(6, 4); cout << "      #####   ###  ###   ###        ###       ### ###        ##       ##" << endl;
	gotoxy(6, 5); cout << "  #####      ########   ###        #######   ###   ###             " << endl << endl;
	textcolor(WHITE);
}

void menu(vector<client> &clientvec, vector<string> &productsvec, vector<transaction> &transactionsvec)
{
	int correctingprogram = 0;
	bool runprogram = false; //Variable to see if I want to run the program again
	bool invalidoption = false;
	do
	{
		if (!invalidoption)
		{
			system("CLS"); //Clear the console
			super();

		}
		else
		{
			system("CLS");
			super();
			textcolor(LIGHT_RED);
			cout << "Invalid option! Enter a valid one!" << endl;
			textcolor(WHITE);
			
		}
		cout << "Welcome!" << endl;
		gotoxy(6, 9); cout << "======================================================";
		gotoxy(8, 10); cout << "1. Clients Management";
		gotoxy(8, 11); cout << "2. Information(Clients, Products or Transactions)";
		gotoxy(8, 12); cout << "3. Make Transactions(Buy)";
		gotoxy(8, 13); cout << "4. Customize Publicity";
		gotoxy(8, 14); cout << "5. Exit";
		gotoxy(6, 15); cout << "======================================================";
		cout << endl << endl <<  " Choose one to proceed: ";
		
		
		
		int option1; //Option from the menu
		while (true) //Wrong input
		{
			
			string input;
			cin.clear();
			getline(cin, input);

			if (all_of(input.begin(), input.end(), isdigit) && input.size() != 0)
			{
				option1 = stoi(input);          // input is
				break;                         // integer
			}
			else {
				system("CLS");
				super();
				textcolor(LIGHT_RED);
				cout << "Invalid option! Enter a valid one!" << endl;
				textcolor(WHITE);
				cout << "Welcome!" << endl;
				gotoxy(6, 12); cout << "======================================================";
				gotoxy(8, 13); cout << "1. Clients Management";
				gotoxy(8, 14); cout << "2. Information(Clients, Products or Transactions)";
				gotoxy(8, 15); cout << "3. Make Transactions(Buy)";
				gotoxy(8, 16); cout << "4. Customize Publicity";
				gotoxy(8, 17); cout << "5. Exit";
				gotoxy(6, 18); cout << "======================================================";
				cout << endl << endl << " Choose one to proceed: ";
			}
		}

		switch (option1)
		{
		case 1:
			ClientsManagement(clientvec, transactionsvec, false);
			runprogram = true;
			cout << endl;
			system("pause");
			break;
		case 2:
			Information(clientvec, productsvec, transactionsvec, false);
			runprogram = true;
			cout << endl;
			system("pause");
			break;
		case 3:
			MakeTransactions(clientvec, productsvec, transactionsvec);
			runprogram = true;
			cout << endl;
			system("pause");
			break;
		case 4:
			Publicity(clientvec, productsvec, transactionsvec);
			runprogram = true;
			cout << endl;
			system("pause");
			break;
		case 5:
			invalidoption = false;
			runprogram = ExitProgram();
			break;
		default:
			cin.clear();
			cout << "Invalid option! Enter a valid one!\n\n ";
			runprogram = true;
			invalidoption = true;
			break;
		}

	} while (runprogram);
}

int main()
{
	string clientsfile;
	string productsfile;
	string transactionsfile;
	vector<client> clientvec;
	vector<string> productsvec;
	vector<transaction> transactionsvec;
	OpenFiles(clientsfile, productsfile, transactionsfile, clientvec, productsvec, transactionsvec);

	menu(clientvec, productsvec, transactionsvec);
	SaveInfoInFiles(clientsfile, productsfile, transactionsfile, clientvec, productsvec, transactionsvec);

	return 0;
}