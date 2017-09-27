#include <lib_include.h>
#include "lib_xmlparser.h"

namespace el {
	int lib_xmlparser_t::open( const char* name ){
		if(NULL != this->doc_ptr){
			return ERR;
		}
		//this->doc_ptr = xmlReadFile(name, "UTF-8", XML_PARSE_RECOVER); //�����ļ�	
		xmlKeepBlanksDefault(0);
		this->doc_ptr = xmlReadFile(name, 0, XML_PARSE_NOBLANKS);
		
		//m_doc = xmlParseFile(pszFilePath);
		//�������ĵ��Ƿ�ɹ���������ɹ���libxml��ָһ��ע��Ĵ���ֹͣ��	
		//һ�����������ǲ��ʵ��ı��롣XML��׼�ĵ�������UTF-8��UTF-16�⻹�����������뱣�档	
		//����ĵ���������libxml���Զ���Ϊ��ת����UTF-8���������XML������Ϣ������XML��׼��.	
		if (NULL == this->doc_ptr){
			ALERT_LOG("");
			return ERR;
		}

		this->node_ptr = xmlDocGetRootElement(this->doc_ptr); //ȷ���ĵ���Ԫ��	
		if (NULL == this->node_ptr){
			ALERT_LOG("");
			this->close();
			return ERR;
		}
		return SUCC;
	}

	void lib_xmlparser_t::close(){
		if(NULL != this->doc_ptr){
			xmlFreeDoc(this->doc_ptr);
			this->doc_ptr = NULL;
			this->node_ptr = NULL;
		}
	}

	int lib_xmlparser_t::strcmp( const xmlNodePtr node_ptr, const char* name ){
		return xmlStrcmp(node_ptr->name,(const xmlChar*)name);
	}

	void lib_xmlparser_t::move2children_node(){
		this->node_ptr = this->node_ptr->xmlChildrenNode;
	}

	void lib_xmlparser_t::move2next_node(){
		this->node_ptr = this->node_ptr->next;
	}

	lib_xmlparser_t::lib_xmlparser_t(){
		this->doc_ptr = NULL;
		this->node_ptr = NULL;
	}

	lib_xmlparser_t::~lib_xmlparser_t(){
		this->close();
	}

	uint32_t lib_xmlparser_t::get_xml_prop( xmlNodePtr cur, const void* prop, manip_t manip /*= std::dec*/ ){
		uint32_t t = 0;
		xmlChar* str;
		if (NULL == cur || NULL == (str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
			ALERT_LOG("[%s]", (char*)prop);
			assert(0);
		} else {
			std::istringstream iss(reinterpret_cast<const char*>(str));
			if (!iss.good()) {
				ALERT_LOG("[%s]", (char*)prop);
				assert(0);
			}
			iss >> manip >> t;
			xmlFree(str);
		}
		return t;
	}

	uint32_t lib_xmlparser_t::get_xml_prop_def( xmlNodePtr cur, const void* prop, const uint32_t def, manip_t manip /*= std::dec*/ ){
		uint32_t t = 0;
		xmlChar* str;
		if (NULL == cur || NULL == (str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
			t = def;
		} else {
			std::istringstream iss(reinterpret_cast<const char*>(str));
			if (!iss.good()) {
				ALERT_LOG("[%s]", (char*)prop);
				assert(0);
			}
			iss >> manip >> t;
			xmlFree(str);
		}
		return t;
	}

}

