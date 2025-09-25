// Section 9 Challenge

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

int main() {

    char input {};
    vector<int> numbers {}; // Using a vector to store integers

    do
    {
        cout << "P - Print numbers" << endl;
        cout << "A - Add a number" << endl;
        cout << "M - Display mean of the numbers" << endl;
        cout << "S - Display the smallest number" << endl;
        cout << "L - Display the largest number" << endl;
        cout << "Q - Quit" << endl;
        cout << endl << "Enter your choice: " << endl;
        cin >> input;
        cout << endl;

        switch (input)
        {
            case 'P':
            case 'p': // print numbers
            {
                if (numbers.empty())
                {
                    cout << "[] - the list is empty" << endl;
                }
                else
                {
                    cout << "[ ";
                    for (int num : numbers)
                    {
                        cout << num << " ";
                    }
                    cout << "]" << endl;;
                }
                break;
            }
            case 'A':
            case 'a': // add a number
            {
                int number;
                cout << "Enter an integer to add: ";
                if (cin >> number)
                {
                    numbers.push_back(number);
                    cout << number << " added" << endl;
                }
                else
                {
                    cin.clear(); // clear the error state
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
                    cout << "Invalid input. Please enter an integer." << endl;
                }
                break;
            }
            case 'M':
            case 'm': // display mean
            {
                if (numbers.empty())
                {
                    cout << "Unable to calculate the mean - no data" << endl;
                }
                else
                {
                    double sum {0.0};
                    for (int num : numbers)
                    {
                        sum += num;
                    }
                    cout << "The mean is " << (sum / numbers.size()) << endl;
                }
                break;
            }
            case 's':
            case 'S': // display smallest number
            {
                if (numbers.empty())
                {
                    cout << "Unable to determine the smallest number - list is empty" << endl;
                }
                else
                {
                    int smallest = numbers[0];
                    for (int num : numbers)
                    {
                        smallest = (num < smallest) ? num : smallest;
                    }
                    cout << "The smallest number is " << smallest << endl;
                }
                break;
            }
            case 'l':
            case 'L': // display largest number
            {
                if (numbers.empty())
                {
                    cout << "Unable to determine the largest number - list is empty" << endl;
                }
                else
                {
                    int largest = numbers[0];
                    for (int num : numbers)
                    {
                        largest = (num > largest) ? num : largest;
                    }
                    cout << "The largest number is " << largest << endl;
                }
                break;
            }
            case 'q':
            case 'Q': // quit
            {
                cout << "Quitting..." << endl;
                break;
            }
            default: // unknown selection
            {
                cout << "Unknown selection, please try again" << endl;
            }
        }
    } while  (input != 'q' && input != 'Q');

    return 0;
}

