#include <lib_include.h>
#include "lib_xmlparser.h"

namespace el {
	int lib_xmlparser_t::open( const char* name ){
		if(NULL != this->doc_ptr){
			return ERR;
		}
		//this->doc_ptr = xmlReadFile(name, "UTF-8", XML_PARSE_RECOVER); //解析文件	
		xmlKeepBlanksDefault(0);
		this->doc_ptr = xmlReadFile(name, 0, XML_PARSE_NOBLANKS);
		
		//m_doc = xmlParseFile(pszFilePath);
		//检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。	
		//一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。	
		//如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.	
		if (NULL == this->doc_ptr){
			ALERT_LOG("");
			return ERR;
		}

		this->node_ptr = xmlDocGetRootElement(this->doc_ptr); //确定文档根元素	
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

