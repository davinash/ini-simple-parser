/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of IniParser
 *  Copyright (C) 2011 Avinash Dongre ( dongre.avinash@gmail.com )
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

#include "IniParser.h"
IniParser::IniParser() {
}

IniParser::~IniParser() {
    m_sectionMap.clear();
}

bool IniParser::validLine(const std::string &line) const {
    std::string temp = line;
    temp.erase(0, temp.find_first_not_of("\t "));
    if (temp[0] == '=')
        return false;
    for (size_t i = temp.find('=') + 1; i < temp.length(); i++)
        if (temp[i] != ' ')
            return true;
    return false;
}
std::vector<std::string> IniParser::getAllSections() {
    std::vector<std::string> section;
    for ( SectionMapType::iterator itr = m_sectionMap.begin();
          itr != m_sectionMap.end();
          ++itr) {
        section.push_back(itr->first);
    }
    return section;
}
void IniParser::Load( const std::string &filename) throw (IniParseException){
    std::ifstream ifs(filename.c_str());
    FileLineType fileLines;
    if ( !ifs.is_open()) 
        throw IniParseException( filename + " Not found ");

    for ( std::string line; std::getline(ifs,line);){
        fileLines.push_back(line);
    }
    ifs.close();
    if ( fileLines.size() == 0 )
        throw IniParseException( filename + " is empty");

    std::string section;
    int index = 0;
    for ( FileLineType::iterator itr = fileLines.begin();
          itr != fileLines.end(); ++itr ) {
        index++;
        std::string curLine = *itr;
        // Check for the empty line.
        if( curLine.find_first_not_of(' ') == curLine.npos)
            continue;
        // Erase leading white spaces
        curLine.erase(0, curLine.find_first_not_of("\t ")); 
        if ( curLine[0] == ';' )	
            continue;
        if ( curLine[0] == '[' || curLine[curLine.size() - 1] == ']' ) {
            std::string::size_type left  = curLine.find('[');
            std::string::size_type right      = curLine.find(']');
            if ( std::string::npos != left && std::string::npos != right){
                section = curLine.substr(left + 1, right - 1);

                std::pair<SectionMapType::iterator, bool> ret;
                ret = m_sectionMap.insert(std::make_pair(section, 
                                                         KeyValueMapType()));
                if ( ret.second == false ) 
                    throw IniParseException("Duplicate section", index);	
            } else 
                throw IniParseException("Invalid section entry line", index);	
            continue;
        }
        if (curLine.find(';') != curLine.npos)// Remove comment if any 
            curLine.erase(curLine.find(';'));
        if ( validLine( curLine) == false )
            throw IniParseException("Invalid line", index);

        std::string::size_type delPos = curLine.find('=');
        if ( delPos == std::string::npos) {
            throw IniParseException("Delimiter is missing ", index);
        }
        std::string key   = curLine.substr(0, delPos);// Get the key
        std::string value = curLine.substr(delPos + 1);// Get the value
        if ( key.empty() || value.empty())
            throw IniParseException("Invalid line", index);

        SectionMapType::iterator found = m_sectionMap.find(section);
        if ( found == m_sectionMap.end()){
            throw IniParseException("Section is missing", index);
        }
        std::pair<KeyValueMapType::iterator, bool> ret;
        ret = found->second.insert(std::make_pair(key,value));
        if ( ret.second == false ) 
            throw IniParseException("Duplicate section", index);	
    }
}


