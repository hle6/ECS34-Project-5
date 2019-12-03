#include "XMLWriter.h"
#include<iostream>

CXMLWriter::CXMLWriter(std::ostream &os) : Os(os)
{

}

CXMLWriter::~CXMLWriter()
{

}

bool CXMLWriter::Flush()
{
       // Outputs all end elements for those that have been started
        
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity)
{
        if(entity.DType == SXMLEntity::EType::StartElement or entity.DType == SXMLEntity::EType::CompleteElement)
        {
                std::string str_start = "<" + entity.DNameData;
		
		for(auto temp : entity.DAttributes)
		{
                        auto first_attribute = std::get<0>(temp);
                        auto second_attribute = std::get<1>(temp);
			str_start += " " + first_attribute + "=" +  '"' + second_attribute + '"';
		}

                if(entity.DType == SXMLEntity::EType::StartElement)
                {
                        str_start += ">";
                        stack.push(entity);
                }else{
                        str_start += "/>"; 
                }
                        
                Os << str_start;
        }
        else if(entity.DType == SXMLEntity::EType::EndElement)
        {
                std::string str_end = "</" + entity.DNameData + ">";
		if(stack.top().DType == SXMLEntity::EType::EndElement){
                        stack.pop();
                }
                Os << str_end;
        }
        else if(entity.DType == SXMLEntity::EType::CharData)
        {
                Os << entity.DNameData;
        }
        else{
                return false;
        }
                
        return true;
}