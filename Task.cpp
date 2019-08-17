
#include "pch.h"
#include <iostream> 

using namespace std;

struct ListNode {
	ListNode * prev;
	ListNode * next;
	ListNode * rand;
	string data;
};

class List {
public:
	void Serialize(FILE * file);
	void Deserialize(FILE * file);
	void Add(string data);
	void ShowRand();
	int Empty() {
		if (head == nullptr)
			return 0;
		else return 1;
	};
private:
	ListNode * head = nullptr;
	ListNode * tail = nullptr;

	int count;
};

void List::Serialize(FILE * file)
{
	int index = 0;
	ListNode* retPrev = nullptr;

	if (Empty() == 0) return;

	fputc(count, file);
	fputc('\n', file);

	for (ListNode* node = head; node != nullptr; node = node->next, index++)
	{
		fputs(node->data.c_str(), file);
		fputc('\n', file);
		*reinterpret_cast<int*> (&(node->prev)) = index;
	}

	for (ListNode* node = head; node != nullptr; node = node->next)
	{
		if (node->rand != nullptr)
			index = *reinterpret_cast<int*> (&(node->rand->prev));
		else
			index = 0;
		fputc(index, file);
		fputc('\n', file);
	}

	for (ListNode* node = head; node != nullptr; node = node->next)
	{
		node->prev = retPrev;
		retPrev = node;
	}
}

void List::Deserialize(FILE * file)
{
	string data;
	char buf[225];
	int index, i;
	ListNode* tmp;
	ListNode* retPrev = nullptr;

	int numLines = fgetc(file);
	if (numLines == 0) return;

	fseek(file, 1, SEEK_CUR);

	for (i = 0; i < numLines; i++)
	{
		int j = -1;
		do
		{
			j++;
			buf[j] = fgetc(file);
		} while (buf[j] != '\n');
		buf[j] = '\0';
		data = buf;
		Add(data);
	}

	i = 0;
	for (ListNode* node = head; node != nullptr; node = node->next)
	{
		tmp = node;
		index = fgetc(file);
		fseek(file, 1, SEEK_CUR);
		if (index != 0)
		{
			if (index > i)
				for (int j = 0; j < index; j++)
					tmp = tmp->next;
			else
				for (int j = index; j < i; j++)
					tmp = tmp->prev;
			node->rand = tmp;
		}
		i++;
	}
}

void List::ShowRand()
{
	if (head == nullptr)
	{
		cout << "List is empty" << endl;
		return;
	}
	for (ListNode* node = head; node != nullptr; node = node->next)
	{
		if (node->rand != nullptr)
			cout << (node->rand->data).c_str() << endl;
		else cout << "nullptr" << endl;
	}
}

void List::Add(string data) {
	ListNode *temp = new ListNode;
	temp->next = nullptr;
	temp->data = data;
	count++;

	if (head != nullptr)
	{
		temp->prev = tail;
		tail->next = temp;
		tail = temp;
		if (tail->prev != nullptr)
			temp->rand = tail->prev;
		else temp->rand = nullptr;
	}
	else
	{
		temp->prev = nullptr;
		temp->next = nullptr;
		temp->rand = nullptr;
		head = tail = temp;
	}
}
int main()
{
	const char* path = "input.dat";
	List inputList;
	List outputList;
	FILE *file;
	errno_t err;
	err = fopen_s(&file, path, "wb");
	if (err != 0)
		cout << "The file " << path << " was not opened" << endl;
	inputList.Add("AAA");
	inputList.Add("BBB");
	inputList.Add("CCCC");
	inputList.Add("DD");
	inputList.Add("EEEEE");
	inputList.Add("FFFF");
	cout << "InputList" << endl;
	inputList.ShowRand();
	inputList.Serialize(file);
	fclose(file);
	err = fopen_s(&file, path, "rb");
	if (err != 0)
		cout << "The file " << path << " was not opened" << endl;
	outputList.Deserialize(file);
	cout << endl << "OutputList " << endl;
	outputList.ShowRand();
	system("pause");
}


