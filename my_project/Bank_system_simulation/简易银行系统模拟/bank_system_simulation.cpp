#include "iostream"
#include "bank_system_simulation.h"

using std::cout;
using std::endl;
using std::string;

//formatting stuff
typedef std::ios_base::fmtflags format;
typedef std::streamsize precis;
format setFormat();
void restore(format f, precis p);

//Bank methods
Bank::Bank(const string &s, long an, double bal)
	:fullName(s), acctNum(an), balance(bal){}

void Bank::Deposit(double amt)
{
	if (amt < 0)
	{
		cout << "Negative deposit not allowed;"
			 << "deposit is cancelled.\n";
	}
	else
	{
		balance += amt;
	}
}

void Bank::Withdrawl(double amt)
{
	/*set up ###.## format*/
	format initialState = setFormat();
	precis prec = cout.precision(2);

	if (amt < 0)
	{
		cout << "Withdrawl amount must be positive; "
			 << "withdrawl canceled.\n";
	}
	else if (amt <= balance)
	{
		balance -= amt;
	}
	else
	{
		cout << "Withdrawl amount of $" << amt
			 << " exceeds your balance.\n"
		     << "withdrawl canceled.\n";
	}
	restore(initialState, prec);
}

double Bank::Balance() const
{
	return balance;
}

void Bank::ViewAcct() const
{
	/*set up ###.## format*/
	format initialState = setFormat(); 
	precis prec = cout.precision(2);

	cout << __FUNCTION__ << endl;
	cout << "Client: " << fullName << endl;
	cout << "Accout Number: " << acctNum << endl;
	cout << "Balance: $" << balance << endl;

	restore(initialState, prec);
}

/*BankPlus methods*/
BankPlus::BankPlus(const std::string &s, long an,
	double bal , double ml, double r)
	:Bank(s, an, bal), maxLoan(ml), oweBank(0.0),rate(r) {}

/*redefine how ViewAcct{} works*/
void BankPlus::ViewAcct() const
{
	/*set up ###.## format*/
	format initialState = setFormat(); 
	precis prec = cout.precision(2);

	Bank::ViewAcct();      //display base portion
	cout << "Maximum loan: $" << maxLoan << endl;
	cout << "Owed to bank: $" << oweBank << endl;

	cout.precision(3);
	cout << "Load Rate: " << 100 * rate << "%\n";
    restore(initialState,prec);
}

/*redefine how Withdraw{} works*/
void BankPlus::Withdrawl(double amt)
{
	/*set up ###.## format*/
	format initialState = setFormat(); 
	precis prec = cout.precision(2);

	cout << __FUNCTION__ << endl;
	double bal = Balance();
	if (amt <= bal)
	{
		Bank::Withdrawl(amt);
	}
	else if (amt <= bal + maxLoan - oweBank)
	{
		double advance = amt - bal;
		oweBank += advance * (1.0 + rate);
		cout << "Bank advances: $" << advance << endl;
		Deposit(advance);
		Bank::Withdrawl(amt);
	}
	else
	{
		cout << "Credit limit exceeded. Transaction cancelled.\n";
	}

	restore(initialState, prec);
}

format setFormat()
{
	//set up ###.## format
	return cout.setf(std::ios_base::fixed,
		             std::ios_base::floatfield);
}

void restore(format f, precis p)
{
	cout.setf(f, std::ios_base::floatfield);
	cout.precision(p);
}