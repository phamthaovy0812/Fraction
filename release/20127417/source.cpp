#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<string>
#include<ctime>
#include<regex>
using namespace std;
class Fraction;
class RandomFractionGenerator;
class FractionToStringDataConverter;
class FractionDataReader;
class Integer;
class FractionToLowestTermUIConverter;
class FractionDataWrite;


class Fraction
{
private:
    long long _num;
    long long _den;
public:
    Fraction()
    {
        _num = 0;
        _den = 1;
    }
    long long getNum()
    {
        return this->_num;
    }
    long long getDen()
    {
        return this->_den;
    }
    long long setNum(long long number)
    {
        this->_num = number;
        return this->_num;
    }
    long long setDen(long long number)
    {
        if (number == 0)
        {
            cerr << " den canot equal 0.";
            exit(-1);
        }
        this->_den = number;
        return this->_den;
    }
public:
    Fraction add(Fraction f)
    {
        this->setNum(this->getNum() * f.getDen() + this->getDen() * f.getNum());
        this->setDen(this->getDen() * f.getDen());
        return *this;
    }



public:
    string toStringFraction()
    {
        stringstream out;

        out << this->_num << "/" << this->_den << ", ";

        string result = out.str();
        return result;
    }

    string toStringMixed(int integer)
    {
        if (integer == 0)
        {
            stringstream out;

            if (this->_den == 1)
            {

                out << " " << this->_num << ", ";
                string result = out.str();
                return result;
            }
            out << " " << this->_num << "/" << this->_den << ", ";
            string result = out.str();
            return result;
        }
        else
        {
            stringstream out;
            out << integer << " " << this->_num << "/" << this->_den << ", ";
            string result = out.str();
            return result;
        }
    }
};

class RandomFractionGenerator
{
public:
    RandomFractionGenerator()
    {
        srand(time(nullptr));
    }

    Fraction next()
    {
        Fraction f;

        f.setNum(rand());
        do
        {
            f.setDen(rand());
        } while (f.getDen() == 0);

        return f;
    }

    Fraction next(int ceiling)
    {
        Fraction f;

        f.setNum(rand() % 10);
        do
        {
            f.setDen(rand() % 10);
        } while (f.getDen() == 0);

        return f;
    }

    static Fraction next(int min, int max)
    {
        Fraction f;

        f.setNum(rand() % (max + 1 - min) + min);
        do
        {
            f.setDen(rand() % (max + 1 - min) + min);
        } while (f.getDen() == 0);

        return f;
    }
};
class FractionToStringDataConverter
{
public:
    static bool isValidFormat(string value)
    {
        stringstream line(value);
        string num, den;

        regex match("\\b\\d+\\b"); // kt co phai so nguyen hay khong 

        getline(line, num, '/');

        bool check1 = regex_match(num, match);

        getline(line, den, '\n');

        bool check2 = regex_match(den, match);

        if (den == "")
            return false;
        if (check1 == true && check2 == true)
            return true;
        else return false;
    }
};

class FractionDataReader
{
private:
    string _connectionString;
public:
    FractionDataReader(string connectionString)
    {
        this->_connectionString = connectionString;
    }
public:
    vector<Fraction> GetAll()
    {
        ifstream ifs;
        vector<Fraction> list;

        ifs.open(this->_connectionString);

        if (!ifs)
        {
            cerr << " can't find file.\n";
            exit(-1);
        }

        FractionToStringDataConverter fsc;
        RandomFractionGenerator rfg;
        Fraction f;
        string s, num, den;
        while (!ifs.eof())
        {
          
            getline(ifs, s);
            if (fsc.isValidFormat(s) == true)
            {
                stringstream line(s);

                getline(line, num, '/');
                f.setNum(stol(num));
                getline(line, den, '\n');
                f.setDen(stol(den));

                list.push_back(f);
            }
        }
        return list;
    }
};



class Integer
{
public:
    static int gcd(int a, int b)
    {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }
};

class FractionToLowestTermUIConverter
{
public:
    string convert(const Fraction& F)
    {

        Fraction f = F;
        if (f.getNum() % f.getDen() == 0)
        {
            long long integer = f.getNum() / f.getDen();
            stringstream out;
            out << integer << ", ";
            string result = out.str();
            return result;
        }

        else if (f.getDen() == 1)
        {
            string result = f.toStringMixed(0);
            return result;

        }
        else
        {
            int gcd = Integer::gcd(f.getNum(), f.getDen());
            if (gcd == 1)
            {
                int integer = f.getNum() / f.getDen();
                f.setNum(f.getNum() - integer * f.getDen());
                string result = f.toStringMixed(integer);
                return result;
            }
            else
            {
                f.setNum(f.getNum() / gcd);
                f.setDen(f.getDen() / gcd);
                string result = f.toStringFraction();
                return result;
            }
        }
    }
};


class FractionDataWrite
{
private:
    string _connectionString;
public:
    FractionDataWrite(string connectionString)
    {
        this->_connectionString = connectionString;
    }
    void WriteFile( vector<Fraction> f)
    {
        ofstream ofs;
        ofs.open(this->_connectionString, ios::out);
        int i = 0;
        while (!ofs.eof()&&i<f.size())
        {
            ofs << f[i].getNum() << "/" << f[i].getDen() << "\n";
            i++;
        }
    }


};


int main()
{
    FractionDataReader reader("data.txt");
    RandomFractionGenerator rfg;
    FractionDataWrite write("data.txt");

    vector<Fraction> list = reader.GetAll();

    int n = list.size();

    FractionToLowestTermUIConverter ltc;

    for (int i = 0; i < n; i++)
    {
        Fraction F = list.at(i);
        cout << ltc.convert(F);
    }



    for (int i = 0; i < 2; i++)
    {
        Fraction f = rfg.next(1, 99);
        list.push_back(f);
    }

    n = list.size();

    cout << " \n vector after random two fraction: ";
    for (int i = 0; i < n; i++)
    {
        Fraction temp = list.at(i);

        cout << ltc.convert(temp);
    }

    write.WriteFile(list);
    return 0;

}
