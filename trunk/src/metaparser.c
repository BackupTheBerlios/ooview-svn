#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "metaparser.h"

void parseText (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *key;
	
	while (xmlStrcmp(cur->name,(const xmlChar *)"user-defined") ) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"generator"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"initial-creator"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"creation-date"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"creator"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"date"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"language"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"editing-cycles"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"editing-duration"))) {

			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("%s\n", key);
		    xmlFree(key);			
 	    }

	cur = cur->next;
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
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "document-meta")) {					/* root-element überprüfen */
		fprintf(stderr,"document of the wrong type, root node != document-meta");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;								/* eine ebene tiefer springen */
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"meta"))){		/* so lange durchlaufen bis er den meta tag findet */
			cur = cur->xmlChildrenNode;								/* noch eine ebene tiefer */
			
			parseText (doc, cur);
				
		}
		if (cur != NULL) cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}
