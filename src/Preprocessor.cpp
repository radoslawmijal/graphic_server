#include "Preprocessor.hh"

Preprocessor::Preprocessor() {}

std::string RemoveBlockComments(const std::string &line, bool &in_multiline_comment) {
    std::string result;
    for (size_t i = 0; i < line.size(); ++i) {
        if (!in_multiline_comment && i + 1 < line.size() && line[i] == '/' && line[i + 1] == '*') {
            in_multiline_comment = true;        // find comment start
            i++;
        }
        else if (in_multiline_comment && i + 1 < line.size() && line[i] == '*' && line[i + 1] == '/') {
            in_multiline_comment = false;       // find comment end
            i++;
        }
        else if (!in_multiline_comment) {
            result += line[i];
        }
    }
    return result;
}

std::string RemoveSingleLineComments(const std::string &line) {
    for (size_t i = 0; i + 1 < line.size(); ++i) {
        if (line[i] == '/' && line[i + 1] == '/') {
            return line.substr(0, i);       // find comment sign and delete
        }
    }
    return line;
}

static bool IsCharCorrectForAName(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
    (c >= '0' && c <= '9') || (c == '_');
}

std::string ConvertMacros(std::string line, std::map<std::string, std::string> &macros){
    size_t first_char_pos = line.find_first_not_of(" \t");
    if (first_char_pos == std::string::npos)    // skip empty line
        return line;

    if (line.substr(first_char_pos, 7) == "#define"){   //find #define
        size_t name_start = first_char_pos + 7;

        while(name_start < line.length() && line[name_start] == ' ' ||
         line[name_start] == '\t'){
            name_start++;                   // find macro name start
         }

        size_t name_end = line.find_first_of(" \t", name_start);    // find macro name end

        if (name_end == std::string::npos){
            macros[line.substr(name_start)] = "";   // macro without value
            return "";};               
        
        std::string name = line.substr(name_start, name_end - name_start);  // name found

        if(name.empty()){return "";};   // empty name
        
        size_t value_start = name_end;
        while(value_start < line.length() && line[value_start] == ' ' ||
         line[value_start] == '\t'){
            value_start++;                  // find macro value start
         };

         std::string value = line.substr(value_start);  // take whole line
         macros[name] = value;

         return "";
    }

    for (const auto& pair : macros){
        const std::string& name = pair.first;   // make macro pairs
        const std::string& value = pair.second;

        if(name.empty()){continue;};   // skip empty names

        size_t pos = 0;

        while ((pos = line.find(name, pos)) != std::string::npos){
            bool start_ok = (pos == 0) || !IsCharCorrectForAName(line[pos - 1]);
            bool end_ok = (pos + name.length() == line.length()) || !IsCharCorrectForAName(line[pos + name.length()]);

            if (start_ok && end_ok){
                line.replace(pos, name.length(), value);
                pos += value.length();
            }else{
                pos += name.length();
            }
        }
    }

    return line;
}


std::string Preprocessor::ProcessFile(const std::string &input_file) {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << input_file << std::endl;
        return "";
    }

    std::string line, output;
    bool in_multiline_comment = false;
    std::map<std::string, std::string> macros;

    while (std::getline(file, line)) {
        
        line = RemoveBlockComments(line, in_multiline_comment);

        line = RemoveSingleLineComments(line);

        line = ConvertMacros(line, macros);

        if (!line.empty())
            output += line + "\n";
    }

    file.close();
    return output;
}