#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>

#include <sstream>
#include <typeinfo>

#include <vector>
#include <utility>
#include <functional>
#include <algorithm>
#include <bitset>
#include <time.h>

#define SAMPLEDSIZE 5000
using namespace std;

//
void print_csv(vector<struct CSVDATA> csvData);
pair<vector<CSVDATA>, vector<CSVDATA> > read_csv(string filepath, vector<struct CSVDATA> csvData, vector<struct CSVDATA> csvData_hash);
vector<CSVDATA> sampledTable(vector<struct CSVDATA> csvData);
void addToSample(int i, vector<struct CSVDATA> csvData, struct EXTRACTED_DATA csvdata_sampledValues, struct CSVDATA csvdata_sampled, vector<struct CSVDATA> csvData_sampled);
void print_sampledTable(vector<struct CSVDATA> csvData_sampled);
EXTRACTED_DATA invertedIndex(vector<struct CSVDATA> csvData_sampled, vector<struct CSVDATA> csvData_compare, EXTRACTED_DATA NOT_IND_full_hash_compare);
EXTRACTED_DATA NOT_IND(EXTRACTED_DATA IND_full_hash, vector<CSVDATA> csvData, EXTRACTED_DATA NOT_IND_full_hash);
void print_IND(EXTRACTED_DATA IND);
//

struct CSVDATA
{
    string Name;
    string Country;
    string Gender;
};

struct EXTRACTED_DATA
{
    vector<string> Names;
    vector<string> Countries;
    vector<string> Genders;
};

vector<CSVDATA> csvData_original, csvData_compare;
vector<CSVDATA> csvData_original_hash, csvData_compare_hash;
vector<struct CSVDATA> csvData_sampled;
vector<struct CSVDATA> csvData_sampled_hash;
EXTRACTED_DATA IND_full;
EXTRACTED_DATA IND_full_hash;
EXTRACTED_DATA NOT_IND_full_compare;
EXTRACTED_DATA NOT_IND_full_original;
EXTRACTED_DATA NOT_IND_full_hash_compare;
EXTRACTED_DATA NOT_IND_full_hash_original;
EXTRACTED_DATA IND_full_HLL;
EXTRACTED_DATA IND_result;

/*读取csv，原值和hash存入不同容器*/
/*original 原；original hash；compare 原；compare hash；*/
pair<vector<CSVDATA>, vector<CSVDATA> > read_csv(string filepath,
                                                 vector<CSVDATA> csvData,
                                                 vector<CSVDATA> csvData_hash)
{
    pair<vector<CSVDATA>, vector<CSVDATA> > p1;
    hash<string> hash_obj;
    ifstream inFile(filepath, ios::in);
    string lineStr;
    getline(inFile, lineStr);
    while (getline(inFile, lineStr))
    {
        stringstream ss(lineStr);
        string str;
        CSVDATA csvdata;
        CSVDATA csvdata_hash;
        getline(ss, str, ',');
        csvdata.Name = str;
        stringstream name;
        name << hash_obj(str);
        // cout << typeid(hash_obj(str)).name() << endl;
        csvdata_hash.Name = name.str();
        getline(ss, str, ',');
        csvdata.Country = str;
        stringstream country;
        country << hash_obj(str);
        csvdata_hash.Country = country.str();
        getline(ss, str, ',');
        csvdata.Gender = str;
        stringstream gender;
        gender << hash_obj(str);
        csvdata_hash.Gender = gender.str();
        csvData.push_back(csvdata);
        csvData_hash.push_back(csvdata_hash);
    }
    return make_pair(csvData, csvData_hash);
}

/*打印 read_csv()*/
void print_csv(vector<struct CSVDATA> csvData)
{
    for (int i = 0; i < csvData.size(); i++)
    {
        cout << csvData[i].Name << "," << csvData[i].Country << "," << csvData[i].Gender << endl;
    }
    cout << endl;
}

/*得到original的sampledtable*/
vector<CSVDATA> sampledTable(vector<struct CSVDATA> csvData)
{
    struct CSVDATA csvdata_sampled;
    EXTRACTED_DATA csvdata_sampledValues;

    for (int i = 0; i < csvData.size(); i++)
    {
        // vector<string>::iterator it;
        if ((csvdata_sampledValues.Names.size() < SAMPLEDSIZE) && (find(csvdata_sampledValues.Names.begin(), csvdata_sampledValues.Names.end(), csvData[i].Name) == csvdata_sampledValues.Names.end()))
        {
            // addToSample(i, csvData, csvdata_sampledValues, csvdata_sampled, csvData_sampled);
            if (find(csvdata_sampledValues.Names.begin(), csvdata_sampledValues.Names.end(), csvData[i].Name) == csvdata_sampledValues.Names.end())
            {
                csvdata_sampledValues.Names.push_back(csvData[i].Name);
            }
            if (find(csvdata_sampledValues.Countries.begin(), csvdata_sampledValues.Countries.end(), csvData[i].Country) == csvdata_sampledValues.Countries.end())
            {
                csvdata_sampledValues.Countries.push_back(csvData[i].Country);
            }
            if (find(csvdata_sampledValues.Genders.begin(), csvdata_sampledValues.Genders.end(), csvData[i].Gender) == csvdata_sampledValues.Genders.end())
            {
                csvdata_sampledValues.Genders.push_back(csvData[i].Gender);
            }
            // cout << typeid(csvdata_sampledValues.Name).name() << endl;
            csvdata_sampled.Name = csvData[i].Name;
            csvdata_sampled.Country = csvData[i].Country;
            csvdata_sampled.Gender = csvData[i].Gender;
            csvData_sampled.push_back(csvdata_sampled);
        }
        else if ((csvdata_sampledValues.Countries.size() < SAMPLEDSIZE) && (find(csvdata_sampledValues.Countries.begin(), csvdata_sampledValues.Countries.end(), csvData[i].Country) == csvdata_sampledValues.Countries.end()))
        {
            // addToSample(i, csvData, csvdata_sampledValues, csvdata_sampled, csvData_sampled);
            if (find(csvdata_sampledValues.Names.begin(), csvdata_sampledValues.Names.end(), csvData[i].Name) == csvdata_sampledValues.Names.end())
            {
                csvdata_sampledValues.Names.push_back(csvData[i].Name);
            }
            if (find(csvdata_sampledValues.Countries.begin(), csvdata_sampledValues.Countries.end(), csvData[i].Country) == csvdata_sampledValues.Countries.end())
            {
                csvdata_sampledValues.Countries.push_back(csvData[i].Country);
            }
            if (find(csvdata_sampledValues.Genders.begin(), csvdata_sampledValues.Genders.end(), csvData[i].Gender) == csvdata_sampledValues.Genders.end())
            {
                csvdata_sampledValues.Genders.push_back(csvData[i].Gender);
            }
            // cout << typeid(csvdata_sampledValues.Name).name() << endl;
            csvdata_sampled.Name = csvData[i].Name;
            csvdata_sampled.Country = csvData[i].Country;
            csvdata_sampled.Gender = csvData[i].Gender;
            csvData_sampled.push_back(csvdata_sampled);
        }
        else if ((csvdata_sampledValues.Genders.size() < SAMPLEDSIZE) && (find(csvdata_sampledValues.Genders.begin(), csvdata_sampledValues.Genders.end(), csvData[i].Gender) == csvdata_sampledValues.Genders.end()))
        {
            // addToSample(i, csvData, csvdata_sampledValues, csvdata_sampled, csvData_sampled);
            if (find(csvdata_sampledValues.Names.begin(), csvdata_sampledValues.Names.end(), csvData[i].Name) == csvdata_sampledValues.Names.end())
            {
                csvdata_sampledValues.Names.push_back(csvData[i].Name);
            }
            if (find(csvdata_sampledValues.Countries.begin(), csvdata_sampledValues.Countries.end(), csvData[i].Country) == csvdata_sampledValues.Countries.end())
            {
                csvdata_sampledValues.Countries.push_back(csvData[i].Country);
            }
            if (find(csvdata_sampledValues.Genders.begin(), csvdata_sampledValues.Genders.end(), csvData[i].Gender) == csvdata_sampledValues.Genders.end())
            {
                csvdata_sampledValues.Genders.push_back(csvData[i].Gender);
            }
            // cout << typeid(csvdata_sampledValues.Name).name() << endl;
            csvdata_sampled.Name = csvData[i].Name;
            csvdata_sampled.Country = csvData[i].Country;
            csvdata_sampled.Gender = csvData[i].Gender;
            csvData_sampled.push_back(csvdata_sampled);
        }
    }
    return csvData_sampled;
}

/*打印 sampledTable()*/
void print_sampledTable(vector<struct CSVDATA> csvData_sampled)
{
    for (int i = 0; i < csvData_sampled.size(); i++)
    {
        cout << csvData_sampled[i].Name << "," << csvData_sampled[i].Country << "," << csvData_sampled[i].Gender << endl;
    }
    cout << endl;
}

/*sampeldTable和compare的IND (full tuple)*/
EXTRACTED_DATA invertedIndex(vector<struct CSVDATA> csvData_sampled, vector<struct CSVDATA> csvData_compare, EXTRACTED_DATA NOT_IND_full_hash_compare)
{
    for (int i = 0; i < csvData_compare.size(); i++)
    {
        for (int j = 0; j < csvData_sampled.size(); j++)
        {
            if ((csvData_compare[i].Name == csvData_sampled[j].Name) && (csvData_compare[i].Country == csvData_sampled[j].Country) && (csvData_compare[i].Gender == csvData_sampled[j].Gender))
            {
                IND_full_hash.Names.push_back(csvData_compare[i].Name);
                IND_full_hash.Countries.push_back(csvData_compare[i].Country);
                IND_full_hash.Genders.push_back(csvData_compare[i].Gender);
            }
        }
    }
    return IND_full_hash;
}

/*invertedIndex后不是IND的tuple original compare*/
EXTRACTED_DATA NOT_IND(EXTRACTED_DATA IND_full_hash, vector<CSVDATA> csvData, EXTRACTED_DATA NOT_IND_full_hash)
{

    for (int i = 0; i < csvData.size(); i++)
    {
        for (int j = 0; j < IND_full_hash.Names.size(); j++)
        {
            if ((csvData[i].Name == IND_full_hash.Names[j]) && (csvData[i].Country == IND_full_hash.Countries[j]) && (csvData[i].Gender == IND_full_hash.Genders[j]))
            {
                csvData.erase(csvData.begin() + i);
            }
        }
    }

    for (int i = 0; i < csvData.size(); i++)
    {
        NOT_IND_full_hash.Names.push_back(csvData[i].Name);
        NOT_IND_full_hash.Countries.push_back(csvData[i].Country);
        NOT_IND_full_hash.Genders.push_back(csvData[i].Gender);
    }

    return NOT_IND_full_hash;
}

/*打印 invertedIndex()*/
void print_IND(EXTRACTED_DATA IND)
{
    for (int i = 0; i < IND.Names.size(); i++)
    {
        cout << IND.Names[i] << "," << IND.Countries[i] << "," << IND.Genders[i] << endl;
    }
    cout << endl;
}

string StrToBitStr(string str)
{
    str = str.substr(0, 19);
    // cout << str << endl;
    long binary;
    binary = atol(str.c_str());

    // cout << binary << endl;

    string r;
    while (binary != 0)
    {
        r += (binary % 2 == 0 ? "0" : "1");
        binary /= 2;
    }
    reverse(begin(r), end(r));
    // cout << r << endl;
    string s = r.substr(0, 4);
    // cout << s << endl;
    int count = 0;
    for (int i = 1; i < 4; i++)
    {
        if (s[i] == '0')
        {
            count++;
        }
        else
        {
            break;
        }
    }
    // cout << count << endl;
    string hll = s.substr(0, 1) + to_string(count);
    // cout << hll << endl;
    // cout << endl;
    return hll;
}

EXTRACTED_DATA HLL(EXTRACTED_DATA NOT_IND_full_hash_compare, EXTRACTED_DATA NOT_IND_full_hash_original, EXTRACTED_DATA NOT_IND_full_compare, EXTRACTED_DATA NOT_IND_full_original)
{
    EXTRACTED_DATA NOT_IND_full_hash_compare_HLL;
    EXTRACTED_DATA NOT_IND_full_hash_original_HLL;
    for (int i = 0; i < NOT_IND_full_hash_compare.Names.size(); i++)
    {
        NOT_IND_full_hash_compare_HLL.Names.push_back(StrToBitStr(NOT_IND_full_hash_compare.Names[i]));
        NOT_IND_full_hash_compare_HLL.Countries.push_back(StrToBitStr(NOT_IND_full_hash_compare.Countries[i]));
        NOT_IND_full_hash_compare_HLL.Genders.push_back(StrToBitStr(NOT_IND_full_hash_compare.Genders[i]));
    }
    for (int i = 0; i < NOT_IND_full_hash_original.Names.size(); i++)
    {
        NOT_IND_full_hash_original_HLL.Names.push_back(StrToBitStr(NOT_IND_full_hash_original.Names[i]));
        NOT_IND_full_hash_original_HLL.Countries.push_back(StrToBitStr(NOT_IND_full_hash_original.Countries[i]));
        NOT_IND_full_hash_original_HLL.Genders.push_back(StrToBitStr(NOT_IND_full_hash_original.Genders[i]));
    }
    // for (int i = 0; i < NOT_IND_full_hash_compare_HLL.Names.size(); i++)
    // {
    //     cout << NOT_IND_full_hash_compare_HLL.Names[i] << "," << NOT_IND_full_hash_compare_HLL.Countries[i] << "," << NOT_IND_full_hash_compare_HLL.Genders[i] << endl;
    // }
    // cout << endl;
    // for (int i = 0; i < NOT_IND_full_hash_original_HLL.Names.size(); i++)
    // {
    //     cout << NOT_IND_full_hash_original_HLL.Names[i] << "," << NOT_IND_full_hash_original_HLL.Countries[i] << "," << NOT_IND_full_hash_original_HLL.Genders[i] << endl;
    // }
    vector<int> IND_record_num;
    for (int i = 0; i < NOT_IND_full_hash_compare_HLL.Names.size(); i++)
    {
        for (int j = 0; j < NOT_IND_full_hash_original_HLL.Names.size(); j++)
        {
            if ((NOT_IND_full_hash_compare_HLL.Names[i] == NOT_IND_full_hash_original_HLL.Names[j]) && (NOT_IND_full_hash_compare_HLL.Countries[i] == NOT_IND_full_hash_original_HLL.Countries[j]) && (NOT_IND_full_hash_compare_HLL.Genders[i] == NOT_IND_full_hash_original_HLL.Genders[j]))
            {
                if (find(IND_record_num.begin(), IND_record_num.end(), i) == IND_record_num.end())
                {
                    IND_record_num.push_back(i);
                }
                // if ((find(IND_full_HLL.Names.begin(), IND_full_HLL.Names.end(), NOT_IND_full_compare.Names[i]) == IND_full_HLL.Names.end()) || (find(IND_full_HLL.Countries.begin(), IND_full_HLL.Countries.end(), NOT_IND_full_compare.Countries[i]) == IND_full_HLL.Countries.end()) || (find(IND_full_HLL.Genders.begin(), IND_full_HLL.Genders.end(), NOT_IND_full_compare.Genders[i]) == IND_full_HLL.Genders.end()))
                // {
                // IND_full_HLL.Names.push_back(NOT_IND_full_compare.Names[i]);
                // IND_full_HLL.Countries.push_back(NOT_IND_full_compare.Countries[i]);
                // IND_full_HLL.Genders.push_back(NOT_IND_full_compare.Genders[i]);
                // }
            }
        }
    }
    // sort(IND_record_num.begin(), IND_record_num.end());
    // IND_record_num.erase(unique(IND_record_num.begin(), IND_record_num.end()), IND_record_num.end());

    for (int i = 0; i < IND_record_num.size(); i++)
    {
        IND_full_HLL.Names.push_back(NOT_IND_full_compare.Names[IND_record_num[i]]);
        IND_full_HLL.Countries.push_back(NOT_IND_full_compare.Countries[IND_record_num[i]]);
        IND_full_HLL.Genders.push_back(NOT_IND_full_compare.Genders[IND_record_num[i]]);
    }
    // for (int i = 0; i < IND_full_HLL.Names.size(); i++)
    // {
    //     cout << IND_full_HLL.Names[i] << "," << IND_full_HLL.Countries[i] << "," << IND_full_HLL.Genders[i] << endl;
    // }
    return IND_full_HLL;
}

/*将hll得到的ind加到总ind中*/
EXTRACTED_DATA IND(EXTRACTED_DATA IND_full, EXTRACTED_DATA IND_full_HLL)
{
    for (int i = 0; i < IND_full_HLL.Names.size(); i++)
    {
        IND_full.Names.push_back(IND_full_HLL.Names[i]);
        IND_full.Countries.push_back(IND_full_HLL.Countries[i]);
        IND_full.Genders.push_back(IND_full_HLL.Genders[i]);
    }
    return IND_full;
}

int main(void)
{
    clock_t begin = clock();

    string filepath1 = "csv/case1.csv";
    string filepath2 = "csv/case2.csv";

    csvData_original = read_csv(filepath1, csvData_original, csvData_original_hash).first;
    csvData_original_hash = read_csv(filepath1, csvData_original, csvData_original_hash).second;
    csvData_compare = read_csv(filepath2, csvData_compare, csvData_compare_hash).first;
    csvData_compare_hash = read_csv(filepath2, csvData_compare, csvData_compare_hash).second;

    csvData_sampled = sampledTable(csvData_original);
    csvData_sampled_hash = sampledTable(csvData_original_hash);

    // print_csv(csvData_compare);
    // print_csv(csvData_compare_hash);
    // print_sampledTable(csvData_sampled);
    // print_sampledTable(csvData_sampled_hash);p

    IND_full = invertedIndex(csvData_sampled, csvData_compare, NOT_IND_full_hash_compare);
    IND_full_hash = invertedIndex(csvData_sampled_hash, csvData_compare_hash, NOT_IND_full_hash_compare);

    // print_IND(IND_full_hash);
    // print_IND(IND_full);

    NOT_IND_full_compare = NOT_IND(IND_full, csvData_compare, NOT_IND_full_compare);
    NOT_IND_full_hash_compare = NOT_IND(IND_full_hash, csvData_compare_hash, NOT_IND_full_hash_compare);

    NOT_IND_full_original = NOT_IND(IND_full, csvData_original, NOT_IND_full_original);
    NOT_IND_full_hash_original = NOT_IND(IND_full_hash, csvData_original_hash, NOT_IND_full_hash_original);
    //    print_IND(NOT_IND_full_hash_original);
    //  print_IND(NOT_IND_full_original);
    //    print_IND(NOT_IND_full_hash_compare);
    // print_IND(NOT_IND_full_compare);

    IND_full_HLL = HLL(NOT_IND_full_hash_compare, NOT_IND_full_hash_original, NOT_IND_full_compare, NOT_IND_full_original);
    // print_IND(IND_full_HLL);
    IND_result = IND(IND_full, IND_full_HLL);
    print_IND(IND_result);
    // cout << IND_result.Names.size() << endl;

    clock_t end = clock();
    cout << "Running time: " << (double)(end - begin) / CLOCKS_PER_SEC * 1000 << "ms" << endl;

    ofstream outFile;
    outFile.open("data333.csv", ios::out); // 打开文件
    for (int i = 0; i < IND_result.Names.size(); i++)
    {
        outFile << IND_result.Names[i] << IND_result.Countries[i] << IND_result.Genders[i] << endl;
    }
    outFile.close();

    return 0;
}

/*
此版完全按照paper
*/
