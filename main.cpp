#include <iostream>
#include <string>
#include <GeneralNN/GNeuralNetwork.h>
#include <vector>
#include <Windows.h>
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;

constexpr unsigned char EMPTY = ' ';
constexpr unsigned char WALL = '#';
constexpr unsigned char SNAKE = 178;
constexpr unsigned char FOOD = 248;

void HideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void ClearConsole()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD written;

	FillConsoleOutputCharacterA(console, ' ', 200 * 200, topLeft, &written);
	SetConsoleCursorPosition(console, topLeft);
}

void SetCursorToStart()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(hConsole, pos);
}

struct BoardPoint
{
	int X;
	int Y;
	bool brandNew;

	BoardPoint() : X(0), Y(0), brandNew(true) {};
	BoardPoint(const int x, const int y) : X(x), Y(y), brandNew(true) {};
	float DistanceFrom(const BoardPoint& other) { return (float)sqrt((other.X - X) * (other.X - X) + (other.Y - Y) * (other.Y - Y)); };
	bool operator==(const BoardPoint& p) { return p.X == X && p.Y == Y; }
	bool operator!=(const BoardPoint& p) { return !operator==(p); }
};

class SnakeGame
{
private:
	vector<string> Board;
	vector<BoardPoint> Snake;
	BoardPoint Food;
	int Score;
	int Width;
	int Height;
	char CurrentDirection;

	vector<string> Image;

	float AbsoluteUpHittingDanger()
	{
		int distance = Snake.front().X;
		for (int i = Snake.front().X - 1; i >= 0; i--)
		{
			if (std::find(Snake.begin(), Snake.end(), BoardPoint(i, Snake.front().Y)) != Snake.end())
			{
				const int distanceToSelf = std::abs(Snake.front().X - i);
				if (distanceToSelf < distance)
					distance = distanceToSelf;
				break;
			}
		}
		return (1.f / distance) * 2.f - 1.f;
	}
	float AbsoluteDownHittingDanger()
	{
		int distance = std::abs(Snake.front().X - (Height - 1));
		for (int i = Snake.front().X + 1; i < (int)Image.size(); i++)
		{
			if (std::find(Snake.begin(), Snake.end(), BoardPoint(i, Snake.front().Y)) != Snake.end())
			{
				const int distanceToSelf = std::abs(Snake.front().X - i);
				if (distanceToSelf < distance)
					distance = distanceToSelf;
				break;
			}
		}
		return (1.f / distance) * 2.f - 1.f;
	}
	float AbsoluteRightHittingDanger()
	{
		int distance = std::abs(Snake.front().Y - (Width - 1));
		for (int i = Snake.front().Y + 1; i < Width; i++)
		{
			if (std::find(Snake.begin(), Snake.end(), BoardPoint(Snake.front().X, i)) != Snake.end())
			{
				const int distanceToSelf = std::abs(Snake.front().Y - i);
				if (distanceToSelf < distance)
					distance = distanceToSelf;
				break;
			}
		}
		return (1.f / distance) * 2.f - 1.f;
	}
	float AbsoluteLeftHittingDanger()
	{
		int distance = Snake.front().Y;
		for (int i = Snake.front().Y - 1; i >= 0; i--)
		{
			if (std::find(Snake.begin(), Snake.end(), BoardPoint(Snake.front().X, i)) != Snake.end())
			{
				const int distanceToSelf = std::abs(Snake.front().Y - i);
				if (distanceToSelf < distance)
					distance = distanceToSelf;
				break;
			}
		}
		return (1.f / distance) * 2.f - 1.f;
	}
public:
	SnakeGame(const int width, const int height) { Initialize(width, height); }
	void Initialize(const int width, const int height)
	{
		srand((unsigned int)time(0));
		Height = height;
		Width = width;
		Score = 0;
		CurrentDirection = 'r';
		Board.resize(Height);

		for (int i = 0; i < Height; i++)
		{
			Board[i].resize(Width);
			for (int j = 0; j < Width; j++)
			{
				Board[i][j] = EMPTY;
			}
		}
		for (int i = 0; i < Height; i++)
		{
			Board[i][0] = WALL;
			Board[i][(size_t)Width - 1] = WALL;
		}
		for (int i = 0; i < Width; i++)
		{
			Board[0][i] = WALL;
			Board[(size_t)Height - 1][i] = WALL;
		}
		CreateFood();
	}
	void PrintBoard()
	{
		SetCursorToStart();
		Image = GetGameImage();
		for (int i = 0; i < Height; i++)
			cout << Image[i] << endl;
	}

	vector<string> GetGameImage()
	{
		vector<string> Image = Board;
		if (Food != BoardPoint())
			Image[Food.X][Food.Y] = FOOD;

		for (const BoardPoint& p : Snake)
		{
			Image[p.X][p.Y] = SNAKE;
		}
		return Image;
	}

	void InitialSnakePos(const int x, const int y)
	{
		Snake.push_back(BoardPoint(x, y));
	}

	void Move()
	{
		if (CurrentDirection == ' ')
			return;

		Snake.back() = Snake.front();
		if (CurrentDirection == 'r')
		{
			Snake.back().Y++;
		}
		else if (CurrentDirection == 'l')
		{
			Snake.back().Y--;
		}
		else if (CurrentDirection == 'u')
		{
			Snake.back().X--;
		}
		else if (CurrentDirection == 'd')
		{
			Snake.back().X++;
		}

		Snake.back().brandNew = false;
		Snake.insert(Snake.begin(), Snake.back());
		Snake.pop_back();


		if (Snake.front() == Food)
		{
			Food = BoardPoint();
			AddSegment();
			CreateFood();
			Score += Width + Height;
		}
		else
		{
			Score -= 1;
		}
	}

	void Turn(char direction)
	{
		if (direction == 'u')
		{
			// nothing to do here
		}
		else if (direction == 'r')
		{
			if (CurrentDirection == 'r')
			{
				CurrentDirection = 'd';
			}
			else if (CurrentDirection == 'l')
			{
				CurrentDirection = 'u';
			}
			else if (CurrentDirection == 'u')
			{
				CurrentDirection = 'r';
			}
			else if (CurrentDirection == 'd')
			{
				CurrentDirection = 'l';
			}
		}
		else if (direction == 'l')
		{
			if (CurrentDirection == 'r')
			{
				CurrentDirection = 'u';
			}
			else if (CurrentDirection == 'l')
			{
				CurrentDirection = 'd';
			}
			else if (CurrentDirection == 'u')
			{
				CurrentDirection = 'l';
			}
			else if (CurrentDirection == 'd')
			{
				CurrentDirection = 'r';
			}
		}
	}

	bool GameOver()
	{
		for (int i = 0; i < (int)Snake.size(); i++)
		{
			// walls
			if (Snake[i].X == 0 || Snake[i].X == Height - 1)
				return true;
			if (Snake[i].Y == 0 || Snake[i].Y == Width - 1)
				return true;
		}

		// self eating
		for (int i = 1; i < (int)Snake.size(); i++)
		{
			if (Snake.front() == Snake[i] && !Snake.front().brandNew)
				return true;
		}

		return false;
	}

	void CreateFood()
	{
		do {
			Food = BoardPoint(rand() % (Height - 2) + 1, rand() % (Width - 2) + 1);
		} while (std::find(Snake.begin(), Snake.end(), Food) != Snake.end());
	}

	void AddSegment()
	{
		Snake.front().brandNew = true;
		Snake.push_back(Snake.front());
	}

	int GetScore()
	{
		return Score;
	}

	const vector<float>& BuildNeuralInput()
	{
		static vector<float> res(5, 0.f);

		const float currentHorizontalFood = (float)(Food.Y - Snake.front().Y);
		const float currentVerticalFood = (float)(Food.X - Snake.front().X);

		const float horFood = (currentHorizontalFood / (Width - 3)) > 0.f ? 1.f : -1.f;
		const float verFood = (currentVerticalFood / (Height - 3)) > 0.f ? 1.f : -1.f;

		if (CurrentDirection == 'r')
		{
			res[0] = horFood;
			res[1] = verFood;
			res[2] = AbsoluteRightHittingDanger();
			res[3] = AbsoluteDownHittingDanger();
			res[4] = AbsoluteUpHittingDanger();
		}
		else if (CurrentDirection == 'l')
		{
			res[0] = -horFood;
			res[1] = -verFood;
			res[2] = AbsoluteLeftHittingDanger();
			res[3] = AbsoluteUpHittingDanger();
			res[4] = AbsoluteDownHittingDanger();
		}
		else if (CurrentDirection == 'u')
		{
			res[0] = -verFood;
			res[1] = horFood;
			res[2] = AbsoluteUpHittingDanger();
			res[3] = AbsoluteRightHittingDanger();
			res[4] = AbsoluteLeftHittingDanger();
		}
		else if (CurrentDirection == 'd')
		{
			res[0] = verFood;
			res[1] = -horFood;
			res[2] = AbsoluteDownHittingDanger();
			res[3] = AbsoluteLeftHittingDanger();
			res[4] = AbsoluteRightHittingDanger();
		}
		return res;
	}
};

const vector<float>& BuildOutput(const vector<Neuron>& output)
{
	static vector<float> res((int)output.size());
	for (int i = 0; i < (int)output.size(); i++)
		res[i] = output[i].Activation;
	return res;
}

int main()
{
	HideCursor();
	NormalRealRandom r(0.0f, 1.f);

	const int width = 10;
	const int height = 10;
	const int Elite = 100;
	vector<GNeuralNetwork> nets(1000);
	for (GNeuralNetwork& n : nets)
	{
		n.BuildFFNetwork({ 5, 4, 3 });
		n.RandomizeWeights();
	}

	int currentGeneration = 0;
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		int totalMovesInCurrentGeneration = 0;
		for (int n = Elite; n < (int)nets.size(); n++)
		{
			SnakeGame game(width, height);
			game.InitialSnakePos(1, 1);
			int moveCount = 0;
			while (!game.GameOver() && !GetAsyncKeyState(VK_ESCAPE) && ++moveCount <= 200)
			{
				const vector<float>& Input = game.BuildNeuralInput();
				const vector<float>& Output = BuildOutput(nets[n].Feed(Input));
				const int Index = (int)std::distance(Output.begin(), std::max_element(Output.begin(), Output.end()));
				if (Index == 0)
					game.Turn('u');
				else if (Index == 1)
					game.Turn('r');
				else if (Index == 2)
					game.Turn('l');
				game.Move();
			}
			totalMovesInCurrentGeneration += moveCount;
			nets[n].Info.Accuracy = (float)game.GetScore();
		}

		const float bestSoFar = nets.front().Info.Accuracy;
		std::sort(nets.begin(), nets.end(), [](const GNeuralNetwork& left, const GNeuralNetwork& right)
			{
				return left.Info.Accuracy > right.Info.Accuracy;
			});


		cout << "Current generation: " << ++currentGeneration << " | Average moves per game: " << totalMovesInCurrentGeneration / (int)nets.size();
		if (bestSoFar < nets.front().Info.Accuracy)
			cout << " | New best: " << nets.front().Info.Accuracy;
		cout << endl;

		vector<int> parents(2);
		for (int i = Elite; i < (int)nets.size(); i++)
		{
			for (int& parent : parents)
				parent = rand() % Elite;

			nets[i].RunThroughEachWeight([&](vector<Layer>& layers, const Point& p, const unsigned int index, float& weight)
				{
					if (index == (unsigned int)-1)
						weight = nets[parents[rand() % (int)parents.size()]].Layers[p.L].Neurons[p.N].Bias;
					else
						weight = nets[parents[rand() % (int)parents.size()]].Layers[p.L].Neurons[p.N].InputVector[index].Weight;

					if (rand() < (int)(RAND_MAX * 0.2f))
						weight += (float)r();
				});
		}
	}

	SnakeGame game(40, 25);
	game.InitialSnakePos(1, 1);

	ClearConsole();

	while (!game.GameOver())
	{
		const vector<float>& Input = game.BuildNeuralInput();
		const vector<float>& Output = BuildOutput(nets.front().Feed(Input));
		const int Index = (int)std::distance(Output.begin(), std::max_element(Output.begin(), Output.end()));
		if (Index == 0)
			game.Turn('u');
		else if (Index == 1)
			game.Turn('r');
		else if (Index == 2)
			game.Turn('l');

		game.Move();

		game.PrintBoard();
		cout << "Current score: " << game.GetScore() << "\t\t";
		Sleep(50);
	}
	game.PrintBoard();
	cout << "Game over: " << game.GetScore() << "\t\t";
	cin.get();
}