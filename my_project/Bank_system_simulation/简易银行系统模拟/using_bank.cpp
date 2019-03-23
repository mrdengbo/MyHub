/*
 *文件名：bank_system_simulation
 *创建者：邓博
 *创建时间：2018/6/10
 *功能：模拟银行的存储系统
*/

#include "iostream"
#include "bank_system_simulation.h"
#include "string"

const int CLIENTS = 4;
#define ORDINARY_ACCOUT '1'
#define ADVANCE_ACCOUT  '2'

/*
//use Bank and BankPlus to test Class
int main(void)
{
	using std::cout;
	using std::endl;

	Bank Peter("Peter", 1437, 4000.00);
	BankPlus DengBo("Dengbo", 1438, 3000.00);
	Peter.ViewAcct();
	cout << endl;
	DengBo.ViewAcct();
	cout << endl;
	cout << "Depositing $1000 into the Peter Accout:\n";
	Peter.Deposit(1000.00);
	cout << "New balance: $" << Peter.Balance() << endl;
	cout << "Withdrawing $4000 from the Peter Accout:\n";
	Peter.Withdrawl(4000.00);
	Peter.ViewAcct();
	cout << endl;

	cout << "DengBo's balance: $" << DengBo.Balance() << endl;
	cout << "Withdrawing $4000 from the Dengbo Accout:\n";
	DengBo.Withdrawl(3500);
	cout << endl;
	DengBo.ViewAcct();
	system("pause");
}
*/

int main(void)
{
	using std::cin;
	using std::cout;
	using std::endl;

	Bank * pClients[CLIENTS];
	std::string temp;
	long tempnum;
	double tempbal;
	char kind;

	for (int i = 0; i < CLIENTS; i++)
	{
		cout << "Enter client's name: ";
		getline(cin, temp);
		cout << "Enter client's accout number: ";
		cin >>tempnum;
		cout << "Enter opening balance: $";
		cin >> tempbal;

		cout << "Enter 1 for ordinary Accout or "
			 << "2 for Advance Accout : ";
		while (cin >> kind && (kind != ORDINARY_ACCOUT && kind != ADVANCE_ACCOUT))
		{
			cout << "Please input either 1 of 2: ";
		}

		if (ORDINARY_ACCOUT == kind)
		{
			pClients[i] = new Bank(temp, tempnum, tempbal);
		}
		else
		{
			double tmax, trate;
			cout << "Enter the overdraft limit: $";
			cin >> tmax;
			cout << "Enter the interest rate "
				 << "as a decimal fraction: ";
			cin >> trate;
			pClients[i] = new BankPlus(temp, tempnum,
				tempbal, tmax, trate);
		}
		while (cin.get() != '\n')
		{
			continue;
		}
	}

	cout << endl;
	for (int i = 0; i < CLIENTS; i++)
	{
		pClients[i]->ViewAcct();
		cout << endl;
	}

	for (int j = 0; j < CLIENTS; j++)
	{
		delete pClients[j];
	}

	cout << "Done.\n";

	system("pause");
}