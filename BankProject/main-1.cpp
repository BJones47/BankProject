/*
name: Braxton Jones
assignment: 6
file: main.cpp
date: 4/26/23
description: this program will maintain bank records using a hash map and allow the.
			 user to change their bank record via withdraw, desposit etc.
*/


#include "hashMap.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

struct bankType
{
	struct transactionType
	{
		transactionType(bool t, double a)
		{
			type = t;
			amount = a;
		}

		bool type;
		double amount;
	};

	bankType(std::string first, std::string last, std::string acctNo,
		double balance,unsigned int pin)
	{
		name = last + ", " + first;
		this->balance = balance;
		this->pin = pin;
		this->acctNo = acctNo;
		locked = false;
	}

	bankType()
	{
		name = "";
		balance = 0;
		pin = 0;
		locked = false;
	}

	std::string name;
	std::string acctNo;
	double balance;
	unsigned int pin;
	bool locked;

	std::vector<transactionType> transactions;
};


int main()
{
	
	std::cout << std::fixed << std::setprecision(2);
	hashMap<std::string, hashMap<std::string, bankType>> bank;
	hashMap<std::string, hashMap<std::string, bankType>> place;
	// read in csv file
	std::ifstream infile;
	infile.open("data.csv");
	std::string  line;

	// clear first line
	std::getline(infile, line, '\r');

	// get first line 
	while (std::getline(infile, line, '\r'))
	{

		// get each parameter in the line 
		if (line != "\n")
		{
		// parameters are first name, last name, location of bank, acct number, balance in acc
		// pin number, and the last 4 digits of the acc.
		line.erase(0, line.find('\n') + 1);
		std::string first = line.substr( 0 , line.find (','));
		line . erase ( 0 , line . find ( ',' ) + 1 );
		std :: string last = line.substr( 0 , line.find( ',' ) );
		line . erase ( 0 , line . find ( ',' ) + 1 );
		std :: string location = line.substr( 0 , line.find( ',' ) );
		line . erase ( 0 , line . find ( ',' ) + 1 );
		std :: string acctNo = line.substr( 0 , line.find( ',' ) );
		line . erase ( 0 , line . find ( ',' ) + 1 );
		std :: string balancestr = line.substr( 0 , line.find( ',' ) );
		double balance = std::stod(balancestr);
		line . erase ( 0 , line . find ( ',' ) + 1 );
		std :: string pinstr = line.substr( 0 , line.find( ',' ) );
		unsigned int pin = std::stoul(pinstr);
		std::string acct4Digits = acctNo.substr(acctNo.size() - 4, 4 );
		bank[location][acct4Digits] = bankType( first ,last ,acctNo ,balance , pin);
		place[location][location] = bankType(first, last, acctNo, balance, pin);
		}
	}	


	// location of user 
	std::string userLocation;
	// last 4 digits of user
	std::string userDig;
	// flag variable for input
	bool input = false;
	// if user failed input but wants to continue skip all steps
	bool cont = false;
	// string for y or n on continue
	std::string contin;
	// pin of user
	int userPin = 0;
	// string for pin
	std::string strPin;
	// count of pin
	int pinCount = 0;
	// choice for account
	std::string choice;
	// bool for exiting the account
	bool exit = false;
	// string for deposit
	std::string deposit;
	// dep ammount
	double depos = 0.0;

	while (!input)
	{
		// prompt user input
		// 1
		std::cout << std::endl;

		std::cout << "Please enter bank branch location: ";
		// get user input then endline
		std::getline(std::cin , userLocation);
		std::cout << std::endl;
		for (int i = 0; i < userLocation.size(); i++)
		{
				userLocation[i] = std::tolower(userLocation[i]);
		}
		// make sure first letter is always capitalized
		userLocation[0] = std::toupper(userLocation[0]);
		for (int i = 0; i < userLocation.size(); i++)
		{
			if (userLocation[i] == ' ')
			{
				userLocation[i+1] = std::toupper(userLocation[i+1]);
			}
		}
		// 2 
		// determine if location is not found.
		// no name found for location go to step 14.
		if (place[userLocation][userLocation].name == "")
		{
			std::cout << "Bank Branch not found";
			std::cout << std::endl;
			std::cout << std::endl;
			cont = true;
		}


		if (!cont)
		{
			std::cout << "Enter last 4 digits of account: ";
			std::getline(std::cin, userDig);

			// 3 
			// Ask for the last 4 digits
			// if no account found with digits go to 14
			
			if (bank[userLocation][userDig].name == "" )
			{
				std::cout << "Account not found";
				std::cout << std::endl;
				std::cout << std::endl;
				cont = true;
			}
			// 4 if found check for a lock
			if ((bank[userLocation][userDig].locked == true))
			{
				std::cout << "Account has a lock";
				std::cout << std::endl;
				std::cout << std::endl;
				cont = true;
			}
		}
		if (!cont)
		{
			std::cout << std::endl;
			while(pinCount < 3)
			{
				// step 5 ask user for pin
				std::cout << "Enter a pin: ";
				std::getline(std::cin, strPin);
				userPin = stoul(strPin);
				std::cout << std::endl;
				// step 6 if user pin = pin stored go to step 8
				if(userPin == bank[userLocation][userDig].pin)
				{
					while(!exit)
					{
						// prompt user of options and get the choice
						std::cout << "(D)eposit" << std::endl;
						std::cout << "(W)ithdrawal" << std::endl;
						std::cout << "(V)iew account" << std::endl;
						std::cout << "(E)xit account" << std::endl;
						std::cout << "Enter choice: ";
						std::getline(std::cin, choice);
						std::cout << std::endl;

						// 8 - 13
						// if user presses options bring to corresponding page.
						// d prompts user for deposit

						if (choice == "d" || choice == "D")
						{
							std::cout << "Enter deposit amount: ";
							std::getline(std::cin, deposit);
							depos = stod(deposit);
							bank[userLocation][userDig].transactions
							.push_back({true, depos});
							bank[userLocation][userDig].balance =
							bank[userLocation][userDig].balance + depos;
							std::cout << std::endl;
							
						}

						// w to withdraw
						else if (choice == "w" || choice == "W")
						{
							std::cout << "Enter withdrawal amount: ";
							std::getline(std::cin, deposit);
							depos = stod(deposit);
							// if you can withdraw and not go negative withdraw
							if (bank[userLocation][userDig].balance - depos > 0)
							{
							bank[userLocation][userDig].transactions
							.push_back({false, depos});
							bank[userLocation][userDig].balance =
							bank[userLocation][userDig].balance - depos;
							}
							// if you cant withdraw output no funds
							else
							{
								std::cout << std::endl;
								std::cout <<
								"Not enough funds to make transaction";
								std::cout << std::endl;
							}
							std::cout << std::endl;
							
						}

						// v to view transactions
						else if (choice == "v" || choice == "V")
						{
							std::cout << "Name: " << 
							bank[userLocation][userDig].name;
							std::cout << std::endl;
							std::cout << "Balance: $" <<
							bank[userLocation][userDig].balance <<
							std::endl;
							std::cout << std::endl;

							// if no transactions in vector output
							if (bank[userLocation][userDig].transactions.size()
								== 0)
							{
								std::cout << "No transactions"
								<< std::endl;
								std::cout << std::endl;
							}

							else
							{
								// list out all transactions
								std::cout << "List of transactions" <<
								std::endl;
								for(int i =0; 
								i < bank[userLocation][userDig].transactions.size();
								i++)
								{
									if (bank[userLocation][userDig].
									transactions[i].type == true)
									{
										std::cout << "Deposit amount $" << 
										bank[userLocation][userDig].
										transactions[i].amount <<
										std::endl;
									}

									else
									{
										std::cout << "Withdrawal amount $" << 
										bank[userLocation][userDig].
										transactions[i].amount <<
										std::endl;
									}
								}
								std::cout << std::endl;
							}
						}
						// if e selected exit account
						else if (choice == "e" || choice == "E")
						{
							cont = true;
							exit = true;
							pinCount = 3;
						}
						// if none of letters selected output invalid
						else
						{
							std::cout << "Invalid choice"
							<< std::endl;
						}
					}
				}
				else 
				{
					std::cout << "Invalid pin" << std::endl;
					std::cout << std::endl;	
					pinCount++;
					// step 7 if pin fails = 3 lock account and go to step 14
					if (pinCount == 3 && !exit)
					{
						std::cout << "3 failed login attempts, account is now locked";
						std::cout << std::endl;
						std::cout << std::endl;
						cont = true;
						bank[userLocation][userDig].locked = true;
					}
				}
				
			}
			
		}

		if (cont)
		{
		// 14
		// prompt user for reinput

			std::cout << "Continue? (Y/N): ";
			std::getline(std::cin, contin);
			if (contin == "n" || contin == "N" )
			{
				input = true;
			}
		}

		// reset all flags
		cont = false;
		pinCount = 0;
		exit = false;
	}

	return 0;
}


