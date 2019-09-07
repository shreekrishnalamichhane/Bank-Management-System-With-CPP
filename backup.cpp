#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>
#include<conio.h>
#include <string>
#include<windows.h>

using namespace std;

/*
opening modes note: 
using namespace ios;
in -> input/read
out -> output/write (default mode) 
app -> append
ate -> update
binary -> binary
*/

/*function prototypes*/

void adminMenu();
void retToMenu();
/*class Declaration*/
class adminOpr{
	char fullName[75];
	int age,userId;
	double balance,amount;
	
	
	public:
		adminOpr(){ 
		/*
		Here These Values are declared so that 
		initially the data in the file has been 
		written without input from user.
		*/
			userId=0;
			age = 0;
			balance = 0;
			strcpy(fullName,"User");
		}
	
		void addUser(){
		/*
			Take Data From User
		*/
			cout<<"Enter Account Id: ";cin>>userId;
			cin.ignore();
			cout<<"Enter Full Name of User: ";cin.getline(fullName,75);
			cout<<"Enter Age of User: ";cin>>age;
			cout<<"Enter initial Balance of User: ";cin>>balance;
				if(balance<1000.0){
					cout<<endl<<"Error - Balance must be atleast 1000.0\nRetry!"<<endl;
					Sleep(2000);
					system("cls");
					addUser();
				}
				cout<<endl<<endl;
			cout<<"Creating Account.."<<endl<<"Please wait.."<<endl;
			Sleep(2000);
			cout<<"Account Created Successfully!"<<endl;
			cout<<"Press enter to return back to menu.";
		}
	
		void displayUser(){
			cout<<"Account Id: "<<userId<<endl;
			cout<<"Name of User: "<<fullName<<endl;
			cout<<"Age of User: "<<age<<endl;
			cout<<"Balance: "<<balance<<endl<<endl;
					
		}
		void editUser(){
			cout<<"Enter These Informations about user: "<<endl;
			cout<<"Enter Account Id: ";cin>>userId;
			cin.ignore();
			cout<<"Enter Full Name of User: ";cin.getline(fullName,75);
			cout<<"Enter Age of User: ";cin>>age;
			cout<<"Updating Account.."<<endl<<"Please wait.."<<endl;
			Sleep(2000);
			cout<<"Account Created Successfully!"<<endl;
			cout<<"Press enter to return back to menu.";
		}
		void deposit(){
			amount = 0;
			cout<<"Deposit Amount: "<<endl;
			cout<<"Enter Amount to deposit :";cin>>amount;
			balance+=amount;
			cout<<"Balance Updated Successfully!"<<endl;
			cout<<"Your New Balance is: "<<balance;
		}
	
		void withdraw(){
			system("cls");
			amount = 0;
			cout<<"Withdraw Amount: "<<endl;
			cout<<"Enter Amount to withdraw :";cin>>amount;
			if(amount>balance){
				cout<<"Balance is too low fot withdrawl."<<endl<<"Retry";
				Sleep(1500);
				withdraw();
			}
			else{
				balance-=amount;
				cout<<"Balance Updated Successfully!"<<endl;
				cout<<"Your New Balance is: "<<balance;
			}	
		}
		int storeUser();
		void displayAllUsers();
		void searchUsers(int);//int means that it will ask for userid for userId for searching.
		void editUser(int);
		void deleteUser(int);
		void depositAndWithdraw(int,int);
};

int adminOpr::storeUser(){
	if(userId==0 && balance == 0){
		cout<<"User Data Not Initialized"<<endl;
		return 0;
	}
	else{
		ofstream fout;
		fout.open("userData.dat",ios::app|ios::binary);
		
		fout.write((char*)this,sizeof(*this));
		fout.close();
		return 1;
	}
}

void adminOpr::displayAllUsers(){
	
	ifstream fin;/*fin will be used instead of cin but for file*/
	fin.open("userData.dat",ios::in|ios::binary);
		if(!fin){
			cout<<"Error - File Not Opened."<<endl;
		}
		else{
			
			fin.read((char*)this,sizeof(*this));
			while(!fin.eof()){
				displayUser();
				fin.read((char*)this,sizeof(*this));
			}
			fin.close();
		}
		cout<<"Press enter to return back to menu.";

}
void adminOpr::searchUsers(int uid){
	
	int is_present=0;
	ifstream fin;
	fin.open("userData.dat",ios::in|ios::binary);
	if(!fin){
		cout<<"Error - File Not Opened."<<endl;
	}
	else{
		
		
		fin.read((char*)this,sizeof(*this));
		while(!fin.eof()){
			if(uid==userId){
				displayUser();	
				is_present++;
			}
			fin.read((char*)this,sizeof(*this));	
		}
		if(is_present==0)
			cout<<"User Not Found.";
			
		fin.close();	
	}
	
}

void adminOpr::editUser(int uid){
	int is_present = 0;
	fstream editFile;
	editFile.open("userData.dat",ios::in|ios::out|ios::ate|ios::binary);
	
	if(!editFile){
		cout<<"Error - File not Opened"<<endl;
	}
	editFile.seekg(0);//seekg(0) means statring byte in the file.
	editFile.read((char*)this,sizeof(*this));
	
	while(!editFile.eof()){
		if(uid==userId){//!= gives value 1, so it is true.
			editUser();
			editFile.seekp(editFile.tellp()-sizeof(*this));
			/*this is done to write data in the same record. 
			 Seekg 0 means that it initialized
			 Here when the data is read initially, then the file pointer
			 points to first byte of next record. So after reading the record, we need 
			 to modify it on the same byte area.  
			*/
			
			editFile.write((char*)this,sizeof(*this));			
			is_present++;
		}
		editFile.read((char*)this,sizeof(*this));
	}
	editFile.close();
	

	if(is_present==0){
		cout<<"User Not Found.";
	}
}
void adminOpr::deleteUser(int uid){
	int is_present=0;
	ifstream fin;
	ofstream fout;
	fin.open("userData.dat",ios::in|ios::binary);

	if(!fin){
		cout<<"Error - File Not Opened";
	}
	else{
	
		fout.open("backup.dat",ios::out|ios::binary);
		fin.read((char*)this,sizeof(*this));
		while(!fin.eof()){
				
			if(uid!=userId){
				fout.write((char*)this,sizeof(*this));
				is_present++;
			
			}
			fin.read((char*)this,sizeof(*this));	
		}
		
		fin.close();
		fout.close();
		
		remove("userData.dat");
		rename("backup.dat","userdata.dat");
		
			
	}
	if(is_present==0){
		cout<<"User not found in directory."<<endl;
	}
}

void adminOpr::depositAndWithdraw(int uid, int option){
	int is_present = 0;
	fstream depWithFile;
	depWithFile.open("userData.dat",ios::in|ios::out|ios::binary);
	
	if(!depWithFile){
		cout<<"Error - File not opened."<<endl;
	}
	
	depWithFile.seekg(0);
	depWithFile.read((char*)this,sizeof(*this));
	
	while(!depWithFile.eof()){
		if(uid==userId){
			depWithFile.seekp(depWithFile.tellp()-sizeof(*this));
			
			if(option == 0)
				deposit();
			else if(option == 1)
				withdraw();
				
		depWithFile.write((char*)this,sizeof(*this));			
		is_present++;
		
		}
		depWithFile.read((char*)this,sizeof(*this));
	}
	depWithFile.close();
	
	if(is_present==0){
		cout<<"User Not Found in Directory."<<endl;
	}
}

/*main function*/
int main(){
	adminMenu();
}

/*Function Declarations*/

void adminMenu(){
	adminOpr ao;
	int choice,uid;
	do{
		system("cls");
		cout<<"1. Add User"<<endl;
		cout<<"2. Display All Users"<<endl;
		cout<<"3. Search User"<<endl;
		cout<<"4. Edit User"<<endl;
		cout<<"5. Delete User"<<endl;
		cout<<"6. Deposit Amount"<<endl;
		cout<<"7. Withdraw Amount"<<endl;
		cout<<"Enter Your Choice: ";cin>>choice;
		Sleep(1000);
		system("cls");
		switch (choice){
		case 1:	
			ao.addUser();
			ao.storeUser();
			break;
		case 2:
			ao.displayAllUsers();
			break;
		case 3:
			system("cls");
			cout<<"Enter Account Id of User to Search: ";cin>>uid;
			ao.searchUsers(uid);
			cout<<"Press enter to return back to menu.";
			break;
		case 4:
			system("cls");
			cout<<"Enter Account Id of user to Edit information: ";cin>>uid;
			ao.editUser(uid);
			cout<<"Press enter to return back to menu.";
			break;
		case 5:
			system("cls");
			cout<<"Enter Account Id of User to delete: ";cin>>uid;		
			ao.deleteUser(uid);
			cout<<"Data removed."<<endl<<"Press Enter key to return back!";
			break;
		case 6:
			system("cls");
			cout<<"Enter Account Id of user to Deposit Amount: ";cin>>uid;
			ao.depositAndWithdraw(uid,0);
			cout<<"Press enter to return back to menu.";
			break;
		case 7:
			system("cls");
			cout<<"Enter Account Id of user to Withdraw Amount: ";cin>>uid;
			ao.depositAndWithdraw(uid,1);
			cout<<"Press enter to return back to menu.";
			break;
		default:
			cout<<"\a";
			cout<<"Invalid Option. Retry!";
			Sleep(2500);
			adminMenu();
		}	
		cin.ignore();
		cin.get();
	}while(choice!=8);
	
}

