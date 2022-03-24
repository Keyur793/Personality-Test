/* Keyur Patel, kpate426, CS251, coded using Xcode
 *
 * Project Description
 *
 * This program is a simple personality test app that allows the user
 * select a number of question and select a database to compare and find
 * the person that he/she is most similar to. This was done implementing
 * the ADTS like maps and sets. It was also done using a header class that
 * handles all of the stuff behind the scenes. The main class handles the UI
 * of the program
 *
 * Mine.people Description
 *
 * The theme of the mine.people file is the characters from a TV show called the
 * "The Office". In the file are nine people which are most of the important characters
 */

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <iostream>
#include "driver.h"

using namespace std;

/* mulipleFactors
 *
 * This function is a helper function that handles questions with
 * multiple factors. Function substrings each Letter and score, erases
 * it then adds it to the map which repeates until the scores string is
 * empty.
 */

void multipleFactors(string scores, string letter, string number,
                     map<char, int>& factors){
    while (!scores.empty()) {
        if (scores.find(' ') > 10) {
            letter = scores.substr(0, 1);
            number = scores.substr(scores.find(':')+1,
                                   scores.size() - 1);
            scores.erase(0, scores.size());
        } else {
            letter = scores.substr(0, 1);
            number = scores.substr(scores.find(':')+1,
                                   scores.find(' ')-2);
            scores.erase(0, scores.find(' ')+1);
        }
        factors.emplace(letter[0], stoi(number));
    }
}
/* seperateLines
 *
 * This function is a helper function that handles each line
 * of the textfile. If there is question with one letter and
 * factor then add to the map. The map then gets put into the
 * object
 */

void seperateLines(map<char, int>& factors, string scores, bool loadPeople,
                   Question Q, Person P, set<Person>& person,
                   set<Question>& questions){
    string letter;
    string number;
    factors.clear();
    scores = scores.substr(scores.find('.')+2, scores.size() - 1);
    // Holds the scores part of the question
    if (scores.find(' ') > 10) {
        letter = scores.substr(0, 1);
        number = scores.substr(scores.find(':')+1, scores.size() - 1);
        factors.emplace(letter[0], stoi(number));
    } else {
        multipleFactors(scores, letter, number, factors);
        // handles questions with multiple letters and factors
    }
    if (!loadPeople) {  // Only does this if function is called to load questions
        Q.factors = factors;
        questions.insert(Q);
    } else {
        P.scores = factors;
        person.insert(P);
    }
}
/* LoadDatabase
 *
 * Main function to load databases. Functions works with either
 * people or questions text files. First gets the question part
 * of the string and calls helper function to seperate the scores
 */
void LoadDatabase(set<Question>& questions, string fileName,
                  set<Person>& person, bool loadPeople) {
    string question;
    string scores;
    ifstream inFS;
    inFS.open(fileName);
    person.clear();
    questions.clear();
    if (!inFS.is_open()) {
        cout << "ERROR: COULD NOT OPEN FILE\n";
    }

    while (inFS.good()) {
        Question Q;
        Person P;
        map<char, int> factors;
        getline(inFS, question);
        scores = question;
        question = question.substr(0, question.find('.') + 1);
        // stores the question part of the string
        if (!loadPeople) {
            Q.questionText = question;
        } else {
            P.name = question;
        }
        if (!scores.empty()) {
            seperateLines(factors, scores, loadPeople, Q, P, person, questions);
        }
    }
}
/* handleInput
 *
 * This function handles all the people databasae inputs. Sets
 * the file name to what ever the user has selected
 */
void handleInput(int userInput, string& fileName){
    
    if (userInput == 1) {
        fileName = "BabyAnimals.people";
    } else if (userInput == 2) {
        fileName = "Brooklyn99.people";
    } else if (userInput == 3) {
        fileName = "Disney.people";
    } else if (userInput == 4) {
        fileName = "Hogwarts.people";
    } else if (userInput == 5) {
        fileName = "MyersBriggs.people";
    } else if (userInput == 6) {
        fileName = "SesameStreet.people";
    } else if (userInput == 7) {
        fileName = "StarWars.people";
    } else if (userInput == 8) {
        fileName = "Vegetables.people";
    } else if (userInput == 9) {
        fileName = "mine.people";
    }
}
/* pickPeople
 *
 * This function gives you the option to select the database to load
 * and returns the name of the person with similar personlity traits.
 * The function calls the mostsimilarto function in driver to get the
 * name of the person similar to.
 */
void pickPeople(set<Question>& questions, set<Person>& person,
                bool loadPeople, map<char, int>& scores) {
    int userInput;
    string fileName;
    
    while (true) {
        cout << "1. BabyAnimals\n2. Brooklyn99\n3. Disney\n4. Hogwarts\n"
                "5. MyersBriggs\n6. SesameStreet\n7. StarWars\n8. Vegetables\n"
                "9. mine\n0. To end program.\n\n";
        cout << "Choose test number (1-9, or 0 to end): ";
        cin >> userInput;

        if (userInput == 0) {
            cout << "Goodbye!\n";
            return;
        }
        handleInput(userInput, fileName);
        LoadDatabase(questions, fileName, person, true);
        string name = mostSimilarTo(scores, person).name;
        name.pop_back();  // removes the unnecessary period
        cout << "You got " <<  name << "!\n\n";
    }
}
/* main
 *
 * This function mainly handles the questions and calls other functions.
 * First the functon loads question database, then asks for questions,
 * finally calls another function to tell you who you are similar to
 */
int main() {
    int userInput;
    int userAnswer;
    cout << "Welcome to the Personality Quiz!\n\n";
    cout << "Choose number of questions: ";
    cin >> userInput;
    cout << "\n";
    set<Question> questions;
    // stores question
    set<Person> person;
    // stores people
    map<Question, int> answers;
    // stores the question and answer
    map<char, int> scores;
    // stores the letter and score of each letter

    LoadDatabase(questions , "questions.txt", person, false);
    Question Q;
    for (int i=0; i < userInput; i++) {
        Q = randomQuestionFrom(questions);
        // gets random question
        cout << "How much do you agree with this statement?" << endl;
        cout << "\"" << Q.questionText << "\"" << endl;
        cout << "\n1. Strongly disagree\n"
                "2. Disagree\n"
                "3. Neutral\n"
                "4. Agree\n"
                "5. Strongly agree\n\n";
        cout << "Enter your answer here (1-5): ";
        cin >> userAnswer;
        cout << "\n";
        answers.emplace(Q, userAnswer);
    }
    scores = scoresFrom(answers);
    pickPeople(questions , person, false, scores);

    return 0;
}
