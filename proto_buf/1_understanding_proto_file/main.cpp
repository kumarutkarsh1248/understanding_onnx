#include<iostream>
#include <fstream>
#include "person.pb.h"
using namespace std;


int main()
{
    PERSON::Person person1, person2, person3, person4;

    //person1 info
    string name1  = "utkarsh";
    int id1 = 01;
    string email1 = "@gamil.com";
    //person2 info
    string name2  = "kumar";
    int id2 = 02;
    string email2 = "@g_amil.com";

    //setting name, id, email field int he person message
    person1.set_name(name1);
    person1.set_id(id1);
    person1.set_email(email1);
    //setting name, id, email field int he person message
    person2.set_name(name2);
    person2.set_id(id2);
    person2.set_email(email2);


    //lets create array of addressbook
    PERSON::AddressBook address_book;
    address_book.add_people()->CopyFrom(person1);
    address_book.add_people()->CopyFrom(person2);

    //populating the address book
    // address_book.add_people()->set_id(10000);  //3rd person
    // address_book.add_people()->set_name("sahil");  //4th person
    cout<< " ---------------------------------"<<endl;

    //accessing the data
    cout<<" peson1 detail\n"<< address_book.people(0).DebugString() <<endl;
    cout<<" peson2 detail \n"<< address_book.people(1).DebugString() <<endl;
    cout<< " --------------------------------"<< endl;

    //we can also prin the details like
    for (int i =0; i<address_book.people_size(); i++)
    {
        cout<< address_book.mutable_people(i)->DebugString() <<endl;
    }
    cout<<"done"<<endl;




    //---------------------------saving the data
    string serialized_data = address_book.SerializeAsString();
    std::ofstream outfile("addressbook.pb", std::ios::out | std::ios::binary);

    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return 1;
    }

    outfile << serialized_data;
    outfile.close();
    cout<<"data successfully loaded and closed"<<endl;

    //-------------reading the data from the addressbook.pb
    std::ifstream infile("address_book.pb", std::ios::in | std::ios::binary);
    if (!infile.is_open()) {
        std::cerr << "Error opening file for reading!" << std::endl;
        return 1;
    }
    
}









    // //get the name, id, email fields
    // if(person1.has_name()){
    //     cout<< " name is "<< person1.mutable_name() <<endl;
    // }else{
    //     cout<< "name is not defined" <<endl;
    // }

    // if(person1.has_email()){
    //     cout<< " email is "<< person1.mutable_email() <<endl;
    // }else{
    //     cout<< "email is not defined" <<endl;
    // }

    // if(person1.has_id()){
    //     cout<< " id is "<< person1.id() <<endl;
    // }else{
    //     cout<< "id is not defined" <<endl;
    // }
    // cout<<"-----------------------------------"<<endl;

    // //printing the prot message with debugstring()
    // cout<< person1.DebugString() <<endl;


    // cout<<"this is the example"<<endl;
