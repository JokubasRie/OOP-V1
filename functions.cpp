#include "functions.h"
#include "Mylib.h"



bool skaitytiDuomenisIsFailo(vector<Student>& studentai, const string& failoPavadinimas) {
    ifstream inputFile(failoPavadinimas);
    if (!inputFile.is_open()) {
        cerr << "Nepavyko atidaryti failo: " << failoPavadinimas << endl;
        return false;
    }

    string headerLine;
    getline(inputFile, headerLine);

    string eilute;
    int lineNumber = 1;
    while (getline(inputFile, eilute)) {
        lineNumber++;
        istringstream iss(eilute);
        Student student;

        if (!(iss >> student.vardas >> student.pavarde)) {
            cerr << "Klaida eilutėje " << lineNumber << ": Nepavyko nuskaityti vardo ir pavardės." << endl;
            continue;
        }

        int temp;
        bool valid = true;

        while (iss >> temp) {
            if (temp >= 0 && temp <= 10) {
                student.nd.push_back(temp);
            } else {
                cerr << "Klaida eilutėje " << lineNumber << ": Neteisingas skaičius ar formatas." << endl;
                valid = false;
                break;
            }
        }

        if (!valid || student.nd.empty()) {
            continue;
        }

        student.egz = student.nd.back();
        student.nd.pop_back();

        student.skaiciuotiGalutiniVid();
        student.skaiciuotiGalutiniMed();

        studentai.push_back(student);
    }

    inputFile.close();
    return true;
}
void skaitytiDuomenisIsVartotojo(vector<Student>& studentai) {
    int studentuKiekis;
    cout << "Keliu studentu duomenis norite ivesti? ";
    cin >> studentuKiekis;

    for (int i = 0; i < studentuKiekis; ++i) {
        Student student;
        string vardas, pavarde;

        cout << "Iveskite " << i + 1 << "-ojo studento varda: ";
        cin >> vardas;
        cout << "Iveskite " << i + 1 << "-ojo studento pavarde: ";
        cin >> pavarde;

        student.vardas = vardas;
        student.pavarde = pavarde;

        char pasirinkimas;
        cout << "Ar norite ivesti namu darbu ir egzamino pazymius patys ar juos sugeneruoti automatiskai? 0 - patys, 1 - sugeneruoti ";
        cin >> pasirinkimas;

        if (pasirinkimas == '1') {
            int kiekNd;
            cout << "Kiek namu darbu pazymiu sugeneruoti? ";
            cin >> kiekNd;
            generuotiAtsitiktiniusBalus(student, kiekNd);
        } else {
            int nd, egzaminas;

            cout << "Iveskite namu darbu pazymius (iveskite -1, jei norite baigti): " << endl;
            while (true) {
                cin >> nd;
                if (nd == -1) break;

                if (cin.fail() || nd < 0 || nd > 10) {
                    cerr << "Ivedamas balas turi buti desimtbaleje sistemoje. Iveskite is naujo: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    student.nd.push_back(nd);
                }
            }

            cout << "Iveskite egzamino rezultata: ";
            while (!(cin >> egzaminas) || egzaminas < 0 || egzaminas > 10) {
                cerr << "Ivedamas balas turi buti desimtbaleje sistemoje. Iveskite is naujo: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            student.egz = egzaminas;
        }

        student.skaiciuotiGalutiniVid();
        student.skaiciuotiGalutiniMed();

        studentai.push_back(student);
    }
}
string pasirinktiFaila() {
    int failoPasirinkimas;
    cout << "Pasirinkite faila, kuri norite nuskaityti:\n";
    cout << "1 - kursiokai.txt\n" << "2 - studentai10000.txt\n" << "3 - studentai100000.txt\n" << "4 - studentai1000000.txt\n";
    cout << "Iveskite pasirinkima (1-4): ";
    cin >> failoPasirinkimas;

    switch (failoPasirinkimas) {
        case 1:
            return "kursiokai.txt";
        case 2:
            return "studentai10000.txt";
        case 3:
            return "studentai100000.txt";
        case 4:
            return "studentai1000000.txt";
        default:
            cerr << "Neteisingas pasirinkimas. Naudojamas kursiokai.txt\n";
            return "kursiokai.txt";
    }
}
int atsitiktiniaiBalai(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}
void generuotiAtsitiktiniusBalus(Student& student, int kiekNd) {
    student.nd.clear();
    for (int i = 0; i < kiekNd; ++i) {
        int atsitiktinisBalas = atsitiktiniaiBalai();
        student.nd.push_back(atsitiktinisBalas);
    }
    student.egz = atsitiktiniaiBalai();
}

void Student::skaiciuotiGalutiniVid() {
    if (nd.size() == 0) {
        galutinisVid = 0;
        return;
    }
    double ndVidurkis = accumulate(nd.begin(), nd.end(), 0.0) / nd.size();
    galutinisVid = 0.4 * ndVidurkis + 0.6 * egz;

}
void Student::skaiciuotiGalutiniMed() {
    if (nd.size() == 0) {
        galutinisMed = 0;
        return;
    }
    vector<int> sorted_nd = nd;
    sort(sorted_nd.begin(), sorted_nd.end());
    if (sorted_nd.size() % 2 == 0)
        galutinisMed = (sorted_nd[sorted_nd.size() / 2 - 1] + sorted_nd[sorted_nd.size() / 2]) / 2.0;
    else
        galutinisMed = sorted_nd[sorted_nd.size() / 2];
    galutinisMed = 0.4 * galutinisMed + 0.6 * egz;
}
void spausdintiStudentusVidurki(const vector<Student>& studentai) {
    cout << setw(15) << left << "Vardas" << setw(15) << left << "Pavarde"
              << setw(20) << left << "Galutinis (Vid.)" << endl;
    cout << "-------------------------------------------" << endl;

    for (const auto& student : studentai) {
        cout << setw(15) << left << student.vardas << setw(15) << left << student.pavarde
                  << setw(20) << left << fixed << setprecision(2) << student.galutinisVid << endl;
    }
}
void spausdintiStudentusMediana(const vector<Student>& studentai) {
    cout << setw(15) << left << "Vardas" << setw(15) << left << "Pavarde"
              << setw(20) << left << "Galutinis (Med.)" << endl;
    cout << "-------------------------------------------" << endl;

    for (const auto& student : studentai) {
        cout << setw(15) << left << student.vardas << setw(15) << left << student.pavarde
                  << setw(20) << left << fixed << setprecision(2) << student.galutinisMed << endl;
    }
}
void spausdintiStudentus(const vector<Student>& studentai) {
    cout << setw(15) << left << "Vardas" << setw(15) << left << "Pavarde"
              << setw(20) << left << "Galutinis (Vid.)" << setw(20) << left << "Galutinis (Med.)" << endl;
    cout << "--------------------------------------------------------------------" << endl;

    for (const auto& student : studentai) {
        cout << setw(15) << left << student.vardas << setw(15) << left << student.pavarde
                  << setw(20) << left << fixed << setprecision(2) << student.galutinisVid
                  << setw(20) << left << fixed << setprecision(2) << student.galutinisMed << endl;
    }
}
bool compareByName(const Student& a, const Student& b) {
    return a.vardas < b.vardas;
}
