#include"CSVWriter.h"

CCSVWriter::CCSVWriter(std::ostream &ou): Out(ou)
{

}

CCSVWriter::~CCSVWriter()
{

}

bool CCSVWriter::WriteRow(const std::vector<std::string> &row)
{
    int length = row.size();
	if(row.size() == 0){
        return false;
    }
	if(Out.good() == false){
        return false;
    }	 
    for(int i = 0;i < length; i++)
	{	
		auto str = row[i];
		Out << str;
	}
    Out << ",";
	Out << "\n";
	
	return true;
}