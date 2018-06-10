#ifndef BANK_SYSTEM_SIMULATION
#define BANK_SYSTEM_SIMULATION

#include "string"

/*ordinary class without overdraft*/
class Bank{
private:
	std::string fullName;
	long long acctNum;
	double balance;

public:
	Bank(const std::string &s = "Nullbody", long an = -1,
		double bal = 0.0);
	void Deposit(double amt);            //客户存款
	virtual void Withdrawl(double amt);   //客户取款
	double Balance() const;              //客户当前余额
	virtual void ViewAcct() const;        //显示客户当前的信息
};

/*Plus Accout Class with overdraft*/
class BankPlus : public Bank{
private:
	double maxLoan;
	double rate;
	double oweBank;

public:
	BankPlus(const std::string &s = "NullBody", long an = -1,
		double bal = 0.0, double ml = 500, double r = 0.11125);
	BankPlus(const Bank &ba, double ml = 500, double r = 0.11125);
	virtual void ViewAcct()const;
	virtual void Withdrawl(double amt);
	void ResetMax(double m) { maxLoan = m; }
	void ResetRate(double r) { rate = r; }
	void ResetOwes() { oweBank = 0; }
};

#endif