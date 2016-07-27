/*
Автомат штампует случайные номера от X до Y на мячиках и бросает их в корзину.
После накопления в корзине Z мячей :
1. Подсчитать сколько мячей с каждым номером присутствует в корзине.Вывести этот список в порядке убывания количества мячей.
2. Выложить из неё все те мячи, которые присутствуют более чем в одном экземпляре. (выложить как дубликаты так и последний оставшийся)
3. Вывести номера с тех мячей, которые совместно с другим мячом из корзины не могут в сумме составить число на любом из удаленных мячей.
*/
#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <conio.h>

int main()
{
	unsigned X = 0;//Начало диапазона
	unsigned Y = 0;//Конец диапазона. Будем считать, что сам Y не входит в диапазон.
	size_t Z = 0;//Количество

	//Получить исходные данные
	std::cout << "Enter: X Y Z" << std::endl;
	std::cin >> X >> Y >> Z;
	if (X < Y)
	{
		//Раскомментировать, если нужен случайный результат
		srand(0/*time(NULL)*/);
		//Наштамповать случайные числа на мячи
		std::vector<unsigned> Balls(Z); //{1, 2, 3, 3, 3, 5, 5, 5, 5, 5};//для проверки третьего пункта
		for (auto &Item : Balls)
		{
			Item = rand() % (Y - X) + X;
		}
		//Показать что получилось
		std::cout << "Balls:" << std::endl;
		for (auto &Item : Balls)
		{
			std::cout << Item << " ";
		}
		std::cout << std::endl;
		////////////////////////////////////////////


		/*1. Подсчитать сколько мячей с каждым номером присутствует в корзине.
		Вывести этот список в порядке убывания количества мячей.*/

		//Определить количество шаров с уникальными номерами
		//Сложность выполнения O(Z)
		//Сложность по памяти O(Y - X)
		std::vector<size_t> UniqueBalls(Y - X, 0);
		//что-то вроде сортировки подсчётом
		for (auto &Item : Balls)
		{
			++UniqueBalls[Item - X];
			//Здесь происходит вычитание из size_t unsigned,
			//если это плохой стиль, то можно воспользоваться map,
			//но тогда сложность алгоритма будет O(Z * log(Y - X))
		}

		//Сложность выполнения O((Y - X) * log(Y - X))
		//Сложность по памяти O(Y - X)
		std::multimap<size_t, unsigned> QuantityMap;
		for (unsigned Index = 0; Index < UniqueBalls.size(); ++Index)
		{
			QuantityMap.insert(std::make_pair(UniqueBalls[Index], Index + X));
		}
		std::cout << "Number\tQuantity" << std::endl;;
		for (auto it = QuantityMap.rbegin(); it != QuantityMap.rend(); ++it)
		{
			if (it->first != 0)
			{
				std::cout << it->second << "\t" << it->first << std::endl;
			}
		}
		/////////////////////////////////////////


		/*2. Выложить из неё все те мячи,
		которые присутствуют более чем в одном экземпляре.
		(выложить как дубликаты так и последний оставшийся)*/
		//Сложность выполнения O(Y - X)
		std::cout << "Single balls:" << std::endl;
		std::vector<std::pair<unsigned, bool>> SingleBalls;//first - index; second - не может составить число удалённого мяча
		SingleBalls.reserve(UniqueBalls.size());
		for (unsigned Index = 0; Index < UniqueBalls.size(); ++Index)
		{
			if (UniqueBalls[Index] == 1)
			{
				//Чтобы облегчить задачу для п. 3 помещаем индексы в массив
				SingleBalls.push_back(std::make_pair(Index + X, true));
				std::cout << SingleBalls.back().first << " ";
			}
		}
		std::cout << std::endl;
		//////////////////////////////////////////


		/*3. Вывести номера с тех мячей,
		которые совместно с другим мячом из корзины
		не могут в сумме составить число на любом из удаленных мячей.*/
		//Будем считать, что речь идёт именно о номерах удалённых шаров.
		//Считаем что под словом "любой" подразумевается "хоть один из" удалённых шаров.
		//Также будем считать, что номер нельзя суммировать сам с собой
		//В пределе сложность O((Y - X)^2), но в общем случае всё будет работать быстрее
		//Память O(Y - X)
		//Выполнить простую проверку каждый с каждым
		for (size_t FirstIndex = 0; FirstIndex < SingleBalls.size(); ++FirstIndex)
		{
			for (auto SecondIndex = FirstIndex + 1;
				SecondIndex < SingleBalls.size(); ++SecondIndex)
			{
				auto Sum =
					SingleBalls[FirstIndex].first + SingleBalls[SecondIndex].first;
				if (Sum < Y)
				{
					if (UniqueBalls[Sum - X] > 1)//Ссылается на удалённый шар
					{
						SingleBalls[FirstIndex].second =
						SingleBalls[SecondIndex].second = false;
					}
				}
				else
				{
					break;
				}
			}
		}
		std::cout << "No pair:" << std::endl;
		for (auto &Item : SingleBalls)
		{
			if (Item.second)
			{
				std::cout << Item.first << " ";
			}
		}
		std::cout << std::endl;
		//////////////////////////////////////////
	}
	std::cout << "Press any key...";
	getch();
	return 0;
}
