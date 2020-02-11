#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//void addToText(string line) {
//
//    ofstream f{"../list.txt", ofstream::app};
//    if (!f.is_open()) {
//        cerr << "Unable to open file." << endl;
//        exit(1);
//    } else {
//        f << line << endl;
//    }
//    f.close();
//}

/**
 * Takes the parameters below and adds it to a file as comma separated values (csv format).
 * If no address is found (ie. address is null), values are added to a different file - default is phone.txt.
 *
 * @param name
 * @param address
 * @param city
 * @param phNumber
 */
void addToTextFile(const string& name, const string& address, const string& city, const string& phNumber){

    //////////////////////////////////////////
    /// Edit destination file names here.
    ofstream fA{"../address.txt", ofstream::app};
    ofstream fP{"../phone.txt", ofstream::app};
    //////////////////////////////////////////


    if (!fA.is_open()) {
        cerr << "Unable to open address file." << endl;
        exit(1);
    } else if (!fP.is_open()){
        cerr << "Unable to open phone file." << endl;
        exit(1);
    } else {

        if(address.empty()){
            fP << name << "," << city << ",BC," << phNumber <<  endl;
        } else {
            fA << name << "," << address << "," << city << ",BC," << phNumber << endl;
        }

    }
    fA.close();
    fP.close();
}

/**
 * To find the first number in the line, which is assumed to be the street number.
 * @param s
 * @return
 */
bool is_number(const string& s){
    return !s.empty() && find_if(s.begin(), s.end(),
            [](unsigned char c) {return !isdigit(c);}) == s.end();
}

/**
 * Checks if a string contains one of the following words:
 * Burnaby
 * Coquitlam
 * New (as in New Westminster)
 * North (as in North Vancouver)
 * Port (as in Port Coquitlam or Port Moody)
 * Squamish
 * West (as in West Vancouver)
 * Whistler
 *
 * However, this will process street names with the above words incorrectly.
 * @return
 */
bool is_city(const string& s){
    static const string cities[] = {"Burnaby", "Coquitlam", "New", "North", "Port", "Squamish", "West", "Whistler"};
    for(const string & city : cities){
        if (s.compare(city) == 0)
            return true;
//        if (s.find(city) != string::npos) {
//            return true;
//        }
    }
    return false;
}

bool is_BC(const string& s){
    return(s.find("BC") != string::npos);
}

int main() {

    //////////////////////////////////////////
    /// Start by changing the file name here
    ifstream fin{"../3000.txt"};
    //////////////////////////////////////////


    if (!fin.is_open()) {
        cerr << "Unable to open file." << endl;
        exit(1);
    } else {
        string line;
        while(getline(fin, line)) {
            string name, address, city, phNumber;
            string strN, strA, strC, strP;

            istringstream iss(line);

            // Start of splitting a single line.
            while (true){
                iss >> strN;
                if(is_number(strN)){
                    address = strN;
                    while(true){
                        iss >> strA;
                        if(is_city(strA)){
                            city = strA;
                            while(true){
                                iss >> strC;
                                if(is_BC(strC)){
                                    while(iss >> strP){
                                        if(strP.size() != 3)
                                            phNumber += strP;
                                    }
                                    break;
                                }
                                city += " " + strC;
                            }
                            break;
                        }
                        address += " " + strA;
                    }
                    break;
                }

                // If there is no integer to be found.
                // (It probably means there isn't a valid street address.)
                if(is_city(strN)){
                    city = strN;
                    while(true){
                        iss >> strC;
                        if(is_BC(strC)){
                            while(iss >> strP){
                                if(strP.size() != 3)
                                    phNumber += strP;
                            }
                            break;
                        }
                        city += " " + strC;
                    }
                    break;
                }
                name.empty() ? name = strN : name += " " + strN;
            }

            // Print to console.
            if(!address.size()){
                cout << name << "," << city << ",BC," << phNumber <<  endl;
            } else {
                cout << name << "," << address << "," << city << ",BC," << phNumber <<  endl;
            }

            // Write to file.
            addToTextFile(name, address, city, phNumber);

        }
        fin.close();
    }

    return 0;
}