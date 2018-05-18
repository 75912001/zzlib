/********************************************************************
	platform:	
	author:		kevin
	copyright:	All rights reserved.
	purpose:	utilities for parsing xml files
				ʹ�÷�����
				g++ xml.cpp -I/usr/include/libxml2 -lxml2 -lel

				sudo apt-get install libxml2
				sudo apt-get install libxml2-dev 
	brief:		ok	
*********************************************************************/

#pragma once

#include "lib_include.h"

#include <libxml/tree.h>

#include "lib_log.h"
#include "lib_util.h"

namespace el{

	struct lib_xmlparser_t{
	public:
		xmlDocPtr doc_ptr;//��������ĵ�ָ��		
		xmlNodePtr node_ptr;//������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�)		

		typedef std::ios_base& (&manip_t)(std::ios_base&);
	public:
		lib_xmlparser_t();
		~lib_xmlparser_t();
		int open(const char* name);		
		void move2children_node();
		void move2next_node();
		int strcmp(const xmlNodePtr node_ptr, const char* name);
		/**
		* @brief ��ĳ��xml���Ե�ֵ��ȡ����	
		* @param val xml������ֵ������ȡ��val��	
		* @param prop xml������	
		* @param manip ��ʲô��ʽ����xml����ֵ��Ĭ����decʮ���ƣ�������oct�˽��ƺ�hexʮ������	
		* @note ע�⣡��Ҫ�������������ȡxml����ֵ���ַ������У�������Խ���Σ�գ������Ҫ��ȡ�ַ�����\n	
		*       ����������������ַ�������string�����������get_xml_prop_raw_str���ַ��������ַ������	
		* @see get_xml_prop_raw_str, get_xml_prop_def
		*/
		template <typename T>
		static void get_xml_prop(xmlNodePtr cur, T& val, const void* prop, manip_t manip = std::dec){
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
				iss >> manip >> val;
				xmlFree(str);
			}
		}
		static uint32_t get_xml_prop(xmlNodePtr cur, const void* prop, manip_t manip = std::dec);
		/**
		* @brief ͬget_xml_prop�������prop���Բ����ڵĻ������Ĭ��ֵdef����val	
		* @see get_xml_prop	
		*/
		template <typename T1, typename T2>
		static void get_xml_prop_def(xmlNodePtr cur, T1& val, const void* prop, const T2& def, manip_t manip = std::dec){
			if (NULL == cur){
				val = def;
				return;
			}

			xmlChar* str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop));
			const char* cc = reinterpret_cast<const char*>(str);
			if (NULL == str || 0 == ::strcmp(cc, "")) {
				val = def;
			} else {
				std::istringstream iss(reinterpret_cast<const char*>(str));
				if (!iss.good()) {
					ALERT_LOG("[%s]", (char*)prop);
					assert(0);
				}
				iss >> manip >> val;
				xmlFree(str);
			}
		}

		static uint32_t get_xml_prop_def(xmlNodePtr cur, const void* prop, const uint32_t def, manip_t manip = std::dec);
		/**
		* @brief ��ĳ��xml���Ե�ֵ��ȡ���ַ�������	
		* @param val xml������ֵ������ȡ��val��	
		* @param prop xml������	
		* @throws XmlParseError	
		* @see get_xml_prop_raw_str_def	
		*/
		template <size_t len>
		static void get_xml_prop_raw_str(xmlNodePtr cur, char (&val)[len], const void* prop){
			xmlChar* str;
			if (NULL == cur || NULL == (str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
				ALERT_LOG("[%s]", (char*)prop);
				assert(0);
			} else {
				::strncpy(val, reinterpret_cast<char*>(str), len - 1);
				val[len - 1] = '\0';
				xmlFree(str);
			}
		}
		/**
		* @brief ͬget_xml_prop_raw_str�������prop���Բ����ڵĻ������Ĭ��ֵdef����val	
		* @see get_xml_prop_raw_str	
		*/
		template <size_t len>
		static void get_xml_prop_raw_str_def(xmlNodePtr cur, char (&val)[len], const void* prop, const char* def){
			xmlChar* str;
			if (NULL == cur || NULL == (str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
				::strncpy(val, def, len - 1);
				val[len - 1] = '\0';
			} else {
				::strncpy(val, reinterpret_cast<char*>(str), len - 1);
				val[len - 1] = '\0';
				xmlFree(str);
			}
		}
		//�õ�һ���ڵ������	
		template <typename T>
		static void get_xml_content(xmlNodePtr cur, T& val){
			xmlChar* str;
			if (NULL == cur || NULL == (str = xmlNodeGetContent(cur))) {
				ALERT_LOG("");
				assert(0);
			} else {
				std::istringstream iss(reinterpret_cast<const char*>(str));
				if (!iss.good()) {
					ALERT_LOG("");
					assert(0);
				}
				iss >> val;
				xmlFree(str);
			}
		}
		/**
		* @brief ��ĳ��xml���Ե�ֵ��ȡ��������	
		* @param arr xml������ֵ������ȡ��arr��	
		* @param prop xml������	
		* @param manip ��ʲô��ʽ����xml����ֵ��Ĭ����decʮ���ƣ�������oct�˽��ƺ�hexʮ������	
		* @throws XmlParseError	
		* @return ��ȡ������arr�е�ֵ�ĸ���	
		* @see get_xml_prop_arr_def	
		*/
		template <typename T1, size_t len>
		static size_t get_xml_prop_arr(xmlNodePtr cur, T1 (&arr)[len], const void* prop, manip_t manip = std::dec){
			xmlChar* str;
			if (NULL == cur || NULL == (str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
				ALERT_LOG("[%s]", (char*)prop);
				assert(0);
			} else {
				size_t i = 0;
				std::istringstream iss(reinterpret_cast<const char*>(str));
				if (!iss.good()) {
					ALERT_LOG("[%s]", (char*)prop);
					assert(0);
				}
				while ((i != len) && (iss >> manip >> arr[i])) {
					++i;
				}

				xmlFree(str);
				return i;
			}
		}
		/**
		* @brief ͬget_xml_prop_arr�������prop���Բ����ڵĻ������Ĭ��ֵdef����arr	
		* @return ��ȡ������arr�е�ֵ�ĸ��������prop�����ڣ��򷵻�0	
		* @see get_xml_prop_arr	
		*/
		template <typename T1, typename T2, size_t len>
		static size_t get_xml_prop_arr_def(xmlNodePtr cur, T1 (&arr)[len], const void* prop, const T2& def, manip_t manip = std::dec){
			xmlChar* str;
			if (NULL == cur || NULL == (str = xmlGetProp(cur, reinterpret_cast<const xmlChar*>(prop)))) {
				for (size_t i = 0; i != len; ++i) {
					arr[i] = def;
				}
				return 0;
			} else {
				size_t i = 0;
				std::istringstream iss(reinterpret_cast<const char*>(str));
				if (!iss.good()) {
					ALERT_LOG("[%s]", (char*)prop);
					assert(0);
				}
				while ((i != len) && (iss >> manip >> arr[i])) {
					++i;
				}

				xmlFree(str);
				return i;
			}
		}
	protected:
	private:
		void close();
		lib_xmlparser_t(const lib_xmlparser_t& cr);
		lib_xmlparser_t& operator=(const lib_xmlparser_t& cr);
	};
//////////////////////////////////////////////////////////////////////////
//example
#if 0
int ex_xml( const char* xml_name )
{
	/*����	
	<?xml version="1.0" encoding="UTF-8"?>
	<!-- edited with XMLSpy v2011 (http://www.altova.com) by kevin (home) -->
	<mini name="mini_name">
		<so name="so_name" version ="1">
			<net>net.so</net>
		</so>
	</mini>
	*/
	ice::lib_xmlparser xml;
	int ret = xml.open(xml_name);
	if(0 != ret){
		return ret;
	}
	xml.move2children_node();

	while(xml.get_node_ptr() != NULL){
		//ȡ���ڵ��е�����	
		if (!xmlStrcmp(xml.get_node_ptr()->name, (const xmlChar *)"so")){
			int i;
			xml.get_xml_prop(i,"version");
			//std::cout << i << std::endl; //�õ�so��version����ֵ�����	

			xml.move2children_node();
			while(xml.get_node_ptr()){
				if(!xmlStrcmp(xml.get_node_ptr()->name,(const xmlChar*)"net")){
					std::string str;
					xml.get_xml_content(str);
					//std::cout << str << std::endl; //�õ�so��ֵ�����				
				}
				xml.move2next_node();
			}
		}
		xml.move2next_node();
	}
	return 0;
}

#endif
}

