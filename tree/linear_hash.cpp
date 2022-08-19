#include <iostream>
#include <string>

using namespace std;

const int INITIAL_VALUE = 0;
const int PRIME_FACTOR = 31;

class PhoneRecord
{
private:
    string persName; // key
    string telNo;    // value

public:
    PhoneRecord(string persName_T, string telNo_T)
    {
        if (persName != persName_T)
            persName = persName_T;
        if (telNo != telNo_T)
            telNo = telNo_T;
        cout << "One record inserted:" << persName << " " << telNo << endl;
    }
    void setRecord(string persName_T, string telNo_T)
    {
        if (persName != persName_T)
            persName = persName_T;
        if (telNo != telNo_T)
            telNo = telNo_T;
        cout << "One record changed:" << persName << " " << telNo << endl;
    }
    string getPersName()
    {
        return persName;
    }
    string getTelNo()
    {
        return telNo;
    }
};

class PhoneBook
{
private:
    int dimension;
    PhoneRecord **pr;

public:
    PhoneBook(int dimension)
    {
        this->dimension = dimension;
        pr = new PhoneRecord *[dimension];
    }
    int hashKey(string key)
    {
        int h = INITIAL_VALUE;

        for (int i = 0; i < key.size(); i++)

            h = (h * PRIME_FACTOR + key[i]) % dimension;

        return h;
    }
    int find_slot(string key)
    {
        int i = hashKey(key);

        if (pr[i] == NULL)
            return i;
        else if (pr[i]->getPersName() == key)
            return i;
        else
        {
            int j = 0;
            while (pr[i] != NULL && pr[i]->getPersName() != key && j < dimension)
            {
                i = (i + 1) % dimension;
                j++;
            }
        }
        return i;
    }
    string lookup(string key)
    {
        int i = find_slot(key);

        if (pr[i] != NULL && pr[i]->getPersName() == key)
            return pr[i]->getTelNo();

        return "no data found";
    }
    void set(string key, string value)
    {
        int i = find_slot(key);

        if (pr[i] != NULL)
            if (pr[i]->getPersName() == key)
                pr[i]->setRecord(key, value); // change the value of the occupied key
            else
                cout << "collision"; // if  the key is different from another, then "collision".

        else
            pr[i] = new PhoneRecord(key, value); // null, alocate memory for the unoccupied slot
    }
    void remove(string key)
    {
        int i = find_slot(key);
        if (pr[i] != NULL && pr[i]->getPersName() == key)
        {
            pr[i] = NULL;
            cout << "One record removed:" << key << endl;
        }
        else
            cout << "Nothing to remove" << endl;
    }
};


int main()
{
    PhoneBook pb(3);

    pb.set("Pers 1", "Tel 1");

    pb.set("Pers 2", "Tel 1");

    pb.set("Pers 3", "Tel 3");

    pb.remove("Pers 2");

    pb.set("Pers 4", "Tel 4");

    pb.set("Pers 4", "Tel 2");

    cout << pb.lookup("Pers 2") << endl;

    pb.remove("Pers 1");

    pb.set("Pers 5", "Tel 1");

    pb.remove("Pers 1");

    pb.remove("Pers 3");

    pb.set("Pers 6", "Tel 6");

    pb.set("Pers 7", "Tel 7");

    return 0;
}
