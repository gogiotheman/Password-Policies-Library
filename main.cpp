#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Policy{
protected:
    bool isChecked;
public:
    virtual void check(const string& password) = 0;
    bool getCheck() const{
        return isChecked;
    };
};

class LengthPolicy: public Policy{
    uint16_t minLength, maxLength;
public:
    LengthPolicy(uint16_t minValue): minLength(minValue), maxLength(255){

    }
    LengthPolicy(uint16_t minValue, uint16_t maxValue): minLength(minValue), maxLength(maxValue){

    }
    void check(const string& password) override{
        if(password.size() >= minLength && password.size() <= maxLength) isChecked = 1;
        else isChecked = 0;
    }
};

class ClassPolicy: public Policy{
    uint16_t minClassCount;
public:
    ClassPolicy(uint16_t minCount): minClassCount(minCount){

    }
    void check(const string& password) override{
        isChecked = 0;
        string copyPassword = password;
        uint16_t k = 0;
        while(!copyPassword.empty()){
            if (copyPassword[0] >= '0' && copyPassword[0] <= '9') {
                k++;
                if(k >= minClassCount){
                    isChecked = 1;
                    break;
                }
                for (uint16_t i = 0; i < copyPassword.size(); i++) {
                    if (copyPassword[i] >= '0' && copyPassword[i] <= '9') {
                        copyPassword.erase(copyPassword.begin() + i);
                        i--;
                    }
                }
            } else if (copyPassword[0] >= 'a' && copyPassword[0] <= 'z') {
                k++;
                if(k >= minClassCount){
                    isChecked = 1;
                    break;
                }
                for (uint16_t i = 0; i < copyPassword.size(); i++) {
                    if (copyPassword[i] >= 'a' && copyPassword[i] <= 'z') {
                        copyPassword.erase(copyPassword.begin() + i);
                        i--;
                    }
                }
            } else if (copyPassword[0] >= 'A' && copyPassword[0] <= 'Z') {
                k++;
                if(k >= minClassCount){
                    isChecked = 1;
                    break;
                }
                for (uint16_t i = 0; i < copyPassword.size(); i++) {
                    if (copyPassword[i] >= 'A' && copyPassword[i] <= 'Z') {
                        copyPassword.erase(copyPassword.begin() + i);
                        i--;
                    }
                }
            } else{
                k++;
                if(k >= minClassCount){
                    isChecked = 1;
                    break;
                }
                for (uint16_t i = 0; i < copyPassword.size(); i++) {
                    if (copyPassword[i] < '0' || (copyPassword[i] > '9' && copyPassword[i] < 'A') ||
                            (copyPassword[i] > 'Z' && copyPassword[i] < 'a') || copyPassword[i] > 'z') {
                        copyPassword.erase(copyPassword.begin() + i);
                        i--;
                    }
                }
            }
        }
    }
};

class IncludePolicy: public Policy{
    char characterType;
public:
    IncludePolicy(char c): characterType(c){

    }
    void check(const string& password) override{
        isChecked = 0;
        if(characterType == 'a'){
            for(uint16_t i = 0; i < password.size(); i++){
                if(password[i] >= 'a' && password[i] <= 'z'){
                    isChecked = 1;
                    break;
                }
            }
        } else if(characterType == 'A'){
            for(uint16_t i = 0; i < password.size(); i++){
                if(password[i] >= 'A' && password[i] <= 'Z'){
                    isChecked = 1;
                    break;
                }
            }
        } else if(characterType == '0'){
            for(uint16_t i = 0; i < password.size(); i++){
                if(password[i] >= '0' && password[i] <= '9'){
                    isChecked = 1;
                    break;
                }
            }
        } else{
            for(uint16_t i = 0; i < password.size(); i++){
                if (password[i] < '0' || (password[i] > '9' && password[i] < 'A') ||
                    (password[i] > 'Z' && password[i] < 'a') || password[i] > 'z'){
                    isChecked = 1;
                    break;
                }
            }
        }
    }
};

class NotIncludePolicy: public Policy{
    char characterType;
public:
    NotIncludePolicy(char c): characterType(c){

    }
    void check(const string& password) override{
        isChecked = 1;
        if(characterType == 'a'){
            for(uint16_t i = 0; i < password.size(); i++){
                if(password[i] >= 'a' && password[i] <= 'z'){
                    isChecked = 0;
                    break;
                }
            }
        } else if(characterType == 'A'){
            for(uint16_t i = 0; i < password.size(); i++){
                if(password[i] >= 'A' && password[i] <= 'Z'){
                    isChecked = 0;
                    break;
                }
            }
        } else if(characterType == '0'){
            for(uint16_t i = 0; i < password.size(); i++){
                if(password[i] >= '0' && password[i] <= '9'){
                    isChecked = 0;
                    break;
                }
            }
        } else{
            for(uint16_t i = 0; i < password.size(); i++){
                if (password[i] < '0' || (password[i] > '9' && password[i] < 'A') ||
                    (password[i] > 'Z' && password[i] < 'a') || password[i] > 'z'){
                    isChecked = 0;
                    break;
                }
            }
        }
    }
};

class RepetitionPolicy: public Policy{
    uint16_t maxCount;
public:
    RepetitionPolicy(uint16_t maxNr): maxCount(maxNr){

    }
    void check(const string& password) override{
        uint16_t k = 1;
        isChecked = 1;
        for(uint16_t i = 1; i < password.size(); i++){
            if(password[i] == password[i - 1]) k++;
            else k = 1;
            if(k > maxCount){
                isChecked = 0;
                break;
            }
        }
    }
};

class ConsecutivePolicy: public Policy{
    uint16_t maxCount;
public:
    ConsecutivePolicy(uint16_t maxNr): maxCount(maxNr){

    }
    void check(const string& password) override{
        uint16_t k = 1;
        isChecked = 1;
        for(uint16_t i = 1; i < password.size(); i++){
            if(password[i] == password[i - 1] + 1) k++;
            else k = 1;
            if(k > maxCount){
                isChecked = 0;
                break;
            }
        }
    }
};

string checkPassword(string password, vector<Policy*> myPolicies){
    for(uint16_t i = 0; i < myPolicies.size(); i++){
        myPolicies[i]->check(password);
        if(myPolicies[i]->getCheck() == 0) return "NOK";
    }
    return "OK";
}

int main(){
    uint16_t n;
    cin >> n;
    string command;
    uint16_t max_length, min_length;
    char c;
    vector<Policy*> myPolicies;
    for(uint16_t i = 0; i < n; i++){
        cin >> command;
        if(command == "length"){
            cin >> min_length;
            scanf("%c", &c);
            if(c == '\n') myPolicies.push_back(new LengthPolicy(min_length));
            else{
                cin >> max_length;
                myPolicies.push_back(new LengthPolicy(min_length, max_length));
            }
        }
        else if(command == "class"){
            cin >> min_length;
            myPolicies.push_back(new ClassPolicy(min_length));
        }
        else if(command == "include"){
            cin >> c;
            myPolicies.push_back(new IncludePolicy(c));
        }
        else if(command == "ninclude"){
            cin >> c;
            myPolicies.push_back(new NotIncludePolicy(c));
        }
        else if(command == "repetition"){
            cin >> max_length;
            myPolicies.push_back(new RepetitionPolicy(max_length));
        }
        else if(command == "consecutive"){
            cin >> max_length;
            myPolicies.push_back(new ConsecutivePolicy(max_length));
        }
    }
    string password;
    while(cin >> password){
        cout << checkPassword(password, myPolicies) << '\n';
    }
};
