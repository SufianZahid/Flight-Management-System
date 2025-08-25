#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#define RESET "\033[0m"
#define RED "\033[31m"
#define BLUE "\033[34m"

using namespace std;

struct Flight
{
    string flightID, destination, departureTime, arrivalTime, departureVenue;
    int totalSeats, availableSeats, firstClassSeats;
};

struct Passenger
{
    string firstName, lastName, cnic, dob, mobile, email, gender;
    int age, seatClass;
};

struct Booking
{
    string flightID;
    Passenger passenger;
};

vector<Flight> flights;
vector<Booking> bookings;

bool isValidFormat(string input, string pattern)
{
    if (input.length() != pattern.length())
        return false;
    for (size_t i = 0; i < input.length(); i++)
    {
        if (pattern[i] == 'X' && !isdigit(input[i]))
            return false;
        if (pattern[i] == '-' && input[i] != '-')
            return false;
        if (pattern[i] == '/' && input[i] != '/')
            return false;
    }
    return true;
}

bool adminLogin(string username, string password)
{
    return username == "admin" && password == "password";
}

void addFlight()
{
    Flight flight;
    cout << "Enter Flight ID: ";
    cin >> flight.flightID;
    cout << "Enter Destination: ";
    cin >> flight.destination;
    cout << "Enter Departure Time: ";
    cin >> flight.departureTime;
    cout << "Enter Arrival Time: ";
    cin >> flight.arrivalTime;
    cout << "Enter Departure Venue: ";
    cin >> flight.departureVenue;
    cout << "Enter Total Seats: ";
    cin >> flight.totalSeats;

    flight.firstClassSeats = max(1, flight.totalSeats / 10);
    flight.availableSeats = flight.totalSeats;
    flights.push_back(flight);

    cout << "Flight added successfully!\n";
}

void deleteFlight()
{
    string flightID;
    cout << "Enter Flight ID to delete: ";
    cin >> flightID;

    bool found = false;
    for (size_t i = 0; i < flights.size(); i++)
    {
        if (flights[i].flightID == flightID)
        {
            flights.erase(flights.begin() + i);
            found = true;
            cout << "Flight deleted successfully!\n";
            break;
        }
    }
    if (!found)
    {
        cout << "Flight not found!\n";
    }
}

void listFlights()
{
    if (flights.empty())
    {
        cout << "No flights available.\n";
        return;
    }

    cout << RED << left << setw(15) << "Flight ID" << setw(20) << "Departure Venue" << setw(20) << "Destination"
         << setw(20) << "Departure Time" << setw(20) << "Arrival Time" << setw(10) << "Seats"
         << setw(20) << "Economy Available" << "First Class Available\n";

    cout << string(150, '=') << RESET << endl;

    for (Flight flight : flights)
    {

        cout << left << setw(15) << flight.flightID << setw(20) << flight.departureVenue
             << setw(20) << flight.destination << setw(20) << flight.departureTime
             << setw(20) << flight.arrivalTime << setw(10) << flight.totalSeats
             << setw(20) << flight.availableSeats - flight.firstClassSeats
             << flight.firstClassSeats << endl;
    }
}

void bookFlight()
{
    string flightID;
    Passenger passenger;

    cout << "Enter Flight ID: ";
    cin >> flightID;

    cout << "Enter First Name: ";
    cin >> passenger.firstName;
    cout << "Enter Last Name: ";
    cin >> passenger.lastName;
    cout << "Enter Age: ";
    cin >> passenger.age;
    cout << "Enter Gender: ";
    cin >> passenger.gender;

    cout << "Enter CNIC (XXXXX-XXXXXXX-X): ";
    cin >> passenger.cnic;
    if (!isValidFormat(passenger.cnic, "XXXXX-XXXXXXX-X"))
    {
        cout << "Invalid CNIC format!\n";
        return;
    }

    cout << "Enter DOB (dd/mm/yyyy): ";
    cin >> passenger.dob;
    if (!isValidFormat(passenger.dob, "XX/XX/XXXX"))
    {
        cout << "Invalid DOB format!\n";
        return;
    }

    cout << "Enter Mobile (XXXX-XXXXXXX): ";
    cin >> passenger.mobile;
    if (!isValidFormat(passenger.mobile, "XXXX-XXXXXXX"))
    {
        cout << "Invalid mobile number format!\n";
        return;
    }

    cout << "Enter Seat Class (1 for Economy, 2 for First Class): ";
    cin >> passenger.seatClass;

    for (Flight &flight : flights)
    {
        if (flight.flightID == flightID)
        {

            if (passenger.seatClass == 1 && flight.availableSeats > flight.firstClassSeats)
            {
                flight.availableSeats--;
                bookings.push_back({flightID, passenger});
                cout << "Economy class booking successful!\n";
            }
            else if (passenger.seatClass == 2 && flight.firstClassSeats > 0)
            {
                flight.firstClassSeats--;
                flight.availableSeats--;
                bookings.push_back({flightID, passenger});
                cout << "First class booking successful!\n";
            }
            else
            {
                cout << "No available seats in the selected class.\n";
            }
            return;
        }
    }
    cout << "Flight not found!\n";
}

void deleteBooking()
{
    string cnic;
    cout << "Enter CNIC of the booking to delete: ";
    cin >> cnic;

    bool found = false;
    for (size_t i = 0; i < bookings.size(); i++)
    {
        if (bookings[i].passenger.cnic == cnic)
        {
            string flightID = bookings[i].flightID;
            int seatClass = bookings[i].passenger.seatClass;

            for (Flight &flight : flights)
            {
                if (flight.flightID == flightID)
                {
                    if (seatClass == 1)
                    {
                        flight.availableSeats++;
                    }
                    else if (seatClass == 2)
                    {
                        flight.firstClassSeats++;
                        flight.availableSeats++;
                    }
                    break;
                }
            }

            bookings.erase(bookings.begin() + i);
            found = true;
            cout << "Booking deleted successfully! Seat restored.\n";
            break;
        }
    }

    if (!found)
    {
        cout << "Booking not found!\n";
    }
}

void viewBookings()
{
    if (bookings.empty())
    {
        cout << "No bookings available.\n";
        return;
    }

    cout << BLUE << left << setw(12) << "Flight ID"
         << setw(15) << "First Name"
         << setw(15) << "Last Name"
         << setw(20) << "CNIC"
         << setw(10) << "Age"
         << setw(12) << "DOB"
         << setw(15) << "Mobile"
         << setw(10) << "Gender"
         << setw(15) << "Seat Class" << endl;

    cout << string(130, '=') << RESET << endl;

    for (Booking booking : bookings)
    {
        Passenger p = booking.passenger;

        cout << left << setw(12) << booking.flightID
             << setw(15) << p.firstName
             << setw(15) << p.lastName
             << setw(20) << p.cnic
             << setw(10) << p.age
             << setw(12) << p.dob
             << setw(15) << p.mobile
             << setw(10) << p.gender
             << setw(15) << (p.seatClass == 1 ? "Economy" : "First Class") << endl;
    }
}

void saveBookingsToFile()
{
    ofstream file("bookings.txt");
    for (Booking booking : bookings)
    {
        Passenger p = booking.passenger;
        file << booking.flightID << ", " << p.firstName << ", " << p.lastName << ", " << p.cnic << ", " << p.age << ", "
             << p.dob << ", " << p.mobile << ", " << p.gender << ", " << p.seatClass << "\n";
    }
    file.close();
}

void adminMenu()
{
    string username, password;
    cout << "Enter Admin Username: ";
    cin >> username;
    cout << "Enter Admin Password: ";
    cin >> password;

    if (!adminLogin(username, password))
    {
        cout << "Invalid credentials!\n";
        return;
    }

    int choice;
    do
    {
        cout << "\nAdmin Menu\n1. Add Flight\n2. Delete Flight\n3. List Flights\n4. View Bookings\n5. Logout\nChoice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            addFlight();
            break;
        case 2:
            deleteFlight();
            break;
        case 3:
            listFlights();
            break;
        case 4:
            viewBookings();
            break;
        case 5:
            cout << "Logging out...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);
}

void userMenu()
{
    int choice;
    do
    {
        cout << "\nUser Menu\n1. View Flights\n2. Book Flight\n3. Delete Booking\n4. Back to Main Menu\nChoice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            listFlights();
            break;
        case 2:
            bookFlight();
            break;
        case 3:
            deleteBooking();
            break;
        case 4:
            cout << "Returning to main menu...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);
}

int main()
{

    int choice;
    do
    {
        cout << "\nAirline Management System\n1. Admin Side\n2. User Side\n3. Exit\nChoice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            adminMenu();
            break;
        case 2:
            userMenu();
            break;
        case 3:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 3);

    saveBookingsToFile();
    return 0;
}
