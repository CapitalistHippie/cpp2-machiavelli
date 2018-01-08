#ifndef MACHIAVELLI_MACH_INFRA_CSVPARSER_H_INCLUDED
#define MACHIAVELLI_MACH_INFRA_CSVPARSER_H_INCLUDED

#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

#include "mach/infra/functionalerror.h"
#include "mach/infra/functionalerrorcategory.h"

namespace mach
{
namespace infra
{
enum class ParsingStatus
{
    Good,
    EndOfFile
}; // enum class ParsingStatus

class CsvRow
{
  private:
    std::stringstream stream;

  public:
    CsvRow()
    {
    }

    CsvRow(std::string row)
      : stream(row)
    {
    }

    ParsingStatus ParseNextColumn(std::string& columnBuffer)
    {
        std::getline(stream, columnBuffer, ';');

        if (stream.eof())
        {
            return ParsingStatus::EndOfFile;
        }

        return ParsingStatus::Good;
    }

    void IgnoreNextColumn()
    {
        stream.ignore(std::numeric_limits<int>::max(), ';');
    }

    template<typename T>
    CsvRow& operator>>(T& val)
    {
        std::string buffer;
        ParseNextColumn(buffer);

        auto bufferStream = std::stringstream(buffer);

        bufferStream >> val;

        return *this;
    }
}; // class CsvRow

class CsvParser
{
  private:
    bool fileOpened;

    std::ifstream fileStream;

  public:
    CsvParser()
      : fileOpened(false)
    {
    }

    void OpenFile(std::string filePath)
    {
        fileStream = std::ifstream(filePath);

        // Check if the file really opened.
        if (!fileStream.is_open())
        {
            throw std::system_error(std::make_error_code(FunctionalError::UnableToOpenFile));
        }

        fileOpened = true;
    }

    ParsingStatus ParseNextRow(CsvRow& rowBuffer)
    {
        if (!fileOpened)
        {
            throw std::system_error(std::make_error_code(FunctionalError::NoFileOpened));
        }

        // If the line starts with a '#' it's a comment.
        while (fileStream.peek() == '#')
        {
            fileStream.ignore(std::numeric_limits<int>::max(), '\n');
        }

        // Read the line into a row.
        std::string lineBuffer;
        std::getline(fileStream, lineBuffer, '\n');

        if (fileStream.eof())
        {
            return ParsingStatus::EndOfFile;
        }

        rowBuffer = CsvRow(std::move(lineBuffer));

        return ParsingStatus::Good;
    }

    void IgnoreNextRow()
    {
        if (!fileOpened)
        {
            throw std::system_error(std::make_error_code(FunctionalError::NoFileOpened));
        }

        // If the line starts with a '#' it's a comment.
        while (fileStream.peek() == '#')
        {
            fileStream.ignore(std::numeric_limits<int>::max(), '\n');
        }

        fileStream.ignore(std::numeric_limits<int>::max(), '\n');
    }
}; // class CsvParser
} // namespace infra
} // namespace mach

#endif // #ifndef MACHIAVELLI_MACH_INFRA_CSVPARSER_H_INCLUDED
