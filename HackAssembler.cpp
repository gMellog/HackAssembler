

#include "HackAssembler.h"


HackAssembler::HackAssembler()
:
    hackAssemblyCode{},
    mappedDests{ getMappedDests() },
    mappedCompsA0{ getMappedCommandsA0() },
    mappedCompsA1{ getMappedCommandsA1() },
    mappedJumps{ getMappedJumps() },
    currentLine{}
{

}

void HackAssembler::start()
{
    hackAssemblyCode.reserve(1000);
    std::string line;

    while (std::getline(std::cin, line))
    {
        const auto commentIndex = line.find("//");
        line = line.substr(0, commentIndex);

        auto it = std::remove(std::begin(line), std::end(line), ' ');
        line.erase(it, std::end(line));

        if (!line.empty())
            hackAssemblyCode.push_back(line);
    }

    replaceSymbols();
    replaceVariables();

    for (; currentLine < hackAssemblyCode.size(); ++currentLine)
    {
        const auto byteCode = getByteCode();
        std::cout << byteCode << '\n';
    }

}
    
std::string HackAssembler::processACommand() const
{
    std::string r;
    r.reserve(16);

    auto value = std::stoi(hackAssemblyCode[currentLine].substr(1));

    for (int i = 0; i < 15; ++i)
    {
        if (value != 0)
        {
            const auto reminder = value % 2;
            r = std::to_string(reminder) + r;
            value /= 2;
        }
        else
        {
            r = "0" + r;
        }
    }

    r = "0" + r;

    return r;
}


std::map<std::string, std::string> HackAssembler::getMappedCommandsA0() const
{
    std::map<std::string, std::string> r;

    r.insert({ "0","101010" });
    r.insert({ "1","111111" });
    r.insert({ "-1","111010" });
    r.insert({ "D","001100" });
    r.insert({ "A","110000" });
    r.insert({ "!D","001101" });
    r.insert({ "!A","110001" });

    r.insert({ "-D","001111" });
    r.insert({ "-A","110011" });
    r.insert({ "D+1","011111" });
    r.insert({ "A+1","110111" });
    r.insert({ "D-1","001110" });
    r.insert({ "A-1","110010" });
    r.insert({ "D+A","000010" });

    r.insert({ "D-A","010011" });
    r.insert({ "A-D","000111" });
    r.insert({ "D&A","000000" });
    r.insert({ "D|A","010101" });

return r;
}

std::map<std::string, std::string> HackAssembler::getMappedCommandsA1() const
{
    std::map<std::string, std::string> r;

    r.insert({ "M","110000" });
    r.insert({ "!M","110001" });
    r.insert({ "-M","110011" });

    r.insert({ "M+1","110111" });
    r.insert({ "M-1","110010" });
    r.insert({ "D+M","000010" });
    r.insert({ "D-M","010011" });
    r.insert({ "M-D","000111" });
    r.insert({ "D&M","000000" });
    r.insert({ "D|M","010101" });

    return r;
}

std::map<std::string, std::string> HackAssembler::getMappedDests() const
{
    std::map<std::string, std::string> r;

    r.insert({ "null", "000" });
    r.insert({ "M", "001" });
    r.insert({ "D", "010" });
    r.insert({ "MD", "011" });
    r.insert({ "A", "100" });
    r.insert({ "AM", "101" });
    r.insert({ "AD", "110" });
    r.insert({ "AMD", "111" });

return r;
}

std::map<std::string, std::string> HackAssembler::getMappedJumps() const
{
    std::map<std::string, std::string> r;

    r.insert({ "null", "000" });
    r.insert({ "JGT", "001" });
    r.insert({ "JEQ", "010" });
    r.insert({ "JGE", "011" });
    r.insert({ "JLT", "100" });
    r.insert({ "JNE", "101" });
    r.insert({ "JLE", "110" });
    r.insert({ "JMP", "111" });

return r;
}

std::string HackAssembler::getDest() const
{
    std::string r;

    const auto& currLine = hackAssemblyCode[currentLine];
    const auto index = currLine.find('=');

    r = currLine.substr(0u, index == std::string::npos ? 0u : index);

    return r == "" ? "null" : r;
}

std::string HackAssembler::getComp() const
{
    const auto& currLine = hackAssemblyCode[currentLine];

    const auto equalityIndex = currLine.find('=');
    const auto from = equalityIndex == std::string::npos ? 0 : equalityIndex + 1;
    const auto endIndex = currLine.find(';');

    std::size_t to;

    if (endIndex == std::string::npos)
    {
        if (equalityIndex == std::string::npos)
        {
            to = 0;
        }
        else
        {
            to = currLine.size();
        }
    }
    else
    {
        to = endIndex;
    }

    return currLine.substr(from, to - from);
}

std::string HackAssembler::getJump() const
{
    std::string r;
    const auto& currLine = hackAssemblyCode[currentLine];

    const auto jumpStartIndex = currLine.find(';');
    const auto from = jumpStartIndex == std::string::npos ? 0u : jumpStartIndex + 1;
    const auto to = jumpStartIndex == std::string::npos ? 0u : currLine.size() - from;
    r = currLine.substr(from, to);

    return r == "" ? "null" : r;
}

std::string HackAssembler::parseCommandPart(const std::map<std::string, std::string>& from, const std::string& part)
{
    auto it = from.find(part);

    const auto output = it == std::end(from) ? "" : it->second;

    return it == std::end(from) ? "" : it->second;
}

std::string HackAssembler::parseComp(const std::string& comp)
{
    std::string r;

    auto itA0 = mappedCompsA0.find(comp);
    if (itA0 == std::end(mappedCompsA0))
    {
        auto itA1 = mappedCompsA1.find(comp);
        if (itA1 != std::end(mappedCompsA1))
        {
            r = "1" + itA1->second;
        }
    }
    else
    {
        r = "0" + itA0->second;
    }


return r;
}

std::string HackAssembler::processCCommand()
{
    std::string r;

    const auto dest = getDest();
    const auto comp = getComp();
    const auto jump = getJump();

if (dest.size() || jump.size())
{
    const auto binaryDest = parseCommandPart(mappedDests, dest);
    const auto binaryComp = parseComp(comp);
    const auto binaryJump = parseCommandPart(mappedJumps, jump);

    const auto a = binaryDest.size();
    const auto b = binaryComp.size();
    const auto c = binaryJump.size();

    const bool validCommand = (a and b) or (not a and c);

    if (validCommand)
    {
        const std::string binaryStartEssential = "111";
        r = binaryStartEssential + binaryComp + binaryDest + binaryJump;
    }
    else
    {
        std::cerr << "line: " << currentLine + 1 << " ";
        std::cerr << "SOMEBODY TRIES WRITE WRONG COMMAND!\n";
        r = "0000000000000000";
    }
}
else
{
    std::cerr << "WRONG COMMAND!\n";
    r = "0000000000000000";
}

return r;
}

std::string HackAssembler::getByteCode()
{
    std::string r;

    if (isACommand())
    {
        r = processACommand();
    }
    else
    {
        r = processCCommand();
    }

    return r;
}

std::map<std::string, int> HackAssembler::initializeSymbolsMap()
{

    std::map<std::string, int> r;

    r.insert({ "SP", 0 });
    r.insert({ "LCL", 1 });
    r.insert({ "ARG", 2 });
    r.insert({ "THIS", 3 });
    r.insert({ "THAT", 4 });

    for (int i = 0; i < 16; ++i)
    {
        r.insert({ "R" + std::to_string(i), i });
    }

    r.insert({ "SCREEN", 16384 });
    r.insert({ "KBD", 24576 });

    return r;
}

std::regex HackAssembler::getLabelCheckRegex() const
{
    return std::regex("\\([A-Za-z_\\.$:][A-Za-z_0-9\\.$:]*\\)");
}

std::regex HackAssembler::getACommandCheckRegex() const
{
    return std::regex("@[A-Za-z_\\.$:][A-Za-z_0-9\\.$:]*");
}

void HackAssembler::replaceSymbols()
{
    std::map<std::string, int> symbolsMap = initializeSymbolsMap();

    std::vector<std::string> codeWithoutSymbols;

    std::size_t lineCounter = 0ul;
for (auto i = 0ul; i < hackAssemblyCode.size(); ++i)
{
    if (std::regex_match(hackAssemblyCode[i], getLabelCheckRegex()))
    {
        const auto symbolWithoutParentheses = hackAssemblyCode[i].substr(1, hackAssemblyCode[i].size() - 2);
        symbolsMap.insert(std::make_pair(symbolWithoutParentheses, lineCounter));

    }
    else
    {
        codeWithoutSymbols.push_back(std::move(hackAssemblyCode[i]));
        ++lineCounter;
    }
}

    hackAssemblyCode = std::move(codeWithoutSymbols);


for (auto& line : hackAssemblyCode)
{
    if (std::regex_match(line, getACommandCheckRegex()))
    {
        auto it = symbolsMap.find(line.substr(1));
        if (it != std::end(symbolsMap))
        {
            line = "@" + std::to_string(it->second);
        }
    }
}
}

void HackAssembler::replaceVariables()
{
std::map<std::string, int> varsMap;
std::size_t freeAddress = 16;

for (auto& line : hackAssemblyCode)
{
    auto it = varsMap.find(line);
    if (std::regex_match(line, getACommandCheckRegex()))
    {
        if (it == std::end(varsMap))
        {
            varsMap.insert(std::make_pair(line, freeAddress));
        }

        const auto lineCounter = it == std::end(varsMap) ? freeAddress++ : it->second;

        line = "@" + std::to_string(lineCounter);
    }
}
}

bool HackAssembler::isACommand() noexcept
{
    return hackAssemblyCode[currentLine][0] == '@';
}
