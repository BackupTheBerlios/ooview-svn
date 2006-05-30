#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>


void parseSPAN (xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *key;
	xmlChar *full_key;
	xmlChar *fk;
	cur = cur->xmlChildrenNode;
	//printf("\n");
	while (cur != NULL) {
		if ( (!xmlStrcmp(cur->name, (const xmlChar *)"span")) ) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			fk = xmlStrcat(full_key, key);
			full_key = fk;
			xmlFree(key);
		}
		cur = cur->next;
	}
		printf("%s\n", fk);
		return;
}

void parseStory (xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	xmlNodePtr temp_cur;
	temp_cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		//temp = xmlNodeListGetString(doc, cur->name, 1);
		//printf(":%s\n", (char*) temp);
		/*if ( (!xmlStrcmp(cur->name, (const xmlChar *)"p")) ) {
				
			temp_cur = cur->xmlChildrenNode;
			if (!xmlStrcmp(temp_cur->name, (const xmlChar *)"span")) {		
					if(xmlNodeListGetString(doc, cur->xmlChildrenNode, 1) == NULL)
					parseSPAN(doc, cur);
					else {
							key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
							printf("%s\n", key);
					}
			}
					
			else{
				
				if(xmlNodeListGetString(doc, cur->xmlChildrenNode, 1) != NULL){
					key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
					printf("%s\n", key);
					xmlFree(key);
				}
				else parseSPAN(doc, cur);
			}
		}
		cur = cur->next;*/
		if (!xmlStrcmp(temp_cur->name, (const xmlChar *)"span")) printf("!");
		temp_cur = cur->xmlChildrenNode;
		if (cur->xmlChildrenNode != NULL) printf("&");
		if (!xmlStrcmp(cur->name, (const xmlChar *)"p")) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("%s\n", key);
			xmlFree(key);
		}
		//temp_cur = cur->xmlChildrenNode;
		cur = cur->next;
		//if(cur != NULL) temp_cur = cur->xmlChildrenNode;
			
	}
	return;
}

static void parseDoc(char *docname) {
	
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);
			
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
						
	cur = xmlDocGetRootElement(doc);
							
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	/* printf("%s\n", (char*) cur->name); */
	if (xmlStrcmp(cur->name, (const xmlChar *) "document-content")) {				/* ROOT NODE (story) */
		fprintf(stderr,"document of the wrong type, root node != story\n");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"body"))){			/* CHILD NODE (storyinfo) */
			cur = cur->xmlChildrenNode;
			while (cur != NULL) {
				if ((!xmlStrcmp(cur->name, (const xmlChar *)"text")))
					parseStory (doc, cur);
				cur = cur->next;
			}	
					
			//parseStory (doc, cur);
		}
															 
		if (cur != NULL) cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int main(int argc, char **argv) {

		char *docname;
				
		if (argc <= 1) {
			printf("Usage: %s docname\n", argv[0]);
			return(0);
		}
		docname = argv[1];
		parseDoc (docname);
			
		return (1);
}
