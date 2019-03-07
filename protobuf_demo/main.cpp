#include <iostream>
#include <fstream>
#include <string>
#include "AddressBook.pb.h"
using namespace std;
void listPeople(const tutorial::AddressBook& address_book) {
	for (int i = 0; i < address_book.people_size(); i++) {
		const tutorial::Person& person = address_book.people(i);

		cout << "Person ID: " << person.id() << endl;
		cout << "  Name: " << person.name() << endl;
		if (person.has_email()) {
			cout << "  E-mail address: " << person.email() << endl;
		}

		for (int j = 0; j < person.phones_size(); j++) {
			const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

			switch (phone_number.type()) {
			case tutorial::Person::MOBILE:
				cout << "  Mobile phone #: ";
				break;
			case tutorial::Person::HOME:
				cout << "  Home phone #: ";
				break;
			case tutorial::Person::WORK:
				cout << "  Work phone #: ";
				break;
			}
			cout << phone_number.number() << endl;
		}
	}
}

int main() {

	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	tutorial::AddressBook addressbook;
	ifstream inf;
	string filename = "address_protobuf";
	ofstream outfile(filename);//新建空文件,不然会重复追加内容
	if (!outfile) {//检查是否打开成功
		cerr << "Failed to create file." << endl;
		return -1;
	}
	outfile.close();//关闭文件
	////打开文件,       蛇皮操作???这块代码去掉也是可以的
	//{
	//	fstream input(filename, ios::in | ios::binary);
	//	if (!addressbook.ParseFromIstream(&input)) {
	//		cerr << "Failed to parse address book." << endl;
	//		return -1;
	//	}
	//	input.close();//这里关闭也是可以的???
	//}
	//addressbook
	{
		for (int i = 0; i < 10; ++i) {
			tutorial::Person *person = addressbook.add_people();
			person->set_id(123456 + i * i * 13);
			person->set_name("xycode-" + to_string(person->id()));
			if (person->id() % 2 == 0)
				person->set_email(person->name() + "@gmail.com");
			for (int j = 0; j < 4; ++j) {
				//这是错误的
				//tutorial::Person::PhoneNumber phone_number;
				//person.add_phones(phone_number);
				tutorial::Person::PhoneNumber *phone_number = person->add_phones();
				phone_number->set_number(to_string(123456+j*i*11));
				if ((i + j) % 2 == 0) phone_number->set_type(tutorial::Person::MOBILE);
				else phone_number->set_type(tutorial::Person::WORK);
			}
			
		}
	}
	//写入文件
	{
		fstream output(filename, ios::out | ios::trunc | ios::binary);
		if (!addressbook.SerializeToOstream(&output)) {
			cerr << "Failed to write address book." << endl;
			return -1;
		}
		output.close();
	}

	{
		// Read the existing address book.
		fstream input(filename, ios::in | ios::binary);
		if (!addressbook.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return -1;
		}
		listPeople(addressbook);
	}

	system("pause");
	return 0;
}
