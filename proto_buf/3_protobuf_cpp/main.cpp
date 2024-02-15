#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"
using namespace std;

// This function fills in a Person message based on user input.
void PromptForAddress(tutorial::Person* person)
{
    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);

    cout << "Enter name: ";
    getline(cin, *person->mutable_name());  //mutable name give the pointer 

    cout << "Enter email address (blank for none): ";
    string email;
    getline(cin, email);
    if (!email.empty()) {
        person->set_email(email);
    }

        
    while (true) {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty()) {
            break;
        }

        tutorial::Person::PhoneNumber* phone_number = person->add_phones();
        phone_number->set_number(number);

        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile") {
        phone_number->set_type(tutorial::Person::PHONE_TYPE_MOBILE);
        } else if (type == "home") {
        phone_number->set_type(tutorial::Person::PHONE_TYPE_HOME);
        } else if (type == "work") {
        phone_number->set_type(tutorial::Person::PHONE_TYPE_WORK);
        } else {
        cout << "Unknown phone type.  Using default." << endl;
        }
    }
}


int main()
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    
    return 0;
}