#include <iostream>
#include <fstream>
#include <conio.h>

#include "Map.h"

#define wait _getch(); break;

int main()
{
	std::vector<std::pair<int, char>> elements;

	for (int i = 0; i < 26; ++i)
		elements.push_back({ i, i + 'a' });

	Map<int, char> map(elements);

	char choice = '0';

	do
	{
		int key; char value;
		Map<int, char>::iterator iter = map.end();
		Node<int, char>* ptr = nullptr;

		std::cout << "|-------------------------------------|\n";
		std::cout << "|              Meniu BST              |\n";
		std::cout << "|-------------------------------------|\n";
		std::cout << "| 1. Insereaza un element in arbore   |\n";
		std::cout << "| 2. Cauta un element in arbore       |\n";
		std::cout << "| 3. Sterge un element din arbore     |\n";
		std::cout << "| 4. Cauta predecesorul unui element  |\n";
		std::cout << "| 5. Cauta succesorul unui element    |\n";
		std::cout << "| 6. Cauta minimul din arbore         |\n";
		std::cout << "| 7. Cauta maximul din arbore         |\n";
		std::cout << "| 8. Afiseaza arborele                |\n";
		std::cout << "| 9. Afiseaza arborele (cu iterare)   |\n";
		std::cout << "| 0. Exit...                          |\n";
		std::cout << "|-------------------------------------|\n";

		choice = _getch(); system("cls");

		switch (choice)
		{
		case '1':
			std::cout << "Introduceti cheia: ";
			std::cin >> key; 
			std::cout << "Introduceti valoarea: ";
			std::cin >> value;
			map.insert(key, value);
			std::cout << "Operatie efectuata cu succes!\n";
			wait;
		case '2':
			std::cout << "Introduceti cheia: ";
			std::cin >> key;
			ptr = map.find(key);
			if (ptr == nullptr)
				std::cout << "Elementul nu se afla in arbore!\n";
			else
				std::cout << "Elementul se afla in arbore si are valoarea: " << ptr->value << "\n";
			wait;
		case '3':
			std::cout << "Introduceti cheia: ";
			std::cin >> key; map.remove(key);
			std::cout << "Operatie efectuata cu succes!\n";
			wait;
		case '4':
			std::cout << "Introduceti cheia: ";
			std::cin >> key;
			ptr = map.find(key);
			if (ptr ==  nullptr)
				std::cout << "Elementul nu se afla in arbore!\n";
			else
			{
				ptr = map.predecessor(ptr);
				std::cout << "Predecesorul elementului cu cheia " << key << " este elementul cu cheia: " << ptr->key << " si valoarea " << ptr->value << "\n";
			}
			wait;
		case '5':
			std::cout << "Introduceti cheia: ";
			std::cin >> key;
			ptr = map.find(key);
			if (ptr == nullptr)
				std::cout << "Elementul nu se afla in arbore!\n";
			else
			{
				ptr = map.successor(ptr);
				std::cout << "Predecesorul elementului cu cheia " << key << " este elementul cu cheia: " << ptr->key << " si valoarea " << ptr->value << "\n";
			}
			wait;
		case '6':
			if (map.empty())
				std::cout << "Map-ul este gol!\n";
			else
			{
				ptr =  map.min(map.getRoot());
				std::cout << "Elementul minim din arbore este: " << ptr->key << ": " << ptr->value << "\n";
			}
			wait;
		case '7':
			if (map.empty())
				std::cout << "Map-ul este gol!\n";
			else
			{
				ptr = map.max(map.getRoot());
				std::cout << "Elementul maxim din arbore este: " << ptr->key << ": " << ptr->value << "\n";
			}
			wait;
		case '8':
			std::cout << "Introduceti modul de afisare (1 - RSD, 2 - SRD, 3 - SDR, 4 - nivele): ";
			std::cin >> key;
			std::cout << "Arborele dvs. este:\n";
			map.print(key);
			wait;
		case '9':
			std::cout << "Introduceti modul de afisare (1 - forward, 2 - backward, 3 - forward cu index, 4 - backward cu index): ";
			std::cin >> key;
			std::cout << "Arborele dvs. este:\n";
			switch (key)
			{
			case 1:
				for (Map<int, char>::iterator iter = map.begin(); iter != map.end(); iter++)
					std::cout << iter->key << ": " << iter->value << "\n";
				break;
			case 2:
				for (Map<int, char>::iterator iter = map.rbegin(); iter != map.rend(); iter++)
					std::cout << iter->key << ": " << iter->value << "\n";
				break;
			case 3:
				for (int i = 0; i < map.size(); i++)
					std::cout << i << ": " << map[i] << "\n";
				break;
			case 4:
				for (int i = map.size() - 1; i >= 0; i--)
					std::cout << i << ": " << map[i] << "\n";
				break;
			}
			wait;
		case '0':
		default:
			break;
		}

		system("cls");
	} while (choice != '0');

	return 0;
}
