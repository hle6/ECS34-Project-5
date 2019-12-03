#include "CSVReader.h"
#include <iostream>
#include <queue>
void CCSVReader::callback_row(void *str, size_t length, void *calldata)
{
    //idea of the csv_parser is from http://manpages.ubuntu.com/manpages/cosmic/man3/csv.3.html
    //idea from testcsv.c
	CCSVReader* ptr = static_cast<CCSVReader*>(calldata);
    const char *str_char = static_cast<const char*>(str);
	ptr->csvrow.push_back(std::string(str_char,length));
}

void CCSVReader::callback_end(int constant, void *calldata)
{
	CCSVReader* ptr = static_cast<CCSVReader*>(calldata);
	if(constant == -1 or constant == '\x0a' or constant== '\x0d'){
		ptr->buffer.push(ptr->csvrow);
		ptr->csvrow.clear();
	}

}

CCSVReader::CCSVReader(std::istream &in) : In(in)
{	
	// code from lecture on 11.8
	csv_init(&csvparser,0);// initialize a parser object
}

CCSVReader::~CCSVReader()
{	
	csv_free(&csvparser);// free a parser object
}

bool CCSVReader::End() const
{	
    if(In.eof()){
        return true;
    }else{
        return false;
    }
}

bool CCSVReader::ReadRow(std::vector<std::string> &row)
{
	//idea of the csv_parser is from http://manpages.ubuntu.com/manpages/cosmic/man3/csv.3.html
    //code from lecture 11.8
	char tempbuffer[100000];
	bool result = true;
	while(result){
		if (buffer.empty() == true){
			In.read(tempbuffer, 100000);
            int byteread = In.gcount();
		    csv_parse(&csvparser, tempbuffer, byteread, callback_row, callback_end, this);
		}else{
			result = false;
		}
	}
	if(!buffer.empty())
	{	
		row.clear();
		row = buffer.front();
		buffer.pop();
        csv_fini(&csvparser, callback_row, callback_end,this); //reinitializes the parser
		return true;
	}

	return false;

}