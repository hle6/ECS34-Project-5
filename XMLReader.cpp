#include "XMLReader.h"
#include<iostream>
#include "XMLEntity.h"


CXMLReader::CXMLReader(std::istream &is): Is(is)
{
    //learned from https://www.xml.com/pub/1999/09/expat/index.html
    XML_SetElementHandler(xmlparser, callback_start, callback_end); //Set handlers for start and end tags
    XML_SetCharacterDataHandler(xmlparser,callback_text); //Set handler for text.
    XML_SetUserData(xmlparser, this); // pass a pointer to this structure to the handlers
}


void CXMLReader::callback_start(void *calldata, const char *name, const char **attributs)
{       
        CXMLReader *CXML = static_cast<CXMLReader*>(calldata);
        SXMLEntity xmlentity;
        xmlentity.DType = SXMLEntity::EType::StartElement;
        xmlentity.DNameData = std::string(name);
        int i = 0;
        bool result = true;
        while (result){
            if (attributs[i]){
                xmlentity.SetAttribute(std::string(attributs[i]), std::string(attributs[i+1]));
                i += 2;
            }else{
                result = false;
            }
        }
        CXML->buffer.push(xmlentity);

}
void CXMLReader::callback_end(void *calldata, const char *name)
{   
        CXMLReader* CXML = static_cast<CXMLReader*>(calldata);
        SXMLEntity xmlentity;
        xmlentity.DType = SXMLEntity::EType::EndElement;
        xmlentity.DNameData = std::string(name);
        CXML->buffer.push(xmlentity);
}

void CXMLReader::callback_text(void *calldata, const char *s, int len)
{   
        auto CXML = static_cast<CXMLReader*>(calldata);
        SXMLEntity xmlentity;
        xmlentity.DType = SXMLEntity::EType::CharData;
        xmlentity.DNameData = std::string(s,len);
        CXML->buffer.push(xmlentity);
}

CXMLReader::~CXMLReader()
{
    XML_ParserFree(xmlparser);
}

bool CXMLReader::End()
{   
    if(buffer.empty() && xmlparser != 0){
        return true;
    }else{
        return false;
    }
    
}
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata)
{    //idea from office hour of professor Nitta
    char tempbuffer[100000];
    SXMLEntity xmlentity;
    while(true){
        if (buffer.empty() == false){
            if (buffer.front().DType == SXMLEntity::EType::CharData){
                if(skipcdata){
                    buffer.pop();
                }else{
                    break;
                }
            }else{
                break;
            }
        }else {
                break;
        }
    }
	
    while (true){
        if(buffer.empty()){
            if (!Is.eof()){
                Is.read(tempbuffer,100000);
                XML_Parse(xmlparser, tempbuffer, Is.gcount(), Is.eof());//Pass a buffer full of document to the parser
                while(true){
                    if (buffer.empty() == false){
                        if (buffer.front().DType == SXMLEntity::EType::CharData){
                            if(skipcdata){
                               buffer.pop();
                            }else{
                                break;
                            }
                        }else{
                             break;
                        }
                    }else {
                        break;
                    }
                }
            }else{
                break;
            }
        }else{
            break;
        }
    }

	if(buffer.empty() == false){
		entity = buffer.front();
		buffer.pop();
	}
	return true;
}