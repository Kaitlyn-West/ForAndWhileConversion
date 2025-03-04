#include<iostream>
#include<fstream>

/*
strip takes a string and removes any
leading or trailing ' '
and all '\t'
*/
void strip(std::string &input)
{
    while(input.find('\t') != std::string::npos)
    {

        int p = input.find('\t');//temp pos var
        //std::cout << "erasing '\\t's" << std::endl;
        //std::cout << "Erasing '" << input[p] << "' from \"" << input << "\" (pos " << p << ")\n";
        input.erase(p, p+1);
    }

    while(input[0] == ' ')
    {
        //std::cout << "erasing ' 's" << std::endl;
        //std::cout << "Erasing '" << input[0] << "' from \"" << input << "\" (pos 0)\n";
        input.erase(0,1);
    }
    while(input[input.size()-1] == ' ')
    {
        //std::cout << "erasing ' 's" << std::endl;
        //std::cout << "Erasing '" << input[input.size()-1] << "' from \"" << input << "\" (pos " << input[input.size()-1] << ")\n";
        input.erase(input.size()-1);
    }
    return;
}

/*
full_header_to_statments takes a string that
contains the full header of a for loop of the
format "for (init;condition;update)" it is
unimportant if the opening bracket of the loop 
is on the same line as the header or not
*/
std::string full_for_header_to_statments(std::string fullHeader)
{
    int pos = fullHeader.find("for (");
    /*
    forPos is the position of the start of the
    /declaration (not neccessarily the same, the
    user may have used a tab or maybe several 
    spaces, or it could be right at start)
    */
    fullHeader.erase(0, pos + 5);
    /*
    getting rid of everthing preceding and 
    including the "for ("
    */
    pos = fullHeader.find_last_of(")");
    //finds end of header
    fullHeader.erase(pos, fullHeader.length());
    //gets rid of everything after the last ")"


    return fullHeader;
}

/*
for_to_initialization takes a string containing
the statments of header of a for loop and
returns a string describing the initialization 
statment of the for loop.
*/
std::string for_to_initialization(std::string header)
{
    header.erase(header.find_first_of(";") + 1, header.length());
    /*
    erases everything after the first semicolon
    */

    strip(header);
    //erase trailing white space
    return header;
}

/*
for_to_condition takes a string containg the
statments of a for loop's header, and returns
a string corresponding to the loop's condition
expression
*/
std::string for_to_condition(std::string header)
{
    header.erase(0,header.find(";") + 1);
    /*
    erases everything before and including the first 
    semicolon
    */
    header.erase(header.find(";"), header.length());
    /*
    erases everthing after the remaining semicolon
    */

    strip(header);
    //erase trailing white space
    return header;
}

/*
for_to_update takes a string containg the statments
of a for loop's header and returns a string
corresponding to the loop's update statment
*/
std::string for_to_update(std::string header)
{
    header.erase(0, header.find_last_of(";")+1);
    //erase everything before the last semicolon
    //everything after the statment was already erased from the header

    strip(header);
    return header;
}


std::string while_to_condition(std::string header)
{
    int pos = header.find("while (");
    header.erase(0, pos+7);

    pos = header.find_last_of(")");
    header.erase(pos);
    strip(header);
    return header;
}

/*
This is a program to convtert a while loop to a 
for loop or vice verca.

the program assumes that the first line of the 
    file is not empty unless there is no
    update statment for a while loop
the program assumes that a for loop will not 
    have any code preceding the for loop.
the program assumes the loops are formated to 
    have a space between the keyword and the
    opening parenthases.
the program assumes that while loops only have
    one expression preceding the loop's header
the program assumes that a while loop with an
    initialization statment will have a newline
    before the loop's header
the program assumes that loops have the opening
    curly brace on the same line as the header
the program assumes that loops have the closing
    brace on its own line.
*/
int main()
{
    std::cout << "Enter the file location: ";
    std::string floc;
    std::cin >> floc;
    std::ifstream input(floc);
    //file that holds the loop to convert
    std::string line{""};
    //holds each line of text read in from the file

    std::string initialization{""};
    //the initalization statment of the loop
    std::string condition{""};
    //the condition expression of the loop
    std::string update{""};
    //the update expression of the loop
    std::string body{""};
    //the body of every loop

    if(input.is_open())
    {//the file successfully opened
        getline(input, line);
        //reading in the first line of the file
        if(line.find("for (") != std::string::npos)
        {//if the first line contains has for( or for ( in it, the it is a for loop that we are reading. 
            //this line must be the header of the for loop
            line = full_for_header_to_statments(line);
            //convert line from the full header to just the statments
            initialization = for_to_initialization(line);
            condition = for_to_condition(line);
            update = for_to_update(line);
            /*
            now we just need the body, which will be
            every statment after this line until the
            end of the file.
            */

            int numCurls = 0;
            while(getline(input, line))
            {
                if(line.find("{") != std::string::npos)
                {//a curl was opened
                    numCurls++;
                }
                if(line.find("}") != std::string::npos)
                {
                    if(numCurls > 0)
                    {//a curl was closed
                        numCurls--;
                    }
                    else if(numCurls < 0)
                    {//only here if there is a negative number of curls
                        std::cout << "The entered for loop has invalid syntax" << std::endl;
                        std::cout << "(more '}' than '{')" << std::endl;
                        return 1;
                    }
                    else
                    {
                        //numCurls == 0
                        //meaning this is the end of the loop
                        //since we assumed the cosing bracket is alone
                        //we can just end the loop
                        break;
                    }
                }
                //strip(line);
                //line.insert(0,"\t");
                line.insert(line.length(), "\n");
                body.append(line);
            }
            input.close();
            //display string vals for debugging:
            /*
            std::cout << "init: \"" << initialization << "\"" << std::endl;
            std::cout << "condition: \"" << condition << "\"" << std::endl;
            std::cout << "body: \"" << body << "\"" << std::endl;
            std::cout << "update: \"" << update << "\"" << std::endl;
            std::cout << std::endl;
            */

            //time to display the while loop:
            std::cout << initialization << std::endl;
            std::cout << "while(" << condition << ") {" << std::endl;
            std::cout << body << std::endl;
            std::cout << "    " << update << ';' << std::endl;
            std::cout << "}" << std::endl;
        }
        else
        {//the file does not have a for loop on the first line
            initialization = line;
            getline(input, line);
            condition = while_to_condition(line);
            //we have the header, so from here on, it is (almost) the same process to get the body
            int numCurls = 0;
            bool needsSemi = false;//this is used to indicate whether or not a line needs to have a semicolon added back if it is not the update statment
            while(getline(input, line))
            {
                if(line.find("{") != std::string::npos)
                {//a curl was opened
                    numCurls++;
                }
                if(line.find("}") != std::string::npos)
                {
                    if(numCurls > 0)
                    {//a curl was closed
                        numCurls--;
                    }
                    else if(numCurls < 0)
                    {//only here if there is a negative number of curls
                        std::cout << "The entered for loop has invalid syntax" << std::endl;
                        std::cout << "(more '}' than '{')" << std::endl;
                        return 1;
                    }
                    else
                    {
                        
                        //numCurls == 0
                        //meaning this is the end of the loop
                        //since we assumed the cosing bracket is alone
                        //we can just end the loop
                        break;
                    }
                }
                //the only diff between for and while is here
                if(needsSemi)
                {                    
                    update.insert(update.length(), ";\n");//add the removed semicolon and a newline
                }
                else
                {
                    update.insert(update.length(), "\n");//only add newline
                }
                body.append(update);//take what we had assumed was the update and put it with the rest of the body
                if(line.find_last_not_of(';') == line.length())
                {//the line contains a semicolon so we have to remove it
                    int pos = line.find_last_of(';');
                    line.erase(pos, pos+1);
                    needsSemi = true;
                }
                else
                {
                    needsSemi = false;
                }
                update = line;//assume this is the last line (meaning it is the update)

            }
            input.close();
            //display vals for debug
            /*
            std::cout << "init: \"" << initialization << "\"" << std::endl;
            std::cout << "condition: \"" << condition << "\"" << std::endl;
            std::cout << "update: \"" << update << "\"" << std::endl;
            std::cout << "body: \"" << body << "\"" << std::endl;
            std::cout << std::endl;
            */
            
            //now to dispay as for loop:
            strip(update);
            std::cout << "for (" << initialization << ' ' << condition << "; " << update << ") {" << std::endl;
            std::cout << body << std::endl;
            std::cout << "}" << std::endl;
        }
    }
    else
    {//there was an error reading the file
        std::cout << "The file failed to open\n" << std::endl;
        input.close();
    }
    return 0;
}
