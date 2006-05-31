/* 
 * odt-dokument enpacken; 
 * zb 
 * mkdir test; unzip -x test.odt -d test
 * $ ./xmlparser test/content.xml
 * kompilieren mit gcc -I/usr/include/libxml2 xmlparser3.c -o xmlparser3 -lxml2
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

void parseText (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *key;
	int i=0;
	
	while ((cur != NULL) || (i<=7)) {
		printf("%s: ",cur->name);
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
	i++;
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

int
main(int argc, char **argv) {

	char *docname;
		
	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

	docname = argv[1];
	parseDoc (docname);

	return (1);
}
