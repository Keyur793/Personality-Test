/* Keyur Patel, kpate426, CS251, coded using Xcode
 *
 * Description:
 *
 * This is the header file of the app. It handles all the stuff behind the
 * scenes. It contains 2 structs, one for people andd one for questions.
 * Mainly this file gets a random question, computes the user sccore, normalizes
 * it, finds the cosine similarity, and finds the cloest match.
 */

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}
/* randomQuestionFrom
 *
 * This function calls the randomElement which returns the question. The question is
 * removed from the set and returned. Also throws an error if a question wasn't
 * found.
*/
Question randomQuestionFrom(set<Question>& questions) {
    Question randq = randomElement(questions);
    if (questions.empty()) {
        throw runtime_error("questions: empty");
    }
    questions.erase(randq);
    return randq;
}
/* scoresFrom
 *
 * This function iterates though the answers the user selected and each score and
 * adds the score and letter to a map. The letter is the key and the score is the
 * value. If the key exists then add the score to that key. If not then create a
 * new key with the corresponding score. Then returns the map score
*/
map<char, int> scoresFrom(map<Question, int>& answers) {
     map<char, int> scores;
     for (auto e : answers) {
         for (auto a : e.first.factors) {
             if (scores.count(a.first)) {
                 scores.at(a.first)+=((e.second-3)*(a.second));
             } else {
                 scores.emplace(a.first, ((e.second-3)*(a.second)));
             }
         }
     }
     return {scores};
}
/* normalize
 *
 * This function computes the normalized score by first adding each score to get
 * the total score then, square rooting the score and dividing each OCEAN score
 * by the total which then gets adds to a map, Then returns the normalized map
*/
map<char, double> normalize(map<char, int>& scores) {
     map<char, double> normalizedScore;
     double indiviualScore;
     double total = 0;
     for (auto e : scores) {
         total += pow(e.second, 2);
     }
     if (total == 0) {  // Throw error if all scores are 0
         throw runtime_error("all scores 0");
     }
     total = sqrt(total);
     for (auto e : scores) {
         indiviualScore = e.second/total;
         normalizedScore.emplace(e.first, indiviualScore);
     }
     return {normalizedScore};
}
/* cosineSimilarityOf
 *
 * This function computes the cosine similarity by iterating though the normalized
 * score and adding the product of each score of each letter in OCEAN in person 1
 * and each score of each letter in OCean in person 2. O1O2 + E1E2... Which then
 * returns the cosine similarity
 */
double cosineSimilarityOf(const map<char, double>& lhs,
                           const map<char, double>& rhs) {
     double total = 0;
     for (auto e : lhs) {
         if (rhs.count(e.first)) {
             total+= (rhs.at(e.first) * e.second);
         }
     }
     return total;
}
/* mostSimilarTo
 *
 * This function finds the person with the most similar score by iterating through
 * each Person in people which then calls the cosinesimilary function to compute the
 * the similary between the 2 people. If the score is the highest then store the score
 * and return the stored score after the loop.
 */
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
         map<char, double> normalizedScore = normalize(scores);
     double closestMatch = -1;
     Person name;  // Stores the current Person with the closest match
     double cosSim;
     if (people.empty()) {  // Throw error if no people loaded
         throw runtime_error("No people");
     }
     for (auto e : people) {
         cosSim = cosineSimilarityOf(normalizedScore, normalize(e.scores));
         if (cosSim > closestMatch) {
             closestMatch = cosSim;
             name = e;
         }
     }
     return name;
}
