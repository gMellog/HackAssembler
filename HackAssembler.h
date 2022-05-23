#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <regex>

struct HackAssembler
{

    HackAssembler();
    void start();

private:

    std::string processACommand() const;

    std::map<std::string, std::string> getMappedCommandsA0() const;
    std::map<std::string, std::string> getMappedCommandsA1() const;
    std::map<std::string, std::string> getMappedDests() const;
    std::map<std::string, std::string> getMappedJumps() const;
    
    std::string getDest() const;
    std::string getComp() const;
    std::string getJump() const;

    std::string parseCommandPart(const std::map<std::string, std::string>& from, const std::string& part);
    std::string parseComp(const std::string& comp);
    std::string processCCommand();
    std::string getByteCode();

    std::map<std::string, int> initializeSymbolsMap();

    std::regex getLabelCheckRegex() const;
    std::regex getACommandCheckRegex() const;
    
    void replaceSymbols();
    void replaceVariables();

    bool isACommand() noexcept;

    std::vector<std::string> hackAssemblyCode;
    std::map<std::string, std::string> mappedDests;
    std::map<std::string, std::string> mappedCompsA0;
    std::map<std::string, std::string> mappedCompsA1;
    std::map<std::string, std::string> mappedJumps;
    std::size_t currentLine;
};